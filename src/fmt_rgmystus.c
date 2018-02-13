//Credits go to SNV

#include "common.h"

typedef struct {
 u1 NameLen;
 u1 Name[12];
 u4 Off;
 u4 Len;
} __attribute__ ((packed)) entry;


static void rgmystusDecompile(char *Output, char *Input) {
  char Tmp[1024], Name[128];
  int I, J, K;
  u1 *Q, *M;
  int L = fileSize(Input);
  u2 *NFiles = readFile(L-2, 2, Input);
  entry *E = readFile(L-*NFiles*sizeof(entry)-2, *NFiles*sizeof(entry), Input);

  printf("  NFiles=%x\n", *NFiles);
  times (I, *NFiles) {
    memcpy(Name, E[I].Name, E[I].NameLen);
    Name[E[I].NameLen] = 0;
    printf("  Extracting: %s\n", Name);
    //printf("    %08x:%08x\n",  E[I].Off,  E[I].Len);
    M = readFile(E[I].Off,  E[I].Len, Input);
    sprintf(Tmp, "%s/%s", Output, Name);
    writeFile(0, E[I].Len, Tmp, M);
    free(M);
  }
}

int rgmystusInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "rgmystus";
  F->Description = "Mystic Towers (rgmystus.dat)";
  F->Decompile = rgmystusDecompile;
  return 1;
}

