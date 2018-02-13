//Credits go to Steve Wilhite, author of GIF format and most of this source code

#include "common.h"

#define USE_COLORKEY 0x01
#define INTERLACED   0x40
#define HAS_PALETTE  0x80


#define GIF_EXT  0x21
#define GIF_IMG  0x2C
#define GIF_END  0x3B

#define EXT_TEXT 0x01
#define EXT_CTRL 0xF9
#define EXT_CMNT 0xFE
#define EXT_USER 0xFF

#define DISP_NONE       0
#define DISP_PRESERVE   1
#define DISP_CLEAR      2
#define DISP_RESTORE    3



typedef struct {
  u1 Id[3];
  u1 Version[3];
  u2 Width;
  u2 Height;
  u1 Info;
  u1 ColorKey;   // index of transparent color
  u1 Aspect;    // "pixel aspect ratio"
} __attribute__ ((packed)) header;

typedef struct {
  u1 Id[8]; // NETSCAPE
  u1 Version[3]; // 2.0
  u1 Len; // 3
  u1 Type; // 1=Loop
  u2 Count; // times to loop animation: 0=forever
  u1 Terminator;
} __attribute__ ((packed)) netscape;

typedef struct {
  u2  X; // X/Y displacement
  u2  Y;
  u2  Width;
  u2  Height;
  u1  Info;
} __attribute__ ((packed)) frameHeader;

typedef struct {
  u1  Info;
  u2  Delay; // animation delay, (in 1/100 of a second)
  u1  ColorKey;
  u1  Terminator;
} __attribute__ ((packed)) control;




#define MAX_CODES 4096

static int curr_size, clear, ending, newcodes, top_slot, slot,
           navail_bytes = 0, nbits_left = 0;
static u1  b1;
static u1  byte_buff[259];
static u1  *pbytes;
static u1  stack[MAX_CODES+1];
static u1  suffix[MAX_CODES+1];
static s2  prefix[MAX_CODES+1];

static u4 code_mask[13] = {
   0L,
   0x0001L, 0x0003L,
   0x0007L, 0x000FL,
   0x001FL, 0x003FL,
   0x007FL, 0x00FFL,
   0x01FFL, 0x03FFL,
   0x07FFL, 0x0FFFL
};

static int fillBuf(u1 **Q, u1 *E) {
  int  I;
  if (navail_bytes > 0) return 0;

  pbytes = byte_buff;
  if (*Q >= E) return 1; // abrupt end
  navail_bytes = *(*Q)++;

  times (I, navail_bytes) {
    if (*Q >= E) return 1; // abrupt end
    byte_buff[I] = *(*Q)++;
  }
  return 0;
}

static int getNextCode(u1 **Q, u1 *E) {
  u4  ret;
  if (!nbits_left) {
    if (fillBuf(Q, E)) return ending;
    b1 = *pbytes++;
    nbits_left = 8;
    navail_bytes--;
  }

  ret = b1 >> (8 - nbits_left);
  while (curr_size > nbits_left) {
    if (fillBuf(Q, E)) return ending;
    b1 = *pbytes++;
    ret |= b1 << nbits_left;
    nbits_left += 8;
    navail_bytes--;
  }
  nbits_left -= curr_size;
  return (ret & code_mask[curr_size]);
}


static u1 *lzwDecode(pic *P, u1 *Q, u1 *E) {
  u1  *sp = stack;
  u2 code=0, fc=0, oc=0, c=0, size=*Q++;
  int X=0, Y=0;

  if (size < 2 || 9 < size) {
    printf("Invalid LZW code size (%x)\n", size);
    abort();
  }

  curr_size = size + 1;
  top_slot = 1 << curr_size;
  clear = 1 << size;
  ending = clear + 1;
  slot = newcodes = ending + 1;
  navail_bytes = nbits_left = 0;
  sp = stack;

  while ((c = getNextCode(&Q, E)) != ending) {
    if (c == clear) {
      // LZW decoder wants us to clear back-ref buffers
      curr_size = size + 1;
      slot = newcodes;
      top_slot = 1 << curr_size;
      while ((c = getNextCode(&Q, E)) == clear);
      if (c == ending) break;
      if (c >= slot) c = 0;
      oc = fc = c;
      picPut(P, X++, Y, c);
      if (X == P->W) {
        X = 0;
        Y++;
      }
      continue;
    }
    code = c;
    if (code >= slot) {
      code = oc;
      if (sp >= stack + MAX_CODES) {
        printf("Code stack overflow\n");
        abort();
      }
      *sp++ = (u1)fc;
    }

    if (code >= MAX_CODES) {
      printf("Unexpected LZW code (%d)\n", code);
      abort(); 
    }

    while (code >= newcodes && sp < stack+MAX_CODES) {
      // this is a backref code
      *sp++ = suffix[code];
      code = prefix[code];
    }
            
    if (sp >= stack+MAX_CODES) {
       printf("Code stack overflow\n");
       abort();
    }

    *sp++ = code;
    if (slot < top_slot) {
      // save it for future backrefs
      fc = code;
      suffix[slot]   = fc;
      prefix[slot++] = oc;
      oc = c;
    }
    if (slot >= top_slot && curr_size < 12) {
      top_slot <<= 1;
      curr_size++;
    }

    while (sp > stack) {
      picPut(P, X++, Y, *--sp);
      if (X == P->W) {
        X = 0;
        Y++;
      }
    }
  }
  return Q;
}


static void deinterlace(pic *P) {
  int  I, J = 0;
  u1 *NewData = ns(u1, P->H*P->I);

  for (I = 0; I < P->H; I += 8, J++)
    memcpy(&NewData[I * P->I], &P->D[J * P->I], P->I);

  for (I = 4; I < P->H; I += 8, J++)
    memcpy(&NewData[I * P->I], &P->D[J * P->I], P->I);

  for (I = 2; I < P->H; I += 4, J++)
    memcpy(&NewData[I * P->I], &P->D[J * P->I], P->I);

  for (I = 1; I < P->H; I += 2, J++)
    memcpy(&NewData[I * P->I], &P->D[J * P->I], P->I);

  free(P->D);
  P->D = NewData;
}



spr *gifLoad(char *Input) {
  s4 Code, Label, Size;
  u1 Disposal;
  frameHeader *F, *OldImageDesc;
  control *Ctrl;
  spr *S;
  pic *P = 0;
  pic **Ps = ns(pic*, 1024*4);
  int NFrames = 0;
  int I, J, C;
  int L = fileSize(Input);
  u1 *M = readFile(0, L, Input);
  header *H = (header*)M;
  u1 *Q = M+sizeof(header);
  u1 *E = M+L;

  unless (!strncmp(H->Id, "GIF", 3)) {
    printf("  Not a GIF file\n");
    abort();
  }

  //printf("  %dx%d\n", H->Width, H->Height);

  S = sprNew();
  S->Palette = ns(u1,4*256);


  if (H->Info & HAS_PALETTE) {
    C = (1 << ((H->Info & 0x7) + 1));
    times (J, C) {
      S->Palette[J*4+0] = *Q++;
      S->Palette[J*4+1] = *Q++;
      S->Palette[J*4+2] = *Q++;
    }
  }


  Ctrl = 0;
  I = 0;
  while (Q < E) {
    // GIF supports simple delta-compression, using Disposal Method
    // to implement it, we first decode image,
    // then compose it with previous image skiping ColorKey indices
    Disposal = DISP_PRESERVE;
    while (Q < E && *Q == GIF_EXT) { // process extensions
      Q++;
      C = *Q++;

      switch (C) {
      case EXT_TEXT: break;
      case EXT_CTRL:
        Ctrl = (control*)Q;
        Disposal = (Ctrl->Info>>2)&0x7;
        break;
      case EXT_CMNT: break;
      case EXT_USER: break;
      }
      do { // data-chain is terminated by zero-length block
        C = *Q++;
        Q += C;
      } while (C);
    }

    if (Q < E && *Q == GIF_END) break;
    if (Q >= E || *Q != GIF_IMG) {
      printf("  Damaged GIF file\n");
      break;
    }
    Q++;
    F = (frameHeader*)Q;
    Q += sizeof(frameHeader);

    //printf("  %d: W=%d H=%d X=%d Y=%d\n"
    //      ,I, F->Width, F->Height, F->X, F->Y);

    P = Ps[NFrames++] = picNew(F->Width, F->Height, 8);

    if (F->Info & HAS_PALETTE) {
      C = (1 << ((F->Info & 0x7) + 1));
      times (J, C) {
        P->P[J*4+0] = *Q++;
        P->P[J*4+1] = *Q++;
        P->P[J*4+2] = *Q++;
      }
    } else {
      memcpy(P->P, S->Palette, 4*256);
    }

    Q = lzwDecode(P, Q, E)+1; // there should be 0-byte left by LZW
    if (F->Info&INTERLACED) deinterlace(P);
  }

  free(M);

  S->NAnis = 1;
  S->Anis = ns(ani, S->NAnis);
  S->Anis[0].NFacs = 1;
  S->Anis[0].Facs = ns(fac,S->Anis[0].NFacs);
  S->Anis[0].Facs[0].NPics = NFrames;
  S->Anis[0].Facs[0].Pics = Ps;

  return S;
}


#define TABLE_SIZE    (8*1024)
#define LARGEST_CODE  4095

static s2 code_size;
static s2 clear_code;
static s2 eof_code;
static s2 bit_offset;
static s2 max_code;
static s2 free_code;

static s2 prior_codes[TABLE_SIZE];
static s2 code_ids[TABLE_SIZE];
static u1 added_chars[TABLE_SIZE];

static FILE *O;

void initTable(s2 min_code_size) {
  code_size = min_code_size + 1;
  clear_code = 1 << min_code_size;
  eof_code = clear_code + 1;
  free_code = clear_code + 2;
  max_code = 1 << code_size;
  memset(code_ids, 0, TABLE_SIZE * sizeof(s2));
}

void flush(s2 N) {
  fwrite(&N, 1, 1, O);
  fwrite(byte_buff, 1, N, O);
}


static void writeCode(s2 code) {
  int temp;
  s2 byte_offset; 
  s2 bits_left;

  byte_offset = bit_offset >> 3;
  bits_left = bit_offset & 7;

  if(byte_offset >= 254) {
    flush(byte_offset);
    byte_buff[0] = byte_buff[byte_offset];
    bit_offset = bits_left;
    byte_offset = 0;
  }

  if(bits_left > 0) {
    temp = ((int)code << bits_left) | byte_buff[byte_offset];
    byte_buff[byte_offset] = (u1)temp;
    byte_buff[byte_offset + 1] = (u1)(temp >> 8);
    byte_buff[byte_offset + 2] = (u1)(temp >> 16);
  } else {
    byte_buff[byte_offset] = (u1)code;
    byte_buff[byte_offset + 1] = (u1)(code >> 8);
  }
  bit_offset += code_size;
}


static void lzwEncode(u1 *Q, u1 *E, int min_code_size) {
  s2 status;
  s2 prefix_code;
  s2 d;
  int hx;
  s2 suffix_char;

  if(min_code_size < 2 || min_code_size > 9) {
    if(min_code_size == 1) min_code_size = 2;
    else {
      printf("  Invalid min_code_size\n");
      abort();
    }
  }

  fputc(min_code_size, O);

  bit_offset = 0;
  initTable(min_code_size);
  writeCode(clear_code);
  suffix_char = *Q++;

  prefix_code = suffix_char;

  while(Q < E) {
    suffix_char = *Q++;
    hx = prefix_code ^ suffix_char << 5;
    d = 1;

    for (;;) {
      if(code_ids[hx] == 0) {
        writeCode(prefix_code);
        d = free_code;

        if(free_code <= LARGEST_CODE) {
          prior_codes[hx] = prefix_code;
          added_chars[hx] = (u1)suffix_char;
          code_ids[hx] = free_code;
          free_code++;
        }

        if(d == max_code) {
          if(code_size < 12) {
            code_size++;
            max_code <<= 1;
          } else {
            writeCode(clear_code);
            initTable(min_code_size);
          }
        }
        prefix_code = suffix_char;
        break;
      }
      if(prior_codes[hx] == prefix_code && added_chars[hx] == suffix_char) {
        prefix_code = code_ids[hx];
        break;
      }

      hx += d;
      d += 2;
      if(hx >= TABLE_SIZE) hx -= TABLE_SIZE;
    }
  }

  writeCode(prefix_code);
  writeCode(eof_code);

  // Make sure the code buffer is flushed
  if (bit_offset > 0) {
    int byte_offset = (bit_offset >> 3);
    if (byte_offset == 255) { // Make sure we don't write a zero by mistake. 
      int bits_left = bit_offset & 7;
      flush(255);
      if (bits_left) {
        byte_buff[0] = byte_buff[byte_offset];
        flush(1);
      }
    } else {
      flush((bit_offset + 7)/8);
    }
  }

  flush(0); // end-of-data

}

// note: early version of gifSave produced broken GIFs due to:
//   1. missing  __attribute__ ((packed)) for  netscape
//   2. error in encoder flushing code
void gifSave(char *Output, spr *S) {
  int I, J;
  pic *P;
  header H;
  frameHeader F;
  control Ctrl;
  netscape NS;
  char Tmp[1024], *Name;

  unless (S->Palette) {
    printf("  GIF cant hold true color images\n");
    abort();
  }

  if (!S->NAnis) {
    printf("  No frames to save\n");
    abort();
  }

  writeFile(0, 0, Output, Output); // creates path
  O = fopen(Output, "wb");

  memcpy(H.Id, "GIF", 3);
  memcpy(H.Version, "89a", 3);
  H.Width = S->Anis[0].Facs[0].Pics[0]->W;
  H.Height = S->Anis[0].Facs[0].Pics[0]->H;
  H.Info = HAS_PALETTE|0x07;
  H.ColorKey = 0;
  H.Aspect = 0;

  if (S->ColorKey != -1) {
    H.ColorKey = S->ColorKey;
    H.Info |= USE_COLORKEY;
  }

  fwrite(&H, 1, sizeof(header), O);

  times (I, 256) {
    fputc(S->Palette[I*4+0], O);
    fputc(S->Palette[I*4+1], O);
    fputc(S->Palette[I*4+2], O);
  }

  memcpy(NS.Id, "NETSCAPE", 8);
  memcpy(NS.Version, "2.0", 3);
  NS.Len = 3;
  NS.Type = 1;
  NS.Count = 0;
  NS.Terminator = 0;
  fputc(GIF_EXT, O);
  fputc(EXT_USER, O);
  fputc(11, O);
  fwrite(&NS, 1, sizeof(netscape), O);

  times (I, S->Anis[0].Facs[0].NPics) {
    P = S->Anis[0].Facs[0].Pics[I];
    unless (P) continue;

    Ctrl.Info = DISP_CLEAR<<2; // Restore to background color.
    Ctrl.Delay = 10;
    Ctrl.ColorKey = 0;
    Ctrl.Terminator = 0;
    if (P->K != -1) {
      Ctrl.Info |= USE_COLORKEY;
      Ctrl.ColorKey = P->K;
    }
    fputc(GIF_EXT, O);
    fputc(EXT_CTRL, O);
    fputc(4, O);
    fwrite(&Ctrl, 1, sizeof(control), O);

    F.X = 0;
    F.Y = 0;
    F.Width = P->W;
    F.Height = P->H;
    F.Info = 0;
    fputc(GIF_IMG, O);
    fwrite(&F, 1, sizeof(frameHeader), O);

    lzwEncode(P->D, P->D + P->W*P->H, 8);
  }

  fputc(GIF_END, O);

  fclose(O);
}

int gifInit(format *F) {
  F->Type = FMT_SPRITE;
  F->Name = "gif";
  F->Description = "Saves/Loads GIF files as sprites";
  F->Save = gifSave;
  F->Load = gifLoad;
  return 1;
}

