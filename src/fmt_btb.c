//Credits go to SNV

#include "common.h"


// Typical LZ with mask word, that says which of the following words are backrefs
static int unpack(u2 *D, u2 *S) {
  u2 C, M, B, *R, *P = D;

  for (B=0; ; B>>=1) {
    if (!B) { // done this mask?
      M = *S++;
      B = 0x8000;
    }
    C = *S++;
    if (!(M&B)) { //non backref?
      *P++ = C;
      continue;
    }

    if (!C) break; // zero backref ends decoding

    R = P - (C>>5);
    C = (C&0x1f)+2;
    while (C-- > 0) *P++ = *R++;
  }
  return (u1*)P - (u1*)D;
}

static void btbDecompile(char *Output, char *Input) {
  int I, J, K, C, Off, R,G,B,A, Len, X,Y, W,H;
  char Tmp[1024], Name[256], Ext[32];
  u1 *Z, Pal[4*256];
  pic *P;
  int L = fileSize(Input);
  u1 *M = readFile(0, L, Input);
  u4 *O = (u4*)M;
  int NFrames = *O/4;
  u1 *Q = M+4*NFrames;

  pathParts(0,Name,Ext,Input);

  downcase(Name);  
  downcase(Ext);

  if (!strcmp(Ext,"bin")) {
    W = 256;
    if (!strcmp(Name,"open")) W = 512;
    else if (!strcmp(Name,"naka")) W = 256;
    else if (!strcmp(Name,"mange")) W = 320;
    else if (!strcmp(Name,"end")) W = 320;
    else if (!strcmp(Name,"logous")) W = 320;
    else if (!strcmp(Name,"logomoji")) W = 256;
    else if (!strcmp(Name,"logoback")) W = 256;
    else if (!strcmp(Name,"scelogoa")) W = 320;
    H = (L-256*2)/W;
    P = picNew(W,H,8);
    P->K = 0;

    Q = M;
    times(J,256) {
      C = *(u2*)Q;
      Q+=2;
      fromA1R5G5B5(A,B,G,R,C);
      P->P[J*4+0] = R;
      P->P[J*4+1] = G;
      P->P[J*4+2] = B;
      P->P[J*4+3] = A*0xFF;
    }
    memcpy(P->D,Q,W*H);

    sprintf(Tmp, "%s/%s.png", Output, Name);
    pngSave(Tmp, P);
    return;
  } else if (!strcmp(Ext,"chr")) {
    //W=24; //objects1
    //W=32;
    //W=48; //objects2
    //W=64;
    W=72; //characters
    //W=80;
    //W=96;
    //W=120; //fire elementral
    //W=128;
    //W=144/3; //ship and dragon
    //W=72*4; //makai
    H = (L-0x800-256*2)/W;
    P = picNew(W,H,8);
    P->K = 0;

    Q = M+0x800;
    times(J,256) {
      C = *(u2*)Q;
      Q+=2;
      fromA1R5G5B5(A,B,G,R,C);
      P->P[J*4+0] = R;
      P->P[J*4+1] = G;
      P->P[J*4+2] = B;
      P->P[J*4+3] = A*0xFF;
    }
    memcpy(P->D,Q,W*H);

    sprintf(Tmp, "%s/%s.png", Output, Name);
    pngSave(Tmp, P);
    return;
  } else if (strcmp(Ext,"dat")) {
    printf("Use on CHR/DAT/BIN files\n");
    return;
  }

  Z = ns(u1,1024*1024);
  times (I, NFrames) {
    Off = O[I];
    Len = (I+1==NFrames ? L : O[I+1]) - Off;
    Q = M+Off;
    printf("%03d: Off=%08X Len=%06X\n", I, Off, Len);
    if (Len<=512) {
      times(J,Len/2) {
        C = *(u2*)Q;
        Q+=2;
        fromA1R5G5B5(A,B,G,R,C);
        Pal[J*4+0] = R;
        Pal[J*4+1] = G;
        Pal[J*4+2] = B;
        Pal[J*4+3] = A*0xFF;
      }
      continue;
    }

    Len = unpack(Z, Q);
    Q = Z;
    W = 256;
    H = Len*2/W;
    printf("  W=%d H=%d ULen=%06X\n", W, H, Len);
    if (!H) continue;

    P = picNew(W,H,8);
    memcpy(P->P,Pal,4*256);
    times (Y,H) times (X,W) {
      if (X&1) C = *Q++ >> 4;
      else C = *Q & 0xF;
      P->D[Y*W + X] = C;
    }
    P->K = 0;

    sprintf(Tmp, "%s/%s_%03d.png", Output, Name, I);
    pngSave(Tmp, P);
  }
}


int btbInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "btb";
  F->Description = "Beyond the Beyond (use on CHR/DAT/BIN files)";
  F->Decompile = btbDecompile;
  return 1;
}

