//Credits go to SNV

#include "common.h"

typedef struct {
  u2 NFiles;
} __attribute__ ((packed)) Header;

typedef struct {
  char Name[14]; // zero-terminated filename
  u4 Off;
  u4 U1; // unknown
} __attribute__ ((packed)) Entry;

static void groupDecompile (char *Output, char *Input) {  
  int I, Len;
  u1 *M;
  char Tmp[1024];
  Header *H = readFile(0, sizeof(Header), Input);
  Entry *Es = readFile(sizeof(Header), H->NFiles*sizeof(Entry), Input);

  times (I, H->NFiles-1) { // last entry is a sentinel, holding EOF offset
    sprintf(Tmp, "%s/%s", Output, Es[I].Name);
    printf("Extracting: %s\n", Es[I].Name);
    Len = Es[I+1].Off-Es[I].Off;
    //printf("  Off=#%08x Len=#%08d\n", Es[I].Off, Len);
    M = readFile(Es[I].Off, Len, Input);
    writeFile(0, Len, Tmp, M);
    free(M);
  }
}

int groupInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "group";
  F->Description = "Stonekeep groupXX archives";
  F->Decompile = groupDecompile;
  return 1;
}

