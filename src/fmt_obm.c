//Credits go to SNV

#include "common.h"


typedef struct {
  u1 Id[3]; // BMP
  u1 Unknown; // 0x10=16bit
  u1 Unknown2[64];
  u4 Width;
  u4 Height;
  //u2 Pixels[Width*Height];
} __attribute__ ((packed)) header;



static pic *obmLoad(char *Input) {
  int X, Y;
  pic *P;
  int L = fileSize(Input);
  u1 *M = readFile(0, L, Input);
  header *H = (header*)M;
  u1 *Q = M+sizeof(header);

  unless (!memcmp(H->Id, "BMP", 3)) {
    printf("  Not an OBM file\n");
    abort();
  }

  //printf("  %dx%d\n", H->Width, H->Height);
  P = picNew(H->Width, H->Height, 32);
  times (Y,P->H)
    times (X,P->W) {
      int R, G, B;
      fromR5G6B5(R,G,B,*(u2*)Q);
      picPut32(P, P->W-X-1, Y, R8G8B8(R,G,B));
      Q += 2;
    }

  return P;
}

int obmInit(format *F) {
  F->Type = FMT_IMAGE;
  F->Name = "obm";
  F->Description = "Myth of Soma images";
  F->Load = obmLoad;
  return 1;
}

