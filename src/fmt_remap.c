#include "common.h"


static void remapSave(char *File, pic *A) {
  int I, X, Y, U, V;
  char Tmp[1024], Dir[1024], Name[256], Ext[64];
  pic *B = pcxLoad(File);
  pathParts(Dir, Name, Ext, File);
  if (A->W != B->W || A->H != B->H || A->B != B->B || A->B!=8) {
    printf("Images must have the same width, height and be 8-bit\n");
    abort();
  }
  times(Y, A->H) {
    times (X, A->W) {
      U = A->D[Y*A->I + X];
      V = B->D[Y*B->I + X];
      B->P[V*4+0] = A->P[U*4+0];
      B->P[V*4+1] = A->P[U*4+1];
      B->P[V*4+2] = A->P[U*4+2];
      B->P[V*4+3] = A->P[U*4+3];
    }
  }
  sprintf(Tmp, "%s/%s_remap.pcx", Dir, Name);
  printf("%s\n", Tmp);
  pcxSave(Tmp, B);
}


int remapInit(format *F) {
  F->Type = FMT_IMAGE;
  F->Name = "remap";
  F->Description = "Remaps target image to palette of source. Both must be in PCX";
  F->Save = remapSave;
  return 1;
}

