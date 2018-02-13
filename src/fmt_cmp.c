//Credits go to SNV

#include "common.h"

//Guess they encrypted names to prevent pirates from localization
static u1 CmpKey[] = {0xF3,0xBF,0xAA,0xF3,0xD2,0xBE,0x9D,0xD2
                     ,0xFC,0xF9,0xB7,0xE1,0xFA,0xB7,0xEB,0xFA
                     ,0xCF,0xCD,0xCC,0xCB,0x9C,0x9C,0xB9,0xCC
                     ,0xE7,0xF6,0xD7,0xF0,0xF9,0xDD,0xC6,0xE8};

static void cmpDecompile(char *Output, char *Input) {
  char Tmp[1024], Name[256];
  u1 Hash[256], Key[256], ArName[128]; 
  int I, J, K, C, Off, Len, NFiles, L;
  u1 *M, *Q, *Z;

  pathParts(0, ArName, 0, Input);

  downcase(ArName);

  if (!strcmp(ArName, "magic")
   || !strcmp(ArName, "treasure")
   || !strcmp(ArName, "status")
   || !strcmp(ArName, "itemgen")
   || !strcmp(ArName, "mono")
   || !strcmp(ArName, "text")) {
    printf("  not an archive CMP\n");
    abort();
  }

  Q = M = readFile(0, 1<<20, Input);
  NFiles = ru4(Q);

  //printf("  NFiles=%d\n", NFiles);
  times (K, NFiles) {
    C = ru4(Q);
    
    times(I, C) {
      Name[I] = *Q++ ^ CmpKey[I%32];
      if (Name[I]=='\\') Name[I] = '/';
    }
    Name[C] = 0;
    Q++; //skip zero byte
    Off = ru4(Q);
    Len = ru4(Q);
    ru4(Q); //skip unknown

    printf("  Extracting: %s\n", Name);
    //printf("    Off=%08x Len=%08x\n", Off, Len);

    Z = readFile(Off, Len, Input);
    sprintf(Tmp, "%s/%s", Output, Name);
    writeFile(0, Len, Tmp, Z);
    free(Z);
  }
}

int cmpInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "cmp";
  F->Description = "Divine Divinity Archive";
  F->Decompile = cmpDecompile;
  return 1;
}

