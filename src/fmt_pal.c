#include "common.h"


static void palSave(char *File, pic *P) {
  writeFile(0, 256*4, File, P->P);
}

static pic *palLoad(char *File) {
  int I, L = fileSize(File);
  pic *P = picNew(16, 16, 8);
  u1 *Pal = readFile(0, L, File);
  times (I, 256) P->D[I] = I;
  if (L == 256*4) memcpy(P->P, Pal, 256);
  else times (I, L/3) { // treat it as 3-planar
    P->P[I*4+0] = Pal[I*3+0];
    P->P[I*4+1] = Pal[I*3+1];
    P->P[I*4+2] = Pal[I*3+2];
    P->P[I*4+3] = 0;
  }
  return P;
}

int palInit(format *F) {
  F->Type = FMT_IMAGE;
  F->Name = "pal";
  F->Description = "Loads/saves raw palettes";
  F->Save = palSave;
  F->Load = palLoad;
  return 1;
}

