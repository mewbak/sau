//Credits go to SNV

#include "common.h"

typedef struct {
  u1 Id[20];
  u4 NFiles;
} __attribute__ ((packed)) header;

typedef struct {
  u4 Off;
  u4 Len;
} __attribute__ ((packed)) entry;

typedef struct {
  u1 Id[100];
  u4 NFrames;
  u4 Unknown;
} __attribute__ ((packed)) spriteHeader;

typedef struct {
  u2 X; //X/Y offsets
  u2 Y;
  u4 Unknown1;
  u4 Unknown2;
} __attribute__ ((packed)) frame;

// TODO: use offsets to center sprite, before extracting
static void pakDecompile(char *Output, char *Input) {  
  int I;
  char Tmp[1024];
  u1 *Q;
  int L = fileSize(Input);
  spriteHeader *SH;
  u1 *M = readFile(0, L, Input);
  header *H = (header*)M;
  entry *E = (entry*)(M + sizeof(header));

  unless (!strncmp(H->Id, "<Pak", 4)) {
    printf("  Not a PAK file.\n");
    abort();
  }

  //printf("  NFiles=%d\n", H->NFiles);

  times (I, H->NFiles) {
    SH = (spriteHeader*)(M+E[I].Off);
    //printf("  %08x:%08x:%d\n", E[I].Off, E[I].Len, SH->NFrames);
    sprintf(Tmp, "%s/%04d.bmp", Output, I);
    writeFile(0, E[I].Len, Tmp, M+E[I].Off + sizeof(spriteHeader) + SH->NFrames*sizeof(frame));
    //sprintf(Tmp, "%s/%04d.bin", Output, I);
    //writeFile(0, E[I].Len, Tmp, M+E[I].Off);
  }
}

int pakInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "pak";
  F->Description = "Helbreath sprite archives";
  F->Decompile = pakDecompile;
  return 1;
}

