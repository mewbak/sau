//Credits go to SCE


/*
TIM files can have up to 32 palettes per bitmap (4 or 8 bit). There is NO qualification for what pixels belong to what palette. It is not specified in any way shape or form. Therefore all use of the palettes are APPLICATION SPECIFIC.

This means that although you can convert a BMP to a TIM you cannot convert a TIM to a BMP if it has more than one palette. However, the problem gets WORSE. Not only is the TIM with a palette application specific. TIM's without a palette are application specific as well. Especially 16 bit TIM's.

It is perfectly valid to have palettes as part of the TIM bitmap for a 16bit TIM and use those palettes to decode 8 or 4 bit data inside the TIM (that is supposedly 16bit). I've seen ALL of these done. The latter is done in FF9. The former is done in FF7.
*/


#include "common.h"

typedef struct {
  u1 ID;
  u1 Version;
  u2 Reserved;
  u4 Flags;
} __attribute__ ((packed)) timHeader;

#define PMODE    7
#define HAS_CLUT 8

static int ModeBits[] = {4,8,16,24,0,0,0,0};

static pic *tim4(int W, int H, u1 *Q) {
  int X,Y;
  pic *P = picNew(W,H,8);
  times(Y,H) times(X,W) picPut(P,X,Y,X&1?*Q++>>4:*Q&0xF);
  return P;
}

static pic *tim8(int W, int H, u1 *Q) {
  int X,Y;
  pic *P = picNew(W,H,8);
  times(Y,H) times(X,W) picPut(P,X,Y,*Q++);
  return P;
}

static pic *tim16(int W, int H, u1 *Q) {
  int X,Y,R,G,B,A;
  pic *P = picNew(W,H,8);
  times(Y,H) times(X,W) {
    fromA1R5G5B5(A,B,G,R,*(u2*)Q);
    Q+=2;
    picPut32(P,X,Y,R8G8B8A8(R,G,B,0xFF));
  }
  return P;
}

static pic *tim24(int W, int H, u1 *Q) {
  int X,Y,R,G,B,A;
  pic *P = picNew(W,H,8);
  times(Y,H) times(X,W) {
    R=*Q++;
    G=*Q++;
    B=*Q++;
    picPut32(P,X,Y,R8G8B8A8(R,G,B,0xFF));
  }
  return P;
}

static pic *timDecompile(char *Output, char *Input) {
  char Tmp[1024], Name[256];
  int I, J, PW, PH, WW, W, H, NColors, R,G,B,A,Bits,Mixed=0;
  pic *P;
  u1 *Pal = 0;
  int L=fileSize(Input);
  timHeader *T = (timHeader*)readFile(0, L, Input);
  u1 *Q = (u1*)(T+1);

  if (T->ID != 0x10) {
    printf("Not a Playstation TIM file\n");
    abort();
  }

  pathParts(0, Name, 0, Input);

  if (T->Flags&HAS_CLUT) {
    NColors = (ru4(Q)-12)/2;
    Pal = ns(u1,4*NColors);
    ru2(Q); //x
    ru2(Q); //y
    PW=ru2(Q);
    PH=ru2(Q);

    times(J,NColors) {
      fromA1R5G5B5(A,B,G,R,*(u2*)Q);
      Q+=2;
      Pal[J*4+0] = R;
      Pal[J*4+1] = G;
      Pal[J*4+2] = B;
      Pal[J*4+3] = A*0xFF;
    }
  }

  Bits = ModeBits[T->Flags&PMODE];

  ru4(Q); //W*H-12
  ru2(Q); //x
  ru2(Q); //y
  WW=ru2(Q);
  H=ru2(Q);

  if (!Bits) {
    Mixed = 1;
    Bits = 4;
  }
  
again:
  W = WW*16/Bits;

  printf("%dx%dx%d Palette=%dx%d\n", W,H,Bits, PW, PH);
  if (Bits==4) P=tim4(W,H,Q);
  else if (Bits==8) P=tim8(W,H,Q);
  else if (Bits==16) P=tim16(W,H,Q);
  else if (Bits==24) P=tim24(W,H,Q);

  if (Bits <= 8) times (J,PH) {
    if (Pal) memcpy(P->P, Pal+J*(1<<Bits)*4, (1<<Bits)*4);
    sprintf(Tmp, "%s/%s_%02d_%d.png", Output, Name, Bits, J);
    pngSave(Tmp, P);
  } else {
    sprintf(Tmp, "%s/%s_%02d.png", Output, Name, Bits);
    pngSave(Tmp, P);
  }

  if (Mixed && Bits != 24) {
    if (Bits==4) Bits = 8;
    else if (Bits==8) Bits = 16;
    else if (Bits==16) Bits = 24;
    goto again;
  }
}

int timInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "tim";
  F->Description = "Playstation Sprite";
  F->Decompile = timDecompile;
  return 1;
}


