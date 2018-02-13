#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <strings.h>

#include <stdint.h>


#define BYTE uint8_t

#define WORD uint16_t

#define DWORD uint32_t
#define stricmp strcasecmp

#define min(a,b) ((a)<(b)?(a):(b))

#ifdef GCC32HACK
#include "win32gcc.h"

void *memset(void *s, int c, size_t n) {
BYTE *b, f;
  b = (BYTE *) s;
  f = (BYTE) c;
  for (; n > 0; n--) {
    *b = f;
    b++;
  }
  return(s);
}

#endif

/*#define DEBUG_UNPAK_INFO 1
#define DEBUG_FIRST_ONLY 1
#define DEBUG_PAL_OFFS   1
#define DEBUG_SKIP_DUMP  1*/

#define FLAG_16B (0x80000000)
#define FLAG_FRM (0x40000000)

typedef struct {
  BYTE IDLength;
  BYTE ColorMapType;
  BYTE ImageType;
  WORD CMapStart;
  WORD CMapLength;
  BYTE CMapDepth;
  WORD XOffset;
  WORD YOffset;
  WORD Width;
  WORD Height;
  BYTE PixelDepth;
  BYTE ImageDescriptor;
} __attribute__((packed)) tga_head;

typedef struct {
  DWORD size1;
  DWORD size2;
  DWORD value;
} __attribute__((packed)) pal_head;

typedef struct {
  char name[9];
  DWORD offs;
} __attribute__((packed)) set_head;

/* http://www.clicketyclick.dk/databases/xbase/format/dbf.html#DBF_STRUCT */

typedef struct {
  BYTE  version;
  BYTE  ymd[3];
  DWORD records;
  WORD  hdr_size;
  WORD  rec_size;
  WORD  reserved1;
  BYTE  t_status;
  BYTE  encrypt;
  DWORD rec_thread;
  BYTE  reserved2[8];
  BYTE  mdx_flag;
  BYTE  lang_driver;
  WORD  reserved3;
} __attribute__((packed)) dbf_file;

typedef struct {
  char  name[11];
  char  type;
  DWORD addr;
  BYTE  size;
  BYTE  d_count;
  WORD  reserved1;
  BYTE  w_a_id;
  WORD  reserved2;
  BYTE  s_f_flag;
  BYTE  reserved3[7];
  BYTE  i_f_f;
} __attribute__((packed)) dbf_head;

#define OFFS_NUM 12
static set_head spr_offs[OFFS_NUM] = {
  {"F_BALLIS", 0x0000679D}, /* palette #34 */
  {"F__BALLI", 0x0000ADB2}, /* palette #57 */
  {"M_F_BALL", 0x0000C612}, /* palette #65 */
  {"BALLISTA", 0x000079E6}, /* palette #40 */
  {"CATAPULT", 0x00007CF2}, /* palette #41 */
  {"F_CATAPU", 0x00000618}, /* palette #2  */
  {"FLAMETHR", 0x00009B6A}, /* palette #51 */
  {"F_CANNON", 0x0000A48E}, /* palette #54 */
  {"M_CANNON", 0x0000C306}, /* palette #64 */
  {"GREMJERM", 0x000036DB}, /* palette #18 */
  {"RHINOCER", 0x00009E76}, /* palette #52 */
  {"SKELETON", 0x00000924}  /* palette #3  */
};

DWORD StrToInt(char *s) {
DWORD result;
  result = 0;
  for (; *s; s++) {
    if ((*s >= '0') && (*s <= '9')) {
      result *= 10;
      result += (*s - '0');
    } else {
      result = 0;
      break;
    }
  }
  return(result);
}

set_head *GetResPalOfs(char *set_file, char *set_item, char *dbf_name, char *dbf_offs, char itype) {
set_head *sh, *result;
BYTE *p, *h;
FILE *fl;
dbf_file *df;
dbf_head *dh;
DWORD i, j, sz;
  result = NULL;
  /* sanity check */
  if (set_file && set_item && dbf_name && dbf_offs) {
    sz = 0;
    p = NULL;
    /* open file */
    fl = fopen(set_file, "rb");
    if (fl) {
      /* read header */
      j = 0;
      fread(&j, 2, 1, fl);
      sh = (set_head *) malloc((j + 1) * sizeof(sh[0]));
      if (sh) {
        fread(sh, sizeof(sh[0]), j + 1, fl);
        /* find item */
        for (i = 0; i < j; i++) {
          /* found */
          if (!strcmp(sh[i].name, set_item)) {
            fseek(fl, sh[i].offs, SEEK_SET);
            sz = sh[i + 1].offs - sh[i].offs;
            break;
          }
        }
        free(sh);
        /* item found */
        if (sz) {
          p = (BYTE *) malloc(sz);
          if (p) {
            fread(p, 1, sz, fl);
          }
        }
      }
      fclose(fl);
    }
    /* item found */
    if (p) {
      h = p;
      /* dbf file header */
      df = (dbf_file *) p;
      p += sizeof(df[0]);
      /* dbf columns header */
      dh = (dbf_head *) p;
      /* columns number */
      df->rec_thread = (df->hdr_size - sizeof(df[0]) - 1) / sizeof(dh[0]);
      /* dbf data */
      p = h;
      p += df->hdr_size;
      /* allocate memory for result */
      result = (set_head *) malloc((df->records + 1) * sizeof(result[0]));
      if (result) {
        /* last record will be null indicating list end */
        memset(result, 0, (df->records + 1) * sizeof(result[0]));
        /* find column dbf_name, dbf_offs */
        for (i = 0; i < df->records; i++) {
          p = h;
          p += df->hdr_size + (df->rec_size*i) + 1;
          for (j = 0; j < df->rec_thread; j++) {
            /* fillin name */
            if (!strcmp(dh[j].name, dbf_name)) {
              strncpy(result[i].name, (char *) p, 8);
              sz = strlen(result[i].name);
              /* trim spaces in filename */
              while (sz && (result[i].name[sz - 1] == 0x20)) {
                sz--;
                result[i].name[sz] = 0;
              }
            }
            /* fillin offs */
            if (!strcmp(dh[j].name, dbf_offs)) {
              sz = *((DWORD *) p);
              /* fix invalid records */
              if (sz == 0x20202020) {
                /* default palette #10 for images in both palettes */
                result[i].offs |= (!itype) ? 0x00001E7A : 0x00001E78;
              } else {
                result[i].offs |= sz;
              }
              /* mark .FRM files */
              result[i].offs |= (itype ? FLAG_FRM : 0);
            }
            /* fillin 16bpp flag */
            if ((!strcmp(dh[j].name, "REMAP_BMP")) && (*p == (BYTE) 'Y')) {
              result[i].offs |= FLAG_16B;
            }
            p += dh[j].size;
          }
        }
      }
      free(h);
    }
  }
  return(result);
}

char *GetBaseFileName(char *name) {
char *result;
  result = name;
  /* sanity check and at least one char */
  if (result && *result) {
    result += strlen(result) - 1;
    /* from end of the string */
    while (name < result) {
      /* file name start */
      if ((*result == '\\') || (*result == '/')) {
        result++;
        break;
      }
      result--;
    }
  }
  return(result);
}

DWORD GetOfsByName(set_head *list, char *name, char itype) {
DWORD result, i;
  /* default palette #10 for images in both palettes */
  result = (!itype) ? 0x00001E7A : 0x00001E78;
  /* sanity check */
  if (list && name) {
    i = 0;
    /* SKELETON.SPR (PLANT.SPR with ball), RHINOKER.SPR, HEADLESS.SPR - bad old unused sprites?.. */
    /* FIRM_DIE, [MONS_FI-MONS_RO] ??? */
    /* PATCH: no palette - select one */
    if (list->offs & FLAG_FRM) {
      /* .FRM */
      if (!stricmp(name, "CAMP_EA")) {
        result = 0x0000279C; /* palette #13 */
        i = 1;
      }
    } else {
      /* .SPR */
      for (i = 0; i < OFFS_NUM; i++) {
        if (!stricmp(name, spr_offs[i].name)) {
          result = spr_offs[i].offs;
          break;
        }
      }
      /* found or not */
      i = (i < OFFS_NUM) ? 1 : 0;
    }
    /* palette not set - find it */
    if (!i) {
      /* for each item */
      for (i = 0; list[i].name[0]; i++) {
        /* found item */
        if (!strcmp(list[i].name, name)) {
          result = list[i].offs;
          break;
        }
      }
    }
  }
  return(result);
}

/* decode image routine */
void unsprfrm(BYTE *p, BYTE *u, DWORD ps, DWORD us) {
BYTE b;
  while (ps && us) {
    b = *p;
    p++; ps--;
    if (b <= 0xEF) {
      *u = b;
      u++; us--;
    } else {
      if (b == 0xF8) {
        if (ps) {
          b = *p;
          p++; ps--;
        }
      } else {
        b = -((char) b);
      }
      b = min(b, us);
      /* not sure about transparent color index, but 0xFF works just fine */
      memset(u, 0xFF, b);
      u += b; us -= b;
    }
  }
#ifdef DEBUG_UNPAK_INFO
  printf("\n\nPK=%u UN=%u\n", (int) ps, (int) us);
#endif
}

int main(int argc, char *argv[]) {
FILE *fl, *f;
tga_head th;
BYTE pal[256][3];
DWORD i, ps, us, sz;
BYTE *p, *u;
char s[32], *t;
set_head *list;
  printf("Seven Kingdoms II .SPR/.FRM to .TGA image extractor\n(c) CTPAX-X Team 2015\nhttp://www.CTPAX-X.org/\n\n");
  if ((argc < 2) || (argc > 3)) {
    printf(
      "Usage: unsprfrm <filename.ext> [palidx]\n"\
      "Where:\n"\
      "filename.ext - .SPR or .FRM file\n"\
      "palidx - palette index started from 0 (optional)\n"\
      "\n"
    );
    return(1);
  }
  /* select palette by file extension */
  us = 0;
  i = strlen(argv[1]);
  if (i >= 4) {
    t = &argv[1][i - 4];
    us |= (!stricmp(t, ".spr")) ? 1 : 0;
    us |= (!stricmp(t, ".frm")) ? 2 : 0;
  }
  if (!us) {
    printf("ERROR: only .SPR or .FRM files allowed.\n");
    return(2);
  }
  us--;
  /* load palette name->offs list */
  if (!us) {
    list = GetResPalOfs("STD.SET", "SPRITE", "NAME", "PAL_PTR", us);
  } else {
    list = GetResPalOfs("STD.SET", "FBUILD", "RES_NAME", "PAL_PTR", us);
  }
  /* error */
  if (!list) {
    printf("ERROR: can\'t parse database file \"STD.SET\".\n");
    return(3);
  }
  /* read colors (palette) */
  t = ((!us) ? "PAL_SPR.RES" : "PAL_FIRM.RES");
  fl = fopen(t, "rb");
  if (!fl) {
    printf("ERROR: can\'t open palette file \"%s\".\n", t);
    return(4);
  }
  /* base name for the input file */
  t = GetBaseFileName(argv[1]);
  /* seek to the palette */
  if (argc == 2) {
    /* temporary cut input file extension */
    argv[1][i - 4] = 0;
    ps = GetOfsByName(list, t, us);
    /* remove flags */
    fseek(fl, ps & (~(FLAG_16B | FLAG_FRM)), SEEK_SET);
    /* restore it back */
    argv[1][i - 4] = '.';
  } else {
    for (i = 0; i < StrToInt(argv[2]); i++) {
      us = 0;
      ps = 0;
      /* size1 */
      fread(&us, 4, 1, fl);
      /* size2 */
      fread(&ps, 4, 1, fl);
      /* error */
      if (ps != 0x0308) {
        fseek(fl, 0, SEEK_SET);
        break;
      }
      /* unknown */
      fread(&ps, 4, 1, fl);
      /* skip current palette */
      fseek(fl, 256*3, SEEK_CUR);
      /* go to next palette */
      fseek(fl, us - 8 - (256*3), SEEK_CUR);
    }
  }
  /* list not needed anymore */
  free(list);
#ifdef DEBUG_PAL_OFFS
  printf("%08X\n", (int) ftell(fl));
#endif
  /* init TARGA file header */
  memset(&th, 0, sizeof(th));
  th.ImageDescriptor = 0x20;
  if (!(ps & FLAG_16B)) {
    th.ColorMapType = 1;
    th.ImageType = 1;
    th.CMapLength = 256;
    th.CMapDepth = 24;
    th.PixelDepth = 8;
    /* skip palette header */
    fseek(fl, 4*3, SEEK_CUR);
    /* read palette */
    memset(pal, 0, 256*3);
    fread(pal, 3, 256, fl);
    /* swap palette */
    for (ps = 0; ps < 256; ps++) {
      pal[ps][0] ^= pal[ps][2];
      pal[ps][2] ^= pal[ps][0];
      pal[ps][0] ^= pal[ps][2];
    }
  } else {
    /* 16 BPP image - no palette */
    th.ImageType = 2;
    th.PixelDepth = 16;
    /* +3 - regular alpha data */
    th.ImageDescriptor |= 0x03;
  }
  fclose(fl);
  /* extract images */
  fl = fopen(argv[1], "rb");
  if (!fl) {
    printf("ERROR: can\'t open \"%s\".\n", argv[1]);
    return(5);
  }
  /* open and extract images */
  i = 0;
  fseek(fl, 0, SEEK_END);
  sz = ftell(fl);
  fseek(fl, 0, SEEK_SET);
  while (ftell(fl) < sz) {
    ps = 0;
    fread(&ps, 4, 1, fl);
    if (ps < 4) {
      break;
    }
    ps -= 4;
    i++;
    sprintf(s, "%s.%03d.tga", t, (int) i);
    printf("%s", s);
    fread(&th.Width, 2, 1, fl);
    fread(&th.Height, 2, 1, fl);
    /* palette images must be unpacked */
    if (th.ImageDescriptor == 0x20) {
      us = th.Width;
      us *= th.Height;
      u = (BYTE *) malloc(us);
      p = (BYTE *) malloc(ps);
      fread(p, 1, ps, fl);
      /* unpack */
      unsprfrm(p, u, ps, us);
      free(p);
    } else {
      us = ps;
      u = (BYTE *) malloc(us);
      fread(u, 1, us, fl);
    }
#ifndef DEBUG_SKIP_DUMP
    /* dump file to disk */
    f = fopen(s, "wb");
    fwrite(&th, sizeof(th), 1, f);
    /* add palette if needed */
    if (th.ImageDescriptor == 0x20) {
      fwrite(pal, 3, 256, f);
    }
    fwrite(u, 1, us, f);
    fclose(f);
#endif
    free(u);
    printf("\n");
#ifdef DEBUG_FIRST_ONLY
    printf("%08x\n", (int) ftell(fl));
    break;
#endif
  }
  printf("\ndone\n");
  return(0);
}
