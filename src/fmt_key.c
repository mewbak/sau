//Credits go to GemRB team

#include <fcntl.h>
#include "common.h"


typedef struct {
  u1 Id[8];
  u4 NBif;
  u4 NRes;
  u4 BifOff;
  u4 ResOff;
} __attribute__ ((packed)) header;


typedef struct {
  u4 Len;
  u4 Name;
  u2 NameLen;
  u2 Flags;
} __attribute__ ((packed)) bifEntry;

typedef struct {
  u1 Id[8];
  u2 Type;
  u4 Flags;
} __attribute__ ((packed)) resEntry;


typedef struct {
  char *Ext;
  char *Tag;
  char *CTag;
  void *Handler;
} type;

static type *Types;


static void initTypes() {
  int I;

  Types = ns(type, 0x10000);
  Types[0x0001].Ext = "bmp";
  Types[0x0002].Ext = "mve";
  Types[0x0004].Ext = "wav";
  Types[0x0004].Tag = "WAVC";
  Types[0x0005].Ext = "wfx";
  Types[0x0006].Ext = "plt";
  Types[0x0006].Tag = "PLT ";
  Types[0x03E8].Ext = "bam";
  Types[0x03E8].Tag = "BAM "; // animation
  Types[0x03E8].CTag = "BAMC"; // animation compressed
  Types[0x03E9].Ext = "wed";
  Types[0x03E9].Tag = "WED ";
  Types[0x03EA].Ext = "chu";
  Types[0x03EA].Tag = "CHUI";
  Types[0x03EB].Ext = "tis";
  Types[0x03EC].Ext = "mos";
  Types[0x03EC].Tag = "MOS"; // static graphics
  Types[0x03EC].CTag = "MOSC"; // static graphics compressed
  Types[0x03ED].Ext = "itm";
  Types[0x03ED].Tag = "ITM ";
  Types[0x03EE].Ext = "spl";
  Types[0x03EE].Tag = "SPL ";
  Types[0x03EF].Ext = "bcs";
  Types[0x03F0].Ext = "ids";
  Types[0x03F1].Ext = "cre";
  Types[0x03F1].Tag = "CRE ";
  Types[0x03F2].Ext = "are";
  Types[0x03F2].Tag = "AREA";
  Types[0x03F3].Ext = "dlg";
  Types[0x03F3].Tag = "DLG ";
  Types[0x03F4].Ext = "2da";
  Types[0x03F4].Tag = "2DA ";
  Types[0x03F5].Ext = "gam";
  Types[0x03F5].Tag = "GAME";
  Types[0x03F6].Ext = "sto";
  Types[0x03F6].Tag = "STOR";
  Types[0x03F7].Ext = "wmp";
  Types[0x03F7].Tag = "WMAP";
  Types[0x03F8].Ext = "eff";
  Types[0x03F8].Tag = "EFF ";
  Types[0x03F9].Ext = "bs";
  Types[0x03FA].Ext = "chr";
  Types[0x03FA].Tag = "CHR ";
  Types[0x03FB].Ext = "vvc";
  Types[0x03FC].Ext = "vef";
  Types[0x03FD].Ext = "pro"; // projectile
  Types[0x03FE].Ext = "bio"; // biography
  Types[0x044C].Ext = "bah";
  Types[0x0802].Ext = "ini";
  Types[0x0803].Ext = "src";

  times (I, 0x10000) {
    if (!Types[I].Ext) {
      Types[I].Ext = ns(char,6);
      sprintf(Types[I].Ext, "%04x", I);
    }
    if (!Types[I].Tag) Types[I].Tag = "";
  }
}

static char KeyDir[1024];

static u1 XorKey[] = {
  0x88, 0xa8, 0x8f, 0xba, 0x8a, 0xd3, 0xb9, 0xf5,
  0xed, 0xb1, 0xcf, 0xea, 0xaa, 0xe4, 0xb5, 0xfb,
  0xeb, 0x82, 0xf9, 0x90, 0xca, 0xc9, 0xb5, 0xe7,
  0xdc, 0x8e, 0xb7, 0xac, 0xee, 0xf7, 0xe0, 0xca,
  0x8e, 0xea, 0xca, 0x80, 0xce, 0xc5, 0xad, 0xb7,
  0xc4, 0xd0, 0x84, 0x93, 0xd5, 0xf0, 0xeb, 0xc8,
  0xb4, 0x9d, 0xcc, 0xaf, 0xa5, 0x95, 0xba, 0x99,
  0x87, 0xd2, 0x9d, 0xe3, 0x91, 0xba, 0x90, 0xca
};

static void decrypt2da(u1 *P, int L) {
  int I;
  times (I, L) P[I] ^= XorKey[I%64];
}



typedef struct {
  u1 Id[8];
  u2 Wide;
  u2 High;
  u2 Cols;
  u2 Rows;
  u4 BlkSize;
  u4 PalOff;
} __attribute__ ((packed)) mosHeader;


static void saveImage(char *Output, u1 *M, int L) {
  int I, J, C, X, Y, DX, DY, MX, MY, U, V, BW, BH, Pitch, PadPitch;
  u1 *Pals, *Pal, *Q, *Pix;
  pic *P;
  u4 *O;
  mosHeader *H = (mosHeader*)M;

  //printf("    %dx%d (%dx%dx%d)\n", H->Wide, H->High, H->Rows, H->Cols, H->BlkSize);

  BW = H->BlkSize;
  BH = H->BlkSize;

  Pals = M + H->PalOff;
  O = (u4*)(Pals + 256*4*H->Rows*H->Cols);
  Pix = (u1*)(O+H->Rows*H->Cols);

  P = picNew(H->Wide, H->High, 24);

  PadPitch = H->Wide%BW;
  if (!PadPitch) PadPitch = BW;

  if (H->BlkSize == 64) {
    times(Y, H->High) {
      times (X, H->Wide) {
        DX = X/64;
        DY = Y/64;
        MX = X%64;
        MY = Y%64;
        Pitch = (DX == H->Cols-1 ? PadPitch : 64);
        Pal = Pals + 256*4*((DY*H->Cols)+DX);
        Q = Pix + O[(DY*H->Cols)+DX];
        C = Q[MY*Pitch + MX]*4;
        picPut24(P, X, Y, R8G8B8(Pal[C+2],Pal[C+1],Pal[C+0]));
      }
    }
  } else {
    times(Y, H->High) {
      times (X, H->Wide) {
        DX = X/BW;
        DY = Y/BH;
        MX = X%BW;
        MY = Y%BH;
        Pitch = (DX == H->Cols-1 ? PadPitch : BW);
        Pal = Pals + 256*4*((DY*H->Cols)+DX);
        Q = Pix + O[(DY*H->Cols)+DX];
        C = Q[MY*Pitch + MX]*4;
        picPut24(P, X, Y, R8G8B8(Pal[C+2],Pal[C+1],Pal[C+0]));
      }
    }
  }

  pngSave(Output, P);

  picDel(P);
}


typedef struct {
  u1 Id[8];
  u2 NFrames;
  u1 NAnis;
  u1 ColorKey;
  u4 Frames;
  u4 Palette;
  u4 Cycles;
} __attribute__ ((packed)) bamHeader;

typedef struct {
  u2 Wide;
  u2 High;
  s2 X;
  s2 Y;
  u4 Off;
} __attribute__ ((packed)) bamFrame;

typedef struct {
  u2 NFrames;
  u2 Cycle;
} __attribute__ ((packed)) bamAnim;


static void saveSprite(char *Output, u1 *M, int L) {
  char Tmp[1024], Txt[1024];
  int I, J, K, C, X, Y, Off, RLE, OX, OY, Wide=16, High=16;
  u1 *Q;
  pic *P;
  spr *S;
  bamHeader *H = (bamHeader*)M;
  bamFrame *F = (bamFrame*)(M+H->Frames);
  bamAnim *A = (bamAnim*)(F+H->NFrames);
  u1 *Pal = M+H->Palette;
  u2 *Cycles = (u2*)(M+H->Cycles);


  //printf("    NFrames=%d\n", H->NFrames);

  times (I, 256) {
    C = Pal[I*4+0];
    Pal[I*4+0] = Pal[I*4+2];
    Pal[I*4+2] = C;
  }

  times(I, H->NFrames) {
    if (F[I].X*2 > Wide) Wide = F[I].X*2;
    if (F[I].Y*2 > High) High = F[I].Y*2;
    if ((F[I].Wide-F[I].X)*2 > Wide) Wide = (F[I].Wide-F[I].X)*2;
    if ((F[I].High-F[I].Y)*2 > High) High = (F[I].High-F[I].Y)*2;
  }

  times(J, H->NAnis) {
    unless(A[J].NFrames) continue;

    S = sprNew();
    S->Palette = ns(u1,4*256);
    memcpy(S->Palette, Pal, 256*4);

    S->NAnis = 1;
    S->Anis = ns(ani, S->NAnis);
    S->Anis[0].NFacs = 1;
    S->Anis[0].Facs = ns(fac,S->Anis[0].NFacs);
    S->Anis[0].Facs[0].NPics = A[J].NFrames;
    S->Anis[0].Facs[0].Pics = ns(pic*, S->Anis[0].Facs[0].NPics);
    S->ColorKey = H->ColorKey;

    times(K, A[J].NFrames) {
      P = S->Anis[0].Facs[0].Pics[K] = picNew(Wide, High, 8);
      P->SharedPalette = 1;
      free(P->P);
      P->P = S->Palette;
      P->K = H->ColorKey;

      picClear(P, H->ColorKey);

      I = Cycles[K+A[J].Cycle];

      if (I == 0xFFFF || !F[I].Wide || !F[I].High) continue;

      Off = F[I].Off&0x3FFFFFFF;
      RLE = (F[I].Off&0x80000000) == 0;
      //printf("    %04d:%08x:%d: %dx%d@%d,%d\n"
      //      , I, Off, RLE, F[I].Wide, F[I].High, F[I].X, F[I].Y);

      OX = P->W/2 - F[I].X;
      OY = P->H/2 - F[I].Y;
      //OX = 0;
      //OY = 0;

      X = Y = 0;
      if (RLE) {
        Q = M+Off;
        while (Y < F[I].High) {
          C = *Q++;
          if (C == H->ColorKey) {
            C = *Q++ + 1;
            while (C--) {
              picPut(P, OX+X++, OY+Y, H->ColorKey);
              if (X == F[I].Wide) {
                X = 0;
                Y++;
              }
            }
          } else {
            picPut(P, OX+X++, OY+Y, C);
            if (X == F[I].Wide) {
              X = 0;
              Y++;
            }
          }
        }
      } else {
        memcpy(P->D, M+Off, F[I].Wide*F[I].High);
      }
    }
    sprintf(Tmp, "%s/%03d.gif", Output, J);
    gifSave(Tmp, S);
    sprDel(S);
  }
}





#define IP_ACM_SIG 0x01032897
typedef struct {
  u4 signature;
  u4 samples;
  u2 channels;
  u2 rate;
  u2 levels : 4;
  u2 subblocks : 12;
} __attribute__ ((packed))ACM_Header;

typedef struct {
  // Parameters of ACM stream
  int levels, subblocks;
  FILE* file;
  int maxlen;

  // Bits
  u4 next_bits; // new bits
  int avail_bits; // count of new bits

  int sb_size, block_size;
  s2 *amp_buffer, *buff_middle;
  int* block_ptr;
} valueUnpacker;

typedef int (*FillerProc) (valueUnpacker*vu, int pass, int ind);


static u1 Table1 [27] =
    {0,  1,  2,   4,  5,  6,   8,  9, 10,
    16, 17, 18,  20, 21, 22,  24, 25, 26,
    32, 33, 34,  36, 37, 38,  40, 41, 42};
//Eng: in base-4 system it is:
//    000 001 002  010 011 012  020 021 022
//    100 101 102  110 111 112  120 121 122
//    200 201 202  210 211 212  220 221 222
static s2 Table2 [125] =
    { 0,  1,  2,  3,  4,   8,  9, 10, 11, 12,  16, 17, 18, 19, 20,  24, 25, 26, 27, 28,  32, 33, 34, 35, 36,
     64, 65, 66, 67, 68,  72, 73, 74, 75, 76,  80, 81, 82, 83, 84,  88, 89, 90, 91, 92,  96, 97, 98, 99,100,
    128,129,130,131,132, 136,137,138,139,140, 144,145,146,147,148, 152,153,154,155,156, 160,161,162,163,164,
    192,193,194,195,196, 200,201,202,203,204, 208,209,210,211,212, 216,217,218,219,220, 224,225,226,227,228,
    256,257,258,259,260, 264,265,266,267,268, 272,273,274,275,276, 280,281,282,283,284, 288,289,290,291,292};
//Eng: in base-8 system:
//    000 001 002 003 004  010 011 012 013 014 ...
//    100 101 102 103 104 ...
//    200 ...
//    ... 
static u1 Table3 [121] =
    {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A,
     0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A,
     0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A,
     0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A,
     0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A,
     0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A,
     0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A,
     0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A,
     0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A,
     0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A,
     0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA};


static void prepare_bits (valueUnpacker *vu, int bits) {
  while (bits > vu->avail_bits) {
    u1 one_byte;
    //this hack is needed for embedded files, so we don't read past the boundary
    if (ftell(vu->file)==vu->maxlen || !fread(&one_byte, 1, 1, vu->file))
      one_byte = 0;
    vu->next_bits |= ((u4)one_byte << vu->avail_bits);
    vu->avail_bits += 8;
  }
}

static int get_bits (valueUnpacker *vu, int bits) {
  prepare_bits (vu,bits);
  int res = vu->next_bits;
  vu->avail_bits -= bits;
  vu->next_bits >>= bits;
  return res;
}

static valueUnpacker *newValueUnpacker (int lev_cnt, int sb_count, FILE* handle, int len) {
  valueUnpacker *vu = n(valueUnpacker);
  vu->levels = lev_cnt;
  vu->subblocks = sb_count;
  vu->file = handle;
  vu->maxlen = len;
  vu->sb_size = 1<<vu->levels;
  vu->block_size = vu->sb_size * vu->subblocks;
  vu->amp_buffer = ns(s2,0x10000);
  vu->buff_middle = vu->amp_buffer + 0x8000;
  return vu;
}


// Filling functions:
static int return0 (valueUnpacker *vu, int pass, int ind) {
  return 0;
}

// used when the whole column #pass is zero-filled
static int zero_fill (valueUnpacker *vu, int pass, int ind) {
  int *sb_ptr = &vu->block_ptr [pass],
       step = vu->sb_size,
       i = vu->subblocks;
  do {
    *sb_ptr = 0;
    sb_ptr += step;
  } while (--i != 0);
  return 1;
}
static int linear_fill (valueUnpacker *vu, int pass, int ind) {
  int i;
  int mask = (1 << ind) - 1;
  s2* lb_ptr = vu->buff_middle + ( (-1l) << (ind-1) );

  for (i=0; i<vu->subblocks; i++)
    vu->block_ptr [i*vu->sb_size + pass] = lb_ptr [get_bits (vu,ind) & mask];
  return 1;
}

// column with number pass is filled with zeros, and also +/-1, zeros are repeated frequently
// efficiency (bits per value): 3-p0-2.5*p00, p00 - cnt of paired zeros, p0 - cnt of single zeros.
// it makes sense to use, when the freqnecy of paired zeros (p00) is greater than 2/3
static int k1_3bits (valueUnpacker *vu, int pass, int ind) {
  int i;
  for (i=0; i<vu->subblocks; i++) {
    prepare_bits (vu,3);
    if ((vu->next_bits & 1) == 0) {
      vu->avail_bits--;
      vu->next_bits >>= 1;
      vu->block_ptr [i*vu->sb_size + pass] = 0; if (++i == vu->subblocks) break;
      vu->block_ptr [i*vu->sb_size + pass] = 0;
    } else if ((vu->next_bits & 2) == 0) {
      vu->avail_bits -= 2;
      vu->next_bits >>= 2;
      vu->block_ptr [i*vu->sb_size + pass] = 0;
    } else {
      vu->block_ptr [i*vu->sb_size + pass] = (vu->next_bits & 4) ?
                                       vu->buff_middle[1] : vu->buff_middle[-1];
      vu->avail_bits -= 3;
      vu->next_bits >>= 3;
    }
  }
  return 1;
}

// column is filled with zero and +/-1
// efficiency: 2-P0. P0 - cnt of any zero (P0 = p0 + p00)
// use it when P0 > 1/3
static int k1_2bits (valueUnpacker *vu, int pass, int ind) {
  int i;
  for (i=0; i<vu->subblocks; i++) {
    prepare_bits (vu,2);
    if ((vu->next_bits & 1) == 0) {
      vu->avail_bits--;
      vu->next_bits >>= 1;
      vu->block_ptr [i*vu->sb_size + pass] = 0;
    } else {
      vu->block_ptr [i*vu->sb_size + pass] = (vu->next_bits & 2) ?
                                      vu->buff_middle[1] : vu->buff_middle[-1];
      vu->avail_bits -= 2;
      vu->next_bits >>= 2;
    }
  }
  return 1;
}

// all the -1, 0, +1 triplets
// efficiency: always 5/3 bits per value
// use it when P0 <= 1/3
static int t1_5bits (valueUnpacker *vu, int pass, int ind) {
  int i;
  for (i=0; i<vu->subblocks; i++) {
    char bits = (char) (get_bits (vu, 5) & 0x1f);
    bits = Table1 [bits];

    vu->block_ptr [i*vu->sb_size + pass] = vu->buff_middle[-1 + (bits & 3)];
    if (++i == vu->subblocks) break;
    bits >>= 2;
    vu->block_ptr [i*vu->sb_size + pass] = vu->buff_middle[-1 + (bits & 3)];
    if (++i == vu->subblocks) break;
    bits >>= 2;
    vu->block_ptr [i*vu->sb_size + pass] = vu->buff_middle[-1 + bits];
  }
  return 1;
}

// -2, -1, 0, 1, 2, and repeating zeros
// efficiency: 4-2*p0-3.5*p00, p00 - cnt of paired zeros, p0 - cnt of single zeros.
// makes sense to use when p00>2/3
static int k2_4bits(valueUnpacker *vu, int pass, int ind) {
  int i;
  for (i=0; i<vu->subblocks; i++) {
    prepare_bits (vu, 4);
    if ((vu->next_bits & 1) == 0) {
      vu->avail_bits--;
      vu->next_bits >>= 1;
      vu->block_ptr [i*vu->sb_size + pass] = 0; if (++i == vu->subblocks) break;
      vu->block_ptr [i*vu->sb_size + pass] = 0;
    } else if ((vu->next_bits & 2) == 0) {
      vu->avail_bits -= 2;
      vu->next_bits >>= 2;
      vu->block_ptr [i*vu->sb_size + pass] = 0;
    } else {
      vu->block_ptr [i*vu->sb_size + pass] =
        (vu->next_bits & 8)?
          ( (vu->next_bits & 4)? vu->buff_middle[2]: vu->buff_middle[1] ):
          ( (vu->next_bits & 4)? vu->buff_middle[-1]: vu->buff_middle[-2] );
      vu->avail_bits -= 4;
      vu->next_bits >>= 4;
    }
  }
  return 1;
}

// -2, -1, 0, 1, 2
// efficiency: 3-2*P0, P0 - cnt of any zero (P0 = p0 + p00)
// use when P0>1/3
static int k2_3bits (valueUnpacker *vu, int pass, int ind) {
  int i;
  for (i=0; i<vu->subblocks; i++) {
    prepare_bits (vu,3);
    if ((vu->next_bits & 1) == 0) {
      vu->avail_bits--;
      vu->next_bits >>= 1;
      vu->block_ptr [i*vu->sb_size + pass] = 0;
    } else {
      vu->block_ptr [i*vu->sb_size + pass] =
        (vu->next_bits & 4)?
          ( (vu->next_bits & 2)? vu->buff_middle[2]: vu->buff_middle[1] ):
          ( (vu->next_bits & 2)? vu->buff_middle[-1]: vu->buff_middle[-2] );
      vu->avail_bits -= 3;
      vu->next_bits >>= 3;
    }
  }
  return 1;
}

// all the +/-2, +/-1, 0  triplets
// efficiency: always 7/3 bits per value
// use it when p0 <= 1/3
static int t2_7bits (valueUnpacker *vu, int pass, int ind) {
  int i;
  for (i=0; i<vu->subblocks; i++) {
    char bits = (char) (get_bits (vu,7) & 0x7f);
    s2 val = Table2 [bits];

    vu->block_ptr [i*vu->sb_size + pass] = vu->buff_middle[-2 + (val & 7)];
      if ((++i) == vu->subblocks) break;
      val >>= 3;
    vu->block_ptr [i*vu->sb_size + pass] = vu->buff_middle[-2 + (val & 7)];
      if ((++i) == vu->subblocks) break;
      val >>= 3;
    vu->block_ptr [i*vu->sb_size + pass] = vu->buff_middle[-2 + val];
  }
  return 1;
}

// fills with values: -3, -2, -1, 0, 1, 2, 3, and double zeros
// efficiency: 5-3*p0-4.5*p00-p1, p00 - cnt of paired zeros, p0 - cnt of single zeros, p1 - cnt of +/- 1.
// can be used when frequency of paired zeros (p00) is greater than 2/3
static int k3_5bits (valueUnpacker *vu, int pass, int ind) {
  int i;
  for (i=0; i<vu->subblocks; i++) {
    prepare_bits (vu,5);
    if ((vu->next_bits & 1) == 0) {
      vu->avail_bits--;
      vu->next_bits >>= 1;
      vu->block_ptr [i*vu->sb_size + pass] = 0; if (++i == vu->subblocks) break;
      vu->block_ptr [i*vu->sb_size + pass] = 0;
    } else if ((vu->next_bits & 2) == 0) {
      vu->avail_bits -= 2;
      vu->next_bits >>= 2;
      vu->block_ptr [i*vu->sb_size + pass] = 0;
    } else if ((vu->next_bits & 4) == 0) {
      vu->block_ptr [i*vu->sb_size + pass] = (vu->next_bits & 8) ?
                                        vu->buff_middle[1]: vu->buff_middle[-1];
      vu->avail_bits -= 4;
      vu->next_bits >>= 4;
    } else {
      vu->avail_bits -= 5;
      int val = (vu->next_bits & 0x18) >> 3;
      vu->next_bits >>= 5;
      if (val >= 2) val += 3;
      vu->block_ptr [i*vu->sb_size + pass] = vu->buff_middle[-3 + val];
    }
  }
  return 1;
}

// fills with values: -3, -2, -1, 0, 1, 2, 3.
// efficiency: 4-3*P0-p1, P0 - cnt of all zeros (P0 = p0 + p00), p1 - cnt of +/- 1.
static int k3_4bits (valueUnpacker *vu, int pass, int ind) {
  int i;
  for (i=0; i<vu->subblocks; i++) {
    prepare_bits (vu,4);
    if ((vu->next_bits & 1) == 0) {
      vu->avail_bits--;
      vu->next_bits >>= 1;
      vu->block_ptr [i*vu->sb_size + pass] = 0;
    } else if ((vu->next_bits & 2) == 0) {
      vu->avail_bits -= 3;
      vu->block_ptr [i*vu->sb_size + pass] = (vu->next_bits & 4) ?
                                        vu->buff_middle[1]: vu->buff_middle[-1];
      vu->next_bits >>= 3;
    } else {
      int val = (vu->next_bits &0xC) >> 2;
      vu->avail_bits -= 4;
      vu->next_bits >>= 4;
      if (val >= 2) val += 3;
      vu->block_ptr [i*vu->sb_size + pass] = vu->buff_middle[-3 + val];
    }
  }
  return 1;
}

// fills with values: +/-4, +/-3, +/-2, +/-1, 0, and double zeros
// efficiency: 5-3*p0-4.5*p00, p00 - cnt of paired zeros, p0 - cnt of single zeros.
// makes sense to use when p00>2/3
static int k4_5bits (valueUnpacker *vu, int pass, int ind) {
  int i;
  for (i=0; i<vu->subblocks; i++) {
    prepare_bits (vu,5);
    if ((vu->next_bits & 1) == 0) {
      vu->avail_bits--;
      vu->next_bits >>= 1;
      vu->block_ptr [i*vu->sb_size + pass] = 0; if (++i == vu->subblocks) break;
      vu->block_ptr [i*vu->sb_size + pass] = 0;
    } else if ((vu->next_bits & 2) == 0) {
      vu->avail_bits -= 2;
      vu->next_bits >>= 2;
      vu->block_ptr [i*vu->sb_size + pass] = 0;
    } else {
      int val = (vu->next_bits &0x1C) >> 2;
      if (val >= 4) val++;
      vu->block_ptr [i*vu->sb_size + pass] = vu->buff_middle[-4 + val];
      vu->avail_bits -= 5;
      vu->next_bits >>= 5;
    }
  }
  return 1;
}

// fills with values: +/-4, +/-3, +/-2, +/-1, 0, and double zeros
// efficiency: 4-3*P0, P0 - cnt of all zeros (both single and paired).
static int k4_4bits (valueUnpacker *vu, int pass, int ind) {
  int i;
  for (i=0; i<vu->subblocks; i++) {
    prepare_bits (vu,4);
    if ((vu->next_bits & 1) == 0) {
      vu->avail_bits--;
      vu->next_bits >>= 1;
      vu->block_ptr [i*vu->sb_size + pass] = 0;
    } else {
      int val = (vu->next_bits &0xE) >> 1;
      vu->avail_bits -= 4;
      vu->next_bits >>= 4;
      if (val >= 4) val++;
      vu->block_ptr [i*vu->sb_size + pass] = vu->buff_middle[-4 + val];
    }
  }
  return 1;
}

// all the pairs of values from -5 to +5
// efficiency: 7/2 bits per value
static int t3_7bits (valueUnpacker *vu, int pass, int ind) {
  int i;
  for (i=0; i<vu->subblocks; i++) {
    char bits = (char) (get_bits (vu,7) & 0x7f);
    u1 val = Table3 [bits];

    vu->block_ptr [i*vu->sb_size + pass] = vu->buff_middle[-5 + (val & 0xF)];
    if (++i == vu->subblocks) break;
    val >>= 4;
    vu->block_ptr [i*vu->sb_size + pass] = vu->buff_middle[-5 + val];
  }
  return 1;
}

static FillerProc Fillers[32] = {
  zero_fill,
  return0,
  return0,
  linear_fill,
  linear_fill,
  linear_fill,
  linear_fill,
  linear_fill,
  linear_fill,
  linear_fill,
  linear_fill,
  linear_fill,
  linear_fill,
  linear_fill,
  linear_fill,
  linear_fill,
  linear_fill,
  k1_3bits,
  k1_2bits,
  t1_5bits,
  k2_4bits,
  k2_3bits,
  t2_7bits,
  k3_5bits,
  k3_4bits,
  return0,
  k4_5bits,
  k4_4bits,
  return0,
  t3_7bits,
  return0,
  return0
};



static void get_one_block(valueUnpacker *vu, int* block) {
  int i, pass,
      pwr = get_bits (vu,4) & 0xF,
      val = get_bits (vu,16) & 0xFFFF,
      count = 1 << pwr,
      v = 0;
  vu->block_ptr = block;


  for (i=0; i<count; i++) {
    vu->buff_middle[i] = v;
    v += val;
  }
  v = -val;
  for (i=0; i<count; i++) {
    vu->buff_middle[-i-1] = v;
    v -= val; 
  }

  for (pass=0; pass<vu->sb_size; pass++) {
    int ind = get_bits (vu,5) & 0x1F;
    if (!Fillers[ind](vu, pass, ind)) break;
  }
}

typedef struct {
  int levels, block_size;
  int* memory_buffer;
} subbandDecoder;

static subbandDecoder *newSubbandDecoder(int lev_cnt) {
  subbandDecoder *sd = n(subbandDecoder);
  sd->levels = lev_cnt;
  sd->block_size = 1 << lev_cnt;
  int memory_size = (sd->levels == 0)? 0: (3*(sd->block_size >> 1) - 2);
  if (memory_size) sd->memory_buffer = ns(int, memory_size);
  return sd;
}

static void sub_4d3fcc (s2* memory, int* buffer, int sb_size, int blocks) {
  int row_0, row_1, row_2, row_3, db_0, db_1;
  int i, j;
  int sb_size_2 = sb_size * 2,
    sb_size_3 = sb_size * 3;
  if (blocks == 2) {
    for (i=0; i<sb_size; i++) {
      row_0 = buffer[0];
      row_1 = buffer[sb_size];
      buffer [0] = buffer[0] + memory[0] + 2*memory[1];
      buffer [sb_size] = 2*row_0 - memory[1] - buffer[sb_size];
      memory [0] = (s2) row_0;
      memory [1] = (s2) row_1;

      memory += 2;
      buffer++;
    }
  } else if (blocks == 4) {
    for (i=0; i<sb_size; i++) {
      row_0 = buffer[0];
      row_1 = buffer[sb_size];
      row_2 = buffer[sb_size_2];
      row_3 = buffer[sb_size_3];

      buffer [0]         =  memory[0] + 2*memory[1] + row_0;
      buffer [sb_size]   = -memory[1] + 2*row_0     - row_1;
      buffer [sb_size_2] =  row_0     + 2*row_1     + row_2;
      buffer [sb_size_3] = -row_1     + 2*row_2     - row_3;

      memory [0] = (s2) row_2;
      memory [1] = (s2) row_3;

      memory += 2;
      buffer++;
    }
  } else {
    for (i=0; i<sb_size; i++) {
      int* buff_ptr = buffer;
      if (blocks &2) {
        row_0 = buff_ptr[0];
        row_1 = buff_ptr[sb_size];

        buff_ptr [0]       =  memory[0] + 2*memory[1] + row_0;
        buff_ptr [sb_size] = -memory[1] + 2*row_0     - row_1;
        buff_ptr += sb_size_2;

        db_0 = row_0;
        db_1 = row_1;
      } else {
        db_0 = memory[0];
        db_1 = memory[1];
      }

      for (j=0; j<blocks >> 2; j++) {
        row_0 = buff_ptr[0];  buff_ptr [0] =  db_0  + 2*db_1  + row_0;  buff_ptr += sb_size;
        row_1 = buff_ptr[0];  buff_ptr [0] = -db_1  + 2*row_0 - row_1;  buff_ptr += sb_size;
        row_2 = buff_ptr[0];  buff_ptr [0] =  row_0 + 2*row_1 + row_2;  buff_ptr += sb_size;
        row_3 = buff_ptr[0];  buff_ptr [0] = -row_1 + 2*row_2 - row_3;  buff_ptr += sb_size;

        db_0 = row_2;
        db_1 = row_3;
      }
      memory [0] = (s2) row_2;
      memory [1] = (s2) row_3;
      memory += 2;
      buffer++;
    }
  }
}

static void sub_4d420c (int *memory, int *buffer, int sb_size, int blocks) {
  int row_0, row_1, row_2, row_3, db_0, db_1;
  int i, j;
  int sb_size_2 = sb_size * 2,
    sb_size_3 = sb_size * 3;
  if (blocks == 4) {
    for (i=0; i<sb_size; i++) {
      row_0 = buffer[0];
      row_1 = buffer[sb_size];
      row_2 = buffer[sb_size_2];
      row_3 = buffer[sb_size_3];

      buffer [0]         =  memory[0] + 2*memory[1] + row_0;
      buffer [sb_size]   = -memory[1] + 2*row_0     - row_1;
      buffer [sb_size_2] =  row_0     + 2*row_1     + row_2;
      buffer [sb_size_3] = -row_1     + 2*row_2     - row_3;

      memory [0] = row_2;
      memory [1] = row_3;

      memory += 2;
      buffer++;
    }
  } else {
    for (i=0; i<sb_size; i++) {
      int* buff_ptr = buffer;
      db_0 = memory[0]; db_1 = memory[1];
      for (j=0; j<blocks >> 2; j++) {
        row_0 = buff_ptr[0];  buff_ptr [0] =  db_0  + 2*db_1  + row_0;  buff_ptr += sb_size;
        row_1 = buff_ptr[0];  buff_ptr [0] = -db_1  + 2*row_0 - row_1;  buff_ptr += sb_size;
        row_2 = buff_ptr[0];  buff_ptr [0] =  row_0 + 2*row_1 + row_2;  buff_ptr += sb_size;
        row_3 = buff_ptr[0];  buff_ptr [0] = -row_1 + 2*row_2 - row_3;  buff_ptr += sb_size;

        db_0 = row_2;
        db_1 = row_3;
      }
      memory [0] = row_2;
      memory [1] = row_3;

      memory += 2;
      buffer++;
    }
  }
}
static void decode_data(subbandDecoder *sd, int* buffer, int blocks) {
  if (!sd->levels) return; // no levels - no work
  int i;
  int *buff_ptr = buffer, *mem_ptr = sd->memory_buffer;
  int sb_size = sd->block_size >> 1; // current subband size

  blocks <<= 1;
  sub_4d3fcc ((s2*)mem_ptr, buff_ptr, sb_size, blocks);
  mem_ptr += sb_size;

  for (i=0; i<blocks; i++) buff_ptr [i*sb_size]++;

  sb_size >>= 1;
  blocks <<= 1;

  while (sb_size != 0) {
    sub_4d420c (mem_ptr, buff_ptr, sb_size, blocks);
    mem_ptr += sb_size << 1;
    sb_size >>= 1;
    blocks <<= 1;
  }
}

typedef struct {
  u1 riff_sig[4];
  u4 total_len_m8;
  u1 wave_sig[8];
  u4 formatex_len;
  u2 wFormatTag, nChannels;
  u4 nSamplesPerSec, nAvgBytesPerSec;
  u2 nBlockAlign, wBitsPerSample;
  u1 data_sig[4];
  u4 raw_data_len;
} __attribute__ ((packed)) RIFF_HEADER;

typedef struct {  u1 wavc_sig[4];  u1 wavc_ver[4];
  u4 uncompressed;
  u4 compressed;
  u4 headersize;
  u2 channels;
  u2 bits;
  u2 samplespersec;
  u2 unknown;
} __attribute__ ((packed)) WAVC_HEADER;


typedef struct {
  int samples; // total count of sound samples
  int channels;
  int samplerate;
  int samples_left; // count of unread samples
  int is16bit; // 1 - if 16 bit file, 0 - otherwise
  FILE* file; // file handle
  int maxlen;

  int levels, subblocks;
  int block_size;
  int *block, *values;
  int samples_ready;
  valueUnpacker* unpacker; // ACM-stream unpacker
  subbandDecoder* decoder; // IP's subband decoder
} acmReader;



static RIFF_HEADER riff = {
  {'R', 'I', 'F', 'F'},
  0,
  {'W', 'A', 'V', 'E', 'f', 'm', 't', ' '},
  16, 1, 2,
  22050, 22050*4,
  4, 16,
  {'d', 'a', 't', 'a'}
};

static int write_riff_header (void *memory, int samples, int channels, int samplerate) {
  riff.raw_data_len = samples * 2;
  riff.total_len_m8 = riff.raw_data_len + sizeof(RIFF_HEADER) - 8;
  riff.nChannels=channels;
  riff.nSamplesPerSec=samplerate;
  riff.nAvgBytesPerSec=channels*2*samplerate;
  memcpy(memory, &riff, sizeof(RIFF_HEADER) );
  return riff.total_len_m8 + 8;
}

static WAVC_HEADER wavc =
 {{'W','A','V','C'}, {'V','1','.','0'},0,0,28,2,16,22050,0x9ffdu};

static void write_wavc_header (FILE *fpoi, int samples, int channels, int compressed, int samplerate) {
  wavc.uncompressed=samples*2;
  wavc.compressed=compressed;
  wavc.channels=channels;
  wavc.samplespersec=samplerate;
  rewind(fpoi);
  fwrite(&wavc, 1, sizeof(WAVC_HEADER) ,fpoi );
}



static int make_new_samples(acmReader *acm) {
  get_one_block(acm->unpacker, acm->block);
  decode_data (acm->decoder, acm->block, acm->subblocks);
  acm->values = acm->block;
  acm->samples_ready = (acm->block_size > acm->samples_left) ? 
                       acm->samples_left : acm->block_size;
  acm->samples_left -= acm->samples_ready;
  return 1;
}

static int read_samples (acmReader *acm, s2* buffer, int count) {
  int i, res;
  for (res=0; res < count; res++) {
    if (acm->samples_ready == 0) {
      if (acm->samples_left == 0) break;
      if (!make_new_samples(acm)) break;
    }
    *buffer = *acm->values >> acm->levels;
    acm->values++;
    buffer++;
    acm->samples_ready--;
  }
  for (i=res; i<count; i++, buffer++) *buffer = 0;
  return res;
}

static acmReader *newAcmReader (int fhandle, int bytes) {
  ACM_Header hdr;
  char tmp[4];
  acmReader *R = n(acmReader);
  R->is16bit = 1;
  R->file=fdopen(fhandle,"rb");
  R->maxlen=bytes;

  R->maxlen+=ftell(R->file);
  fread(tmp,1,sizeof(tmp),R->file);
  
  if(!memcmp(tmp,"WAVC",4)) fseek(R->file,24,SEEK_CUR);
  else fseek(R->file,-4,SEEK_CUR);
  
  fread (&hdr, 1, sizeof(ACM_Header), R->file);
  if (hdr.signature != IP_ACM_SIG) {
    return 0;
  }
  R->samples_left = (R->samples = hdr.samples);
  R->channels = hdr.channels;
  R->samplerate = hdr.rate;
  R->levels = hdr.levels;
  R->subblocks = hdr.subblocks;
  
  R->block_size = (1 << R->levels) * R->subblocks;
  R->block = ns(u4,R->block_size);

  R->unpacker = newValueUnpacker(R->levels, R->subblocks, R->file, R->maxlen);
  R->decoder = newSubbandDecoder(R->levels);
  return R;
}

static u1 *decodeACM(int fhandle, int L, int forcestereo, int *OutLen) {
  int riff_chans;
  int samples_written;
  int rawsize=0;
  int cnt, cnt1;
  RIFF_HEADER riff;
  acmReader *acm;
  u1 *Z = 0;

  Z=0;
  //handling normal riff, it is not a standard way, but hopefully able to handle most of the files
  read(fhandle,&riff, 4);
  if(!memcmp(riff.riff_sig,"RIFF",4)) {    
    if(read(fhandle,&riff.total_len_m8, sizeof(RIFF_HEADER)-4 )==sizeof(RIFF_HEADER)-4 ) {
      // data_sig-wformattag=16
      if(riff.formatex_len != (u1*)riff.data_sig - (u1*)&riff.wFormatTag) {
        cnt=riff.formatex_len-24;
        lseek(fhandle,cnt,SEEK_CUR);
        read(fhandle,riff.data_sig,8);//8 = sizeof sig+sizeof length
        riff.formatex_len=16;
      }
      if(!memcmp(riff.data_sig,"fact",4)) { //skip 'fact' (an optional RIFF tag)
        cnt=riff.raw_data_len; 
        lseek(fhandle,cnt,SEEK_CUR);
        read(fhandle,riff.data_sig,8); //8 is still the same 
      }
      Z = ns(u1, riff.raw_data_len+sizeof(RIFF_HEADER));
      L-=sizeof(RIFF_HEADER);
      if(riff.raw_data_len > L)
      {
        riff.total_len_m8=L+sizeof(RIFF_HEADER);
        riff.raw_data_len=L;
      }
      memcpy(Z,&riff,sizeof(RIFF_HEADER) );
      samples_written = riff.raw_data_len+sizeof(RIFF_HEADER);
      read(fhandle, (u1*)Z+sizeof(RIFF_HEADER),riff.raw_data_len);
      return Z;
    }
  }
  lseek(fhandle,-4,SEEK_CUR);
  acm = newAcmReader(fhandle, L);
    
  cnt = acm->samples;
  riff_chans = acm->channels;
  if(forcestereo && (riff_chans==1)) riff_chans=2;
    
  Z=ns(u1, cnt*2+sizeof(RIFF_HEADER));
  samples_written = sizeof(RIFF_HEADER);  
  memset(Z,0,sizeof(Z));

  *OutLen = write_riff_header (Z, cnt, riff_chans, acm->samplerate);
    
  cnt1 = read_samples (acm, (s2*)(Z+samples_written), cnt);
  rawsize=cnt1*sizeof(s2);
  samples_written+=rawsize;
  cnt -= cnt1;

end:
  if (acm) {
    if (acm->decoder->memory_buffer) free(acm->decoder->memory_buffer);
    if (acm->unpacker->amp_buffer) free(acm->unpacker->amp_buffer);
    if (acm->file) fclose (acm->file);
    free(acm);
  }
  return Z;
}




typedef struct {
  u1 Id[8];
  u4 NFiles;
  u4 NTiles;
  u4 FTOff;
} __attribute__ ((packed)) bifHeader;

typedef struct {
  u4 Flags;
  u4 Off;
  u4 Len;
  u2 Type;
  u2 Unknown;
} __attribute__ ((packed)) fileEntry;


#include "zlib/zlib.h"
static int cbfUnpackFile( FILE *fIn ) {
  u4 namelen;
  char namebuf[1024], Tmp[1024];
  uLongf cmplen, uncmplen;
  FILE *fOut;
  void *destBuf, *srcBuf;

  if ( fread( &namelen, 4, 1, fIn ) != 1 ) return 0;
  if ( fread( namebuf, 1, namelen, fIn ) != namelen ) return 0;
  if ( fread( &uncmplen, 4, 1, fIn ) != 1 ) return 0;
  if ( fread( &cmplen, 4, 1, fIn ) != 1 ) return 0;

  sprintf(Tmp, "%s/sau/data/%s", KeyDir, *namebuf=='\\' ? namebuf+1 : namebuf);
  writeFile(0, 0, Tmp, Tmp);
  fOut = fopen( Tmp, "wb" );
  if ( fOut == NULL ) return 0;
  srcBuf = malloc( cmplen );
  destBuf = malloc( uncmplen );
  if ( !srcBuf || !destBuf || fread( srcBuf, 1, cmplen, fIn )!=cmplen )
    {
      fclose( fOut );
      if ( destBuf ) free( destBuf );
      if ( srcBuf ) free( srcBuf );
      return 0;
    }
  uncompress( destBuf, &uncmplen, srcBuf, cmplen );
  fwrite( destBuf, 1, uncmplen, fOut );
  fclose( fOut );
  free( destBuf );
  free( srcBuf );
  return 1;
}

static void cbfUnpack(char *filename) {
  char signature[4], version[4];
  FILE *fIn = fopen( filename, "rb" );
  fread( signature, 1, 4, fIn );
  fread( version, 1, 4, fIn );
  while( cbfUnpackFile( fIn ) );
  fclose( fIn );
}


static int bifcUnpackBlock( FILE *fIn, FILE *outfile ) {
  uLongf cmplen, uncmplen;
  void *destBuf, *srcBuf;
  int status;

  if ( fread( &uncmplen, 4, 1, fIn ) != 1 ) return 0;
  if ( fread( &cmplen, 4, 1, fIn ) != 1 ) return 0;

  srcBuf = malloc( cmplen );
  destBuf = malloc( uncmplen );

  if (fread( srcBuf, 1, cmplen, fIn )!=cmplen) {
    if ( destBuf ) free( destBuf );
    if ( srcBuf ) free( srcBuf );
    return 0;
  }

  status=uncompress( destBuf, &uncmplen, srcBuf, cmplen );
  fwrite( destBuf, 1, uncmplen, outfile );

  free( destBuf );
  free( srcBuf );

  if ( status == Z_OK ) return 1;
  else return 0;
}

static void bifcUnpack(char *filename) {
  char Tmp[1024], Name[128];
  char signature[4], version[4];
  u4 uncmplen;
  FILE *fOut, *fIn;

  pathParts(0, Name, 0, filename);

  fIn = fopen( filename, "rb" );

  sprintf(Tmp, "%s/sau/data/%s.bif", KeyDir, Name);
  writeFile(0, 0, Tmp, Tmp);
  fOut = fopen( Tmp, "wb" );

  fread( signature, 1, 4, fIn );
  fread( version, 1, 4, fIn );
  fread( &uncmplen, 4, 1, fIn );

  while(bifcUnpackBlock( fIn, fOut));

  fclose( fIn );
  fclose( fOut );
}

static char *DataDirs[] = {"sau/", "", "CD1/", "CD2/", "CD3/", "CD4/", "CD5/", 0};
static u1 *bifGetEntry(char *BifFile, int I, int TileIndex, int *Len) {
  int J, L;
  char Path[1024], Dir[1024], Name[128], Ext[32];
  u1 *M;
  bifHeader *H;
  fileEntry *E;
  char **DD;

again:
  pathParts(Dir, Name, Ext, BifFile);
  if (isalpha(*Dir)) *Dir = tolower(*Dir);
  for(DD = DataDirs; *DD; *DD++) {
    sprintf(Path, "%s/%s%s/%s.%s", KeyDir, *DD, Dir, Name, Ext);
    if (fileExist(Path)) goto found;
  }

  if (isalpha(*Dir)) *Dir = toupper(*Dir);
  for(DD = DataDirs; *DD; *DD++) {
    sprintf(Path, "%s/%s%s/%s.%s", KeyDir, *DD, Dir, Name, Ext);
    if (fileExist(Path)) goto found;
  }

  strcpy(Ext, "cbf");
  if (isalpha(*Dir)) *Dir = tolower(*Dir);
  for(DD = DataDirs; *DD; *DD++) {
    sprintf(Path, "%s/%s%s/%s.%s", KeyDir, *DD, Dir, Name, Ext);
    if (fileExist(Path)) goto found;
  }

  if (isalpha(*Dir)) *Dir = toupper(*Dir);
  for(DD = DataDirs; *DD; *DD++) {
    sprintf(Path, "%s/%s%s/%s.%s", KeyDir, *DD, Dir, Name, Ext);
    if (fileExist(Path)) goto found;
  }

  printf("    Cant open %s/%s\n", KeyDir, BifFile);
  printf("    Make sure BIFs have right case and copied from CD into data/\n");

  return 0;

found:

  //printf("    Located: %s\n", Path);

  if (!strcmp(Ext, "cbf")) {
    cbfUnpack(Path);
    goto again;
  }

  L = fileSize(Path);
  H = readFile(0,sizeof(bifHeader),Path);

  if (!memcmp(H->Id, "BIFC", 4)) {
    bifcUnpack(Path);
    goto again;
  }


  E = readFile(H->FTOff,H->NFiles*sizeof(fileEntry),Path);

  //printf("    %08x:%08x\n", E[I].Off, E[I].Len);

  *Len = E[I].Len;

  M = readFile(E[I].Off,E[I].Len,Path);
  free(H);
  free(E);

  //abort();
  return M;
}


static u1 *zlibUnpack(void *M, int L, int *OutLen) {
  char buffer[1024];
  char signature[4], version[4];
  u1 *Z;
  u4 uncmplen;
  FILE *fOut;
  FILE *fIn;
  uLongf T;

  T = *OutLen = *(u4*)(M+8);
  Z = ns(u1, *OutLen);
  uncompress(Z, &T, M+12, L);
  *OutLen = T;

  return Z;
}


//animate.ids contains monster names
static void keyDecompile(char *Output, char *Input) {
  fileList *FL;
  char Tmp[1024], OutName[512], Dir[512], Name[128], *Ext; 
  int Len, BifIndex, TileIndex, Index;
  int I, J, K, C, X, Y, NFiles, Id, SubType, FD;
  u1 *Q, *Z;
  int L = fileSize(Input);
  u1 *M = readFile(0, L, Input);
  header *H = (header*)M;
  bifEntry *B;
  resEntry *R;

  pathParts(KeyDir, 0, 0, Input);

  if (L < 1024 || memcmp(H->Id, "KEY V1  ", 8)) {
    printf("  Not an infinity engine key file.\n");
    abort();
  }

  initTypes();

  B = (bifEntry*)(M+H->BifOff);
  R = (resEntry*)(M+H->ResOff);

  printf("  %d files inside of %d BIFs.\n", H->NRes, H->NBif);

  times (I, H->NBif) {
    Q = M+B[I].Name;
    for (J=0; Q[J]; J++) if(Q[J]=='\\') Q[J]='/';
    if (*Q == '/') B[I].Name++;
    //printf("    %04d:%08x:%04x: %s\n", I, B[I].Len, B[I].Flags, M+B[I].Name);
  }

  times (I, H->NRes) {
    Index = R[I].Flags&0x3FFF;
    TileIndex = (R[I].Flags>>14)&0x3F;
    BifIndex = (R[I].Flags>>20);
    memcpy(Name, R[I].Id, 8);
    Name[8] = 0;
    Ext = Types[R[I].Type].Ext;

    if (TileIndex) continue; // ignore tiles
    if (!strcmp(Ext, "mve")) continue; // movies are useless

    //unless (!strcmp(Ext, "cre")) continue;

    pathParts(0, Dir, 0, M+B[BifIndex].Name);
    sprintf(OutName, "%s/%s.%s", Dir, Name, Ext);

    downcase(OutName);

    sprintf(Tmp, "%s/%s", Output, OutName);
    pathParts(Dir, Name, 0, OutName);

    //if(I<16082) continue;
    //unless(!strcmp(Dir,"ar100a")) continue;

    printf("  Extracting:%05d: %s\n", I, OutName);
    //printf("    %04d:%04x:%04x:%04x\n", I, BifIndex, TileIndex, Index);

    Z = bifGetEntry(M+B[BifIndex].Name, Index, TileIndex, &Len);
    if (!Z) continue;

    if (!strcmp(Ext, "2da") && L>=2 && *(u2*)Z == 0xFFFF) {
      decrypt2da(Z+2, Len-2);
      writeFile(0,Len-2,Tmp,Z+2);
    } else if (!strcmp(Ext, "bam")) {
      if (!memcmp(Z, "BAMC", 4)) {
        Q = zlibUnpack(Z, Len, &Len);
        free(Z);
        Z = Q;
      }
      sprintf(Tmp, "%s/%s/%s", Output, Dir, Name);
      saveSprite(Tmp, Z, Len);
    } else if (!strcmp(Ext, "mos")) {
      if (!memcmp(Z, "MOSC", 4)) {
        Q = zlibUnpack(Z, Len, &Len);
        free(Z);
        Z = Q;
      }
      sprintf(Tmp, "%s/%s/%s.png", Output, Dir, Name);
      saveImage(Tmp, Z, Len);
    } else if (!strcmp(Ext, "wav")) {
      sprintf(Tmp, "%s/%s/%s.tmp", Output, Dir, Name);
      writeFile(0,Len,Tmp,Z);
      free(Z);
      FD = open(Tmp, O_RDONLY);
      Z = decodeACM(FD, Len, 0, &Len);
      close(FD);
      unlink(Tmp);
      sprintf(Tmp, "%s/%s/%s.wav", Output, Dir, Name);
      writeFile(0,Len,Tmp,Z);
    } else {
      writeFile(0,Len,Tmp,Z);
    }
    free(Z);
  }

#if 1
  sprintf(Tmp, "%s/Sounds", KeyDir);
  FL = getFileList(Tmp);
  times (I, FL->Size) {
    strcpy(Name, FL->Names[I]);
    if (strlen(Name) < 4 || strcmp(Name+strlen(Name)-3, "wav")) continue;
    printf("  Extracting: Sounds/%s\n", Name);

    sprintf(Tmp, "%s/Sounds/%s", KeyDir, Name);
    FD = open(Tmp, O_RDONLY);
    Z = decodeACM(FD, fileSize(Tmp), 0, &Len);
    close(FD);

    sprintf(Tmp, "%s/%s", Output, Name);
    writeFile(0,Len,Tmp,Z);
    free(Z);
  }
#endif
}

int keyInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "key";
  F->Description = "Infinity Engine Directory (Baldur's Gate, Icewind Dale)";
  F->Decompile = keyDecompile;
  return 1;
}

