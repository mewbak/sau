//Credits go to SNV

#include "common.h"

typedef struct {
  u4 Id; // 0x0606033D
  u2 Version; // 0x0000
} __attribute__ ((packed)) header;

#define MAGIC 0x0606033D
#define NAME  0x8081E087
#define IMAGE 0xDC3C2177

typedef struct {
  u4 Id;
  u4 Len; // size of this chunk
  u1 Data[1];
} __attribute__ ((packed)) chunk;

typedef struct {
  u4 A; //1
  u4 B; //always 0x3521F539?
  u4 NFrames;
  u1 C; //always 0x01?
} __attribute__ ((packed)) seq;


typedef struct {
  u1 A; // sometimes 0, sometimes 1
  u4 Len; // W*H*BytesPerPixel + 32
  u4 B; // always 0x0002B5D5?
  u4 C; // always 0x0024430C?
  u2 D; // 0x0028 for 32-bit images, 0x0506 for 16-bit images
  u2 E; // always 0x1000?
  u2 F; // always 0x1000?
  u1 Type; //0x0E for 32-images 0x05 for 16-bit images
  u2 G; // always 0x1000?
  u2 W;
  u2 H;
  u4 Pitch; //W*4
  u2 Pad;
  //Data[W*H*BytesPerPixel];
} __attribute__ ((packed)) frame;


static void majDecompile(char *Output, char *Input) {
  char Tmp[1024], Name[256];
  int I, J, Off, Len, W, H;
  seq *S;
  frame *F;
  pic *P;
  int L = fileSize(Input);
  header *Header = (header*)readFile(0, L, Input);
  chunk *C = (chunk*)(Header+1);

  if (Header->Id != MAGIC) {
    printf("  Not a supported majesty *.group.bin file\n");
    abort();
  }

  Name[0] = 0;
  while ((u1*)C < (u1*)Header+L) {
    if (C->Id == NAME) strcpy(Name, C->Data);
    if (C->Id != IMAGE) goto next;

    S = (seq*)C->Data;
    
    printf("  NFrames=%d A=%08X B=0x%08X C=0x%02X\n", S->NFrames, S->A, S->B, S->C);

    F = (frame*)(S+1);

    times (J, S->NFrames) {
      printf("    W=%d H=%d F=0x%04X\n", F->W, F->H, F->F);
      //hd(F, 32);

      W = F->W;
      H = F->H;

      P = picNew(W,H,32);
      if (F->Type==0x000E) {
        u1 *Q = (u1*)(F+1);
        for (I = 0; I<W*H; I++) {
          int R,G,B,A;
          R = Q[I*4+0];
          G = Q[I*4+1];
          B = Q[I*4+2];
          A = Q[I*4+3];
          P->D[I*4+0] = B;
          P->D[I*4+1] = G;
          P->D[I*4+2] = R;
          P->D[I*4+3] = A;
        }
      } else if (F->Type==0x0005) {
        u2 *Q = (u1*)(F+1);
        for (I = 0; I<W*H; I++) {
          int R,G,B,A;
          fromA4R4G4B4(B, G, R,  A, Q[I]);
          P->D[I*4+0] = R;
          P->D[I*4+1] = G;
          P->D[I*4+2] = B;
          P->D[I*4+3] = A;
        }
      } else if (F->Type==0x0007) {
        u2 *Q = (u1*)(F+1);
        for (I = 0; I<W*H; I++) {
          int R,G,B,A;
          fromR5G5B5A1(A, R, G, B, Q[I]);
          P->D[I*4+0] = B;
          P->D[I*4+1] = G;
          P->D[I*4+2] = R;
          P->D[I*4+3] = A*0xFF;
        }
      } else {
        printf("  unsupported format: F->Type == 0x%04X\n", F->Type);
        abort();
      }

      sprintf(Tmp, "%s/%s_%d.png", Output, Name, J);
      pngSave(Tmp, P);
      picDel(P);
      F = (frame*)((u1*)F + F->Len);
    }

next:
    C = (chunk*)((u1*)C + C->Len + 4);
  }
}

int majInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "maj";
  F->Description = "Majesty for IPhone/IPad (use on *.group.bin files)";
  F->Decompile = majDecompile;
  return 1;
}


