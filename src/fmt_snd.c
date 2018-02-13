//Credits go to SNV

#include "common.h"

typedef struct {
  u4 Len;
  // SndEntry Entries[Len];
} __attribute__ ((packed)) SndHeader;

// Somehow LZ manages to compress PCM files!
typedef struct {
  u1 Name[40];
  u4 Off;
  u4 Len;  // compressed length
  u4 ULen; // uncompressed length
} __attribute__ ((packed)) SndEntry;


// HOMM3 wav files are uncompressed
typedef struct {
  u1 Name[40];
  u4 Off;
  u4 Len; // uncompressed length
} __attribute__ ((packed)) H3SndEntry;


static void sndDecompile (char *Output, char *Input) {
  int I;
  char Tmp[1024];
  u1 *In, *Out;
  SndEntry *E;

  SndHeader *H = readFile(0, sizeof(SndHeader), Input);

  unless (H || !H->Len || H->Len>0xfffe
       || H->Len*sizeof(SndEntry) > fileSize(Input)-4) {
    printf("Not a SND file: %s\n", Input);
    abort();
  }

  pathParts(0, Tmp, 0, Input);
  downcase(Tmp);
  if (!strcmp(Tmp, "heroes3") || !strcmp(Tmp, "h3ab_ahd")) {
    H3SndEntry *E = readFile(sizeof(SndHeader)
                            ,H->Len*sizeof(H3SndEntry), Input);
    times (I, H->Len) {
      printf("Extracting: %s\n", E[I].Name);
      In =  readFile(E[I].Off, E[I].Len, Input);
      sprintf(Tmp, "%s/%s.wav", Output, E[I].Name);
      writeFile(0, E[I].Len, Tmp, In);
      free(In);
    }
    return;
  }

  E = readFile(sizeof(SndHeader), H->Len*sizeof(SndEntry), Input);
  times (I, H->Len) {
    //printf("O=%08x L=%08x U=%08x: %s\n"
    //      ,E[I].Off, E[I].Len, E[I].ULen, E[I].Name);
    printf("Extracting: %s\n", E[I].Name);
    In =  readFile(E[I].Off, E[I].Len, Input);

    sprintf(Tmp, "%s/%s.wav", Output, E[I].Name);
    if (E[I].ULen == E[I].Len) { // uncompressed
      writeFile(0, E[I].Len, Tmp, In);
    } else {
      Out = inflateBufIE(E[I].ULen, E[I].Len, In, 1);
      if (Out) {
        writeFile(0, E[I].ULen, Tmp, Out);
        free(Out);
      } else {
        // I tested it on a pirated version of MM7 and 02Flame01 was damaged.
        // these pirates love to repack audio with OGG
        printf("  Damaged file. Skipping.\n");
      }
    }
    free(In);
  }
}

int sndInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "snd";
  F->Description = "SND archives (Might & Magic 6-8, Heroes of Might & Magic 3)";
  F->Decompile = sndDecompile;
  return 1;
}

