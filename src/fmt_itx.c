//Credits go to SNV

#include "common.h"

static void itxDecompile(char *Output, char *Input) {
	char Tmp[1024], Name[128], ArchiveDir[1024];
  int X, Y;
  u4 C;
  int L = fileSize(Input);
  u1 *M = readFile(0, L, Input);
  pic *P;
  u1 *Q, *Z, *End=M+L;
  int CX = s2be(M);
  int CY = s2be(M+2);
  int W = s2be(M+4);
  int H = s2be(M+6);

  pathParts(ArchiveDir, Name, 0, Input);

  //fprintf(stderr, "%dx%d+%d,%d %d=%d\n", W, H, CX ,CY, W*H*4, L-8);

  P = picNew(W, H, 32);

  Q = M + 8;

  times(Y, H) times(X, W) {
    C = R8G8B8A8(Q[0],Q[1],Q[2],Q[3]);
    picPut32(P, X, Y, C);
    Q+=4;
  }

  sprintf(Tmp, "%s/%s+%d+%d.png", Output, Name, CX, CY);
  pngSave(Tmp, P);
  picDel(P);
}

int itxInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "itx";
  F->Description = "Illarion (http://illarion.org/)";
  F->Decompile = itxDecompile;
  return 1;
}


