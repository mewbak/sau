#include "common.h"

static void hSave (char *Output, bytes *B) {
  int I;
  char *P, Tmp[1024];
  FILE *O = fopen(Output, "w");
  
  pathParts(0, Tmp, 0, Output);

  
  for (P = Tmp; *P; P++) if (*P == '.') *P = '_';
  fprintf(O, "u1 %s[] = {", Tmp);

  times (I, B->L) {
    if(I%8==0) fprintf(O, "\n");
    fprintf(O, " 0x%02X", B->D[I]);
    if(I+1 != B->L) fprintf(O, ",");
  }
  fprintf(O, "};\n", Tmp);
  fclose(O);
}

int hInit(format *F) {
  F->Type = FMT_BYTES;
  F->Name = "h";
  F->Description = "Convert <bytes> to a C/C++ header";
  F->Save = hSave;
  return 1;
}

