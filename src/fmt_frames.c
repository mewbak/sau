#include "common.h"


static void nameToINJKE(char *Name, int *I, char *N, int *J, int *K, char *E) {
  char *Q, *T, Tmp[256];
  strcpy(Tmp, Name);
  Q = Tmp-1;

  T = Q = Q + 1;
  unless (Q = strchr(Q, '-'))
    {printf("Invalid Filename: %s\n", Name); abort();}
  *Q = 0;

  *I = atoi(T);

  T = Q = Q + 1;
  unless (Q = strchr(Q, '-'))
    {printf("Invalid Filename: %s\n", Name); abort();}
  *Q = 0;

  strcpy(N, T);

  T = Q = Q + 1;
  unless (Q = strchr(Q, '-'))
    {printf("Invalid Filename: %s\n", Name); abort();}
  *Q = 0;

  *J = atoi(T);

  T = Q = Q + 1;
  unless (Q = strchr(Q, '.'))
    {printf("Invalid Filename: %s\n", Name); abort();}
  *Q = 0;

  *K = atoi(T);

  T = Q = Q + 1;

  strcpy(E, T);
}

void saveFrames(char *Output, spr *S) {
  char Tmp[1024], *Name;
  int I, J, K;

  times(I, S->NAnis) {
    times(J, S->Anis[I].NFacs) {
      times(K, S->Anis[I].Facs[J].NPics) {
        unless (S->Anis[I].Facs[J].Pics[K]) continue;
        Name = S->Anis[I].Name ? S->Anis[I].Name : "";
        sprintf(Tmp, "%s/%04d-%s-%02d-%03d.png"
               ,Output, I, Name, J, K);
        pngSave(Tmp, S->Anis[I].Facs[J].Pics[K]);
      }
    }
  }
}

spr *loadFramesSP(char *DirName, int SharedPalette) {
  char Name[128], Ext[64], Tmp[1024];
  int L, I, J, K;
  spr *S = sprNew();
  S->Palette = ns(u1, 4*256);

  fileList *FL = getFileList(DirName);
  times (L, FL->Size) {
    nameToINJKE(FL->Names[L], &I, Tmp, &J, &K, Ext);
    S->NAnis = max(S->NAnis, I);
    //printf("%d-%s-%d-%d.%s\n", I, Tmp, J, K, Ext);
    //printf("%s\n", FL->Names[L]);
  }

  times (L, FL->Size) {
    nameToINJKE(FL->Names[L], &I, Tmp, &J, &K, Ext);
    unless (S->Anis) {
      ++S->NAnis;
      S->Anis = ns(ani, S->NAnis);
    }
    S->Anis[I].NFacs = max(S->Anis[I].NFacs, J);
  }

  times (L, FL->Size) {
    nameToINJKE(FL->Names[L], &I, Tmp, &J, &K, Ext);
    unless (S->Anis[I].Name) S->Anis[I].Name = strdup(Tmp);
    unless (S->Anis[I].Facs) {
      ++S->Anis[I].NFacs;
      S->Anis[I].Facs = ns(fac, S->Anis[I].NFacs);
    }
    S->Anis[I].Facs[J].NPics = max(S->Anis[I].Facs[J].NPics, K);
  }

  times (L, FL->Size) {
    nameToINJKE(FL->Names[L], &I, Tmp, &J, &K, Ext);
    unless (S->Anis[I].Facs[J].Pics) {
      ++S->Anis[I].Facs[J].NPics;
      S->Anis[I].Facs[J].Pics = ns(pic*, S->Anis[I].Facs[J].NPics);
    }
    sprintf(Tmp, "%s/%s", DirName, FL->Names[L]);
    if (!strcmp(Ext, "pcx")) {
      S->Anis[I].Facs[J].Pics[K] = pcxLoad(Tmp);
      if (L == 0) {
        memcpy(S->Palette, S->Anis[I].Facs[J].Pics[K]->P, 256*4);
      }
      if (SharedPalette) {
        free(S->Anis[I].Facs[J].Pics[K]->P);
        S->Anis[I].Facs[J].Pics[K]->SharedPalette = 1;
        S->Anis[I].Facs[J].Pics[K]->P = S->Palette;
      }
    } else {
      printf("Unsupported extension: %s\n", FL->Names[L]);
    }
  }

  freeFileList(FL);
  return S;
}

spr *loadFrames(char *DirName) {
  return loadFramesSP(DirName, 1);
}

int framesInit(format *F) {
  F->Type = FMT_SPRITE;
  F->Name = "frames";
  F->Description = "Specifies directory of PCX images as a sprite";
  F->Save = saveFrames;
  F->Load = loadFrames;
  return 1;
}

