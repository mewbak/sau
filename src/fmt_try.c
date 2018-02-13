//Credits go to SNV

#include "common.h"


static void tryDecompile(char *Output, char *Input) {
  char Tmp[1024], Name[256], Ext[32];
  int I, J, K, C, N,X,Y, W, H, NColors, R,G,B,A,Bits, NFrames;
  pic *P;
  int L = fileSize(Input);
  u1 *M = readFile(0, L, Input);
  u1 *Q;

  pathParts(0, Name, Ext, Input);
  downcase(Name);

  times (I,256) {
    Bits = 4;  
    W = 128+I;
    H = L*8/Bits/W;
    P = picNew(W,H,8);
    Q = M;//+64+32;
    if (Bits == 8) times(Y,H) times(X,W) picPut(P,X,Y,*Q++);
    else times(Y,H) times(X,W) picPut(P,X,Y,X&1?*Q++>>4:*Q&0xF);

    sprintf(Tmp, "%s/%s_%03d.png", Output, Name, W);
    pngSave(Tmp, P);
  }
}

int tryInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "try";
  F->Description = "Save Image as with varying width";
  F->Decompile = tryDecompile;
  return 1;
}


