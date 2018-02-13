//Credits go to SNV

#include "common.h"


typedef struct {
  u4 Off;
  u4 Len;
} __attribute__ ((packed)) entry;


static void libDecompile(char *Output, char *Input) {
  char Tmp[1024], Name[256], Ext[32];
  int I, J;
  int L = fileSize(Input);
  u1 *M = readFile(0, L, Input);
  u1 *Q;
  int NFiles = *(u4*)M;
  entry *E = (entry*)(M+4);


  times (I, NFiles) {
    sprintf(Name, "%04d_%08X", I, E->Off);
    printf("  Extracting: %s\n", Name);
    //printf("    %08X:%08X\n", E->Off, E->Len);

    Q = M+E->Off;
    Ext[0] = 0;
    if (!strncmp(Q, "FORM",4)) strcpy(Ext,".lbm");
    if (!strncmp(Q, "HLIB",4)) strcpy(Ext,".tlb");

    sprintf(Tmp, "%s/%s%s", Output, Name, Ext);
    writeFile(0, E->Len, Tmp, Q);
skip:
    E++;
  }
}


int libInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "lib";
  F->Description = "Cyberlore Library (Al-Qadim, Entomorph)";
  F->Decompile = libDecompile;
  return 1;
}

