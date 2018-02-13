//Credits go to SNV

#include "common.h"


static u1 DomPal[] = {
 0x29, 0x37, 0x39, 0x13, 0x13, 0x13, 0x34, 0x26,
 0x1A, 0x39, 0x30, 0x20, 0x1A, 0x1A, 0x1A, 0x3F,
 0x35, 0x05, 0x3F, 0x2E, 0x0C, 0x3F, 0x26, 0x1A,
 0x33, 0x31, 0x22, 0x2D, 0x29, 0x1B, 0x26, 0x22,
 0x16, 0x20, 0x1B, 0x10, 0x31, 0x27, 0x19, 0x2A,
 0x20, 0x13, 0x37, 0x37, 0x09, 0x2F, 0x2F, 0x11,
 0x27, 0x27, 0x16, 0x20, 0x20, 0x18, 0x28, 0x28,
 0x28, 0x21, 0x21, 0x21, 0x07, 0x07, 0x07, 0x32,
 0x29, 0x28, 0x34, 0x22, 0x1F, 0x36, 0x1A, 0x16,
 0x3B, 0x38, 0x3C, 0x34, 0x31, 0x35, 0x27, 0x25,
 0x27, 0x20, 0x1F, 0x21, 0x19, 0x18, 0x1A, 0x13,
 0x12, 0x13, 0x0C, 0x0C, 0x0C, 0x06, 0x05, 0x06,
 0x37, 0x2B, 0x1B, 0x2F, 0x23, 0x13, 0x28, 0x1C,
 0x0D, 0x21, 0x16, 0x08, 0x1A, 0x10, 0x04, 0x32,
 0x35, 0x20, 0x22, 0x29, 0x16, 0x14, 0x1E, 0x0D,
 0x09, 0x13, 0x07, 0x19, 0x18, 0x06, 0x2C, 0x2B,
 0x16, 0x25, 0x3A, 0x30, 0x1A, 0x31, 0x23, 0x11,
 0x28, 0x18, 0x0A, 0x1F, 0x0E, 0x04, 0x16, 0x06,
 0x3B, 0x3B, 0x2D, 0x2C, 0x2C, 0x1F, 0x1E, 0x1D,
 0x13, 0x38, 0x11, 0x08, 0x1F, 0x04, 0x00, 0x3D,
 0x3D, 0x3D, 0x32, 0x32, 0x2F, 0x28, 0x28, 0x22,
 0x1D, 0x1D, 0x16, 0x13, 0x13, 0x0B, 0x23, 0x2A,
 0x1F, 0x12, 0x21, 0x10, 0x05, 0x18, 0x06, 0x04,
 0x2A, 0x03, 0x00, 0x1A, 0x00, 0x3A, 0x39, 0x29,
 0x32, 0x2F, 0x20, 0x2A, 0x26, 0x18, 0x22, 0x1D,
 0x11, 0x1A, 0x14, 0x0C, 0x27, 0x32, 0x38, 0x1A,
 0x28, 0x2F, 0x10, 0x1F, 0x27, 0x08, 0x17, 0x1F,
 0x02, 0x0F, 0x17, 0x2C, 0x3A, 0x38, 0x1F, 0x32,
 0x30, 0x15, 0x2B, 0x28, 0x0C, 0x24, 0x21, 0x05,
 0x1D, 0x19, 0x01, 0x16, 0x13, 0x3A, 0x3A, 0x3A,
 0x2F, 0x2F, 0x2F, 0x1B, 0x1B, 0x1B, 0x00, 0x3F,
 0x00, 0x3F, 0x3F, 0x00, 0x21, 0x20, 0x0C, 0x2B,
 0x35, 0x17, 0x20, 0x2D, 0x0F, 0x16, 0x25, 0x09,
 0x0D, 0x1E, 0x04, 0x06, 0x16, 0x01, 0x3F, 0x28,
 0x28, 0x37, 0x1C, 0x1C, 0x2F, 0x11, 0x11, 0x28,
 0x09, 0x09, 0x20, 0x03, 0x03, 0x00, 0x00, 0x00,
 0x1B, 0x01, 0x01, 0x21, 0x04, 0x04, 0x27, 0x09,
 0x09, 0x2D, 0x0F, 0x0F, 0x34, 0x16, 0x16, 0x35,
 0x35, 0x35, 0x2D, 0x2D, 0x2D, 0x25, 0x25, 0x25,
 0x1D, 0x1D, 0x1D, 0x15, 0x15, 0x15, 0x34, 0x34,
 0x3F, 0x22, 0x22, 0x38, 0x13, 0x13, 0x31, 0x08,
 0x08, 0x2A, 0x00, 0x00, 0x23, 0x39, 0x28, 0x28,
 0x3A, 0x3C, 0x3B, 0x2F, 0x3C, 0x30, 0x25, 0x36,
 0x28, 0x1C, 0x30, 0x22, 0x14, 0x2A, 0x1D, 0x0E,
 0x25, 0x19, 0x08, 0x1F, 0x16, 0x04, 0x19, 0x13,
 0x01, 0x13, 0x10, 0x00, 0x0E, 0x0D, 0x39, 0x2F,
 0x20, 0x36, 0x2B, 0x1D, 0x34, 0x27, 0x1B, 0x32,
 0x23, 0x19, 0x30, 0x20, 0x17, 0x3F, 0x3F, 0x3F,
 0x29, 0x37, 0x39, 0x13, 0x13, 0x13, 0x34, 0x26,
 0x1A, 0x39, 0x30, 0x20, 0x1A, 0x1A, 0x1A, 0x3F,
 0x35, 0x05, 0x3F, 0x2E, 0x0C, 0x3F, 0x26, 0x1A,
 0x33, 0x31, 0x22, 0x2D, 0x29, 0x1B, 0x26, 0x22,
 0x16, 0x20, 0x1B, 0x10, 0x31, 0x27, 0x19, 0x2A,
 0x20, 0x13, 0x37, 0x37, 0x09, 0x2F, 0x2F, 0x11,
 0x27, 0x27, 0x16, 0x20, 0x20, 0x18, 0x28, 0x28,
 0x28, 0x21, 0x21, 0x21, 0x07, 0x07, 0x07, 0x32,
 0x29, 0x28, 0x34, 0x22, 0x1F, 0x36, 0x1A, 0x16,
 0x3B, 0x38, 0x3C, 0x34, 0x31, 0x35, 0x27, 0x25,
 0x27, 0x20, 0x1F, 0x21, 0x19, 0x18, 0x1A, 0x13,
 0x12, 0x13, 0x0C, 0x0C, 0x0C, 0x06, 0x05, 0x06,
 0x37, 0x2B, 0x1B, 0x2F, 0x23, 0x13, 0x28, 0x1C,
 0x0D, 0x21, 0x16, 0x08, 0x1A, 0x10, 0x04, 0x32,
 0x35, 0x20, 0x22, 0x29, 0x16, 0x14, 0x1E, 0x0D,
 0x09, 0x13, 0x07, 0x19, 0x18, 0x06, 0x2C, 0x2B,
 0x16, 0x25, 0x3A, 0x30, 0x1A, 0x31, 0x23, 0x11,
 0x28, 0x18, 0x0A, 0x1F, 0x0E, 0x04, 0x16, 0x06,
 0x3B, 0x3B, 0x2D, 0x2C, 0x2C, 0x1F, 0x1E, 0x1D,
 0x13, 0x38, 0x11, 0x08, 0x1F, 0x04, 0x00, 0x3D,
 0x3D, 0x3D, 0x32, 0x32, 0x2F, 0x28, 0x28, 0x22,
 0x1D, 0x1D, 0x16, 0x13, 0x13, 0x0B, 0x23, 0x2A,
 0x1F, 0x12, 0x21, 0x10, 0x05, 0x18, 0x06, 0x04,
 0x2A, 0x03, 0x00, 0x1A, 0x00, 0x3A, 0x39, 0x29,
 0x32, 0x2F, 0x20, 0x2A, 0x26, 0x18, 0x22, 0x1D,
 0x11, 0x1A, 0x14, 0x0C, 0x27, 0x32, 0x38, 0x1A,
 0x28, 0x2F, 0x10, 0x1F, 0x27, 0x08, 0x17, 0x1F,
 0x02, 0x0F, 0x17, 0x2C, 0x3A, 0x38, 0x1F, 0x32,
 0x30, 0x15, 0x2B, 0x28, 0x0C, 0x24, 0x21, 0x05,
 0x1D, 0x19, 0x01, 0x16, 0x13, 0x3A, 0x3A, 0x3A,
 0x2F, 0x2F, 0x2F, 0x1B, 0x1B, 0x1B, 0x00, 0x3F,
 0x00, 0x3F, 0x3F, 0x00, 0x21, 0x20, 0x0C, 0x2B,
 0x35, 0x17, 0x20, 0x2D, 0x0F, 0x16, 0x25, 0x09,
 0x0D, 0x1E, 0x04, 0x06, 0x16, 0x01, 0x3F, 0x28,
 0x28, 0x37, 0x1C, 0x1C, 0x2F, 0x11, 0x11, 0x28,
 0x09, 0x09, 0x20, 0x03, 0x03, 0x00, 0x00, 0x00,
 0x1B, 0x01, 0x01, 0x21, 0x04, 0x04, 0x27, 0x09,
 0x09, 0x2D, 0x0F, 0x0F, 0x34, 0x16, 0x16, 0x35,
 0x35, 0x35, 0x2D, 0x2D, 0x2D, 0x25, 0x25, 0x25,
 0x1D, 0x1D, 0x1D, 0x15, 0x15, 0x15, 0x34, 0x34,
 0x3F, 0x22, 0x22, 0x38, 0x13, 0x13, 0x31, 0x08,
 0x08, 0x2A, 0x00, 0x00, 0x23, 0x39, 0x28, 0x28,
 0x3A, 0x3C, 0x3B, 0x2F, 0x3C, 0x30, 0x25, 0x36,
 0x28, 0x1C, 0x30, 0x22, 0x14, 0x2A, 0x1D, 0x0E,
 0x25, 0x19, 0x08, 0x1F, 0x16, 0x04, 0x19, 0x13,
 0x01, 0x13, 0x10, 0x00, 0x0E, 0x0D, 0x39, 0x2F,
 0x20, 0x36, 0x2B, 0x1D, 0x34, 0x27, 0x1B, 0x32,
 0x23, 0x19, 0x30, 0x20, 0x17, 0x3F, 0x3F, 0x3F};

static u1 SpritePal[] = {
 0x3F, 0x00, 0x3F, 0x3F, 0x00, 0x3F, 0x3F, 0x00,
 0x3F, 0x29, 0x37, 0x39, 0x3F, 0x35, 0x05, 0x3F,
 0x2E, 0x0C, 0x3F, 0x26, 0x1A, 0x33, 0x31, 0x22,
 0x2D, 0x29, 0x1B, 0x26, 0x22, 0x16, 0x20, 0x1B,
 0x10, 0x31, 0x27, 0x19, 0x2A, 0x20, 0x13, 0x37,
 0x37, 0x09, 0x2F, 0x2F, 0x11, 0x27, 0x27, 0x16,
 0x20, 0x20, 0x18, 0x28, 0x28, 0x28, 0x21, 0x21,
 0x21, 0x07, 0x07, 0x07, 0x32, 0x29, 0x28, 0x34,
 0x22, 0x1F, 0x36, 0x1A, 0x16, 0x3B, 0x38, 0x3C,
 0x34, 0x31, 0x35, 0x2D, 0x2B, 0x2E, 0x27, 0x25,
 0x27, 0x20, 0x1F, 0x21, 0x19, 0x18, 0x1A, 0x13,
 0x12, 0x13, 0x0C, 0x0C, 0x0C, 0x06, 0x05, 0x06,
 0x37, 0x2B, 0x1B, 0x2F, 0x23, 0x13, 0x28, 0x1C,
 0x0D, 0x21, 0x16, 0x08, 0x1A, 0x10, 0x04, 0x32,
 0x35, 0x20, 0x22, 0x29, 0x16, 0x14, 0x1E, 0x0D,
 0x09, 0x13, 0x07, 0x19, 0x18, 0x06, 0x2C, 0x2B,
 0x16, 0x25, 0x3A, 0x30, 0x1A, 0x31, 0x23, 0x11,
 0x28, 0x18, 0x0A, 0x1F, 0x0E, 0x04, 0x16, 0x06,
 0x3B, 0x3B, 0x2D, 0x2C, 0x2C, 0x1F, 0x1E, 0x1D,
 0x13, 0x38, 0x11, 0x08, 0x1F, 0x04, 0x00, 0x3D,
 0x3D, 0x3D, 0x32, 0x32, 0x2F, 0x28, 0x28, 0x22,
 0x1D, 0x1D, 0x16, 0x13, 0x13, 0x0B, 0x23, 0x2A,
 0x1F, 0x12, 0x21, 0x10, 0x05, 0x18, 0x06, 0x04,
 0x2A, 0x03, 0x00, 0x1A, 0x00, 0x3A, 0x39, 0x29,
 0x32, 0x2F, 0x20, 0x2A, 0x26, 0x18, 0x22, 0x1D,
 0x11, 0x1A, 0x14, 0x0C, 0x27, 0x32, 0x38, 0x1A,
 0x28, 0x2F, 0x10, 0x1F, 0x27, 0x08, 0x17, 0x1F,
 0x02, 0x0F, 0x17, 0x2C, 0x3A, 0x38, 0x1F, 0x32,
 0x30, 0x15, 0x2B, 0x28, 0x0C, 0x24, 0x21, 0x05,
 0x1D, 0x19, 0x01, 0x16, 0x13, 0x3A, 0x3A, 0x3A,
 0x2F, 0x2F, 0x2F, 0x1B, 0x1B, 0x1B, 0x00, 0x3F,
 0x00, 0x3F, 0x3F, 0x00, 0x21, 0x20, 0x0C, 0x2B,
 0x35, 0x17, 0x20, 0x2D, 0x0F, 0x16, 0x25, 0x09,
 0x0D, 0x1E, 0x04, 0x06, 0x16, 0x01, 0x3F, 0x28,
 0x28, 0x37, 0x1C, 0x1C, 0x2F, 0x11, 0x11, 0x28,
 0x09, 0x09, 0x20, 0x03, 0x03, 0x00, 0x00, 0x00,
 0x1B, 0x01, 0x01, 0x21, 0x04, 0x04, 0x27, 0x09,
 0x09, 0x2D, 0x0F, 0x0F, 0x34, 0x16, 0x16, 0x35,
 0x35, 0x35, 0x2C, 0x2C, 0x2C, 0x24, 0x24, 0x24,
 0x1B, 0x1B, 0x1B, 0x13, 0x13, 0x13, 0x32, 0x32,
 0x3B, 0x26, 0x26, 0x38, 0x1C, 0x1C, 0x34, 0x13,
 0x13, 0x31, 0x00, 0x00, 0x23, 0x39, 0x28, 0x28,
 0x3A, 0x3C, 0x3B, 0x2F, 0x3C, 0x30, 0x25, 0x36,
 0x28, 0x1C, 0x30, 0x22, 0x14, 0x2A, 0x1D, 0x0E,
 0x25, 0x19, 0x08, 0x1F, 0x16, 0x04, 0x19, 0x13,
 0x01, 0x13, 0x10, 0x00, 0x0E, 0x0D, 0x39, 0x2F,
 0x20, 0x37, 0x2C, 0x1E, 0x35, 0x28, 0x1C, 0x34,
 0x25, 0x1A, 0x30, 0x20, 0x17, 0x3F, 0x3F, 0x3F};

u1 Pal[256*4], SprPal[128*4];

typedef struct {
  u4 U1;
  u2 U2;
  u2 NColors;
  u2 W;
  u2 H;
  //u1 Pal[NColors];
  //u1 Pixels[W*H];
} __attribute__ ((packed)) rm;

static void expandPal(u1 *D, u1 *S, int N, int Shift) {
  while (N--) {
    *D++ = *S++ << Shift;
    *D++ = *S++ << Shift;
    *D++ = *S++ << Shift;
    *D++ = 0;
  }
}

static void deinterlace(pic *P, u1 *S, int W, int H) {
  int X, Y, XX, YY;
  times (Y, W) {
    times (X, H) {
      XX = (X%4)*(W/4) + Y/4;
      YY = (Y%4)*(H/4) + X/4;
      picPut(P,Y,X,S[YY*W+XX]);
    }
  }
}

static void save4PP(char *Output, u1 *M, int L) {
  char Tmp[1024];
  int I, J, X, Y, Wide, High;
  pic *P;

  times (I, 512) {
    Wide = I+4;
    High = L/Wide;
    P = picNew(Wide,High,8);
    memcpy(P->P, Pal, 4*256);
    memcpy(P->D, M, Wide*High);
    sprintf(Tmp, "%s/%04d.png", Output, I);
    pngSave(Tmp, P);
    picDel(P);
  }
}


static void domDecompile(char *Output, char *Input) {
  char Tmp[1024], Ext[32], Name[256];
  int I, J, Off, Len, Wide, High;
  int L = fileSize(Input);
  u1 *M = readFile(0, L, Input);
  u1 *Q, *Z;
  pic *P;

  pathParts(0, Name, Ext, Input);
  downcase(Name);
  downcase(Ext);

  expandPal(Pal, DomPal, 256, 2);
  expandPal(SprPal, SpritePal, 128, 2);

  Q = M;
  if (!strcmp(Ext,"rm")) {
    rm *H = (rm*)M;
    P = picNew(H->W, H->H, 8);
    expandPal(P->P, (u1*)(H+1), H->NColors, 2);
    deinterlace(P, M+sizeof(rm)+H->NColors*3, H->W, H->H);
    sprintf(Tmp, "%s/%s.png", Output, Name);
    pngSave(Tmp, P);
  } else if (!strcmp(Ext,"4pp") || !strcmp(Ext,"dat")) {
    if (0) {
uniform:
      memcpy(P->P, Pal, 4*256);
      times (I, L/(P->W*P->H)) {
        sprintf(Tmp, "%s/%04d.png", Output, I);
        deinterlace(P, Q, P->W, P->H);
        //memcpy(P->D, Q, P->W*P->H);
        pngSave(Tmp, P);
        Q += P->W*P->H;
      }
      return;
    }

    sprintf(Tmp, "%s/%s.png", Output, Name);
    if (!strcmp(Name, "monster")) {
      P = picNew(40, 48, 8);
      goto uniform;
    } else if (!strcmp(Name, "lizz")) {
      P = picNew(32, 40, 8);
      goto uniform;
    } else if (!strcmp(Name, "permspri")) {
      P = picNew(20, 24, 8);
      goto uniform;
    } else if (!strcmp(Name, "prmspr16")) {
      P = picNew(40, 48, 8); //sprites 0000 to 0659
      //P = picNew(80, 24, 8); //rest of the sprites
      goto uniform;
    } /*else if (!strcmp(Name, "spcomp")) {
      P = picNew(36, 32, 8);
      goto uniform;
    }*/ else {
      //save4PP(Output, M, L);
      printf("  cant handle this 4pp file\n");
      abort();
    }
  } else {
    printf("  cant handle *.%s files\n", Ext);
    abort();
  }
}

int domInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "dom";
  F->Description = "Dominus (use on *.rm, *anm, *.4pp, *.dat, files)";
  F->Decompile = domDecompile;
  return 1;
}


