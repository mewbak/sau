#include "common.h"


typedef struct {
  u1 IDLength;   // 00h Size of Image ID field
  u1 CMapType;   // 01h Color map type: 0=none, 1=has palette
  u1 ImageType;  // 02h 0=none,1=indexed,2=rgb,3=greyscale,
                 //   9 = indexed + rle
                 //   10 = rgb + rle
                 //   11 = compressed grayscale
                 //   32 = indexed + Huffman, Delta, and RLE
                 //   33 = indexed + Huffman, Delta, and RLE 4-pass quadtree-type process.
  u2 CMapOffset; // 03h offset of the first palette entry
  u2 CMapColors; // 05h number of colors in color map
  u1 CMapDepth;  // 07h Depth of color map entries
  u2 XOffset;    // 08h X origin of image
  u2 YOffset;    // 0Ah Y origin of image
  u2 Width;      // 0Ch Width of image
  u2 Height;     // 0Eh Heigth of image
  u1 BPP;        // 10h Bits Per Pixel: 8,16,24,32
  u1 Descriptor; // 11h Image descriptor byte
     // Bits 3-0 - number of attribute bits associated with each pixel.
     // Bit 4    - reserved.  Must be set to 0.
     // Bit 5    - screen origin bit.
     //            0 = Origin in lower left-hand corner.
     //            1 = Origin in upper left-hand corner.
     //            Must be 0 for Truevision images.
     // Bits 7-6 - Data storage interleaving flag.
     //            00 = non-interleaved.
     //            01 = two-way (even/odd) interleaving.
     //            10 = four way interleaving.
     //            11 = reserved.
     // This entire byte should be set to 0.  Don't ask me.

     // descriptor: 00vhaaaa
     //   * h horizontal flip
     //   * v vertical flip
     //   * a alpha bits (alpha channel depth)

  // u1 IDField[IDLength];
  // u1 CMap[CMapLength];
  // u1 BMap[Width*Height*BPP/8];
} __attribute__ ((packed)) TGAHeader;

void tgaSave(char *File, pic *P) {
  int I, J;
  FILE *F;
  u1 Pix[4];
  TGAHeader TH;

  if (P->B != 8) {
    printf("Only 8-bit TGAs are supported");
    abort();
  }

  memset(&TH, 0, sizeof(TGAHeader));
  TH.Width = P->W;
  TH.Height = P->H;
  TH.BPP = P->B;
  TH.Descriptor = 0x20; // v-flip

  if (P->B > 8) {
    TH.ImageType = 2;
    TH.CMapType = 0;
  } else {
    TH.ImageType = 1;
    TH.CMapType = 1;
    TH.CMapDepth = 32;
    TH.CMapColors = 256;
    TH.CMapOffset = sizeof(TGAHeader);
  }

  writeFile(0, 0, File, File);
  F = fopen(File, "wb");
  fwrite(&TH, 1, sizeof(TGAHeader), F);

  if (P->B == 8) {
    times (I, 256) {
      fputc(P->P[I*4+2], F);
      fputc(P->P[I*4+1], F);
      fputc(P->P[I*4+0], F);
      fputc(255-P->P[I*4+3], F);
    }
    times(I, P->H) times(J, P->W) fputc(P->D[I*P->I + J], F);
  }

  fclose(F);
}

int tgaInit(format *F) {
  F->Type = FMT_IMAGE;
  F->Name = "tga";
  F->Description = "TGA images (Truevision Graphic Array)";
  F->Save = tgaSave;
  //F->Load = tgaLoad;
  return 1;
}

