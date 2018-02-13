//Credits go to OpenBOR community

#include "common.h"


typedef struct {
  u4 Size; //size of this entry
  u4 Off; // from start of file
  u4 Len;
  u1 Name[1];
} __attribute__ ((packed)) entry;

static void borDecompile(char *Output, char *Input) {  
  char Tmp[1024], Name[256], Ext[64];
  int L = fileSize(Input);
  u1 *M, *Q, *Z, *End;
  entry *E;

  M = readFile(0, L, Input);

  unless (!memcmp(M, "PACK", 4)) {
    printf("  Not a BOR PACK file.\n");
    abort();
  }
  End = (M+L-4);
  Q = M + *(u4*)End;

  while (Q < End) {
    E = (entry*)Q;
    Q += E->Size;

    printf("  Extracting: %s\n", E->Name);
    //printf("    Off=%08X Len=%08X\n", E->Off, E->Len);
    for(Z=E->Name; *Z; Z++) if (*Z == '\\') *Z = '/';

    sprintf(Tmp, "%s/%s", Output, E->Name);
    writeFile(0, E->Len, Tmp, M+E->Off);
  }
}

int borInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "bor";
  F->Description = "Beats of Rage archives (use on *.pak files)";
  F->Decompile = borDecompile;
  return 1;
}

