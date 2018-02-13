//Credits go to SNV

#include "common.h"

static u1 Game_Pal[] = {
 0x00, 0x00, 0x00, 0xDF, 0x8E, 0x4D, 0x9E, 0x5D,
 0x00, 0x9E, 0x9E, 0x9E, 0x7D, 0x7D, 0x7D, 0x6D,
 0x6D, 0x6D, 0x4D, 0x4D, 0x4D, 0xDF, 0xCF, 0xBE,
 0x5D, 0x00, 0x1C, 0x7D, 0x00, 0x1C, 0x9E, 0x2C,
 0x1C, 0xBE, 0x4D, 0x2C, 0xBE, 0x9E, 0x3C, 0x9E,
 0x6D, 0x2C, 0x8E, 0x4D, 0x1C, 0x6D, 0x2C, 0x00};

static u1 Intro6_Pal[] = {
 0x00, 0x00, 0x00, 0x00, 0x50, 0xE0, 0x30, 0x20,
 0x10, 0xD0, 0x90, 0x40, 0xA0, 0x70, 0x30, 0x80,
 0x50, 0x20, 0x50, 0x30, 0x10, 0x00, 0x60, 0x00,
 0x00, 0x10, 0x60, 0x00, 0x70, 0xE0, 0xC0, 0xF0,
 0xF0, 0x00, 0x40, 0x00, 0x00, 0x00, 0x90, 0x60,
 0xB0, 0xE0, 0x00, 0x40, 0xC0, 0x00, 0x20, 0x00};

static u1 Intro7_Pal[] = {
 0x00, 0x00, 0x00, 0xE0, 0xE0, 0xE0, 0xC0, 0xC0,
 0xC0, 0xA0, 0xA0, 0xA0, 0x70, 0x70, 0x70, 0x50,
 0x50, 0x50, 0x30, 0x30, 0x30, 0xF0, 0xB0, 0x60,
 0xC0, 0x80, 0x20, 0x90, 0x50, 0x00, 0x50, 0x00,
 0x10, 0x30, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x40,
 0x00, 0x00, 0xA0, 0x00, 0x00, 0xE0, 0x00, 0x00};

static u1 IntroA_Pal[] = {
 0x00, 0x00, 0x00, 0xD0, 0x90, 0x80, 0xC0, 0x80,
 0x60, 0xA0, 0x50, 0x40, 0x60, 0x30, 0x10, 0x50,
 0x20, 0x00, 0xE0, 0xE0, 0xE0, 0xB0, 0xB0, 0xB0,
 0xA0, 0xA0, 0xA0, 0x80, 0x80, 0x80, 0x70, 0x70,
 0x70, 0x50, 0x50, 0x50, 0xF0, 0xF0, 0xF0, 0x60,
 0x60, 0x60, 0x40, 0x70, 0x30, 0x30, 0x50, 0x20};

static u1 Endseq0_Pal[] = {
 0x10, 0x10, 0x10, 0x90, 0x90, 0xB0, 0x80, 0x80,
 0xA0, 0x70, 0x70, 0x80, 0x30, 0x30, 0x30, 0xB0,
 0xB0, 0xD0, 0xF0, 0xE0, 0xE0, 0x50, 0x50, 0x60,
 0x60, 0x60, 0x90, 0x50, 0x50, 0x80, 0x40, 0x40,
 0x60, 0x80, 0x80, 0xC0, 0x90, 0x90, 0xD0, 0x70,
 0x70, 0xA0, 0x60, 0x60, 0x70, 0x40, 0x40, 0x50};

static u1 Boat_Pal[] = {
 0x60, 0x60, 0x40, 0xB0, 0x60, 0x20, 0x90, 0xB0,
 0xD0, 0x40, 0x60, 0xD0, 0x40, 0x90, 0xD0, 0xB0,
 0xB0, 0xD0, 0x60, 0x60, 0x40, 0x90, 0x90, 0x60,
 0x40, 0x20, 0x00, 0xD0, 0x90, 0x40, 0xB0, 0x60,
 0x00, 0xF0, 0xD0, 0xB0, 0x40, 0x40, 0x20, 0xF0,
 0x20, 0x00, 0x60, 0x40, 0x00, 0x90, 0x40, 0x00};

static u1 Pig_Pal[] = {
 0x00, 0x00, 0x00, 0xE3, 0x92, 0x61, 0xE3, 0x92,
 0x00, 0x82, 0x51, 0x00, 0xC3, 0x82, 0x20, 0x61,
 0x61, 0x61, 0x30, 0x30, 0x30, 0xC3, 0x30, 0x30,
 0xB2, 0x61, 0x00, 0x92, 0x51, 0x10, 0x71, 0x41,
 0x10, 0x71, 0x92, 0x41, 0x61, 0x71, 0x41, 0x51,
 0x51, 0x51, 0x92, 0x30, 0x30, 0x82, 0x82, 0x82};

// RLE format is the same as in LBM
static u1 *decodeRLE(u1 *U, u1 *Q, int W) {
  int X=0, C;
  while (X<W) {
    C = *Q++;
    if (C<128) {
      C += 1;
      while (C--) U[X++] = *Q++;
    } else if (C>128) {
      C = 257-C;
      while (C--) U[X++] = *Q;
      *Q++;
    } else {
      //nop
    }
  }
  return Q;
}

static void bg(char *Output, u1 *M, int L, char *Name, u1 *Pal, int UB) {
  char Tmp[1024];
  int I, J, X, Y, C;
  u1 U[1024]; //holds unpacked bitplane
  u1 *Z, *T, *Q = M;
  int W = ru2be(Q); //width/8
  int H = ru2be(Q); //height
  int B = *Q++; //number of bitplanes
  int F = *Q++; //flags: 1=RLE, 2=hasPalette

  printf("%dx%dx%x\n", W*8,H,B);
  pic *P = picNew(W*8,H,8);

  if (F&2) {
    times(I, 1<<B) {
      P->P[I*4+0] = *Q++<<4;
      P->P[I*4+1] = *Q&0xF0;
      P->P[I*4+2] = *Q++<<4;
    }
  } else if (Pal) {
    times(I, 1<<B) {
      P->P[I*4+0] = Pal[I*3+0];
      P->P[I*4+1] = Pal[I*3+1];
      P->P[I*4+2] = Pal[I*3+2];
    }
  }

  Z = P->D;
  times (Y, H) {
    T = Z;
    times (J, B) {
      if (F&1) Q = decodeRLE(U,Q,W);
      else times (X, W) U[X] = *Q++;
      Z = T;
      times (X, W) {
        times (C, 8) *Z++ |= ((U[X]>>(7-C))&1) << J;
      }
    }
  }

  sprintf(Tmp, "%s/%s.png", Output, Name);
  pngSave(Tmp, P);
}


static void fg(char *Output, u1 *M, int L, char *Name, u1 *Pal, int UB) {
  char Tmp[1024];
  int I, J, X, Y, C, K;
  u1 *Z, *T;
  pic *P,*A;
  int N = *(u2*)M/2; //number of frames

  if (!Pal) {
    printf("fg: no palette provided!");
    abort();
  }


  times (I, N) {
    u1 *Q = M + *(u2*)(M+2*I);
    int X = (s2)ru2(Q);
    int Y = (s2)ru2(Q);
    int B = UB ? UB : 4;
    int F = 0;
    int W = ru2(Q); //width/8
    int H = ru2(Q);
    int Len = ru2(Q); //W*H

    if (strcmp(Name, "endseq1")) F = *Q++;

    printf("%03d:%04X: %dx%dx%d+%d,%d (%d) F=%x\n"
          , I, (int)(Q-M), W*8,H,B, X,Y, Len, F);

    P = picNew(W*8,H,8);
    P->K = 16;

    times(J, 1<<B) {
      P->P[J*4+0] = Pal[J*3+0];
      P->P[J*4+1] = Pal[J*3+1];
      P->P[J*4+2] = Pal[J*3+2];
    }

    // first goes mask plane
    Z = P->D;
    T = Q;
    times (Y, H) {
      times (X, W) {
        times (C, 8) *Z++ = ((*Q>>(7-C))&1) ? 0 : 16;
        ++Q;
      }
    }

    Q++;

    //B=3;
    // then color planes
    times (J, B) {
      Q = T+Len;
      T = Q;
      Z = P->D;
      times (Y, H) times (X, W) {
        times (C, 8) {
          if (*Z!=16) *Z |= ((*Q>>(7-C))&1) << J;
          Z++;
        }
        ++Q;
      }
    }
    sprintf(Tmp, "%s/%s_%03d.png", Output, Name,I);
    pngSave(Tmp, P);
  }
}


static void fg2(char *Output, u1 *M, int L, char *Name, u1 *Pal, int UB) {
  char Tmp[1024];
  int I, J, X, Y, C, K;
  u1 *Z, *T, *Q = M;
  pic *P;
  
  if (!Pal) {
    printf("fg2: no palette provided!");
    abort();
  }

  for (I=0; Q<M+L-16; I++) {
    int X = (s2)ru2(Q);
    int Y = (s2)ru2(Q);
    int B = UB ? UB : 4;
    int F = 0;
    int W = ru2(Q); //width/8
    int H = ru2(Q);
    int Len = ru2(Q); //W*H
    F = *Q++;
    if (W>1024/8||H>1000||!W || !H) break;



    printf("%03d:%04X: %dx%dx%d+%d,%d (%d) F=%x\n"
          , I, (int)(Q-M), W*8,H,B, X,Y, Len, F);

    P = picNew(W*8,H,8);
    P->K = 16;

    times(J, 1<<B) {
      P->P[J*4+0] = Pal[J*3+0];
      P->P[J*4+1] = Pal[J*3+1];
      P->P[J*4+2] = Pal[J*3+2];
    }

    // first goes mask plane
    Z = P->D;
    T = Q;
    times (Y, H) {
      times (X, W) {
        times (C, 8) *Z++ = ((*Q>>(7-C))&1) ? 0 : 16;
        ++Q;
      }
    }

    Q++;

    //B=3;
    // then color planes
    times (J, B) {
      Q = T+Len;
      T = Q;
      Z = P->D;
      times (Y, H) times (X, W) {
        times (C, 8) {
          if (*Z!=16) *Z |= ((*Q>>(7-C))&1) << J;
          Z++;
        }
        ++Q;
      }
    }
    sprintf(Tmp, "%s/%s_%03d.png", Output, Name,I);
    pngSave(Tmp, P);
  }
}



static struct {
  char *Name;
  void *F;
  u1 *Pal; // most heimdail sprites dont contain palette
  int B;   // number of bitplanes, this files uses
} Hs[] = {
  {"axed", bg},

  {"baddy", fg, Game_Pal, 3},

  {"char1", fg, Game_Pal, 3},
  {"char2", fg, Game_Pal, 3},
  {"char3", fg, Game_Pal, 3},

  {"combat1", fg, Game_Pal, 4}, //wrong palette?

  // fixme doors 1-4 have wrong palette
  {"door0", fg, Game_Pal, 4},
  {"door1", fg, Game_Pal, 4},
  {"door2", fg, Game_Pal, 4},
  {"door3", fg, Game_Pal, 4},
  {"door4", fg, Game_Pal, 4},

  {"pit0", fg, Game_Pal, 4},
  {"pit1", fg, Game_Pal, 4},
  {"pit2", fg, Game_Pal, 4},
  {"pit3", fg, Game_Pal, 4},
  {"pit4", fg, Game_Pal, 4},
  {"pit5", fg, Game_Pal, 4},
  {"pit6", fg, Game_Pal, 4},
  {"pit7", fg, Game_Pal, 4},

  {"special", fg2, Game_Pal, 4},

  {"endseq0", fg, Endseq0_Pal},
  {"endseq1", bg},
  {"endseq2", bg},
  {"lngboat0", bg},
  {"lngboat1", fg, Boat_Pal},
  {"lngboat2", fg, Boat_Pal},
  {"lngboat3", fg, Boat_Pal},
  {"lngboat4", fg, Boat_Pal},

  //{"pig0", bg, Pig_Pal},
  {"pig1", fg, Pig_Pal, 3},
  {"pig2", fg, Pig_Pal, 3},
  {"pig3", fg, Pig_Pal, 3},
  {"pig4", fg, Pig_Pal, 3},
  {"pig5", fg, Pig_Pal, 3},
  {"pig6", fg, Pig_Pal, 3},
  {"pig7", fg, Pig_Pal, 3},

  {"intro1", bg,Intro7_Pal},
  {"intro2", bg},
  {"intro3", bg},
  {"intro4", bg},
  {"intro5", bg,Intro6_Pal},
  {"intro6", bg},
  {"intro7", bg},
  {"intro8", bg},
  {"intro9", bg},
  {"introa", bg},
  {"introb", bg},
  {"introc", bg,IntroA_Pal},
  {"introd", bg,IntroA_Pal},
  {"introe", bg},
  {"introf", bg},

  {0,0}
};

static void heimdallDecompile(char *Output, char *Input) {
  int I;
  char Tmp[1024], Name[256], Ext[32];
  int L = fileSize(Input);
  u1 *M = readFile(0, L, Input);

  pathParts(0, Name, Ext, Input);
  downcase(Name);
  downcase(Ext);
  if (strcmp(Ext, "bin") && strcmp(Ext, "dat")) {
    printf("use on BIN and DAT files\n");
    abort();
  }

  for (I=0; Hs[I].Name; I++) if (!strcmp(Hs[I].Name, Name)) {
    ((void (*)(char*, u1*, int, char*, u1*, int))Hs[I].F)
      (Output, M, L, Name, Hs[I].Pal, Hs[I].B);
    return;
  }

  printf("this file isnt supported\n");
  abort();
}


int heimdallInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "heimdall";
  F->Description = "Heimdall (use on *.bin and *.dat files)";
  F->Decompile = heimdallDecompile;
  return 1;
}

