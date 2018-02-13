//Credits go to CTPAX-X Team, who published original asm code
//              SNV, C/C++ version of LZW decoder

#include "common.h"

typedef struct {
  u1 Id[4];
  u4 Off;
  u1 Flags;
  u2 NFiles;
} __attribute__ ((packed)) header;


typedef struct {
  u1 Name[13];
  u1 Compression;
  u4 Len;
  u4 ULen;
} __attribute__ ((packed)) entry;



#define setCodeSize(NewSize) do {\
  CodeSize  = (NewSize); \
  CodeMask  = (1 << CodeSize) - 1; \
  ResetCode = (1 << CodeSize) - 2; \
  IncBits   = (1 << CodeSize) - 3; \
 } while (0)

#define SIZE 0x4680
#define BSIZE 4000
static u2 PBuf[SIZE]; // prefix buf
static u1 SBuf[SIZE]; // suffix buf
static u1 OBuf[BSIZE]; // stack

u1 *lzwDecode(int ULen, int Len, u1 *In) {
  u1 *BP;
  u4 A=0, B=0, C=0, D=0, Bits=0, SD=0, NextCode, Pre, Suf, SA;
  u1 *Out = ns(u1, ULen);
  u1 *Src = In;
  u1 *Dst = Out;
  u1 CodeSize, AvailBits;
  u2 CodeMask, ResetCode, IncBits;
  u2 *P = PBuf;
  u1 *S = SBuf;
  u1 *O = OBuf;

  setCodeSize(9);
  AvailBits = 0;

reset:
  NextCode = 0x100;
  D = SD;

  do { //read code
    Bits = (Bits<<8) | *Src++; // read byte
    AvailBits += 8;
    if (!D || --D) continue;
    Src = In;
    SD = 0;
  } while (CodeSize > AvailBits);
  SD = D;
  AvailBits -= CodeSize;
  A = (Bits>>AvailBits)&CodeMask; // read bits
  Pre = A;
  Suf = A;
  *Dst++ = A;

  for (;;) { // main loop
    D = SD;

    do { //read code
      Bits = (Bits<<8) | *Src++;
      AvailBits += 8;
      if (!D || --D) continue;
      Src = In;
      SD = 0;
    } while (CodeSize > AvailBits);
    SD = D;
    AvailBits -= CodeSize;
    A = (Bits>>AvailBits)&CodeMask;
    if (A == CodeMask) break; // we are done
    if (A == ResetCode) goto reset;
    BP = O;
    B = A;
    if (A >= NextCode) {
      D = Suf;
      *BP++ = (u1)D;
      B = Pre;
    }

    SA = A;
    C = 0;

    while (B > 0xFF) { //backref
      B = (s4)(s2)(u2)B; //sign extend
      *BP++ = S[(s4)B];
      B = P[(s4)B];
      if (++C >= BSIZE) {
        printf("LZW: buffer overrun\n");
        return Out;
      }
    }
    *BP = (u1)B;
    Suf = (u1)*BP; //movzx

    while(BP >= O) *Dst++ = *BP--; //unstack crap

    P[NextCode] = Pre;
    S[NextCode] = Suf;
    ++NextCode;
    if (NextCode > IncBits && CodeSize < 14) setCodeSize(CodeSize+1);
    Pre = SA;
  }

  return Out;
}

static void epfDecompile(char *Output, char *Input) {  
  int I;
  char Tmp[1024];
  u1 *Q;
  int L = fileSize(Input);
  u1 *M = readFile(0, L, Input);
  header *H = (header*)M;
  entry *E = (entry*)(M + H->Off);
  int Off = sizeof(header);

  unless (!strncmp(H->Id, "EPFS", 4)) {
    printf("  Not an EPFS file.\n");
    abort();
  }

  times (I, H->NFiles) {
    printf("  Extracting: %s\n", E->Name);

    sprintf(Tmp, "%s/%s", Output, E->Name);
    if (E->Compression) { //LZW
      Q = lzwDecode(E->ULen, E->Len, M+Off);
      writeFile(0, E->ULen, Tmp, Q);
      free(Q);
    } else { //plaintext
      writeFile(0, E->ULen, Tmp, M+Off);
    }

    Off += E->Len;
    E++;
  }
}


// Used in:
// Alien Breed: Tower Assault, Arcade Pool, Overdrive, Project X, Sensible Golf
// Spirou, Jungle Book, Lion King, Smurfs, Tintin in Tibet, Universe
int epfInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "epf";
  F->Description = "East Point File System";
  F->Decompile = epfDecompile;
  return 1;
}

