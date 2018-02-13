//Credits go to SNV

#include "common.h"


typedef struct {
  u1 Id[3]; // SPL
  u1 Type; // 0x38='8'=8bit 0x4E='N'=16bit
  u1 Unused[4];
  u1 Description[60];
  u1 Name[60];
  u4 U1;
  u4 U2;
  u4 U3;
  u4 U4;
  u4 NFrames;
  //splRect Offs[NFrames];

  // following three fields present only when Type='8'
  //u4 Unknown;
  //u2 NColors;
  //u2 Palette[NColors];

  //frameHeader Frames[NFrames]
} __attribute__ ((packed)) header;


typedef struct {
  int X1; // top left X
  int Y1; // top left Y
  int X2; // bottom right X
  int Y2; // bottom right Y
  int X3;
  int Y3;
} __attribute__ ((packed)) splRect;


typedef struct {
  int X1; // top left X
  int Y1; // top left Y
  int X2; // bottom right X
  int Y2; // bottom right Y
  int H;  // height
  int Len;
  int Off; // I guess, it is where image should be loaded in memory
} __attribute__ ((packed)) frameHeader;


#define COLOR_KEY 0xFF

spr *splLoad(char *Input) {
  char Tmp[1024];
  int I, J, N, C, X, Y, NColors;
  spr *S;
  pic *P;
  frameHeader *F;
  int L = fileSize(Input);
  u1 *M = readFile(0, L, Input);
  header *H = (header*)M;
  splRect *R = (splRect*)(M+sizeof(header));
  u1 *E, *Q = (u1*)(R+H->NFrames);

  if (!strncmp(H->Id, "SPR", 3)) {
    printf("  SPR files are useless, just bunch of rects\n");
    abort();
  }

  unless (!strncmp(H->Id, "SPL", 3)) {
    printf("  Not a SPL file\n");
    abort();
  }

  if (H->Type != 'N' && H->Type != '8') {
    printf("  Unsupported sprite type (#%02x)\n", H->Type);
    abort();
  }

  S = sprNew();

  //printf("  %dx%dx%d %d\n", H->NFrames, H->Width, H->Height, *(u4*)Q);


  if (H->Type == '8') {
    Q+=4; // guess what we skipping

    S->ColorKey = COLOR_KEY;
    S->Palette = ns(u1, 4*256);

    NColors = *(u2*)Q; Q+=2;
    //printf("%d\n", NColors);
    times (I, NColors) { // this time palette is 16-bit
      fromR5G6B5(S->Palette[I*4+0],S->Palette[I*4+1],S->Palette[I*4+2],*(u2*)Q);
      S->Palette[I*4+3] = 0;
      Q += 2;
    }
    S->Palette[COLOR_KEY*4+0] = 0x00;
    S->Palette[COLOR_KEY*4+1] = 0xFF;
    S->Palette[COLOR_KEY*4+2] = 0xFF;
  }

  F = (frameHeader*)Q;
  Q += H->NFrames*sizeof(frameHeader);

  S->NAnis = 1;
  S->Anis = ns(ani, S->NAnis);
  S->Anis[0].NFacs = 1;
  S->Anis[0].Facs = ns(fac,S->Anis[0].NFacs);
  S->Anis[0].Facs[0].NPics = H->NFrames;
  S->Anis[0].Facs[0].Pics = ns(pic*,H->NFrames);

  times (I, H->NFrames) {
    //printf("  X1=%d Y1=%d X2=%d Y2=%d H=%d L=%d O=#%08x Width=%d Height=%d\n"
    //      ,F[I].X1, F[I].Y1, F[I].X2, F[I].Y2, F[I].H, F[I].Len, F[I].Off
    //      ,F[I].X2-F[I].X1, F[I].Y2-F[I].Y1);


    // we draw sprite right in the center of image
    // FIXME: some sprites get cropped. Fix size calculation
    P = picNew((F[0].X2-F[0].X1)*2, (F[0].Y2-F[0].Y1)*2, H->Type=='8'?8:32);
    S->Anis[0].Facs[0].Pics[I] = P;
    if (H->Type == '8') {
      P->SharedPalette = 1;
      free(P->P);
      P->P = S->Palette;
      picClear(P, COLOR_KEY);
      for (Y=P->H/2+F[I].Y1, E=Q+F[I].Len; Q<E; Y++) {
        //printf("  %02d,%02d: ", Y,*Q);
        N = *(u2*)Q; Q+=2; // number of pixel chunks
        X = P->W/2+F[I].X1;
        while (N--) {
          //printf("%d,%d; ", *(u2*)Q, *(u2*)(Q+2));
          X += *(u2*)Q; Q+=2; // increment
          C = *(u2*)Q; Q+=2;  // length to copy
          while (C--) picPut(P, X++, Y, *Q++);
        }
        //printf("\n");
      }
    } else {
      picClear(P, R8G8B8(0x00,0xFF,0xFF));
      for (Y=P->H/2+F[I].Y1, E=Q+F[I].Len; Q<E; Y++) {
        //printf("  %02d,%02d: ", Y,*Q);
        N = *(u2*)Q; Q+=2; // number of pixel chunks
        X = P->W/2+F[I].X1;
        while (N--) {
          //printf("%d,%d; ", *(u2*)Q, *(u2*)(Q+2));
          X += *(u2*)Q; Q+=2; // increment
          C = *(u2*)Q; Q+=2;  // length to copy
          while (C--) {
            int R, G, B;
            fromR5G6B5(R,G,B,*(u2*)Q);
            picPut32(P, X++, Y, R8G8B8(R,G,B));
            Q += 2;
          }
        }
        //printf("\n");
      }
    }
  }

  free(M);

  return S;
}

// FIXME: add support for OBM, MRF, and LIB
int splInit(format *F) {
  F->Type = FMT_SPRITE;
  F->Name = "spl";
  F->Description = "Myth of Soma sprites";
  F->Load = splLoad;
  return 1;
}

