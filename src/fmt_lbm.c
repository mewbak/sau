//Credits go to netpbm team (see ilbmtoppm.c)

#include "common.h"




#define CHK_FORM	0x464F524D
#define CHK_PBM		0x50424D20
#define CHK_BMHD	0x424D4844
#define CHK_CMAP	0x434D4150
#define CHK_DPPS	0x44505053
#define CHK_CRNG	0x43524E47
#define CHK_TINY	0x54494E59
#define CHK_BODY	0x424F4459

#define cmpNone         0
#define cmpByteRun1     1

#define mskNone                0
#define mskHasMask             1
#define mskHasTransparentColor 2
#define mskLasso               3

#define RowBytes(cols) ((((cols) + 15) / 16) * 2)

static int runLength(u1 *P, int I, int N) {
  int R = 1;
  while (R<128 && N-I>=2 && P[I+0]==P[I+1]) {R++; I++;}
  return R;
}

static int lbmLineRLE(u1 *O, u1 *P, int N) {
  int I, J, V, T, C, R;
  for (T=I=0; I < N; ) {
    C = 0, R = 1, J = I;
again:
    while (C < 128 && J < N && (R=runLength(P,J,N)) < 2) {C++; J++;}
    if (C && R == 2) {
      if (C<127) {
        R = 0;
        C += 2;
        J+=2;
        goto again;
      } else {
      }
    }
    if (C) {
      O[T++] = C-1;
      while(C--) O[T++] = P[I++];
    }
    if (R > 1) {
      O[T++] = 257-R;
      O[T++] = P[J];
      I += R;
    }
  }
  return T;
}

// FIXME: align chunks to 16-bit words
void lbmSave(char *FileName, pic *P) {
  u1 *D = ns(u1, 1024*1024*8);
  int B, I, J, L = 0;

  s4bePut(D+L, CHK_FORM); L+=4;
  s4bePut(D+L, 0); L+=4;
  s4bePut(D+L, CHK_PBM); L+=4; // PBM (Planar Bitmap) type LBM

  s4bePut(D+L, CHK_BMHD); L+=4;
  s4bePut(D+L, 0x14); L+=4;
  s2bePut(D+L, P->W); L+=2;
  s2bePut(D+L, P->H); L+=2;
  s2bePut(D+L, 0); L+=2; // x-origin
  s2bePut(D+L, 0); L+=2; // y-origin
  D[L++] = P->B; // number of color planes
  D[L++] = 0; // color key type (mask)
  D[L++] = cmpByteRun1; // compression type
  D[L++] = 0; // color map flags
  s2bePut(D+L, 0xFF); L+=2; // color key
  //s2bePut(D+L, 0x0506); L+=2; // aspect ratio
  //s2bePut(D+L, 320); L+=2; // page width
  //s2bePut(D+L, 200); L+=2; // page height
  s2bePut(D+L, 0x0101); L+=2; // aspect ratio
  s2bePut(D+L, 640); L+=2; // page width
  s2bePut(D+L, 480); L+=2; // page height

  s4bePut(D+L, CHK_CMAP); L+=4;
  s4bePut(D+L, 3*256); L+=4;
  times(I, 256) {
    D[L++] = P->P[I*4+0];
    D[L++] = P->P[I*4+1];
    D[L++] = P->P[I*4+2];
  }


  s4bePut(D+L, CHK_DPPS); L+=4;
  s4bePut(D+L, 110); L+=4;
  s2bePut(D+L+0, 2);
  s2bePut(D+L+14, 360);
  s2bePut(D+L+18, 320);
  s2bePut(D+L+20, 240);
  s2bePut(D+L+22, 2);
  s2bePut(D+L+24, 15);
  s2bePut(D+L+26, 1);
  s2bePut(D+L+30, 1);
  s2bePut(D+L+34, 1);

  s2bePut(D+L+74, 1);
  s2bePut(D+L+90, 1);
  s2bePut(D+L+106, 1);

  L += 110;

  //Deluxe Paint normally writes 4 CRNG chunks in an ILBM when the user asks it to "Save Picture". 
  times (I,16) {
    s4bePut(D+L, CHK_CRNG); L+=4;
    s4bePut(D+L, 8); L+=4;

    s2bePut(D+L, 0); L+=2; // padding;
    s2bePut(D+L, 0); L+=2; // color cycle rate
    s2bePut(D+L, 0); L+=2; // cycle colors, if nonzero

    // here we select upper and lower color registers
    if (I == 0) {s2bePut(D+L, 0x101F); L+=2;}
    else if (I == 1) {s2bePut(D+L, 0x202F); L+=2;}
    else if (I == 2) {s2bePut(D+L, 0x606F); L+=2;}
    else if (I == 3) {s2bePut(D+L, 0x909F); L+=2;}
    else {s2bePut(D+L, 0x0000); L+=2;}
  }

#if 0
  s4bePut(D+L, CHK_TINY); L+=4;
  s4bePut(D+L, 0x100B); L+=4;
  L += 0x100B+1;
#endif

  s4bePut(D+L, CHK_BODY); L+=4;
  s4bePut(D+L, 0); L+=4;
  B = L;

  times(I, P->H) L += lbmLineRLE(D+L, P->D+I*P->I, (P->W+1)/2*2);

  s4bePut(D+4, L-8);
  s4bePut(D+B-4, L-B);

  if(L&1) D[L++] = 0;

  writeFile(0, L, FileName, D);

  free(D);
}

static void rev(void *X, int N) {
  int T, I;
  u1 *P = (u1*)X;
  times (I, N/2) {T = P[I]; P[I] = P[N-I-1]; P[N-I-1] = T;}
}


typedef struct {
  u4 Len;
  u2 W;
  u2 H;
  u2 X;
  u2 Y;
  u1 BPP;
  u1 ColorKeyType;
  u1 Enc;
  u1 CMAPFlags;
  u2 ColorKey;
  u2 AspectRatio;
  u2 PageWidth;
  u2 PageHeight;
} __attribute__ ((packed)) bmhd;


static u1 *decodeRLE(u1 *D, u1 *S, int ULen) {
  int C;
  u1 *End = D+ULen;

  while (D < End) {
    C = *S++;
    if (C < 128) {
      C += 1;
      while (C--) *D++ = *S++;
    } else if (C > 128) {
      C = 257-C;
      while (C--) *D++ = *S;
      *S++;
    } else {
       //printf("NOOP\n");
    }
  }

  return S;
}

static const u1 bit_mask[] = {1, 2, 4, 8, 16, 32, 64, 128};
static u1 *decodeRow(u4 *D, u1 *S, bmhd *H) {
  int plane, col, cols, cbit, bytes, nPlanes;
  u1 *ilp;
  u1 *Line = ns(u1,H->W*5);
  u4 *chp;

  cols = H->W;
  bytes = RowBytes(cols);
  nPlanes = H->BPP + (H->ColorKeyType==mskHasMask ? 1 : 0);
  for (plane = 0; plane < nPlanes; plane++) {
    int mask = 1 << plane;
    if (H->Enc == 1) S = decodeRLE(Line, S, bytes);
    else if (H->Enc == 2) {
      memcpy(Line, S, bytes);
      S += bytes;
    } else {
      printf("  Unknown encoding (%d)\n", H->Enc);
      abort();
    }

    ilp = Line;
    chp = D;

    cbit = 7;
    for(col = 0; col < cols; col++, cbit--, chp++) {
      if (cbit < 0) {
        cbit = 7;
        ilp++;
      }
      if(*ilp & (1<<cbit)) *chp |= mask;
      else *chp &= ~mask;
    }
  }

  return S;
}

// ILBMs are made for Amiga and have interleaved bitplanes
// PBMs made for PC and have all planes combined into bytes
pic *lbmLoad(char *Input) {
  int L = fileSize(Input);
  u1 *M = readFile(0, L, Input);
  pic *P;
  u1 *Q, *Z, *End=M+L;
  u4 *Line;
  int I, J, K, N, X, Y, C, Len, NColors=0;
  u1 Type[5],*CMAP=0,*BMHD=0,*BODY=0,*CRNG=0,*TINY=0,*DPPS=0, *CAMG=0, *Pal=0;
  bmhd *H=0;

  if (L<16 || strncmp(M,"FORM",4)) {
    printf ("  Not an IFF file\n");
    abort();
  }

  for (Q = M+12; Q+8 < End; Q += Len) {
    memcpy(Type, Q, 4);
    Type[4] = 0;
    Q += 4;
    if      (!strcmp(Type,"BMHD")) BMHD = Q; // Bitmap Header
    else if (!strcmp(Type,"BODY")) BODY = Q; // Pixels
    else if (!strcmp(Type,"CMAP")) CMAP = Q; // Color Map
    else if (!strcmp(Type,"CRNG")) CRNG = Q; // Color Cycling Ranges
    else if (!strcmp(Type,"CAMG")) CAMG = Q; // Amiga Display Mode
    else if (!strcmp(Type,"TINY")) TINY = Q; // Thumbnail
    else if (!strcmp(Type,"DPPS")) DPPS = Q; // Deluxe Paint Program Settings
    else {
      printf ("  Unknown IFF chunk (%s)\n", Type);
      //break;
    }
    Len = ru4(Q);
    rev(&Len, 4);
    if (Len%2) Len++; // odd-sized chunks have a padding byte
    if (Len < 0) break;
  }

  if (CMAP) {
    Q = CMAP;
    Len = ru4(Q);
    rev(&Len, 4);
    NColors = Len/3;
    Pal = ns(u1, max(256, NColors)*4);
    times (I, NColors) {
      Pal[I*4+0] = *Q++;
      Pal[I*4+1] = *Q++;
      Pal[I*4+2] = *Q++;
      Pal[I*4+3] = 0;
    }
  }

  if (BMHD) {
    H = (bmhd*)BMHD;
    rev(&H->Len,4);
    rev(&H->W,2);
    rev(&H->H,2);
    rev(&H->X,2);
    rev(&H->Y,2);
    rev(&H->ColorKey,2);

    printf("  %dx%dx%d:%d,%d:%d\n", H->W, H->H, H->BPP, H->X, H->Y, H->Enc);

    unless (H->BPP > 8 || Pal) {
      printf("  CMAP is missing\n");
      abort();
    }

    if (H->ColorKeyType == mskHasMask) {
      printf("  cant handle masked LBMs\n");
      abort();
    }

    P = picNew(H->W, H->H, H->BPP > 8 ? 32 : 8);

    // if images you decode gets garbled, use the following instead
    //P = picNew(H->W+H->W%2, H->H, H->BPP > 8 ? 32 : 8);

    unless (H->BPP > 8) memcpy(P->P, Pal, 256*4);
    Line = ns(u4,H->W*4);
  } else {
    printf("  BMHD is missing\n");
    abort();
  }

  unless (BODY) {
    printf("  BODY is missing\n");
    abort();
  }


  if (NColors > 256) {
    printf("  Cant handle %d-color images\n", NColors);
    abort();
  }


  memcpy(Type, M+8, 4);
  Type[4] = 0;
  if (!strcmp(Type,"PBM ")) {
    Z = P->D;
    Q = BODY;
    Len = ru4(Q);
    if (H->BPP != 8) {
      printf("  Cant handle %d-bit PBM files\n", H->BPP);
      abort();
    } else {
      if (H->Enc == cmpNone) {
        times (Y, H->H) {
          memcpy(Z+Y*P->W, Q, H->W);
          Q += H->W + H->W%2; //every row is aligned
        }
      } else if (H->Enc == cmpByteRun1) {
        decodeRLE(P->D, Q, P->W*P->H);
      } else {
        printf("  Unknown encoding (%d)\n", H->Enc);
        abort();
      }
    }
  } else if (!strcmp(Type,"ILBM")) {
    Z = P->D;
    Q = BODY;
    Len = ru4(Q);
    if (H->BPP > 8) {
      printf("  Cant handle %d-bit ILBM files\n", H->BPP);
      abort();
    } else {
      times (Y, H->H) {
        Q = decodeRow(Line, Q, H);
        times (X, H->W) picPut(P, X, Y, Line[X]);
        //skip mask here
        Z += H->W;
      }
    }
  } else {
    printf("  Unsupported IFF file type (%s)\n", Type);
    abort();
  }

  return P;
}

int lbmInit(format *F) {
  F->Type = FMT_IMAGE;
  F->Name = "lbm";
  F->Description = "LBM/PBM images (Deluxe Paint)";
  F->Save = lbmSave;
  F->Load = lbmLoad;
  return 1;
}

