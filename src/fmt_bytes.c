#include "common.h"


static void bytesSave (char *Output, bytes *B) {
  writeFile(0, B->L, Output, B->D);
}

static bytes *bytesLoad (char *Input) {
  bytes *B = n(bytes);
  B->L = fileSize(Input);
  B->D = readFile(0, B->L, Input);
  return B;
}

int bytesInit(format *F) {
  F->Type = FMT_BYTES;
  F->Name = "bytes";
  F->Description = "treat input/output as a plain stream of bytes";
  F->Save = bytesSave;
  F->Load = bytesLoad;
  return 1;
}

