//Credits go to SNV

#include "common.h"

typedef struct {
  u1 Id[4];
  u4 U1;
  u4 U2;
  u4 U3;
  u4 FTOff;
  u4 NFiles;
} __attribute__ ((packed)) Header;

typedef struct {
  u4 U;
  u4 Off; // or number for first entry (for directories)
  u4 Len; // or number of files for directories
  u4 Type; // 0=File, 1=Folder; we set it to 666 for unpacked files
  char Name[16]; // zero-terminated filename
} __attribute__ ((packed)) Entry;

static void resDecompile (char *Output, char *Input) {  
  int I, SP=0;
  Entry *Stack[256];
  u1 *M;
  char Tmp[1024], Tmp2[1024];
  Entry *Es, *E, *Dir;
  Header *H = readFile(0, sizeof(Header), Input);

  if(strncmp(H->Id, "&YA1", 4)) {
    printf("Not a RES file: %s\n", Input);
    abort();
  }

  //printf("FTOff=#%08x NFiles=%d U1=%x U2=%x U3=%x\n"
  //      ,H->FTOff, H->NFiles, H->U1, H->U2, H->U3);

  Es = readFile(H->FTOff, H->NFiles*sizeof(Entry), Input);

  Dir = Stack[SP++] = n(Entry);
  Dir->Len = H->NFiles;
  Dir->Off = 0;
  *Dir->Name = 0;

  while (SP) {
    if (Dir->Len-- <= 0) {
      Dir = Stack[--SP - 1];
      continue;
    }
    E = Es + Dir->Off++;
    times (I,16) if(E->Name[I]=='\\') E->Name[I]='/';
    if (E->Type == 0) {
      if (SP == 1) continue;
      *Tmp2 = 0;
      times (I, SP) {
        strncat(Tmp2, Stack[I]->Name, 16);
        if(I) strcat(Tmp2, "/");
      }
      strcat(Tmp2, E->Name);
      sprintf(Tmp, "%s/%s", Output, Tmp2);
      printf("Extracting: %s\n", Tmp2);
      //printf("  #%04x: Type=%d Off=#%08x Len=%d U=#%x\n"
      //      ,Dir->Off-1, E->Type, E->Off, E->Len, E->U);
      M = readFile(E->Off, E->Len, Input);
      writeFile(0, E->Len, Tmp, M);
      free(M);
      E->Type = 666;
    } else if (E->Type == 1) {
      //printf("Dir:#%04x:%s: Type=%d Off=#%08x Len=%d U=#%x\n"
      //      ,Dir->Off-1, E->Name, E->Type, E->Off, E->Len, E->U);
      Dir = Stack[SP++] = E;
      E->Type = 666;
    } else {
      // skip
    }
  }

  times (I, H->NFiles) { // just in case, some files have no directory
    E = Es+I;
    if (E->Type != 0) continue;

    sprintf(Tmp, "%s/%s", Output, E->Name);
    printf("Extracting: %s\n", E->Name);
    //printf("  #%04x: Type=%d Off=#%08x Len=%d U=#%x\n"
    //      ,Dir->Off-1, E->Type, E->Off, E->Len, E->U);
    M = readFile(E->Off, E->Len, Input);
    writeFile(0, E->Len, Tmp, M);
    free(M);
  }
}

int resInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "res";
  F->Description = "Rage of Mages 1-2 (Allods) archives";
  F->Decompile = resDecompile;
  return 1;
}

