//Credits go to SNV

#include "common.h"
#include "diablo_pal.h"


/*

During the development, Diablo used CEL format for everything. But the release
version uses CEL 2 for missiles and creatures, as it has more efficient RLE
scheme for sparse shapes.

The old CEL animations can still be seen in towners/twnf/twnfw.cel,
towners/townwmn1/wmnw.cel and /towners/smith/smithw.cel.

                                                       -- SNV

*/


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


// FIXME: *.til files should be used to make sense of tilesets
static spr *celLoad(char *Input) {
  int NAnis, A, I, J, E, K, C, X, Y, GW, GH, G, LastC, MaxC, BLen=1024*1024;
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
  if (!strcmp(Name, "l1s")) sprintf(Tmp, "%s/l1.pal", Dir);
  else if (!strcmp(Name, "l2s")) sprintf(Tmp, "%s/l2.pal", Dir);
  else if (!strcmp(Name, "towns")) sprintf(Tmp, "%s/town.pal", Dir);
  else sprintf(Tmp, "%s/%s.pal", Dir, Name);

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

  // can still be loaded, if we fix offsets manually
  if (!strcmp(Name, "unravw")) {
    Anis[1] = 0x2707;
    Anis[2] = 0x49C5;
    Anis[3] = 0x6BA3;
    NAnis = 4;
  }

  S->NAnis = 1;
  S->Anis = ns(ani, S->NAnis);
  S->Anis[0].NFacs = NAnis;
  S->Anis[0].Facs = ns(fac,S->Anis[0].NFacs);

  GW = -1;
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

      // skip offset table, if present
      FH = (frameHeader*)(D+J);
      if (FH->Offs[0] == sizeof(frameHeader)) J += sizeof(frameHeader);

      G = 0;
      MaxC = 1;
      LastC = -1;
      for (K=0; J < E && J < L; ) {
        C = D[J++];
        if (C&0x80) {
          MaxC = max(G,MaxC);
          G = 0;
          C = 0x100 - C;
          MaxC = max(C,MaxC);
          //printf("run %d\n", C);
          while (C--) B[K++] = 0xFF;
        } else {
          //printf("copy %d\n", C);
          if (G) {
            G += C;
            if (C != LastC) {
              MaxC = max(G,MaxC);
              G = 0;
            }
          } else {
            G += C;
            MaxC = max(G,MaxC);
          }
          LastC = C;
          while (C--) B[K++] = D[J++];
        }
      }

      // CEL files omit width and height. Guess them.

      // known offenders
      if (!strcmp(Name, "smaltext")) MaxC = 13;
      else if (!strcmp(Name, "option")) MaxC = 27;
      else if (!strcmp(Name, "hf_logo3")) MaxC = 430;
      else if (!strcmp(Name, "diabsmal")) {
        if (K == 36500) MaxC = 365; // hellfire
        else MaxC = 296;
      } else if (!strcmp(Name, "pentspin")) MaxC = 48;
      else if (!strcmp(Name, "pentspn2")) MaxC = 12;
      else if (!strcmp(Name, "textslid")) MaxC = 12;
      else if (!strcmp(Name, "towns")) MaxC = 64;
      else if (!strcmp(Name, "miniwatr")) MaxC = 64;
      else if (!strcmp(Name, "firewal1")) MaxC = 160;
      else if (!strcmp(Name, "burncros")) MaxC = 160;
      else if (!strncmp(Name, "rhinos", 6)) MaxC = 160;
      else if (!strncmp(Name, "worm", 4)) MaxC = 160;
      else if (!strcmp(Name, "objcurs")) {
        if (K==28*28) MaxC = 28;
        else if (K==33*29) MaxC = 33;
        else if (K==35*23) MaxC = 23;
        else if (K==32*32) MaxC = 32;
        else if (K==28*56) MaxC = 28;
        else if (K==56*84) MaxC = 56;
      } else if (!strcmp(Name, "quotes")) {
        MaxC = 640;
      } else if (!strcmp(Name, "l1")
              || !strcmp(Name, "l2")
              || !strcmp(Name, "l3")
              || !strcmp(Name, "l4")
              || !strcmp(Name, "town.cel")) {
        if (K==32*32) MaxC = 32;
        else {
          MaxC = 32;
          K=32*32;
        }
      }

      if (!K) GH = GW = 1;
      else {
        if (!(K%MaxC)) {
          GH = K/MaxC;
          GW = K/GH;
        } else if (GW != -1 && !(K%GW)) { // try old guess
          GH = K/GW;
        } else { // better than nothing
          GH = GW = sqrt(K);
        }
      }

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

int celInit(format *F) {
  F->Type = FMT_SPRITE;
  F->Name = "cel";
  F->Description = "Diablo sprites";
  F->Load = celLoad;
  return 1;
}

