#include "common.h"


static void rawSave(char *File, pic *P) {
  writeFile(0, P->W*P->H*P->B/8, File, P->D);
}


int rawInit(format *F) {
  F->Type = FMT_IMAGE;
  F->Name = "raw";
  F->Description = "Saves image as a raw array of pixels";
  F->Save = rawSave;
  return 1;
}

