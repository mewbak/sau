//Credits go to SNV

#include "common.h"
#include "diablo_pal.h"


// animation sequence header
typedef struct {
  u4 NFrames;
  //u4 Offs[NFrames+1]; // offset for each frame, including end of last frame
  // NOTE: offsets are relative to start of animation sequence
} __attribute__ ((packed)) header;

typedef struct {
  u2 Offs[5]; // sometimes sprites split into 5 successive 32-line chunks
              // all this to speed-up clipping code
} __attribute__ ((packed)) frameHeader;


static spr *cl2Load(char *Input) {
  int NAnis, A, I, J, E, K, C, X, Y, GW, GH, BLen=1024*1024;
  char Tmp[1024], Dir[1024], Name[256], *Pal;
  spr *S;
  pic *P;
  frameHeader *FH;
  int L = fileSize(Input);
  u1 *B = malloc(BLen), *D = readFile(0, L, Input);
  u4 *Anis, *Offs, Zero=0;
  header *H;

  pathParts(Dir, Name, 0, Input);

  S = sprNew();
  S->Palette = ns(u1,4*256);
  memcpy(S->Palette, DiabloPalette, 4*256);

  // some CELs have custom palette
  sprintf(Tmp, "%s/%s.pal", Dir, Name);

  if (fileExist(Tmp)) {
    C = fileSize(Tmp);
    Pal = readFile(0, C, Tmp);
    times (I, C/3) {
      S->Palette[I*4+0] = Pal[I*3+0];
      S->Palette[I*4+1] = Pal[I*3+1];
      S->Palette[I*4+2] = Pal[I*3+2];
    }
    free(Pal);
  }

  S->Palette[255*4+0] = 0x00;
  S->Palette[255*4+1] = 0xFF;
  S->Palette[255*4+2] = 0xFF;

  downcase(Name);

  // Animation table is optional, but we can guess it's presence.
  // if the last entry doesnt equal CEL size, then we got it
  Offs = (u4*)D;
  if (Offs[Offs[0]+1] != L) {
   for (NAnis = 1; Offs[NAnis-1]<Offs[NAnis]; NAnis++);
    Anis = (u4*)D;
  } else {
    NAnis = 1;
    Anis = &Zero;
  }

  S->NAnis = 1;
  S->Anis = ns(ani, S->NAnis);
  S->Anis[0].NFacs = NAnis;
  S->Anis[0].Facs = ns(fac,S->Anis[0].NFacs);


  times (A, NAnis) {
    printf("  %d: #%04x/#%04x\n", A, Anis[A], L);
    H = (header*)(D+Anis[A]);
    Offs = (u4*)(D+Anis[A]+sizeof(header));

    S->Anis[0].Facs[A].NPics = H->NFrames;
    S->Anis[0].Facs[A].Pics = ns(pic*, S->Anis[0].Facs[A].NPics);

    times (I, H->NFrames) {
      memset(B, 0xFF, 1024);
      J = Anis[A]+Offs[I];
      E = Anis[A]+Offs[I+1];
      printf("    %d: #%04x #%04x/#%04x\n", I, E-J, J, L);

      GH = -1;
      FH = (frameHeader*)(D+J);
      if (FH->Offs[0] == sizeof(frameHeader)) {
        // use frame header to determine frame height
        for (K=0; FH->Offs[K] && K<5; K++);
        GH = (K)*32;

        //printf("    skipping header\n");
        J += sizeof(frameHeader);
      }

      for (K=0; J < E && J < L; ) {
        C = D[J++];
        if (C<0x80) {
          //printf("run0 %d\n", C);
          while (C--) B[K++] = 0xFF;
        } else if (C < 0xBF) {
          C = 0xBF - C;
          //printf("run %d\n", C);
          while (C--) B[K++] = D[J];
          J++;
        } else {
          C = 0x100 - C;
          //printf("copy %d\n", C);
          while (C--) B[K++] = D[J++];
        }
      }

      // CEL files omit width and height. Guess them.
      GW = -1;
      if (GH == -1) GH = 32;
      for (Y = GH; Y <= 160; Y+=32) {
        if (!(K%Y)) {
          GW = K/Y;
          break;
        }
      }

      if (GW == -1) {
        for (Y = 3; Y <= 160; Y++) {
          if (!(K%Y)) {
            GW = K/Y;
            break;
          }
        }
      }

      if (GW == -1) GW = sqrt(K);

      // known offenders
      if (!strcmp(Name, "wlbat")) GW=96;
      if (!strcmp(Name, "blood4")) GW=96;
      if (!strcmp(Name, "firema")) GW=96;
      GH=K/GW;

      printf("      %d=%dx%d\n", K, GW, GH);

      P = S->Anis[0].Facs[A].Pics[I] = picNew(GW, GH, 8);
      P->SharedPalette = 1;
      free(P->P);
      P->P = S->Palette;
      memcpy(P->D, B, GW*GH);

      times (Y, P->H/2) { // must be reversed
        times (X, P->W) {
          C = P->D[Y*P->I+X];
          P->D[Y*P->I+X] = P->D[(P->H-Y-1)*P->I+X];
          P->D[(P->H-Y-1)*P->I+X] = C;
        }
      }
    }
  }

  free(B);

  return S;
}

int cl2Init(format *F) {
  F->Type = FMT_SPRITE;
  F->Name = "cl2";
  F->Description = "Diablo sprites";
  F->Load = cl2Load;
  return 1;
}

