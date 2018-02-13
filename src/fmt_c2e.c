//Credits go to SNV

#include "common.h"

typedef struct {
  u1 U1; //0x0C
  u1 W;  //
  u1 H;
  s1 X;
  s1 Y;
  u1 K; //0x01
  u1 NColors;
  //u1 Palette[NColors*3];
  //u1 Data[W*H]
} __attribute__ ((packed)) frame;


static void c2eDecompile(char *Output, char *Input) {
  char Tmp[1024], Name[256];
  pic *P;
  frame *F;
  int I, J, Off, Len, X,Y, CX=1, CY=1, W, H, FPL=4;
  int L = fileSize(Input);
  u1 *Q;
  u1 *M = readFile(0, L, Input);
  u4 *Offs = (u4*)(M+0x24);
  u4 *Lens = (u4*)(M+0xCA4);
  int NFrames = *(u4*)(M+0x1928);

  pathParts(0, Name, 0, Input);

  times (I, NFrames) {
    Off = Offs[I];
    Len = Lens[I];
    F = (frame*)(M+Off);
    //printf("%04d: Off=#%06X/#%06X Len=%d\n", I, Off, L, Len);
    CX = max(CX,abs(F->X)+(F->W+1)/2);
    CY = max(CY,abs(F->Y)+(F->H+1)/2);
  }

  W = CX*2;
  H = CY*2;

  times (I, NFrames) {
    Off = Offs[I];
    Len = Lens[I];
    F = (frame*)(M+Off);
    Q = (u1*)(F+1);

    P = picNew(W, H, 8);
    times (J, F->NColors) {
      P->P[J*4+0] = *Q++;
      P->P[J*4+1] = *Q++;
      P->P[J*4+2] = *Q++;
    }

    times (Y,F->H) times (X, F->W)
      picPut(P
            ,(P->W-F->W+1)/2 - F->X+X
            ,(P->H-F->H+1)/2 - F->Y+Y
            ,Q[Y*F->W+X]);

    sprintf(Tmp, "%s/%03d.png", Output, I);
    pngSave(Tmp, P);
  }
}

int c2eInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "c2e";
  F->Description = "Chronicles of Inotia 2: Wanderer of Luone Sprites";
  F->Decompile = c2eDecompile;
  return 1;
}


