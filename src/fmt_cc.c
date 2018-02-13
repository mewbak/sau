//Credits go to people at xeen.wikia.com and openkb project

// CCs are a lot like MPQs: hashes, compression and encryption

#include "common.h"
#include "cc.h"
#include "mm4_pals.h"

#define E_PLAIN   0
#define E_XOR     1
#define E_LZW     2
#define E_LZW2    3 /* MM3 */
typedef struct {
  u2 NFiles;
} __attribute__ ((packed)) header;


typedef struct {
  u2 Hash;
  u1 Off[3];
  u1 Len[3];
} __attribute__ ((packed)) entry;

typedef struct {
  u2 Off;
  u2 CK; // offset to color key or bitmask
} __attribute__ ((packed)) kbFrame;

static void decodeFT(u1 *buf, int l) {
  u1 al, ah=0xAC;
  int i;
  
  times (i, l) {
    al = buf[i];
    al = (((int)al << 2) | ((int)al >> 6)) & 0xff; // assembly: ROL al,2
    al = (al + ah) & 0xff;
    buf[i] = al;
    ah = (ah + 0x67) & 0xff;
  } 
}

static void xorDecode(u1 *Bs, int Len, u1 Key) {
  int I;
  times (I, Len) Bs[I] ^= Key;
}


static u2 dohash(char *buf) {
  u1 h0, h1, h2, h3;
  u4 temp;

  h0 = h1 = 0;

  while (*buf != 0) {
    // swap h0 and h1
    h2 = h1; h1 = h0; h0 = h2; 
    // rotate left
    h2 = (((u4)h0 << 1) & 0xff) | (((u4)h1 >> 7) & 0xff);
    h3 = (((u4)h1 << 1) & 0xff) | (((u4)h0 >> 7) & 0xff);
    h0 = h2;
    h1 = h3;
    // calc toUpper(h1)
    h2 = (*buf++) & 0x7f;
    if (h2 >= 0x60) h2 -= 0x20;
    // add char to h1
    temp = ((u4)h0 << 8) | (u4)h1;
    temp = (temp + h2) & 0xffff;
    h0 = (temp >> 8) & 0xff;
    h1 = temp & 0xff; 
  }
  return h0*256 + h1;
}  

#define MBUFFER_SIZE 1024
#define MBUFFER_EDGE (MBUFFER_SIZE - 3) 

static u1 *lzwDecode(u1 *Z, int L, int ULen) {
  u1 *Q=Z, *E=Z+L, *S, *B;
  int I, J;

  S = B = ns(u1,2*ULen);

  /*
   * The data is kept in BIT-positioned "blocks".
   * We use several variables to iterate it with some
   * level of sanity:
   */
  int pos = 0;  /* Position in bits */

  int byte_pos = 0; /* Position in bytes */
  int bit_pos = 0; /* Extra shift in bits */


  // Each "block" can take from 9 to 12 bits of data.
  // Last 8 bits are "the value" 
  // First N bits are "the key"
  int step = 9;  /* Bits per step */

  /* Those masks help us get the relevant bits */ 
  u2 keyMask[4] = {
    0x01FF,   // 0001 1111 
    0x03FF,   // 0011 1111
    0x07FF,   // 0111 1111
    0x0FFF,   // 1111 1111
  };

  /*
   * If the "the key" is unset (00), the value is read as-is.
   * Otherwise, the whole "block" (N+8 bytes) is treated
   * as a dictionary key, and the dictionary is queried.
   * The exceptions are the 0x0100 value, which means
   * the dictionary must be cleared, and the 0x0101 value,
   * which marks END OF FILE.
   */

  // Each dictionary entry consists of an index to a previous entry and value
  u2 dict_key[768 * 16];
  u1 dict_val[768 * 16];
  u2 dict_index = 0x0102; // Start populating dictionary from 0x0102
  u2 dict_range = 0x0200; // Allow that much entries before increasing step

  u1 queue[0xFF];
  int queued = 0;

  u1 mbuffer[MBUFFER_SIZE];  /* Read buffer */ 


  u2 next_index = 0;  // block of data we currently examine
  u1 last_char  = 0;  // value from previous iteration
  u2 last_index = 0;  // block from previous iteration

  int big_index;  // used to safely load and shift 3 bytes
  u2 keep_index;  // used to keep "next_index" before making it "last_index"
  int reset_hack=0;  // HACK -- delay dictionary reset to next iteration

  memcpy(mbuffer, Q, min(MBUFFER_SIZE, E-Q));
  Q += min(MBUFFER_SIZE, E-Q);

  while (1) {
    if (reset_hack)  {  // dictionary reset needed?
      step = 9;
      dict_range = 0x0200;
      dict_index = 0x0102;
    }

    // Since "pos" is in bits, we get position in bytes + small offset in bits
    byte_pos = pos / 8;
    bit_pos = pos % 8;

    pos += step;  /* And advance to the next chunk */

    /* Edge of buffer, read more data from file */
    if (byte_pos >= MBUFFER_EDGE) {
        int bytes_extra = MBUFFER_SIZE - byte_pos;//~= 3
        int bytes_left = MBUFFER_SIZE - bytes_extra;//~= 1021

        /* Copy leftovers */
        times (J, bytes_extra) mbuffer[J] = mbuffer[bytes_left + J];

        /* Read in the rest */
        memcpy(mbuffer+bytes_extra, Q, min(bytes_left, E-Q));
        Q += min(bytes_left, E-Q);

        /* Reset cursor */
        pos = bit_pos + step;  /* Add all unused bits */
        byte_pos = 0;
        /* On dictionary reset, use byte offset as bit offset*/
        if (reset_hack) bit_pos = bytes_extra;
    }

    //printf("%04d\t0x%04X:%01X\n", pos, byte_pos, bit_pos);

    /* Read index from position "byte_pos", bit offset "bit_pos" */
    big_index = 
      ((mbuffer[byte_pos+2] & 0x00FF) << 16) | 
      ((mbuffer[byte_pos+1] & 0x00FF) << 8) | 
      (mbuffer[byte_pos] & 0x00FF);

    big_index >>= bit_pos;
    big_index &= 0x0000FFFF;

    next_index = big_index;
    next_index &= keyMask[ (step - 9) ];

    /* Apply the value as-is, continuing with dictionary reset, C) */
    if (reset_hack)  {
      last_index = next_index;
      last_char = (next_index & 0x00FF);
      *B++ = last_char;
      reset_hack = 0; // done with the hack
      continue;
    }

    if (next_index == 0x0101) break; // end

    if (next_index == 0x0100) {  // reset dictionary
      reset_hack = 1; // Postpone it into next iteration 
      /* Note: this hack avoids code duplication, but  makes the algorithm
       * harder to follow. Basically, what happens, when the "reset" 
       * command is being hit, is that 
       * A) the dictionary is reset
       * B) one more value is being read (this is the code duplication bit)
       * C) the value is applied to output as-is
       * D) we continue as normal
       */
      continue;
    }

    keep_index = next_index; // Remember *real* "next_index"

    if (next_index >= dict_index) { // No dictionary entry to query, step back
      next_index = last_index;
      queue[queued++] = last_char;
    }

    while (next_index > 0x00ff) { // Quering dictionary?
      queue[queued++] = dict_val[next_index];
      next_index = dict_key[next_index]; // Next query
    }

    last_char = (next_index & 0x00FF);
    queue[queued++] = last_char;

    while (queued) *B++ = queue[--queued]; // Unqueue

    dict_key[dict_index] = last_index; // "goto prev entry"
    dict_val[dict_index] = last_char;  // the value
    dict_index++;

    last_index = keep_index; // Save *real* "next_index"

    /* Edge of dictionary, increase the bit-step, making range twice as large. */
    if (dict_index >= dict_range && step < 12) {
      step += 1;
      dict_range <<= 1;
    }
  }


  if (B-S != ULen) printf("  Size mismatch (%d!=%d)\n", (int)(B-S), ULen);
  return S;
}


u1 CGA_To_EGA[] = {
  0, // 00 // black   // bin:00
  3, // 01 // cyan    // bin:01
  5, // 02 // magenta // bin:10
  7, // 03 // white   // bin:11
  };

u1 CGA_To_EGA_Mask[] = {
  0, // 00 // black   // bin:00
  7, // 03 // white   // bin:11
  3, // 01 // cyan    // bin:01
  5, // 02 // magenta // bin:10
  };


static u1 EGAPal[] =  {
  0x00,0x00,0x00,0x00,  // 00 // dark black
  0x00,0x00,0xAA,0x00,  // 01 // dark blue
  0x00,0xAA,0x00,0x00,  // 02 // dark green
  0x00,0xAA,0xAA,0x00,  // 03 // cyan
  0xAA,0x00,0x00,0x00,  // 04 // dark red
  0xAA,0x00,0xAA,0x00,  // 05 // magenta
  0xAA,0x55,0x00,0x00,  // 06 // brown
  0xAA,0xAA,0xAA,0x00,  // 07 // dark white / light gray 
  0x55,0x55,0x55,0x00,  // 08 // dark gray / light black
  0x55,0x55,0xFF,0x00,  // 09 // light blue
  0x55,0xFF,0x55,0x00,  // 10 // light green
  0x55,0xFF,0xFF,0x00,  // 11 // light cyan
  0xFF,0x55,0x55,0x00,  // 12 // light red
  0xFF,0x55,0xFF,0x00,  // 13 // light magenta
  0xFF,0xFF,0x55,0x00,  // 14 // light yellow
  0xFF,0xFF,0xFF,0x00,  // 15 // bright white
  0x00,0xFF,0xFF,0x00}; // color key


static void saveImage(char *Output, u1 *M, int L, char *Name, char *Ext) {
  char Tmp[1024];
  kbFrame *F;
  spr *S;
  pic *P;
  u1 *Q;
  int I, J, K, X, Y, W, H, NFrames=*(u2*)M;
  F = (kbFrame*)(M+2);

  //printf("  %d\n", NFrames);

  S = sprNew();
  S->Palette = ns(u1, 4*256);
  if (!strcmp(Ext, "256")) {
    times (I, 256) {
      S->Palette[I*4+0] = KBPal[I*4+0]<<2;
      S->Palette[I*4+1] = KBPal[I*4+1]<<2;
      S->Palette[I*4+2] = KBPal[I*4+2]<<2;
      S->Palette[I*4+3] = 0;
    }
  } else {
    times (I, 17) {
      S->Palette[I*4+0] = EGAPal[I*4+0];
      S->Palette[I*4+1] = EGAPal[I*4+1];
      S->Palette[I*4+2] = EGAPal[I*4+2];
      S->Palette[I*4+3] = 0;
    }
  }

  S->NAnis = 1;
  S->Anis = ns(ani, S->NAnis);
  S->Anis[0].NFacs = 1;
  S->Anis[0].Facs = ns(fac,S->Anis[0].NFacs);
  S->Anis[0].Facs[0].NPics = NFrames;
  S->Anis[0].Facs[0].Pics = ns(pic*, S->Anis[0].Facs[0].NPics);

  times (I, NFrames) {
    Q = M+F->Off;
    W = *(u2*)Q; Q+=2;
    H = *(u2*)Q; Q+=2;
    P = S->Anis[0].Facs[0].Pics[I] = picNew(W, H, 8);
    P->SharedPalette = 1;
    free(P->P);
    P->P = S->Palette;
    if (!strcmp(Ext, "4")) {
      X = Y = 0;
      times (J, (W*H+3)/4) {
        times (K, 4) {
          picPut(P, X, Y, CGA_To_EGA[(*Q>>(6-K*2))&0x3]);
          if (++X == W) {X=0; Y++;}
        }
        Q++;
      }
      if (F->CK) goto mask;
    } else if (!strcmp(Ext, "16")) {
      X = Y = 0;
      times (J, (W*H+1)/2) {
        times (K, 2) {
          picPut(P, X, Y, (*Q>>(4-K*4))&0x7);
          if (++X == W) {X=0; Y++;}
        }
        Q++;
      }
      if (F->CK) goto mask;
    } else {
      memcpy(P->D, Q, W*H);
      if (F->CK) P->K = *(M+F->CK);
    }
    if (0) {
mask:
      Q = M+F->CK;
      P->K = 16;
      X = Y = 0;
      times (J, (W*H+7)/8) {
        times (K, 8) {
          if ((*Q>>(7-K))&0x1) picPut(P, X, Y, P->K);
          if (++X == W) {X=0; Y++;}
        }
        Q++;
      }
    }
    F++;
  }

  sprintf(Tmp, "%s/%s/%s.gif", Output, Ext, Name);
  gifSave(Tmp, S);
  sprDel(S);
}



typedef struct {
  u2 Offs[2]; // offsets of frame cells
} __attribute__ ((packed)) frameHeader;


typedef struct {
  u2 X; // X displacement, from the left
  u2 Width;
  u2 Y; // X displacement, from the top
  u2 Height;
  //u1 Data[]; // RLE compressed pixels
} __attribute__ ((packed)) cell;

//#define putPixel( x, y, colorIndex ) Out[y*width+x] = colorIndex
#define putPixel( x, y, colorIndex ) picPut(P, (x)+SX, (y)+SY, colorIndex)
static void decodeCell(pic *P, u1 *cellData, int SX, int SY) {
  int i, dp = 0;                          // Pointer within data stream
  int xPos, yPos;                         // The position within the color raster
  int lineLength, byteCount;              // total bytes/bytes read in this scan line
  int opcode, cmd, len, opr1, opr2;       // Used to decode the drawing commands
  int xOffset, yOffset, width, height;    // Cell position and size

  // The pattern steps used in the pattern command
  int patternSteps[] = { 0, 1, 1, 1, 2, 2, 3, 3, 0, -1, -1, -1, -2, -2, -3, -3 };

  // Read the position and size of the cell from the data stream
  xOffset = *((u2*)(&cellData[dp])); dp += 2;
  width   = *((u2*)(&cellData[dp])); dp += 2;
  yOffset = *((u2*)(&cellData[dp])); dp += 2;
  height  = *((u2*)(&cellData[dp])); dp += 2;

  for( yPos = yOffset, byteCount = 0 ; yPos < height + yOffset ; yPos++, byteCount = 0 )
  {
    // The number of bytes in this scan line
    lineLength = cellData[dp++];

    if( lineLength > 0 )
    {
      // Skip the transparent color at the beginning of the scan line
      xPos = cellData[dp++] + xOffset; byteCount++;

      while( byteCount < lineLength )
      {
        // The next byte is an opcode that determines what 
        // operators are to follow and how to interpret them.
        opcode = cellData[dp++]; byteCount++;

        // Decode the opcode
        len = opcode & 0x1F;
        cmd = ( opcode & 0xE0 ) >> 5;
        switch( cmd )
        {
          case 0:   // The following len + 1 bytes are stored as indexes into the color table.
          case 1:   // The following len + 33 bytes are stored as indexes into the color table.
            for( i = 0 ; i < opcode + 1 ; i++, xPos++ )
            {
              opr1 = cellData[dp++]; byteCount++;
              putPixel( xPos, yPos, opr1 );
            }
            break;

          case 2:   // The following byte is an index into the color table, draw it len + 3 times.
            opr1 = cellData[dp++]; byteCount++;
            for( i = 0 ; i < len + 3 ; i++, xPos++ )
            {
              putPixel( xPos, yPos, opr1 );
            }
            break;

          case 3:   // Stream copy command.
            opr1 = *((u2*)&cellData[dp]);
            dp += 2; byteCount += 2;
            //printf("%d\n", sizeof(u2));
            for( i = 0 ; i < len + 4 ; i++, xPos++ )
            {
              //if (dp-opr1+i >= 0)
                putPixel( xPos, yPos, cellData[dp-opr1+i]);
            }
            break;

          case 4:   // The following two bytes are indexes into the color table, draw the pair len + 2 times.
            opr1 = cellData[dp++]; byteCount++;
            opr2 = cellData[dp++]; byteCount++;
            for( i = 0 ; i < len + 2 ; i++, xPos += 2 )
            {
              putPixel( xPos+0, yPos, opr1 );
              putPixel( xPos+1, yPos, opr2 );
            }
            break;

          case 5:   // Skip len + 1 pixels filling them with the transparent color.
            xPos += len + 1;
            break;

          case 6:   // Pattern command.
          case 7:
            // The pattern command has a different opcode format
            len = opcode & 0x07;
            cmd = ( opcode >> 2 ) & 0x0E;

            opr1 = cellData[dp++]; byteCount++;
            for( i = 0 ; i < len + 3 ; i++, xPos++ )
            {
              putPixel( xPos, yPos, opr1 );
              opr1 += patternSteps[cmd + ( i % 2 )];
            }
            break;
        }
      }
    }
    else
    {
      // Skip the specified number of scan lines
      yPos += cellData[dp++];
    }
  }
}


static void saveVGA(char *Output, char *Name, char *Ext, u1 *M, int L) {
  int I, J, K, X, Y, EY=1, EX=1, SX=320, SY=240, Key=0xFF;
  char Tmp[1024];
  pic *P, *PP;
  u1 *Pal = pal_mm4;
  cell *C;
  int NFrames = *(u2*)M;
  frameHeader *FH = (frameHeader*)(M+2);

  // sprites are draw in the middle of shit. to fix we need to crop this crap
  times (I, NFrames) {
    P = 0;
    times (J, 2) { // every frame consists of base and delta cells
      unless (FH[I].Offs[J]) continue;
      C = (cell*)(M+FH[I].Offs[J]);
      //printf("%02d,%d:#%04x:#%04x: %dx%d+%d,%d\n"
      //      ,I, J, FH[I].Offs[0], FH[I].Offs[1], C->Width, C->Height, C->X, C->Y);

      if (!P) {
        P = picNew(320, 240, 8);
        P->K = Key;
        memset(P->D, P->K, P->W*P->H);
      }
      decodeCell(P, (u1*)C, 0, 0);
    }
    if (P) {
      times (Y, P->H) times (X, P->W) if (picGet(P,X,Y) != P->K) {
        SX = min(SX, X);
        SY = min(SY, Y);
        EX = max(EX, X);
        EY = max(EY, Y);
      }
    }
  }

  times (I, NFrames) {
    P = 0;
    times (J, 2) { // every frame consists of base and delta cells
      unless (FH[I].Offs[J]) continue;
      C = (cell*)(M+FH[I].Offs[J]);
      if (!P) {
        //P = picNew(C->Width+C->X, C->Height+C->Y, 8);
        P = picNew(SX<EX?EX-SX:1, SY<EY?EY-SY:1, 8);
        times (K, 256) {
          P->P[K*4+0] = (Pal[K*3+0]<<2)|0x3;
          P->P[K*4+1] = (Pal[K*3+1]<<2)|0x3;
          P->P[K*4+2] = (Pal[K*3+2]<<2)|0x3;
          P->P[K*4+3] = 0;
        }
        P->K = Key;
        memset(P->D, P->K, P->W*P->H);
      }
      decodeCell(P, (u1*)C, -SX, -SY);
    }
    if (P) {
      sprintf(Tmp, "%s/%s/%s/%02d.png", Output, Ext, Name, I);
      pngSave(Tmp, P);
      picDel(P);
    }
  }
}


static void saveFont(char *Output, u1 *Q) {
  char Tmp[1024];
  int I, K=0, X=0, Y=0, W=8, H=1024;
  pic *P = picNew(W, H, 8);
  P->K = 0;
  P->P[0] = 0xFF;
  P->P[1] = 0xFF;
  P->P[2] = 0xFF;

  times (I,1024) {
    times (K, 8) {
      picPut(P, X, Y, (*Q>>(7-K))&0x1);
      if (++X == W) {X=0; Y++;}
    }
    Q++;
  }
  sprintf(Tmp, "%s/font.png", Output);
  pngSave(Tmp, P);
  picDel(P);
}


static void saveRAW(char *Output, char *Name, u1 *M) {
  int I, J;
  pic *P = picNew(320, 200, 8);
  char Tmp[1024];
  u1 *Pal = pal_mm4;
  if (!strcmp(Name, "intro")) Pal = pal_intro;
  if (!strcmp(Name, "intro1")) Pal = pal_intro;
  if (!strcmp(Name, "logobak")) Pal = pal_intro1;
  if (!strcmp(Name, "miror-s")) Pal = pal_mirror;

  times (I, 256) {
    P->P[I*4+0] = (Pal[I*3+0]<<2)|0x3;
    P->P[I*4+1] = (Pal[I*3+1]<<2)|0x3;
    P->P[I*4+2] = (Pal[I*3+2]<<2)|0x3;
    P->P[I*4+3] = 0;
  }

  memcpy(P->D, M, 320*200);

  sprintf(Tmp, "%s/raw/%s.png", Output, Name);
  pngSave(Tmp, P);
  picDel(P);
}


static void ccDecompile (char *Output, char *Input) {
  int I, FTEnc=E_XOR, Enc=E_XOR, Off, CLen, Len;
  u1 *Q, *M;
  u1 Key = 0x35;
  char Tmp[1024], *File, **Hashed, CCName[256], Name[256], Ext[32];
  int L = fileSize(Input);
  u1 *Z = readFile(0, L, Input);
  header *H = (header*)Z;
  entry *Es = (entry*)(H+1);

  pathParts(0, CCName, 0, Input);
  downcase(CCName);

  Hashed = ns(char*,0x10000);

#if 0
  for (I=0; Names[I]; I++) {
    int Hash = dohash(Names[I]);
    if (Hashed[Hash]) {
      printf("Hash collision between %s and %s\n", Hashed[Hash], Names[I]);
      abort();
    }
    Hashed[Hash] = strdup(Names[I]);
  }
#endif

  if (!strcmp(CCName, "416"))  { // kings bounty 4 and 16-color images
    FTEnc = E_PLAIN;
    Enc = E_LZW;
    for (I=0; KB416_CC[I]; I++) Hashed[dohash(KB416_CC[I])]=strdup(KB416_CC[I]);
  } else if (!strcmp(CCName, "256")) { // kings bounty 256-color images
    FTEnc = E_PLAIN;
    Enc = E_LZW;
    for (I=0; KB256_CC[I]; I++) Hashed[dohash(KB256_CC[I])]=strdup(KB256_CC[I]);
  } else  if (!strcmp(CCName, "mm3")) { // mm3
    Enc = E_LZW2;
    for (I=0; MM3_CC[I]; I++) Hashed[dohash(MM3_CC[I])] = strdup(MM3_CC[I]);
  } else if (!strcmp(CCName, "xeen")) { // mm4
    for (I=0; Xeen_CC[I]; I++) Hashed[dohash(Xeen_CC[I])] = strdup(Xeen_CC[I]);
    Hashed[0x2A0C] = strdup("SAVE1");
    Hashed[0x2A1C] = strdup("SAVE2");
    Hashed[0x2A2C] = strdup("SAVE3");
    Hashed[0x2A3C] = strdup("SAVE4");
    Hashed[0x284C] = strdup("SAVE5");
    Hashed[0x2BB9] = strdup("UNKNOWN1.VOC");
  } else if (!strcmp(CCName, "intro")) { // mm5 intro
    for (I=0; Intro_CC[I]; I++) Hashed[dohash(Intro_CC[I])]=strdup(Intro_CC[I]);
    Hashed[0x53BD] = strdup("UNKNOWN1");
    Hashed[0xB41C] = strdup("UNKNOWN2");
    Hashed[0x0EB2] = strdup("UNKNOWN3");
    Hashed[0xEDA5] = strdup("UNKNOWN4");
    Hashed[0x761E] = strdup("UNKNOWN1.VOC");
    Hashed[0xC790] = strdup("UNKNOWN2.VOC");
  } else if (!strcmp(CCName, "dark")) { // mm5
    for (I=0; Dark_CC[I]; I++) Hashed[dohash(Dark_CC[I])] = strdup(Dark_CC[I]);
    Hashed[0x2A0C] = strdup("SAVE1");
    Hashed[0x2A1C] = strdup("SAVE2");
    Hashed[0x2A2C] = strdup("SAVE3");
    Hashed[0x2A3C] = strdup("SAVE4");
    Hashed[0x284C] = strdup("SAVE5");
    Hashed[0x2A5C] = strdup("SAVE6");
    Hashed[0x770E] = strdup("UNKNOWN1.M");
    Hashed[0x5A66] = strdup("UNKNOWN2.M");
    Hashed[0x5A6A] = strdup("UNKNOWN3.M");
    Hashed[0xB0FE] = strdup("UNKNOWN1.VOC");
  } else if (!strcmp(CCName, "swrd")) { // swords of xeen
    for (I=0; Dark_CC[I]; I++) Hashed[dohash(Dark_CC[I])] = strdup(Dark_CC[I]);
    Hashed[0x2A0C] = strdup("SAVE1");
    Hashed[0x2A1C] = strdup("SAVE2");
    Hashed[0x2A2C] = strdup("SAVE3");
    Hashed[0x2A3C] = strdup("SAVE4");
    Hashed[0x284C] = strdup("SAVE5");
    Hashed[0x2A5C] = strdup("SAVE6");
    Hashed[0x770E] = strdup("UNKNOWN1.M");
    Hashed[0x5A66] = strdup("UNKNOWN2.M");
    Hashed[0x5A6A] = strdup("UNKNOWN3.M");
    Hashed[0xB0FE] = strdup("UNKNOWN1.VOC");
  } else {
    printf("  Unknown CC file\n");
    abort();
  }


  if (FTEnc == E_XOR) decodeFT((u1*)Es, H->NFiles*sizeof(entry));


#if 0
  times (I, H->NFiles) {
    File = Hashed[Es[I].Hash];
    if (File) printf ("%s\n", File);
  }
  return;
#endif

  times (I, 0x10000) unless (Hashed[I]) {
    sprintf(Tmp, "file_%04x", I);
    if (!Hashed[I]) Hashed[I] = strdup(Tmp);
  }

  //printf ("Files=%d\n\n", H->NFiles);
  times (I, H->NFiles) {
    File = Hashed[Es[I].Hash];
    downcase(File);
    pathParts(0, Name, Ext, File);

    Off = (Es[I].Off[2]<<16)|(Es[I].Off[1]<<8)|Es[I].Off[0];
    Len = (Es[I].Len[2]<<16)|(Es[I].Len[1]<<8)|Es[I].Len[0];
    printf("  Extracting: %s\n", File);
    //printf("  Hash=#%04x Off=#%06x/#%06x Len=#%04x\n", Es[I].Hash, Off, L, Len);
    M = Z+Off;
    if (Enc==E_XOR) xorDecode(M, Len, Key);
    else if (Enc==E_LZW) {
      CLen = Len;
      Len = *(u4*)M;
      M = lzwDecode(M+4,CLen-4,Len);
    } else if (Enc==E_LZW2) {
      CLen = Len;
      Len = (M[2]<<8)|M[3];
      printf("    Compressed=%d:%d (dumping as is)\n", CLen, Len);
      // mm3 was reversed, but I'm too lazy to add support
      // it also uses different sprite format
      // see doc/viewers/txt/MM3VGA.TXT and doc/viewers/src/MM3DEC.ASM
      //Q = lzwDecode(M+4,CLen-4,Len);
      //M = Q;
    }

    if ((!strcmp(CCName, "256")||!strcmp(CCName, "416"))
        && (!strcmp(Ext, "4") || !strcmp(Ext, "16")
            || !strcmp(Ext, "256"))) {
      saveImage(Output, M, L, Name, Ext);
    } else if ((!strcmp(CCName, "256")||!strcmp(CCName, "416"))
            &&  !strcmp(Ext, "ch")) {
      saveFont(Output, M);
    } else if (!strcmp(Ext, "raw")) {
      saveRAW(Output, Name, M);
    } else if (!strcmp(Ext, "vga") || !strcmp(Ext, "icn") || !strcmp(Ext, "obj")
            || !strcmp(Ext, "0bj") || !strcmp(Ext, "mon") || !strcmp(Ext, "att")) {
      saveVGA(Output, Name, Ext, M, Len);
    } else {
      if (!Ext[0]) strcpy(Ext, "other");
      sprintf(Tmp, "%s/%s/%s", Output, Ext, File);
      writeFile(0, Len, Tmp, M);
    }
  }

  times (I, 0x10000) free(Hashed[I]);
  free(Hashed);
}

int ccInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "cc";
  F->Description = "Kings Bounty and Might & Magic 3/4/5 archives";
  F->Decompile = ccDecompile;
  return 1;
}


