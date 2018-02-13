//Credits go to SNV

#include "common.h"

typedef struct {
  u1 Crap1[0x3C];
  u4 FT1;
  u1 Crap2[0xc];
  u4 FT2;
  u4 NFiles;
  u1 Crap3[0x20];
} __attribute__ ((packed)) tgw;


typedef struct {
  u1 Name[80];
  u4 Progress; // 1/0xFFFFFFFF ration
  u4 Len;
  u4 Crap[4];
} __attribute__ ((packed)) e1;

typedef struct {
  u4 S; // start offset
  u4 E; // end offset
} __attribute__ ((packed)) e2;



static u4 swp4(u4 V) {
  u1 *U = (u1*)&V;
  return (U[0]<<24) | (U[1]<<16) | (U[2]<<8) | U[3];
}

#define fd(X) ((u1*)(X)+8)
#define fs(X) swp4(*((u4*)(X)+1))
#define fn(X) ((u1*)(X)+8+fs(X))

typedef struct {
  u1 Crap1[0xc];
  u2 W;
  u2 H;
  u1 Crap2[0x2c];
} __attribute__ ((packed)) hedr;

typedef struct {
  u1 Type[4];
  u4 Size;
} __attribute__ ((packed)) fram;

#define cvt(R,G,B,A,C) 

static void saveTGR(char *Output, u1 *M, int L) {
  char Tmp[1024];
  pic *P;
  int I, J, X, Y, C, R, G, B, A;
  hedr *H = (hedr*)fd(M+12);
  u1 *Q = M+12;
  u1 *Z;
  int NFrames = (M+L-Q+H->W*H->H*2-1)/(H->W*H->H*2);

  printf("  %dx%d NFrames=%d\n", H->W, H->H, NFrames);

  times (I, NFrames) {
    Q = fn(Q);
    Z = fd(Q);
    P = picNew(H->W, H->H, 32);
    for (J = 0; J < P->W*P->H*4; ) {
      fromA4R4G4B4(A, R, G, B, ru2(Z));

      P->D[J+0] = R;
      P->D[J+1] = G;
      P->D[J+2] = B;
      P->D[J+3] = 0xFF; //A;
      J += 4;
    }
    sprintf(Tmp, "%s/%04d.png", Output, I);
    pngSave(Tmp, P);
  }
}


static void tgwDecompile(char *Output, char *Input) {
  char Tmp[1024], Dir[80], Name[80], Ext[80];
  int I, J, Off, Len;
  pic *P;
  u1 *Q;
  int L = fileSize(Input);
  u1 *M = readFile(0, L, Input);
  tgw *H = (tgw*)M;
  e1 *E = (e1*)(M+H->FT1);
  e2 *O = (e2*)(M+H->FT2);

  times (I, H->NFiles) {
    Len = E[I].Len; //O[I].E-O[I].S;
    Off = O[I].S;
    Q = M+Off;
    for (J=0;  E[I].Name[J]; J++) if(E[I].Name[J]=='\\')E[I].Name[J]='/';

    pathParts(Dir, Name, Ext, E[I].Name);
    //if (strcmp(Name, "SPLASH SCREEN")) continue;
    //if (strcmp(Name, "ADELLON_MAJERE_PORTRAIT")) continue;
    //if (strcmp(Name, "HERO_AMULET_ICON")) continue;
    //if (strcmp(Name, "CREDITS BACKDROP")) continue;
    //if (strcmp(Name, "MENU BACKGROUD")) continue;

    printf ("Extracting: %s\n", E[I].Name);
    printf("  %04d: Off=%08X/%08X Len=%08x\n", I, Off, L, Len);

    sprintf(Tmp, "%s/%s", Output, E[I].Name);
    /*if (!strcmp(Ext, "TGR")) {
      saveTGR(Tmp, Q, Len);
    } else*/ {
      writeFile(0, Len, Tmp, Q);
    }
  }
}

int tgwInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "tgw";
  F->Description = "Kohan: Immortal Sovereigns archives";
  F->Decompile = tgwDecompile;
  return 1;
}


