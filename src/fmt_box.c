//Credits go to trass3r of OpenBB project

#include "common.h"
#include "bb.h"

typedef struct {
  u1 Name[256];
  u1 Path[256];
  u2 Year; // SYSTEMTIME
  u2 Month;
  u2 DayOfWeek;
  u2 Day;
  u2 Hour;
  u2 Minute;
  u2 Second;
  u2 Milliseconds;
  u4 Len;
  //u1 Data[Len];
} __attribute__ ((packed)) header;


typedef struct {
  u1 Magic[3]; //MFB
  u1 Version[3]; //101
  u2 Wide;
  u2 High;
  u2 X;
  u2 Y;
  u2 Flags; // 0x2=compressed, 1 and 4 are unknown
  u2 NFrames;
  //u1 Frames[];
} __attribute__ ((packed)) mfbHeader;



static int unRLE(u1 *in, int inSize, u1 *out, int outSize) {
  int inPos = 0, outPos = 0;
  int CK = in[0];

  while (outPos < outSize) {
    if (in[inPos] != CK) out[outPos++] = in[inPos++];
    else {
      u1 count = in[inPos + 1];
      inPos += 2;
      while (count--) out[outPos++] = CK;
    }
  }
  return CK;
}


static void saveSprite(char *Output, u1 *M, int L, int Pal) {
  int I, J, Len;
  char Tmp[1024];
  u1 *Q, *Z;
  pic *P;
  spr *S;
  mfbHeader *H = (mfbHeader*)M;
  Z = (u1*)(H+1);

  S = sprNew();
  S->Palette = ns(u1,4*256);
  memcpy(S->Palette, getPalette(Pal), 256*4);
  times(I, 256) S->Palette[I*4+3] = 0;

  S->NAnis = 1;
  S->Anis = ns(ani, S->NAnis);
  S->Anis[0].NFacs = 1;
  S->Anis[0].Facs = ns(fac,S->Anis[0].NFacs);
  S->Anis[0].Facs[0].NPics = H->NFrames;
  S->Anis[0].Facs[0].Pics = ns(pic*, S->Anis[0].Facs[0].NPics);

  //printf("    %dx%dx%d:%d:%d\n", H->Wide, H->High, H->NFrames, H->X,H->Y);
  times(I, H->NFrames) {
    P = S->Anis[0].Facs[0].Pics[I] = picNew(H->Wide, H->High, 8);
    P->SharedPalette = 1;
    free(P->P);
    P->P = S->Palette;
    if (H->Flags&2) {
      Len = ru4(Z);
      S->ColorKey = P->K = unRLE(Z, Len, P->D, H->Wide*H->High);
      Z += Len;
    } else {
      Len = H->Wide*H->High;
      memcpy(P->D, Z, Len);
      Z += Len;
    }
    //sprintf(Tmp, "%s/%03d.png", Output, I);
    //pngSave(Tmp, P);
  }
  sprintf(Tmp, "%s.gif", Output);
  gifSave(Tmp, S);
  sprDel(S);

}

static void boxDecompile(char *Output, char *Input) {  
  int I, Pal;
  char Tmp[1024], Ext[256], Name[256];
  int L = fileSize(Input);
  u1 *M, *Q;

  M = Q = readFile(0, L, Input);

  unless (!strncmp(Q, "BOX", 3)) {
    printf("  Not a BOX file.\n");
    abort();
  }
  Q += 8; //skip archive header

  while(Q<M+L) {
    header *H = (header*)Q; Q += sizeof(header);
    printf("  Extracting %s\n", H->Name);
    //printf("    Off=%08X Len=%06X\n", Q-M, H->Len);

    pathParts(0, Name, Ext, H->Name);
    downcase(Ext);

    if (!strcmp(Ext, "mfb")) {
      sprintf(Tmp, "%s/%s", Output, Name);
      Pal = 0;
      if (!strcmp(Name, "loading")) Pal = 3;
      else if (Name[0] == 'm' && !strcmp(Name+2, "back")) Pal = 3;
      else if (!strcmp(Name, "mission")) Pal = 2;
      else if (!strcmp(Name, "yakoff")) Pal = 3;
      else if (!strcmp(Name, "credits")) Pal = 3;
      saveSprite(Tmp, Q, H->Len, Pal);
    } else {
      sprintf(Tmp, "%s/%s", Output, H->Name);
      writeFile(0, H->Len, Tmp, Q);
    }
skip:
    Q += H->Len;
  }
}

int boxInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "box";
  F->Description = "Beasts & Bumpkins archives";
  F->Decompile = boxDecompile;
  return 1;
}

