//Credits go to SNV

#include "common.h"

typedef struct {
  u1 U1; //0x8C
  u2 W;  //
  u2 H;
  u4 U2;
  u1 K;
  u1 NColors;
  //u1 Palette[NColors*3];
  //u1 Data[W*H]
} __attribute__ ((packed)) bar;


static int u4cmp (u4 *a, u4 *b) {
  return *a - *b;
}


static void saveImage(char *Output, char *Name, u1 *M, int L) {
  char Tmp[1024];
  int I, J, R,G,B,A;
  pic *P;
  bar *H = (bar*)M;
  u1 *Pal = (u1*)(H+1);
  u1 *Q = Pal+H->NColors*3;
  int X = H->W*H->H;
  int Y = L-sizeof(bar)-H->NColors*3;


  if (!H->W || !H->H) return;
  if (X < 0 || X > Y) return;

  printf("  %dx%d %d < %d\n", H->W,H->H, X, Y);

  P = picNew(H->W, H->H, 8);
  //P->K = 0;
  times (I, H->NColors) {
    P->P[I*4+0] = Pal[I*3+0];
    P->P[I*4+1] = Pal[I*3+1];
    P->P[I*4+2] = Pal[I*3+2];
  }
  memcpy(P->D, Q, H->W*H->H);
  sprintf(Tmp, "%s/%s.png", Output, Name);
  pngSave(Tmp, P);
}

static void bdxDecompile(char *Output, char *Input) {
  char Tmp[1024], Dir[1024], Name[128], Ext[32];
  int I, J, L, Off, Len;
  u4 NFiles = fileSize(Input)/4;
  u4 *Offs = (u4*)readFile(0, NFiles*4, Input);
  u1 *Q, *M;

  printf("NFiles=%d\n", NFiles);

  pathParts(Dir, Name, Ext, Input);
  sprintf(Tmp, "%s/%s.bar", Dir, Name);
  L = fileSize(Tmp);
  M = readFile(0, L, Tmp);

  qsort(Offs, NFiles, 4, u4cmp);

  times (I, NFiles) {
    Off = Offs[I];

    // we sub 4 to remove the size of the next file
    Len = (I+1 < NFiles ? Offs[I+1]-4 : L) - Off;

    //if (I != 647) continue;

    printf("%04d: Off=#%06X/#%06X Len=%d\n", I, Off, L, Len);

    Q = M+Off;

    sprintf(Tmp, "%04d", I);
    if (Len > 11) saveImage(Output, Tmp, Q, Len);

#if 0
    sprintf(Tmp, "%s/%04d", Output, I);
    writeFile(0, Len, Tmp, Q);
#endif
  }
}

int bdxInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "bdx";
  F->Description = "Chronicles of Inotia: Legend of Feanor Graphics";
  F->Decompile = bdxDecompile;
  return 1;
}


