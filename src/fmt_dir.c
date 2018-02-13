// Magic & Mayhem .SPR format

#include "common.h"

void moveFiles(char *Dst, fileList *FL) {
}

int dirInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "dir";
  F->Description = "Specifies directory as input/output";
  F->Compile = moveFiles;
  F->Decompile = getFileList;
  return 1;
}

