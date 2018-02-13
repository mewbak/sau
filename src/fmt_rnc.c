//Credits go to Rob Norton. Darkmere/EPF code by SNV
#include "common.h"

// RNC is, much like GZIP, used in a lot of games (mainly amiga ones)
// Used in: Bubsy (jaguar), Darkmere, Dragonstone, Dungeon Keeper, ...

static s4 rnc_ulen (void *packed);
static char *rnc_error (s4 errcode);
static s4 rnc_crc (void *data, s4 len);

#define RNC_FILE_IS_NOT_RNC    -1
#define RNC_HUF_DECODE_ERROR   -2
#define RNC_FILE_SIZE_MISMATCH -3
#define RNC_PACKED_CRC_ERROR   -4
#define RNC_UNPACKED_CRC_ERROR -5

typedef struct {
    u4 bitbuf;     // holds between 16 and 32 bits
    int bitcount;  // how many bits does bitbuf hold?
} bit_stream;

typedef struct {
    int num; // number of nodes in the tree
    struct {
      u4 code;
      int codelen;
      int value;
    } table[32];
} huf_table;

static void read_huftable (huf_table *h, bit_stream *bs, u1 **p);
static u4 huf_read (huf_table *h, bit_stream *bs,
                         u1 **p);

static void bitread_init (bit_stream *bs, u1 **p);
static void bitread_fix (bit_stream *bs, u1 **p);
static u4 bit_peek (bit_stream *bs, u4 mask);
static void bit_advance (bit_stream *bs, int n, u1 **p);
static u4 bit_read (bit_stream *bs, u4 mask,
                         int n, u1 **p);

static u4 bs4 (u1 *p);
static u4 ls4 (u1 *p);
static u4 bword (u1 *p);
static u4 lword (u1 *p);

static u4 mirror (u4 x, int n);

// Return an error string corresponding to an error return code.
static char *rnc_error (s4 errcode) {
    static char *const errors[] = {
      "No error",
      "File is not RNC-1 format",
      "Huffman decode error",
      "File size mismatch",
      "CRC error in packed data",
      "CRC error in unpacked data",
      "Unknown error"
    };

    errcode = -errcode;
    if (errcode < 0)
      errcode = 0;
    if (errcode > sizeof(errors)/sizeof(*errors) - 1)
      errcode = sizeof(errors)/sizeof(*errors) - 1;
    return errors[errcode];
}

// Decompress a packed data block. Returns pointer to unpacked data.
static u1 *decode (void *packed, int L, int *ULen) {
    u1 *input = packed;
    u1 *output, *unpacked;
    u1 *inputend, *outputend;
    bit_stream bs;
    huf_table raw, dist, len;
    u4 ch_count, ret_len, out_crc;
    int Enc = input[3];

    *ULen = 0;
    unless (!memcmp(input, "RNC", 3)) {
      printf("rnc: not an RNC input\n");
      return ns(u1, 16);
    }

    *ULen = ret_len = bs4 (input+4);
    if (!ret_len) return ns(u1, 16); //seen that in Darkmere
    output = unpacked = ns(u1, 2*ret_len);
    if (input[3] == 0) { //unpacked
      memcpy(unpacked, input+8, ret_len);
      return unpacked;
    } else if (input[3] != 1) {
      printf("rnc: invalid encoding (%d)\n", input[3]);
      return unpacked;
    }

    outputend = output + ret_len;
    inputend = input + 18 + bs4(input+8);

    input += 18; // skip header

    // Check the packed-data CRC. Also save the unpacked-data CRC
    // for later.
    if (rnc_crc(input, inputend-input) != bword(input-4)) {
      printf("rnc: packed data CRC mismatch\n");
      return unpacked;
    }
    out_crc = bword(input-6);

    bitread_init (&bs, &input);
    bit_advance (&bs, 2, &input); // discard first two bits

    // Process chunks.
    while (output < outputend) {
      read_huftable (&raw, &bs, &input);
      read_huftable (&dist, &bs, &input);
      read_huftable (&len, &bs, &input);
      ch_count = bit_read (&bs, 0xFFFF, 16, &input);

      while (1) {
          s4 length, posn;

          length = huf_read (&raw, &bs, &input);
          if (length == -1) {
            printf("rnc: huffman decode error\n");
            return unpacked;
          }
          if (length) {
            while (length--)
                *output++ = *input++;
            bitread_fix (&bs, &input);
          }
          if (--ch_count <= 0)
            break;

          posn = huf_read (&dist, &bs, &input);
          if (posn == -1) {
            printf("rnc: huffman decode error\n");
            return unpacked;
          }
          length = huf_read (&len, &bs, &input);
          if (length == -1) {
            printf("rnc: huffman decode error\n");
            return unpacked;
          }
          posn += 1;
          length += 2;
          while (length--) {
            *output = output[-posn];
            output++;
          }
      }
    }

    if (outputend != output) {
      printf("rnc: unpacked size mismatch\n");
      return unpacked;
    }

    // Check the unpacked-data CRC.
    if (rnc_crc(outputend-ret_len, ret_len) != out_crc) {
      printf("rnc: unpacked data CRC mismatch\n");
      return unpacked;
    }

    return unpacked;
}

// Read a Huffman table out of the bit stream and data stream given.
static void read_huftable (huf_table *h, bit_stream *bs, u1 **p) {
    int i, j, k, num;
    int leaflen[32];
    int leafmax;
    u4 codeb;  // big-endian form of code

    num = bit_read (bs, 0x1F, 5, p);
    if (!num)
      return;

    leafmax = 1;
    for (i=0; i<num; i++) {
      leaflen[i] = bit_read (bs, 0x0F, 4, p);
      if (leafmax < leaflen[i])
          leafmax = leaflen[i];
    }

    codeb = 0L;
    k = 0;
    for (i=1; i<=leafmax; i++) {
      for (j=0; j<num; j++)
          if (leaflen[j] == i) {
            h->table[k].code = mirror (codeb, i);
            h->table[k].codelen = i;
            h->table[k].value = j;
            codeb++;
            k++;
          }
      codeb <<= 1;
    }

    h->num = k;
}

// Read a value out of the bit stream using the given Huffman table.
static u4 huf_read (huf_table *h, bit_stream *bs, u1 **p) {
    int i;
    u4 val;

    for (i=0; i<h->num; i++) {
      u4 mask = (1 << h->table[i].codelen) - 1;
      if (bit_peek(bs, mask) == h->table[i].code)
          break;
    }
    if (i == h->num)
      return -1;
    bit_advance (bs, h->table[i].codelen, p);

    val = h->table[i].value;

    if (val >= 2) {
      val = 1 << (val-1);
      val |= bit_read (bs, val-1, h->table[i].value - 1, p);
    }
    return val;
}

// Initialises a bit stream with the first two bytes of the packed
// data.
static void bitread_init (bit_stream *bs, u1 **p) {
    bs->bitbuf = lword (*p);
    bs->bitcount = 16;
}

// Fixes up a bit stream after literals have been read out of the
// data stream.
static void bitread_fix (bit_stream *bs, u1 **p) {
    bs->bitcount -= 16;
    bs->bitbuf &= (1<<bs->bitcount)-1; // remove the top 16 bits
    bs->bitbuf |= (lword(*p)<<bs->bitcount); // replace with what's at *p
    bs->bitcount += 16;
}

// Returns some bits.
static u4 bit_peek (bit_stream *bs, u4 mask) {
    return bs->bitbuf & mask;
}

// Advances the bit stream.
static void bit_advance (bit_stream *bs, int n, u1 **p) {
    bs->bitbuf >>= n;
    bs->bitcount -= n;
    if (bs->bitcount < 16) {
      (*p) += 2;
      bs->bitbuf |= (lword(*p)<<bs->bitcount);
      bs->bitcount += 16;
    }
}

// Reads some bits in one go (ie the above two routines combined).
static u4 bit_read (bit_stream *bs, u4 mask, int n, u1 **p) {
    u4 result = bit_peek (bs, mask);
    bit_advance (bs, n, p);
    return result;
}

// Return the big-endian s4word at p.
static u4 bs4 (u1 *p) {
    u4 n;
    n = p[0];
    n = (n << 8) + p[1];
    n = (n << 8) + p[2];
    n = (n << 8) + p[3];
    return n;
}

// Return the little-endian s4word at p.
static u4 ls4 (u1 *p) {
    u4 n;
    n = p[3];
    n = (n << 8) + p[2];
    n = (n << 8) + p[1];
    n = (n << 8) + p[0];
    return n;
}

// Return the big-endian word at p.
static u4 bword (u1 *p) {
    u4 n;
    n = p[0];
    n = (n << 8) + p[1];
    return n;
}

// Return the little-endian word at p.
static u4 lword (u1 *p) {
    u4 n;
    n = p[1];
    n = (n << 8) + p[0];
    return n;
}

// Mirror the bottom n bits of x.
static u4 mirror (u4 x, int n) {
    u4 top = 1 << (n-1), bottom = 1;
    while (top > bottom) {
      u4 mask = top | bottom;
      u4 masked = x & mask;
      if (masked != 0 && masked != mask)
          x ^= mask;
      top >>= 1;
      bottom <<= 1;
    }
    return x;
}

// Calculate a CRC, the RNC way. It re-computes its CRC table every
// time it's run, but who cares? ;-)
static s4 rnc_crc (void *data, s4 len) {
    unsigned short crctab[256];
    unsigned short val;
    int i, j;
    u1 *p = data;

    for (i=0; i<256; i++) {
      val = i;

      for (j=0; j<8; j++) {
          if (val & 1)
            val = (val >> 1) ^ 0xA001;
          else
            val = (val >> 1);
      }
      crctab[i] = val;
    }

    val = 0;
    while (len--) {
      val ^= *p++;
      val = (val >> 8) ^ crctab[val & 0xFF];
    }

    return val;
}


typedef struct {
  u1 Id[3]; //EPF
  u1 NFiles;
  u4 Size; //size of whole archive
} __attribute__ ((packed)) epf;

typedef struct {
  u4 Off; //relative to start of file
  u4 Len; //compressed length
} __attribute__ ((packed)) epfEntry;


static void rev(void *X, int N) {
  int T, I;
  u1 *P = (u1*)X;
  times (I, N/2) {T = P[I]; P[I] = P[N-I-1]; P[N-I-1] = T;}
}

#define RowBytes(cols) ((((cols) + 15) / 16) * 2)
static u1 *decodeRow(u4 *D, u1 *S, int W, int H, int BPP, int HasMask) {
  int plane, col, cols, cbit, bytes, nPlanes;
  u1 *ilp;
  u1 *Line = ns(u1,W*5);
  u4 *chp;

  cols = W;
  bytes = RowBytes(cols);
  nPlanes = BPP + (HasMask ? 1 : 0);
  for (plane = 0; plane < nPlanes; plane++) {
    int mask = 1 << plane;
    memcpy(Line, S, bytes);
    S += bytes;

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

// real darkmere palettes are dynamic
static u1 PalBG1[] = {
  0xFF,0x00,0xFF, 0xDD,0xBB,0x22, 0x00,0x11,0x11, 0x88,0x66,0x33,
  0x33,0x44,0x22, 0x44,0x22,0x00, 0x22,0x55,0x55, 0xCC,0xCC,0xCC,
  0x11,0x22,0x11, 0x88,0x88,0x77, 0x00,0x33,0x33, 0xAA,0x88,0x44,
  0x55,0x66,0x55, 0x66,0x44,0x00, 0xBB,0x66,0x00, 0x00,0x00,0x00
};

static u1 PalBG2[] = { //forest
  0xFF,0x00,0xFF, 0x44,0x55,0x00, 0x77,0x77,0x55, 0x77,0x66,0x11,
  0x33,0x33,0x11, 0x33,0x22,0x00, 0x55,0x77,0x66, 0xDD,0xDD,0xBB,
  0x22,0x22,0x11, 0x66,0x77,0x33, 0x22,0x44,0x44, 0x99,0x88,0x33,
  0x55,0x55,0x33, 0x55,0x44,0x00, 0x22,0x33,0x00, 0x00,0x00,0x00
};

static u1 PalBG3[] = {
  0xFF,0x00,0xFF, 0x55,0x77,0x00, 0x77,0x88,0x77, 0x88,0x66,0x33,
  0x33,0x33,0x22, 0x44,0x22,0x11, 0xFF,0xBB,0x11, 0xDD,0xDD,0xBB,
  0x11,0x11,0x00, 0x77,0x99,0x00, 0xFF,0x77,0x00, 0xAA,0x88,0x44,
  0x55,0x66,0x44, 0x66,0x44,0x11, 0x33,0x55,0x00, 0x00,0x00,0x00
};


static u1 PalBG4[] = {
  0xFF,0x00,0xFF, 0x55,0x77,0x00, 0x77,0x88,0x77, 0x88,0x66,0x33,
  0x33,0x33,0x22, 0x44,0x22,0x11, 0x77,0x66,0x11, 0xDD,0xDD,0xBB,
  0x11,0x11,0x00, 0x77,0x99,0x00, 0x55,0x44,0x00, 0xAA,0x88,0x44,
  0x55,0x66,0x44, 0x66,0x44,0x11, 0x22,0x33,0x00, 0x00,0x00,0x00
};


static u1 PalFG[] = {
  0xFF,0x00,0xFF, 0x44,0x66,0x00, 0x5D,0x77,0x82, 0x88,0x66,0x33,
  0x44,0x44,0x33, 0x44,0x22,0x00, 0xAA,0x77,0x55, 0xDD,0xDD,0xBB,
  0x22,0x22,0x11, 0x77,0x99,0x00, 0x77,0x44,0x33, 0xAA,0x88,0x44,
  0x55,0x66,0x55, 0x66,0x44,0x11, 0x33,0x44,0x00, 0x00,0x00,0x00
};

static pic *tryGfx(int *Hits, u1 *M, int L, int DW) {
  int I, J, X, Y, B;
  int BPP = 4;
  int Len = L*8/BPP;
  int H=Len/DW;
  int W=Len/(H ? H : 1);
  pic *P = picNew(W, H, 8);
  u4 *Line = ns(u4, W*4);
  u1 *Q = M, *Z, *Pal;

  Pal = PalFG;
  //Pal = PalFG;


  *Hits = 0;

  times (I, 16) {
    P->P[I*4+0] = Pal[I*3+0];
    P->P[I*4+1] = Pal[I*3+1];
    P->P[I*4+2] = Pal[I*3+2];
  }

  if (Len%DW) return P;

  times (J, BPP) {
    Z = P->D;
    times (Y, H) {
      for(X = 0; X < W; Q++) {
        times (B, 8) {
          if (*Q & (1<<(7-B))) { // favor connected crap
            if (Y && Z[X] && Z[X-W]) *Hits += 1;
            if (X && Z[X] && Z[X-1]) *Hits += 1;
            Z[X] |= (1<<J);
          }
          X++;
        }
      }
      Z += W;
    }
  }

  return P;
}

#define SZ (sizeof(Ws)/sizeof(Ws[0]))


#if 1
static void saveGfx(char *Output, u1 *M, int L) {
  int I, Hits, BestHits=-1;
  int Ws[] = {8, 16, 24, 32, 48, 64, 80, 96, 112, 128, 144, 160, 256, 320};
  //int Ws[] = {32};
  pic *P=0, *T;
  times (I, SZ) {
    T = tryGfx(&Hits, M, L, Ws[I]);
    if (Hits > BestHits) {
      if (P) picDel(P);
      P = T;
      BestHits = Hits;
    } else {
      picDel(T);
    }
  }
  if (P) pngSave(Output, P);
}
#else
static void saveGfx(char *Output, u1 *M, int L) {
  char Tmp[1024];
  int I, Hits;
  for (I=4; I<256; I++) {
    sprintf(Tmp, "%s.%03d.png", Output, I);
    pngSave(Tmp, tryGfx(&Hits, M, L, I));
  }
}
#endif

//make && for i in input/d1/*.epf; do ./sau rnc=$i out/$(echo $i | sed 's/....$//'); done
static void rncDecompile(char *Output, char *Input) {  
  int I, ULen;
  char Tmp[1024], ARName[256], Ext[32];
  u1 *Q;
  int L = fileSize(Input);
  u1 *M = readFile(0, L, Input);
  epf *H = (epf*)M;
  epfEntry *E = (epfEntry*)(H+1);

  pathParts(0, ARName, 0, Input);
  downcase(ARName);

  if (strncmp(H->Id, "EPF", 3)) { //single file
    if (strncmp(H->Id, "RNC", 3)) {
      printf("  not an RNC file\n");
      abort();
    }
    sprintf(Tmp, "%s/%s", Output, ARName);
    Q = decode(M, L, &ULen);
    writeFile(0, ULen, Tmp, Q);
    return;
  }

  rev(&H->Size,4);
  times (I, H->NFiles) {
    //if (I != 82) goto skip;
    rev(&E->Off,4);
    rev(&E->Len,4);
    printf("  %03d: Off=%06X Len=%06X\n", I, E->Off, E->Len);

    Q = decode(M+E->Off, E->Len, &ULen);

    if (!ULen) goto skip;

    Ext[0] = 0;
    if (ULen >16 && !strncmp(Q, "FORM", 4)) strcpy(Ext, ".lbm");
    else if(strlen(ARName)>3 && !memcmp(ARName, "gfx", 3)) {
      sprintf(Tmp, "%s/%s_%03d%s", Output, ARName, I, ".png");
      saveGfx(Tmp, Q, ULen);
      //abort();
      goto skip;
    }
    sprintf(Tmp, "%s/%s_%03d%s", Output, ARName, I, Ext);
    writeFile(0, ULen, Tmp, Q);
skip:
    E++;
  }
}



int rncInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "rnc";
  F->Description = "RNC/EPF/CRU files (also handle Darkmere sprites)";
  F->Decompile = rncDecompile;
  return 1;
}

