//Credits go to SNV

#include "common.h"

typedef struct { //header for XUA/XUB files
  u1 Id[6];
  u2 NFrames;
  u2 U2;
  u2 X;
  u2 Y;
  u2 W;
  u2 H;
  u2 U3;
  u2 U4;
  u2 U5;
  u2 U6;
  u1 Pal[256*3];
  u1 Data[1 /* W*H*NFrames */];
} __attribute__ ((packed)) xua;

typedef struct {
  u1 Id[12];
  u4 W;
  u4 H;
  u1 Pal[256*3];
  u1 Data[1 /* W*H */];
} __attribute__ ((packed)) jun;


typedef struct {
  u1 Id[10];
  u4 NFrames;
  u4 W;
  u4 H;
  u1 Pal[256*3];
  u1 Data[1 /* W*H */];
} __attribute__ ((packed)) jus;


typedef struct {
  u1 Id[10];
  u4 NFrames;
  u2 W;
  u2 H;
  u1 Pal[256*3];
  u1 Data[1 /* W*H */];
} __attribute__ ((packed)) fan;

typedef struct {
  u1 Id[4]; //xmap
  u4 X; //0x13
  u4 Y; //0x1e
  u4 W; //0x280
  u4 H; //0x3c0
  u4 FootCount; //0xFC
  u4 FootSize;  //0xFC00
  u4 C; //0x28
  u4 D; //0x3C
  u4 E; //0x10
  u4 F; //0x10
  u1 Pal[256*3];
  u1 Data[1 /* W*H */];
} __attribute__ ((packed)) xmap;

static void expandPal(u1 *D, u1 *S, int N, int Shift) {
  while (N--) {
    *D++ = *S++ << Shift;
    *D++ = *S++ << Shift;
    *D++ = *S++ << Shift;
    *D++ = 0;
  }
}

static void xuaDecompile(char *Output, char *Input) {
  char Tmp[1024], Name[256], Ext[32];
  int I, J, X, Y, C, Off, Len, Wide, High;
  int L = fileSize(Input);
  u1 *M = readFile(0, L, Input);
  u1 *Q, *Z;
  pic *P;

  pathParts(0, Name, Ext, Input);
  downcase(Ext);

  if (!strcmp(Ext, "xua")) { //XUAs are plain text sprites
    xua *H = (xua*)M;
    P = picNew(H->W, H->H, 8);
    expandPal(P->P, H->Pal, 256, 0);
    times (I, H->NFrames) {
      memcpy(P->D, H->Data+H->W*H->H*I, H->W*H->H);
      printf("  Extracing: %04d.png\n", I);
      sprintf(Tmp, "%s/%04d.png", Output, I);
      pngSave(Tmp, P);
    }
  } else if (!strcmp(Ext, "xub")) { //XUBs are RLE compressed sprites
    xua *H = (xua*)M;
    P = picNew(H->W, H->H, 8);
    expandPal(P->P, H->Pal, 256, 0);
    Z = H->Data;
    times (I, H->NFrames) {
      Q = Z;
      Z = Q;
      Z += ru4(Q) + 4;
      memset(P->D, 0, P->W*P->H);
      X = Y = 0;
      while (Q < Z && Y < H->H) {
        if (*Q) picPut(P, X++, Y, *Q++);
        else {
          X += Q[1];
          Q += 2;
        }
        if (X >= H->W) {
          X = 0;
          Y++;
        }
      }
      printf("  Extracting: %04d.png\n", I);
      sprintf(Tmp, "%s/%04d.png", Output, I);
      pngSave(Tmp, P);
    }
  } else if (!strcmp(Ext, "jun")) {
    jun *H = (jun*)M;
    P = picNew(H->W, H->H, 8);
    expandPal(P->P, H->Pal, 256, 0);
    memcpy(P->D, H->Data, H->W*H->H);
    sprintf(Tmp, "%s/%s.png", Output, Name);
    pngSave(Tmp, P);
  } else if (!strcmp(Ext, "jus")) {
    jus *H = (jus*)M;
    P = picNew(H->W, H->H, 8);
    expandPal(P->P, H->Pal, 256, 0);
    times (I, H->NFrames) {
      memcpy(P->D, H->Data+H->W*H->H*I, H->W*H->H);
      printf("  Extracing: %04d.png\n", I);
      sprintf(Tmp, "%s/%04d.png", Output, I);
      pngSave(Tmp, P);
    }
  } else if (!strcmp(Ext, "fan")) {
    fan *H = (fan*)M;
    P = picNew(H->W, H->H, 8);
    expandPal(P->P, H->Pal, 256, 0);
    times (I, H->NFrames) {
      memcpy(P->D, H->Data+H->W*H->H*I, H->W*H->H);
      printf("  Extracing: %04d.png\n", I);
      sprintf(Tmp, "%s/%04d.png", Output, I);
      pngSave(Tmp, P);
    }
  } else if (!strcmp(Ext, "maa")) {
    xmap *H = (xmap*)M;
    P = picNew(H->W, H->H, 8);
    expandPal(P->P, H->Pal, 256, 0);
    memcpy(P->D, M+L-H->W*H->H-H->FootSize, H->W*H->H);
    sprintf(Tmp, "%s/%s.png", Output, Name);
    pngSave(Tmp, P);
  } else {
    printf("  cant handle %s\n", Ext);
    abort();
  }
}

int xuaInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "xua";
  F->Description = "Original Mulan archives (use on XUA/XUB/JUN/MAA/JUS/FAN files)";
  F->Decompile = xuaDecompile;
  return 1;
}

