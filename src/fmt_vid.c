#include "common.h"


// Neither MM6, nor HOMM3 VID files include file sizes,
// so we have to calculate them from offsets and archive size
typedef struct {
  u1 Name[40];
  u4 Offset;
} __attribute__ ((packed)) VidEntry;

typedef struct {
  u4 Len;
  // VidEntry Entries[Len];
} __attribute__ ((packed)) VidHeader;


void vidDecompile (char *Output, char *Input) {
  int I;
  char Tmp[1024];
  u4 Length;
  u1 *In;
  VidEntry *E;
  VidHeader *H = readFile(0, sizeof(VidHeader), Input);
  unless (H || !H->Len || H->Len>0xfffe
       || H->Len*sizeof(VidEntry) > fileSize(Input)-4) {
    printf("Not a VID file: %s\n", Input);
    abort();
  }

  E = readFile(sizeof(VidHeader), H->Len*sizeof(VidEntry), Input);
  times (I, H->Len) {
    Length = (I+1!=H->Len ? E[I+1].Offset : fileSize(Input)) - E[I].Offset;
    printf("Extracting: %s\n", E[I].Name);
    //printf("  Off=#%x Len=#%x\n", E[I].Offset, Length);
    sprintf(Tmp, "%s/%s", Output, E[I].Name);
    u1 *In = readFile(E[I].Offset, Length, Input);
    writeFile(0, Length, Tmp, In);
    free(In);
  }
}

int vidInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "vid";
  F->Description = "VID archives (Might & Magic 6-8, Heroes of Might & Magic 3)";
  F->Decompile = vidDecompile;
  return 1;
}

