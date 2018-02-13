#include "common.h"
#include "inits.h"

#define SAU_VERSION "1.1"
char *Input, *Output;
char ProgDir[1024];
char ProgName[128];

int NFormats;
format Formats[MAX_FORMATS];

static void extractExtension(char *Ext, char *Path) {
  pathParts(0, 0, Ext, Path);
  downcase(Ext);
}

int main(int ArgC, char **ArgV) {
  spr *S;
  pic *P;
  bytes *Bs;
  fileList *FL;
  char *Q;
  int I, J, K;
  u1 *H;
  char iFmt[128], oFmt[128];
  char Tmp[1024];

  srand(time(0));

  pathParts(ProgDir, ProgName, 0, ArgV[0]);
  unless (*ProgDir) strcpy(ProgDir, ".");

  for (NFormats = 0; Inits[NFormats]; NFormats++) {
    unless(((int(*)(format*))Inits[NFormats])(&Formats[NFormats])) {
      printf("Failed to init %s handler\n", Formats[NFormats].Name);
      NFormats--;
    }
  }

  unless (ArgC == 3) {
usage:
    printf("Sprite and Archive Utility v%s by Nikita Sadkov\n"
           "\n"
           "Usage: %s <InputFormat>=<InputFile> <OutputFormat>=<OutputFile>\n"
           "       %s <InputFile> <OutputFile> (guess format from extension)\n"
           "Example: %s dir=./Input mpq=Output.mpq\n"
           "         %s ./Input Output.mpq\n"
           "\n"
           "Formats Supported:\n"
          ,SAU_VERSION, ProgName, ProgName, ProgName, ProgName);

    times (I, NFormats) {
      printf("  %s: %s\n", Formats[I].Name, Formats[I].Description);
    }

    exit(-1);
  }
  Input = strchr(ArgV[1], '=');
  if (Input) {
    I = Input-ArgV[1];
    memcpy(iFmt, ArgV[1], I);
    iFmt[I] = 0;
    Input++;
  } else {
    Input = ArgV[1];
    extractExtension(iFmt, Input);
    if (!*iFmt || folderP(Input)) strcpy(iFmt, "dir");
  }

  Output = strchr(ArgV[2], '=');
  if (Output) {
    I = Output-ArgV[2];
    memcpy(oFmt, ArgV[2], I);
    oFmt[I] = 0;
    Output++;
  } else {
    Output = ArgV[2];
    extractExtension(oFmt, Output);
    if (!*oFmt || folderP(Output)) strcpy(oFmt, "dir");
  }

  unless(fileExist(Input)) {
    printf("Cant find %s\n", Input);
    abort();
  }

  unless (Output) goto usage;


  printf("Processing: %s\n", Input);
again:
  times (I, NFormats) {
    format *A = Formats+I;
    if (strcmp(A->Name, iFmt)) continue;
    times (J, NFormats) {
      format *B = Formats+J;
      if (strcmp(B->Name, oFmt)) continue;

      if (!strcmp(A->Name, "dir") && B->Type == FMT_SPRITE) {
        strcpy(iFmt, "frames");
        goto again;
      }

      if (A->Type == FMT_SPRITE && !strcmp(B->Name, "dir")) {
        strcpy(oFmt, "frames");
        goto again;
      }

      if (!strcmp(A->Name, "dir") && B->Type == FMT_ARCHIVE) {
        unless (B->Compile) {
          printf("Cant compile <%s>s\n", B->Name);
          abort();
        }
        FL = getFileList(Input);
        ((void (*)(char*,fileList*))B->Compile)(Output, FL);
        freeFileList(FL);
        goto succeed;
      }

      if (!strcmp(B->Name, "dir") && A->Type == FMT_ARCHIVE) {
        unless (A->Decompile) {
          printf("Cant compile <%s>s\n", B->Name);
          abort();
        }
        ((void (*)(char*,char*))A->Decompile)(Output, Input);
        goto succeed;
      }

      if (A->Type == FMT_ARCHIVE || A->Type != B->Type) {
        printf("Cant convert <%s> to <%s>\n", iFmt, oFmt);
        abort();
      }

      if (A->Type == FMT_BYTES) {
        unless (A->Load) {
          printf("Cant load <%s>s\n", A->Name);
          abort();
        }
        unless (B->Save) {
          printf("Cant save <%s>s\n", B->Name);
          abort();
        }

        Bs = ((bytes *(*)(char*))A->Load)(Input);
        ((void (*)(char*,bytes*))B->Save)(Output, Bs);
        bytesDel(Bs);
      }

      if (A->Type == FMT_SPRITE) {
        unless (A->Load) {
          printf("Cant load <%s>s\n", A->Name);
          abort();
        }
        unless (B->Save) {
          printf("Cant save <%s>s\n", B->Name);
          abort();
        }

        S = ((spr *(*)(char*))A->Load)(Input);
        ((void (*)(char*,spr*))B->Save)(Output, S);
        sprDel(S);
      }

      if (A->Type == FMT_IMAGE) {
        unless (A->Load) {
          printf("Cant load <%s>s\n", A->Name);
          abort();
        }
        unless (B->Save) {
          printf("Cant save <%s>s\n", B->Name);
          abort();
        }

        P = ((pic *(*)(char*))A->Load)(Input);
        ((void (*)(char*,pic*))B->Save)(Output, P);
        picDel(P);
      }
      goto succeed;
    }
    printf("What is '%s'?\n", oFmt);
    abort();
  }
  printf("What is '%s'?\n", iFmt);
  abort();

succeed:
  //printf("Done.\n");
  return 0;
}

