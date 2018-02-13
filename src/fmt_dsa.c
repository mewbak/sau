//Credits go to Dmitry Groshev, who published original NVFVIEW.ASM
//              SNV (I converted it to C/C++)

#include "common.h"

typedef struct {
  u2 Flags;
  u1 Name[14];
  u4 Off;
} __attribute__ ((packed)) entry;

typedef struct {
  u1 Type;
  u2 NFrames;
} __attribute__ ((packed)) nvf;

typedef struct {
  u2 W;
  u2 H;
} __attribute__ ((packed)) nvf1;

typedef struct {
  u2 W;
  u2 H;
  u4 L;
} __attribute__ ((packed)) nvf3;

static void rev(void *X, int N) {
  int T, I;
  u1 *P = (u1*)X;
  times (I, N/2) {T = P[I]; P[I] = P[N-I-1]; P[N-I-1] = T;}
}

#define getBits(N) \
  ({int gbN=N, gbI; \
    u4 gbR = 0; \
    while (Avail < gbN) {Bits |= (*Src++ << Avail); Avail+=8;} \
    times(gbI, gbN) {gbR = (gbR<<1)|(Bits&1); Bits >>= 1; Avail--;} \
    gbR;})

static u1 *decodeLZSS(u1 *M, int L) {
  u4 Bits=0, Avail=0;
  int ULen, Skip, C, U, N, Off;
  u1 *Dst, *Src, *DstBase, *SrcBase, *P;

  Src = SrcBase = ns(u1, L);
  memcpy(Src, M, L);
  rev(Src, L);

  Skip = Src[0];
  ULen = Src[1]|(Src[2]<<8)|(Src[3]<<16);
  Dst = DstBase = ns(u1, ULen);

  Src += 4;

  while (Skip--) getBits(1);

  while (Dst < DstBase+ULen && Src < SrcBase+L) {
    C = getBits(1);
    if (!C) {
      C = 1;
      do {C += (U=getBits(2));} while (U==3);
      while (C-- && Dst < DstBase+ULen) *Dst++ = getBits(8);
      if (Dst >= DstBase+ULen) break;
    }
    C = getBits(2);
    N = M[4+C];
    if (C != 3) Off = getBits(N);
    else {
      if (!getBits(1)) N = 7;
      Off = getBits(N);
      C = 3;
      do {C += (U=getBits(3));} while (U==7);
    }
    C += 2;

    P = Dst-Off-1;
    while (C-- && Dst < DstBase+ULen) *Dst++ = *P++;
  }

  free(SrcBase);
  rev(DstBase, ULen);

  return DstBase;
}

static void decodeRLE(u1 *Dst, u1 *Src, int ULen, int Len) {
  int C;
  u1 *D=Dst, *S=Src;
  while (D < Dst+ULen) {
    C = *S++;
    if (C != 0x7F) *D++ = C;
    else {
      C = *S++;
      while (C--) *D++ = *S;
      S++;
    }
  }
}


static void decodeRLE2(u1 *Dst, u1 *Src, int ULen, int Len) {
  int C;
  u1 *D=Dst, *S=Src;
  //hd(Src, Len);
  while (D < Dst+ULen) {
    C = *S++;
    if (C < 0x80) {
      C += 1;
      while (C--) *D++ = *S++;
    } else {
      C = 0x101-C;
      while (C--) *D++ = *S;
      S++;
    }
  }
}


static void saveNVF(char *Output, u1 *M, int L) {
  char Tmp[1024];
  int I, J, X, Y, NColors;
  u1 *Q, *Z;
  nvf *H = (nvf*)M;
  pic *P, **Ps = ns(pic*, H->NFrames);
  if (H->Type == 0) {
    nvf1 *O = (nvf1*)(H+1);
    Q = (u1*)(O+1);
    times (I, H->NFrames) {
      Ps[I] = P = picNew(O->W, O->H, 8);
      times (Y, O->H) times (X, O->W) picPut(P, X, Y, *Q++);
    }
  } else if (H->Type == 1) {
    nvf1 *O = (nvf1*)(H+1);
    Q = (u1*)(O+H->NFrames);
    times (I, H->NFrames) {
      Ps[I] = P = picNew(O->W, O->H, 8);
      times (Y, O->H) times (X, O->W) picPut(P, X, Y, *Q++);
      O++;
    }
  } else if (H->Type == 2) {
    nvf1 *O = (nvf1*)(H+1);
    u4 *S = (u4*)(O+1);
    Q = (u1*)(S+H->NFrames);
    times (I, H->NFrames) {
      Ps[I] = P = picNew(O->W, O->H, 8);
      Z = decodeLZSS(Q, S[I]);
      memcpy(P->D, Z, O->W*O->H);
      free(Z);
      Q += S[I];
    }
  } else if (H->Type == 3) {
    nvf3 *O = (nvf3*)(H+1);
    Q = (u1*)(O+H->NFrames);
    times (I, H->NFrames) {
      Ps[I] = P = picNew(O->W, O->H, 8);
      Z = decodeLZSS(Q, O->L);
      memcpy(P->D, Z, O->W*O->H);
      free(Z);
      Q += O->L;
      O++;
    }
  } else if (H->Type == 4) {
    nvf1 *O = (nvf1*)(H+1);
    u4 *S = (u4*)(O+1);
    Q = (u1*)(S+H->NFrames);
    times (I, H->NFrames) {
      Ps[I] = P = picNew(O->W, O->H, 8);
      decodeRLE(P->D, Q, O->W*O->H, S[I]);
      Q += S[I];
    }
  } else if (H->Type == 5) {
    nvf3 *O = (nvf3*)(H+1);
    Q = (u1*)(O+H->NFrames);
    times (I, H->NFrames) {
      Ps[I] = P = picNew(O->W, O->H, 8);
      decodeRLE(P->D, Q, O->W*O->H, O->L);
      Q += O->L;
      O++;
    }
  } else {
    printf("    invalid NVF type (%d)\n", H->Type);
    abort();
  }

  NColors = ru2(Q);
  times (I, H->NFrames) {
    P = Ps[I];
    times (J, NColors) {
      //if (Q[J*3+0] == Q[J*3+1] == Q[J*3+2] == 0xFF) {} //use global palette
      P->P[J*4+0] = Q[J*3+0]<<2;
      P->P[J*4+1] = Q[J*3+1]<<2;
      P->P[J*4+2] = Q[J*3+2]<<2;
    }
    sprintf(Tmp, "%s/%04d.png", Output, I);
    pngSave(Tmp, P);
    picDel(P);
  }
}

typedef struct {
  u1 Id[4];
  u2 Type;
  u2 W;
  u2 H;
  u2 U1; //0x100 (number of colors?)
  u1 U2[18];  //0-bytes
} __attribute__ ((packed)) aif;

static void saveAIF(char *Output, u1 *M, int L) {
  char Tmp[1024];
  int I, J, NColors;
  u1 *Q, *Z;
  aif *H = (aif*)M;
  pic *P = picNew(H->W, H->H, 8);
  int Len = L-sizeof(aif)-0x300;
  Q = (u1*)(H+1);
  if (H->Type == 0) memcpy(P->D, Q, H->W*H->H);
  else if (H->Type == 1) decodeRLE(P->D, Q, H->W*H->H, Len);
  else if (H->Type == 2) decodeRLE2(P->D, Q, H->W*H->H, Len);
  else if (H->Type == 3) {
    Z = decodeLZSS(Q, Len);
    memcpy(P->D, Z, H->W*H->H);
    free(Z);
  } else {
    printf("    invalid AIF type (%d)\n", H->Type);
    abort();
  }
  NColors = 0x100;
  Q = M+L-NColors*3;
  times (J, NColors) {
    //if (Q[J*3+0] == Q[J*3+1] == Q[J*3+2] == 0xFF) {} //use global palette
    P->P[J*4+0] = Q[J*3+0]<<2;
    P->P[J*4+1] = Q[J*3+1]<<2;
    P->P[J*4+2] = Q[J*3+2]<<2;
  }
  if (H->W && H->H) { //libpng sucks
    sprintf(Tmp, "%s.png", Output, I);
    pngSave(Tmp, P);
  }
  picDel(P);
}


typedef struct {
  u1 Id[0x20];
  u2 W;
  u2 H;
  u2 NColors;
} __attribute__ ((packed)) raw;


static void saveRAW(char *Output, u1 *M, int L) {
  char Tmp[1024];
  int I, J, NColors;
  u1 *Q;
  raw *H = (raw*)M;
  pic *P;
  H->W+=1;
  H->H+=1;
  P = picNew(H->W, H->H, 8);
  memcpy(P->D, M+sizeof(raw)+H->NColors*3, H->W*H->H);

  Q = M+sizeof(raw);
  memset(P->P, 0, 256*4);
  times (J, H->NColors) {
    //if (Q[J*3+0] == Q[J*3+1] == Q[J*3+2] == 0xFF) {} //use global palette
    P->P[J*4+0] = Q[J*3+0]<<2;
    P->P[J*4+1] = Q[J*3+1]<<2;
    P->P[J*4+2] = Q[J*3+2]<<2;
  }
  if (H->W && H->H) { //libpng sucks
    sprintf(Tmp, "%s.png", Output, I);
    pngSave(Tmp, P);
  }
  picDel(P);
}


typedef struct {
  u2 W;
  u2 H;
  u1 NFrames;
  u1 Looped;
} __attribute__ ((packed)) aceAnim1;

typedef struct {
  u4 Off;
  u2 Num;
  u2 W;
  u2 H;
  s2 X;
  s2 Y;
  u1 NFrames;
  u1 Looped;
} __attribute__ ((packed)) aceAnimN;

typedef struct {
  u4 L;
  s2 X;
  s2 Y;
  u2 W;
  u2 H;
  u1 Type;
  u1 Unused;
} __attribute__ ((packed)) aceFrame;

typedef struct {
  u1 Id[4];
  u2 U1; //1
  u1 NAnims;
  u1 Speed; // how fast animation should be played
} __attribute__ ((packed)) ace;

static void saveACE(char *Output, u1 *M, int L) {
  char Tmp[1024];
  int I, J, K, X, Y;
  u1 *Q, *Z;
  aceFrame *O;
  ace *Ace = (ace*)M;
  pic *P, **Ps, ***Pss = ns(pic**, Ace->NAnims);
  u4 *NF = ns(int, Ace->NAnims);

  if (Ace->NAnims == 1) {
    K = 0;
    aceAnim1 *H = (aceAnim1*)(Ace+1);
    Pss[K] = ns(pic*, H->NFrames);
    NF[K] = H->NFrames;
    Q = (u1*)(H+1);
    times (I, H->NFrames) {
      O = (aceFrame*)Q;
      Q = (u1*)(O+1);
      Pss[K][I] = P = picNew(O->W, O->H, 8);
      if (O->Type == 0) memcpy(P->D, Q, O->W*O->H);
      else if (O->Type == 1) decodeRLE(P->D, Q, O->W*O->H, O->L);
      else if (O->Type == 2) decodeRLE2(P->D, Q, O->W*O->H, O->L);
      else if (O->Type == 50) {
        Z = decodeLZSS(Q, O->L);
        memcpy(P->D, Z, O->W*O->H);
        free(Z);
      } else {
        printf("    invalid ACE type (%d)\n", O->Type);
        abort();
      }
      Q += O->L;
    }
    goto end;
  }


  aceAnimN *H = (aceAnimN*)(Ace+1);
  times (K, Ace->NAnims) {
    Pss[K] = ns(pic*, H->NFrames);
    NF[K] = H->NFrames;
    Q = M+H->Off;
    times (I, H->NFrames) {
      O = (aceFrame*)Q;
      Q = (u1*)(O+1);
      Pss[K][I] = P = picNew(O->W, O->H, 8);
      if (O->Type == 0) memcpy(P->D, Q, O->W*O->H);
      else if (O->Type == 1) decodeRLE(P->D, Q, O->W*O->H, O->L);
      else if (O->Type == 2) decodeRLE2(P->D, Q, O->W*O->H, O->L);
      else if (O->Type == 50) {
        Z = decodeLZSS(Q, O->L);
        memcpy(P->D, Z, O->W*O->H);
        free(Z);
      } else {
        printf("    Invalid ACE type (%d)\n", O->Type);
        abort();
      }
      Q += O->L;
    }
    H++;
  }

end:
  //NOTE: some ACE files use colors from global palette
  Q = M+L-0x300;
  times (K, Ace->NAnims) {
    times (I, NF[K]) {
      P = Pss[K][I];
      times (J, 256) {
        //if (Q[J*3+0] == Q[J*3+1] == Q[J*3+2] == 0xFF) {} //use global palette
        P->P[J*4+0] = Q[J*3+0]<<2;
        P->P[J*4+1] = Q[J*3+1]<<2;
        P->P[J*4+2] = Q[J*3+2]<<2;
      }
      sprintf(Tmp, "%s/%04d_%04d.png", Output, K, I);
      pngSave(Tmp, P);
      //picDel(P);
    }
  }
}


typedef struct {
  u1 Id[4];
  u1 NPages; //1
  u1 NAnims;
  u1 N3; //10
} __attribute__ ((packed)) bob;

typedef struct {
  u1 Flags; //0
  u2 NDescs; //number of phase descriptors
} __attribute__ ((packed)) phase;

typedef struct {
  u1 FrameNum;
  u1 Unknown;
  u2 Latency;
} __attribute__ ((packed)) phaseDesc;

typedef struct {
  u4 Len;
  u4 Off; //offset of additional descriptor, from start of bob2
  u4 W;
  u4 H;
  u4 NAnmis;
  //bobAnim [NAnims];
} __attribute__ ((packed)) bob2;

typedef struct {
  u1 Id[4]; //ASCII chars
  u2 X;
  u1 Y;
  u1 H;
  u2 W;
  u1 Flags; //0
  u1 NFrames;
  //u4 Offs[NFrames]; //relative to start of bob2
                      //all frames are compressed together
} __attribute__ ((packed)) bobAnim;

static void dsaDecompile(char *Output, char *Input) {
  char Tmp[1024], Name[256], Ext[32];
  int I, J, Len;
  u4 Id;
  int L = fileSize(Input);
  u1 *M = readFile(0, L, Input);
  u1 *Q;
  int NFiles = *(u2*)M;
  entry *E = (entry*)(M+2);


  times (I, NFiles) {
    if (!strcmp(E->Name, "DUMMY")) goto skip;
    Len = (I==NFiles-1 ? L : (E+1)->Off)-E->Off;
    if (!Len) goto skip;
    //if (strcmp("GHAUPTMA.ACE", E->Name)) goto skip;
    printf("  Extracting: %s\n", E->Name);

    pathParts(0, Name, Ext, E->Name);
    downcase(Ext);

    //printf("    %08X:%08X:%04X\n", E->Off, Len, E->Flags);

    Q = M+2+sizeof(entry)*NFiles+E->Off;
    if (!strcmp(Ext,"ace")) {
      sprintf(Tmp, "%s/ACE/%s", Output, Name);
      saveACE(Tmp, Q, Len);
    } else if (!strcmp(Ext,"aif")) {
      sprintf(Tmp, "%s/AIF/%s", Output, Name);
      saveAIF(Tmp, Q, Len);
    } else if (!strcmp(Ext,"nvf")) {
      sprintf(Tmp, "%s/NVF/%s", Output, Name);
      saveNVF(Tmp, Q, Len);
    } else if (!strcmp(Ext,"raw")) {
      sprintf(Tmp, "%s/RAW/%s", Output, Name);
      saveRAW(Tmp, Q, Len);
    } else {
      sprintf(Tmp, "%s/%s", Output, E->Name);
      writeFile(0, Len, Tmp, Q);
    }
skip:
    E++;
  }
}


int dsaInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "dsa";
  F->Description = "Realms of Arkania 2 (use on *.DAT files)";
  F->Decompile = dsaDecompile;
  return 1;
}

