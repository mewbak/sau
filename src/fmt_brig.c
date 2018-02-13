//Credits go to SNV

#include "common.h"


static void brigDecompile(char *Output, char *Input) {
  char Tmp[1024], Name[256], Ext[32];
  int I, J, K, C, N,X,Y, W, H, NColors, R,G,B,A,Bits, NFrames;
  pic *P;
  int L = fileSize(Input);
  u1 *M = readFile(0, L, Input);
  u1 *Q;

  pathParts(0, Name, Ext, Input);
  downcase(Name);

  times (I,149) {
    //if (I>15 && I<133) continue;
    Bits = 4;  
    W = 256;
    H = L*8/Bits/W;
    P = picNew(W,H,8);
    Q = M;//+64+32;
    if (Bits == 8) times(Y,H) times(X,W) picPut(P,X,Y,*Q++);
    else times(Y,H) times(X,W) picPut(P,X,Y,X&1?*Q++>>4:*Q&0xF);

    Q = M+32*I;
    times(J,16) {
      fromA1R5G5B5(A,B,G,R,*(u2*)Q);
      Q+=2;
      P->P[J*4+0] = R;
      P->P[J*4+1] = G;
      P->P[J*4+2] = B;
      P->P[J*4+3] = A*0xFF;
    }
    sprintf(Tmp, "%s/%s_%03d.png", Output, Name, I);
    pngSave(Tmp, P);
  }
}

int brigInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "brig";
  F->Description = "Brigandine Grand Edition (PSX Game)";
  F->Decompile = brigDecompile;
  return 1;
}


