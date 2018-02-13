//Credits go to SNV

#include "common.h"

//It seems similar format was used in Kingdom of Paradise

typedef struct {
 u4 NFiles;
 u4 Offs[1/*NFiles*/];
} __attribute__ ((packed)) header;

typedef struct {
 u2 U0;
 u2 U1; //#130
 u4 U2;
 u4 U3;
 u4 U4;

 u4 Off; //PalW*PalH+0x10
 u4 U5;
 u4 PalH;
 u4 PalW;
} __attribute__ ((packed)) img;

typedef struct {
 u1 TileWidth; //usually 16
 u2 BH; //bg height
 u4 U1;
 u1 U2;
 u4 FH; //fg height
 u4 W; //unknown
} __attribute__ ((packed)) tiles;

typedef struct {
  u2 Type;
  u2 NFrames;
  u4 FramesOff;
} __attribute__ ((packed)) framesHeader;

typedef struct {
  u1 W; //low 4-bits hold SourceW/8
  s1 X; 
  u1 P; //low 4-bits selects palette (hi-bits hold palette shift?)
  s1 Y;
  u2 Col; //SourceX/8
  u2 Row; //SourceY/8
} __attribute__ ((packed)) line;

typedef struct {
  u1 U[10];
  u2 H; //Height/8
  u4 Lines; //offset to scanline descriptors
} __attribute__ ((packed)) frame;

typedef struct {
  u2 U;
  u2 NFrames;
  u2 Off; // offset to animFrame sequence
} __attribute__ ((packed)) anim;

typedef struct {
  u2 N; // index into frame table
  u2 W; // wait time in 1/24 of second?
} __attribute__ ((packed)) animFrame;

typedef struct {
  u2 X;
  u2 NAnims;
  u4 Off;
} __attribute__ ((packed)) animEntry;

//0x3F3F3F3F ("????") is used for NIL?


static void saveImage(char *Output, char *Path, u1 *M, int L, u1 *Fs, int FsL) {
  char Tmp[1024], Dir[1024], Name[256];
  int I, J, K, C, X, Y, TX, TY, TW, TH, CX, CY, XX, YY, AddX;
  img *H = (img*)M;
  framesHeader *FsH = (framesHeader*)Fs;
  frame *F =  (frame*)(Fs+FsH->FramesOff);
  tiles *TS;
  u1 *Pal = (u1*)(H+1);
  u1 *Q = M+H->Off+0x10;
  u1 *Z;
  int Wide, High;
  int NColors = (Q-Pal)/4;
  pic *P, *PP, *T;
  line *E;

  PP = picNew(H->PalW, H->PalH, 32);

  Z = PP->D;
  times (Y, H->PalH) {
    times(X, H->PalW) {
      *Z++ = *Pal++;
      *Z++ = *Pal++;
      *Z++ = *Pal++;
      *Pal = 0xFF;
      *Z++ = *Pal++;
    }
  }
  printf("  Extracting: %s/palette.png\n", Path);
  sprintf(Tmp, "%s/%s/palette.png", Output, Path);
  pngSave(Tmp, PP);

  TS = (tiles*)Q;
  Z = (u1*)(TS+1);

  Wide = TS->W;
  High = TS->BH;//(M+L-Q)/Wide;

  printf("    %dx%d Colors=%d Pal=%dx%d TileWidth=%d\n"
       , Wide, High, NColors, H->PalW, H->PalH, TS->TileWidth);

  P = picNew(Wide, High, 8);
  //times (I, H->PalH) {
    Q = Z;
    times (Y, High) {
      times(X, Wide/2) {
        picPut(P, X*2  , Y, *Q%16);
        picPut(P, X*2+1, Y, *Q/16);
        *Q++;
      }
    }
    //printf("  Extracting: %s/tiles%04d.png\n", Path, I);
    //sprintf(Tmp, "%s/%s/tiles%04d.png", Output, Path, I);
    //pngSave(Tmp, P);
  //}

  printf("  Extracting: %s/tiles.png\n", Path);
  sprintf(Tmp, "%s/%s/tiles.png", Output, Path);
  pngSave(Tmp, P);

  //TX = X/TS->TileWidth;
  //TY = Y/TS->TileWidth;

  // following is research on Type4
#if 0
  XX = YY = 0;
  times (I, FsH->NFrames) {
    T = picNew(128, 128, 32);
    E = (line*)(Fs+F[I].Lines);

    hd(F+I, sizeof(*F));
    printf("  Extracting: %s/tiles/%04d.png\n", Path, I);
    sprintf(Tmp, "%s/%s/tiles/%04d.png", Output, Path, I);

    AddX = 0;
    times (Y, F[I].H) {
      //hd(E, sizeof(*E));
      TW = (E->W&0xF)*8;
      //printf("%d\n", TW);
      times (TY, 8) {
        times (TX, TW) {
          CX = T->W/2;
          CY = T->H/2;
          C = picGet(P, TX+E->Col*8+XX, TY+E->Row*8+YY);
          picPut32(T, CX+TX, CY+TY+Y*8, picGet32(PP, C, E->P&0xF));
        }
      }
      AddX += TW;
      E++;
    }
    //printf("%d\n", AddX);
    XX += AddX;
    if (XX>=256) {
      XX = 0;
      YY += 8;
    }
    AddX = 0;
    pngSave(Tmp, T);
    //abort();
    picDel(T);
  }
#endif
}

// make && for i in input/data/obj/*/*.p10; do ./sau p10=$i dir=./out/$i; done
// make && for i in input/data/map/*/*.p00; do ./sau p10=$i dir=./out/$i; done
static void extract(char *Output, char *Path, u1 *M, int L, char *Ext) {
  char Tmp[1024];
  int I, J, Off, NextOff, Len, FramesLen;
  u1 *Q, *Frames=0;
  header *H = (header*)M;

  times (I, H->NFiles) {
    Off = H->Offs[I];

    NextOff = L;
    for (J = I+1; J < H->NFiles; J++) if (H->Offs[J]) {
      NextOff = H->Offs[J];
      break;
    }

    Len = NextOff - Off;
    Q = M+Off;

    if (!Off) continue;

    if (!strcmp(Ext, "p99")) {
      sprintf(Tmp, "%s/%04X.p10", Path, I);
      extract(Output, Tmp, Q, Len, "p10");
    } else if (!strcmp(Ext, "p10") && I == 0) {
      printf("  Extracting: %s/frames.bin\n", Path);
      sprintf(Tmp, "%s/%s/frames.bin", Output, Path);
      writeFile(0, Len, Tmp, Q);
      Frames = Q;
      FramesLen = Len;
    } else if (!strcmp(Ext, "p10") && I == 1) {
      saveImage(Output, Path, Q, L, Frames, FramesLen);
      continue;
    } else if (!strcmp(Ext, "p10") && I == 2) {
      printf("  Extracting: %s/sound.p20\n", Path);
      sprintf(Tmp, "%s/%s/sound.p20", Output, Path);
      writeFile(0, Len, Tmp, Q);
    } else if (!strcmp(Ext, "p00") && I == 0) {
      sprintf(Tmp, "%s/tileset", Path);
      extract(Output, Tmp, Q, Len, "p10");
    } else if (!strcmp(Ext, "p00") && I == 1) {
      sprintf(Tmp, "%s/map", Path);
      extract(Output, Tmp, Q, Len, "pXX");
    } else if (!strcmp(Ext, "p00") && I == 2) {
      printf("  Extracting: %s/dialog.bin\n", Path);
      sprintf(Tmp, "%s/%s/dialog.bin", Output, Path);
      writeFile(0, Len, Tmp, Q);
    } else {
      printf("  Extracting: %s/%04X.bin\n", Path, I);
      sprintf(Tmp, "%s/%s/%04X.bin", Output, Path, I);
      writeFile(0, Len, Tmp, Q);
    }
  }
}

static void p10Decompile(char *Output, char *Input) {
  char Ext[32];
  int L = fileSize(Input);
  u1 *M = readFile(0, L, Input);
  pathParts(0, 0, Ext, Input);
  extract(Output, ".", M, L, Ext);
}

int p10Init(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "p10";
  F->Description = "Popolocrois Archive (use on p00/p10/p99 files)";
  F->Decompile = p10Decompile;
  return 1;
}


