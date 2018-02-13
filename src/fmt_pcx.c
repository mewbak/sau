#include "common.h"


typedef struct {
  u1 Manufacturer; // set to 10 = ZSoft
  u1 Version; // 0 = Paintbrush v2.5
              // 2 = Paintbrush v2.8 w palette
              // 3 = Paintbrush v2.8 w/o palette
              // 4 = Paintbrush for Windows
              // 5 = Paintbrush v3.0+
  u1 Encoding; // 1 = RLE
  u1 BPP; //  bits per pixel, 1, 2, 4 or 8 (per plane)
  u2 XMin;
  u2 YMin;
  u2 XMax;
  u2 YMax;
  u2 HDpi;
  u2 VDpi;
  u1 ColorMap[48];
  u1 Reserved; // set to 0
  u1 NPlanes; // number of color planes (like R, G and B)
  u2 BytesPerLine;
  u2 PaletteInfo; // 1 = Color/BW, 2 = Grayscale
  u2 HScreenSize;
  u2 VScreenSize;
  u1 Filler[54]; // set to 0
} __attribute__ ((packed)) PCXHeader;

pic *pcxLoad(char *FileName) {
  pic *P;
  int I, C, Sz;
  u1 *M, *D;
  PCXHeader *PCX;
  Sz = fileSize(FileName);
  D = readFile(0, Sz, FileName);
  unless(D) {
    printf("Cant to open %s\n", FileName);
    abort();
  }
  PCX = (PCXHeader *)D;
  P = picNew(PCX->XMax-PCX->XMin+1, PCX->YMax-PCX->YMin+1, PCX->BPP);
  M = D+Sz-256*3;
  times (I, 256) {
    P->P[I*4+0] = M[I*3+0];
    P->P[I*4+1] = M[I*3+1];
    P->P[I*4+2] = M[I*3+2];
    P->P[I*4+3] = 0;
  }
  M = D+sizeof(PCXHeader);
  for(I = 0; I < P->W*P->H; ) {
    C = *M++;
    if ((C&0xC0) != 0xC0) P->D[I++] = C;
    else {
      C ^= 0xC0;
      while(C-- > 0) P->D[I++] = *M;
      M++;
    }
  }
  return P;
}

static int pcxLineRLE(FILE *F, u1 *P, int N) {
  int I, V, T, C;
  for (T=I=0; I < N; ) {
    V = P[I++];
    C = 1;
    while (I < N && P[I] == V && C < 63) {C++; I++;}
    if (C == 1 && (V&0xC0) != 0xC0) {
      putc(V, F);
      T += 1;
    } else {
      putc(C|0xC0, F);
      putc(V, F);
      T += 2;
    }
  }
  return T;
}

void pcxSave(char *File, pic *P) {
  int I, X, Y;
  FILE *F;
  u1 *L;
  PCXHeader PH;
  memset(&PH, 0, sizeof(PCXHeader));
  PH.Manufacturer = 10;
  PH.Version = 5;
  PH.Encoding = 1;
  PH.BPP = 8;
  PH.XMax = P->W-1;
  PH.YMax = P->H-1;
  PH.HDpi = 300;
  PH.VDpi = 300;
  PH.NPlanes = (P->B==8?1:3); // lame linux tools dont support 32-bit PCX files
  PH.BytesPerLine = P->W; // it should be aligned by two
  PH.PaletteInfo = 1;

  writeFile(0, 0, File, File);
  F = fopen(File, "wb");
  fwrite(&PH, 1, sizeof(PCXHeader), F);

  if (P->B > 8) { // PCX color planes are interleaved
    L = ns(u1, PH.BytesPerLine);

    times (Y, P->H) {
      for (I=PH.NPlanes-1; I>=0; I--) {
        times (X, P->W) L[X] = P->D[Y*P->I + X*P->B/8 + I];
        pcxLineRLE(F, L, PH.BytesPerLine);
      }
    }

    free(L);
  } else {
    times (Y, P->H) pcxLineRLE(F, P->D+Y*P->I, (P->W*P->B+7)/8);
    fputc(0x0C, F); // VGA palette ID
    times(I, 256) {
      fputc(P->P[I*4+0], F);
      fputc(P->P[I*4+1], F);
      fputc(P->P[I*4+2], F);
    }
  }

  fclose(F);
}

void pcxSavePalette(char *File, u1 *Palette) {
  int I;
  pic *P = picNew(16,16,8);
  memcpy(P->P, Palette, 4*256);
  times(I, 256) P->D[I]=I;
  pcxSave(File, P);
  picDel(P);
}


int pcxInit(format *F) {
  F->Type = FMT_IMAGE;
  F->Name = "pcx";
  F->Description = "PCX images (ZSoft Paintbrush)";
  F->Save = pcxSave;
  F->Load = pcxLoad;
  return 1;
}

