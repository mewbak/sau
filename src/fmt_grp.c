//Credits go to SNV

#include "common.h"
#include "starcraft_pal.h"


typedef struct {
  u2 NFrames;
  u2 Width;  // virtual width and height
  u2 Height;
} __attribute__ ((packed)) header;

typedef struct {
  u1 X; // X/Y displacement
  u1 Y;
  u1 Width;  // real width and height
  u1 Height;
  u4 Off;
} __attribute__ ((packed)) frameHeader;

static spr *grpLoad(char *Input) {
  int I, J, C, X, Y;
  spr *S;
  pic *P, *R;
  u2 *Offs;
  int L = fileSize(Input);
  u1 *Q, *E, *D = readFile(0, L, Input);
  header *H = (header*)D;
  frameHeader *F = (frameHeader*)(D+sizeof(header));

  printf("  %dx%dx%d\n", H->NFrames, H->Width, H->Height);

  S = sprNew();
  S->Palette = ns(u1,4*256);
  memcpy(S->Palette, starcraft_pal, 4*256);
  S->Palette[0*4+0] = 0x00;
  S->Palette[0*4+1] = 0xFF;
  S->Palette[0*4+2] = 0x00;
  // colors 8-15 used as player colors. should we recolor them here?

  S->NAnis = 1;
  S->Anis = ns(ani, S->NAnis);
  S->Anis[0].NFacs = 1;
  S->Anis[0].Facs = ns(fac,S->Anis[0].NFacs);
  S->Anis[0].Facs[0].NPics = H->NFrames;
  S->Anis[0].Facs[0].Pics = ns(pic*, S->Anis[0].Facs[0].NPics);

  times (I, S->Anis[0].Facs[0].NPics) {
    printf("  X=%d Y=%d W=%d H=%d Off=#%06X\n"
          ,F[I].X, F[I].Y, F[I].Width, F[I].Height, F[I].Off);
    P = picNew(F[I].Width, F[I].Height, 8);

    Offs = (u2*)(D+F[I].Off);
    times (Y, Offs[0]/2) {
      Q = (u1*)Offs + Offs[Y];
      E = (Y+1<F->Height ? (u1*)Offs+Offs[Y+1] : D+L);
      for (X=0; Q<E; ) {
        C = *Q++;
        if (C&0x80) {
          C &= 0x7F;
          //printf("run0:%d\n", C);
          while (C-- && Q<E) picPut(P, X++, Y, 0);
        } else if (C&0x40) {
          C &= 0x3F;
          //printf("run:%d\n", C);
          while (C-- && Q<E) picPut(P, X++, Y, *Q);
          Q++;
        } else {
          //printf("copy:%d,%x\n", C, Q);
          while (C-- && Q<E) picPut(P, X++, Y, *Q++);
        }
      }
    }
    R = S->Anis[0].Facs[0].Pics[I] = picNew(H->Width, H->Height, 8);
    R->SharedPalette = 1;
    free(R->P);
    R->P = S->Palette;
    picBlt(R,P,0,F[I].X,F[I].Y,0,0,P->W,P->H);
  }

  return S;
}

int grpInit(format *F) {
  F->Type = FMT_SPRITE;
  F->Name = "grp";
  F->Description = "Starcraft sprites";
  F->Load = grpLoad;
  return 1;
}

