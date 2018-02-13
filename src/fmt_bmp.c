#include "common.h"


typedef struct {
  u1 Id[2]; // BM - common BMP
            // BA - OS/2 bitmap array
            // CI - OS/2 color icon
            // &CP - OS/2 color pointer
            // IC - OS/2 icon
            // *PT - OS/2 pointer
  u4 Size; // size of whole BMP file
  u2 Creator1; // set it to 0
  u2 Creator2; // set it to 0
  u4 Off; // image data offset
} __attribute__ ((packed)) fileHeader;


typedef struct {
  u4 Size; // 12 - BITMAPCOREHEADER/OS21XBITMAPHEADER (OS/2, win3.1)
           // 64 - BITMAPCOREHEADER2/OS22XBITMAPHEADER (OS/2)
           // 40 - BITMAPINFOHEADER (this one)
           // 52 - BITMAPV2INFOHEADER (Removes optional RGB bit masks. Adds mandatory RGB bit masks)
           // 56 - BITMAPV3INFOHEADER (Adds mandatory Alpha channel bit mask)
           // 108 - win95/NT4 (Adds Color Space type and Gamma correction)
           // 124 - win98/2k (Adds ICC Color Profiles)
  u4 Width;
  u4 Height;
  u2 NPlanes; // set it to 1 for non-interleaved images
              // you can set this to 0, but GIMP will refuse load
  u2 BPP; // BMP allows 1, 4, 8, 16, 24 and 32 bit images
  u4 Compression; // 0 - uncompressed
                  // 1 - 8-bit RLE
                  // 2 - 4-bit RLE
                  // 3 - bit field (or Huffman 1D, if Size=64)
                  // 4 - JPEG (or RLE-24, if Size=64)
                  // 5 - PNG
                  // 6 - alpha bitfields
  u4 Len; // size of image data
  u4 PixelsPerMeterX; // set these to 0
  u4 PixelsPerMeterY;
  u4 NColors; // if set to zero, calculate it using BPP
  u4 NImportantColors; // useless hint for color manager (set it to 0)
  //u1 Palette[4*NColors];
  //u1 Data[];
} __attribute__ ((packed)) header;

pic *bmpLoad(char *File) {
  abort();
}

void bmpSave(char *Output, pic *P) {
  int I, X, Y, LL = (P->W*P->B+7)/8;
  u4 C;

  u1 *M = ns(u1,4096+(LL+4)*P->H);

  fileHeader *FH = (fileHeader*)M;
  header *H = (header*)(FH+1);
  u1 *Q = (u1*)(H+1);

  H->Size = sizeof(header);
  H->Width = P->W;
  H->Height = P->H;
  H->BPP = P->B;
  H->NPlanes = 1;

  if (P->B == 8) {
    H->NColors = 256;
    times(I, 256) {
      *Q++ = P->P[I*4+2];
      *Q++ = P->P[I*4+1];
      *Q++ = P->P[I*4+0];
      *Q++ = P->P[I*4+3];
    }
  }

  FH->Off = Q-M;
  for (Y=P->H-1; Y>=0; Y--) {
    if (P->B == 8) times (X, P->W) *Q++ = picGet(P,X,Y);
    else if (P->B == 24) times (X, P->W) {
      fromR8G8B8(Q[0],Q[1],Q[2], picGet24(P,X,Y));
      Q+=3;
    } else if (P->B == 32) times (X, P->W) {
      *(u4*)Q = picGet32(P,X,Y);
      Q+=4;
    } else {
      printf("  cant save %d-bit image as BMP\n", P->B);
      abort();
    }
    if (LL&0x3) times (X, 4-(LL&0x3)) *Q++ = 0; // padding
  }

  FH->Id[0] = 'B';
  FH->Id[1] = 'M';
  FH->Size = Q-M;
  H->Len = Q-M-FH->Off;
  writeFile(0, Q-M, Output, M);
}

int bmpInit(format *F) {
  F->Type = FMT_IMAGE;
  F->Name = "bmp";
  F->Description = "BMP images (Microsoft Windows)";
  F->Save = bmpSave;
  //F->Load = bmpLoad;
  return 1;
}

