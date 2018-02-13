//Credits go to SNV
//              russian modding retards, for keeping format specs closed

#include <time.h>
#include "zlib/zlib.h"
#include "common.h"


typedef struct {
  u1 Id[4];
  u4 Off; // file table offset
} __attribute__ ((packed)) header;


typedef struct {
  u1 U1;
  u1 U2;
  u1 U3;
  u1 Bits;
  u1 Chns;
  u4 Freq;
  u4 Size; // size of Data
  u2 U4;
  //u1 Data[Size];
} __attribute__ ((packed)) h4sHeader;


typedef struct {
  u4 Unknown;
  u4 High;
  u4 Wide;
  u4 Size;
  //u1 Data[Size];
} __attribute__ ((packed)) rawHeader;

struct {
  char *Head;
  char *Ext;
} Exts[] = {
  {"actor_sequence", ".df2"},
  {"adv_actor", ".act"},
  {"adv_object", ".df2"},
  {"animation", ".df2"},
  {"battlefield_preset_map", ".bpm"},
  {"bitmap_raw", ".raw"},
  {"bink", ".bik"},
  {"castle", ".cst"},
  {"combat_actor", ".act"},
  {"combat_header_table_cache", ".cht"},
  {"combat_object", ".df2"},
  {"font", ".df2"},
  {"game_maps", ".h4c"},
  {"layers", ".df2"},
  {"sound", ".h4s"},
  {"strings", ".tbl"},
  {"table", ".tbl"},
  {"terrain", ".ter"},
  {"transition", ".trn"},
  {0,0}};

static char *inferExt (char *Name, char *H4R) {
  char *P;
  int I;
  char Head[512];

  if (!strcmp("sound/dialogue/new_turn", Name)) return ".h4s";
  if (!strcmp("sound/main_menu", Name)) return ".mp3";

  if (!strcmp(H4R, "music")
   || !strncmp("sound/dialogue/", Name, 15)
   || !strncmp("sound/voice_over/", Name, 17))
    return ".mp3";

  strcpy(Head, Name);
  P = strchr(Head, '/');
  if (P) *P = 0;
  for(I=0; Exts[I].Head; I++)
    if(!strcmp(Head, Exts[I].Head)) return Exts[I].Ext;
  return "";
}

static void ungz(char *Output, char *Input) {
  int S;
  u1 Buf[4096];
  gzFile In = gzopen(Input, "rb");
  FILE *Out = fopen(Output, "wb");
  do {
    S = gzread(In, Buf, 4096);
    fwrite(Buf, 1, S, Out);
  } while(S > 0);
  gzclose(In);
  fclose(Out);
}

static u1 *readString(char *Dst, u1 *Src) {
  int L = *(u2*)Src;
  memcpy(Dst, Src+2, L);
  Dst[L] = 0;
  return Src+L+2;
}



static void h4rDecompile(char *Output, char *Input) {
  FILE *Txt;
  pic *P;
  char Tmp[1024], Tmp2[1024], Name[512], Link[512], Comment[512], *Ext, H4RName[64];
  int Off, Len, ULen, Compression, Wide, High;
  time_t Date;
  int I, J, C, X, Y, L = fileSize(Input), FTLen=1024*1024*4, NFiles;
  u1 *Q, *M, *Z, *FT;
  header *H = readFile(0, sizeof(header), Input);

  if (memcmp(H->Id, "H4R", 3)) {
    printf("  Not a H4R file.");
    abort();
  }

  pathParts(0, H4RName, 0, Input);
  downcase(H4RName);

  if (FTLen > L-H->Off) FTLen = L-H->Off;

  FT = readFile(H->Off, FTLen, Input);

  NFiles = *(u4*)FT;
  printf("  NFiles=%d\n", NFiles);

  Q = FT+4;
  times (I, NFiles) {
    Off = *(u4*)Q; Q+=4;
    Len = *(u4*)Q; Q+=4;
    ULen = *(u4*)Q; Q+=4;
    Date = *(u4*)Q; Q+=4;

    Q = readString(Name, Q);
    Q = readString(Comment, Q);
    Q = readString(Link, Q); // lame developers reused some content
    Compression = *(u4*)Q; Q+=4;

    downcase(Name);
    downcase(Link);

    pathParts(0, Tmp, 0, Name);
    strcpy(Name, Tmp);
    for(Z=Name; *Z; Z++) if (*Z == '.') *Z = '/';
    Ext = inferExt(Name, H4RName);

    //unless (!strcmp(Ext, ".h4c")) continue;

    printf("  Extracting: %s\n", Name, Ext);

#if 0
    strftime(Tmp, 1024, "%H:%M:%S, %a %b %d %Y", localtime(&Date));
    printf("  Date: %s\n", Tmp);
    if (Link[0]) printf("  Link: %s\n", Link);
    if (Comment[0]) printf("  Comment: %s\n", Comment);
    printf("  I=%04d Off=#%08X Len=%d:%d C=%d\n", I, Off, Len, ULen, Compression);
    printf("\n");
#endif

    if (Link[0]) goto skip_conversion; // windows doesnt support symlinks

    M = readFile(Off, Len, Input);
    if (Compression == 3) sprintf(Tmp, "%s/%s%s.gz", Output, Name, Ext);
    else sprintf(Tmp, "%s/%s%s", Output, Name, Ext);
    writeFile(0, Len, Tmp, M);
    free(M);

    if (Compression == 3) {
      sprintf(Tmp2, "%s/%s%s", Output, Name, Ext);
      ungz(Tmp2, Tmp);
      unlink(Tmp);
      strcpy(Tmp, Tmp2);
    }

    if (!strcmp(Ext, ".raw")) {
      rawHeader *RAW = readFile(0, ULen, Tmp);
      P = picNew(RAW->Wide, RAW->High, 24);
      M = (u1*)(RAW+1);
      times (Y, RAW->High) times (X, RAW->Wide) {
        picPut24(P, X, Y, R8G8B8(M[2],M[1],M[0]));
        M += 3;
      }
      sprintf(Tmp2, "%s/%s%s", Output, Name, ".png");
      pngSave(Tmp2, P);
      unlink(Tmp);
      free(RAW);
    } else if(!strcmp(Ext, ".h4s")) {
      h4sHeader *H4S = readFile(0, ULen, Tmp);
      //printf("  Audio: Freq=%x Chns=%d Bits=%d\n", H4S->Freq, H4S->Chns, H4S->Bits);
      sprintf(Tmp2, "%s/%s%s", Output, Name, ".wav");
      wavSave(Tmp2, (u1*)(H4S+1), H4S->Size, H4S->Bits, H4S->Chns, H4S->Freq);
      unlink(Tmp);
      free(H4S);
    } else if(!strcmp(Ext, ".tbl")) {
       // dump tables to plain text, with tab separated fields
       // NOTE: we replace 0x0A with 0x0D, so it can be readed back loselessly
       sprintf(Tmp2, "%s/%s%s", Output, Name, ".txt");
       writeFile(0, 0, Tmp2, Tmp2);
       Txt = fopen(Tmp2, "wb");
       M = Z = readFile(0, ULen, Tmp);
       Y = *(u4*)Z; Z+=4;
       while (Y--) {
         X = *(u2*)Z; Z+=2;
         while (X--) {
           C = *(u2*)Z; Z+=2;
           times (J, C) {
              if (Z[J]==0x92) Z[J]='\'';
              //if (Z[J]==0x85) Z[J]='\''; // '...' symbol
              if (Z[J]==0x0A) Z[J]=0x0D;
              if (Z[J]=='\t') Z[J]=' ';
           }
           fwrite(Z, 1, C, Txt); Z+=C;
           fputc(X?'\t':'\n', Txt);
         }
       }
       unlink(Tmp);
       free(M);
       fclose(Txt);
    } else if(!strcmp(Ext, ".h4c")) {
      // map files have some junk at the start
      M = readFile(0, ULen, Tmp);
      unlink(Tmp);
      writeFile(0, ULen-16, Tmp, M+16);
      free(M);
    }
skip_conversion: ;
  }
}

int h4rInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "h4r";
  F->Description = "Heroes of Might & Magic 4 archives";
  F->Decompile = h4rDecompile;
  return 1;
}

