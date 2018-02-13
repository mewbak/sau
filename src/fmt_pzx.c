//Credits go to SNV

#include "common.h"

typedef struct {
  u1 Id[4]; //PZX\1
  u4 O[3]; //offsets
} __attribute__ ((packed)) header;


static void pzxDecompile(char *Output, char *Input) {
  char Tmp[1024], Name[256];
  int I, J, K, C, Off, Len, ULen, NFiles, L = fileSize(Input);
  u1 *Q, *Z;
  u1 *M = readFile(0, L, Input);
  header *H = (header*)M;

  pathParts(0, Name, 0, Input);

  if(memcmp(H->Id,"PZX", 3)) {
    printf("  not a PZX file\n");
    abort();
  }

  Q = (u1*)(H+1);
  while(*(u2*)Q != 0x9C78) Q++; // scan for zip header
  Q -= 8;
  ULen = ru4(Q);
  Len = ru4(Q);
  Z = inflateBuf(ULen, Len, Q);
  sprintf(Tmp, "%s/%s_a", Output, Name);
  writeFile(0, ULen, Tmp, Z);
}

int pzxInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "pzx";
  F->Description = "Zenonia sprite";
  F->Decompile = pzxDecompile;
  return 1;
}

