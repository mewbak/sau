//Credits go to SNV

#include "common.h"


#define MIN_LEN 256
#define MAX_FILES 20000

static u1 PNGHeader[] = {
  0x89,0x50,0x4e,0x47, 0x0d,0x0a,0x1a,0x0a,
  0x00,0x00,0x00,0x0d, 0x49,0x48,0x44,0x52};

static u1 PNGFooter[] = {
  0x00,0x00,0x00,0x00, 0x49,0x45,0x4e,0x44, 0xae,0x42,0x60,0x82};

static int isPNG(u1 *S, u1 *E) {
  u1 *Q;
  if (!memcmp(PNGHeader,S,16)) {
    for (Q = S+16; Q < E-12; Q++) if (!memcmp(Q,PNGFooter,12)) return Q-S+12;
    return 0;
  }
  return 0;
}

static u1 JPGHeader[] = {
  0xFF,0xD8,0xFF,0xE0, 0x00,0x10,0x4A,0x46, 0x49,0x46,0x00,0x01};

static int isJPG(u1 *S, u1 *E) {
  if (!memcmp(JPGHeader,S,12)) {
    return -0x20;
  }
  return 0;
}

static u1 OGGHeader[] = {
  0x4F,0x67,0x67,0x53, 0x00,0x02,0x00,0x00, 0x00,0x00,0x00,0x00};

static int isOGG(u1 *S, u1 *E) {
  if (!memcmp(OGGHeader,S,12)) {
    return -0x20;
  }
  return 0;
}

// Heimdall's ADF images are just a stream of RNCs
// they seemingly use different compression format
static u1 RNCHeader[] = {0x52,0x4E,0x43,0x01};
static int isRNC(u1 *S, u1 *E) {
  if (!memcmp(RNCHeader,S,4)) {
    u4 LU = u4be(S+4);
    u4 LC = u4be(S+8);
    if (LC > LU|| S+18+LC>E) return 0;
    // FIXME: RNCs have a checksum, which could also be used
    return 18+LC;
  }
  return 0;
}


typedef struct {
  u1 ID;
  u1 Version;
  u2 Reserved;
  u4 Flags;
} __attribute__ ((packed)) timHeader;


#define PMODE    7
#define HAS_CLUT 8

static int isTIM(u1 *S, u1 *E) {
  u1 *Q;
  int PL,PX,PY,PW,PH,L,X,Y,W,H,Bits;
  static int ModeBits[] = {4,8,16,24,0,0,0,0};
  timHeader *T = (timHeader*)S;
  if (T->ID != 0x10) return 0;
  if (T->Version) return 0;
  if (T->Reserved) return 0;
  if (T->Flags&0xFFFFFF00) return 0;


  Bits = T->Flags&PMODE;
  if (Bits>4) return 0;
  if (Bits > 8) return 0; //very unlikely
  if (!(T->Flags&HAS_CLUT)) return 0; //very unlikely
  Q = (u1*)(T+1);
  PL = ru4(Q);
  if (!PL) return 0;
  PX = ru2(Q);
  PY = ru2(Q);
  PW = ru2(Q);
  PH = ru2(Q);
  if (!PW || !PH || PX > 2048 || PY > 2048) return 0;
  if (PW > 256 || PH > 2048) return 0;
  if (PL != PW*PH*2+12) return 0;
  Q += PL-12;

  L = ru4(Q); //W*H-12
  X = ru2(Q);
  Y = ru2(Q);
  W = ru2(Q);
  H = ru2(Q);
  if (!W || !H || W > 2048 || H > 2048) return 0;
  if (L != W*H*2+12) return 0;
  Q += L-12;
  return Q-S;
}


static u1 HMIHeader[] = {0x48, 0x4D, 0x49, 0x2D, 0x4D, 0x49,
                         0x44, 0x49, 0x53, 0x4F, 0x4E, 0x47};
static int isHMI(u1 *S, u1 *E) {
  if (!memcmp(HMIHeader,S,12)) {
    return -0x20;
  }
  return 0;
}


static struct {
  char *Ext;
  void *Check;
} Scanners[] = {
  {"png", isPNG},
  {"jpg", isJPG},
  {"ogg", isOGG},
  {"rnc", isRNC},
  {"tim", isTIM},
  {"hmi", isHMI},
  {0,0}
};


typedef struct {
  char *Ext;
  int Off;
  int Len;
} file;

static void scanDecompile(char *Output, char *Input) {
  int I, J, K, NFiles=0, Len;
  char Tmp[1024], Name[256];
  int L = fileSize(Input);
  u1 *M = readFile(0, L, Input);
  u1 *S = M;
  u1 *E = S+L;
  file *Files = ns(file, MAX_FILES);

head:
   for (; S < E-MIN_LEN; S++) {
    for (I=0; Scanners[I].Check; I++) {
      Files[NFiles].Len = ((int (*)(u1*,u1*))Scanners[I].Check)(S,E);
      if (!Files[NFiles].Len) continue;
      Files[NFiles].Ext = Scanners[I].Ext;
      Files[NFiles].Off = S-M;
      if (Files[NFiles].Len > 0) S += Files[NFiles].Len;
      else S -= Files[NFiles].Len;
      NFiles++;
      if (NFiles == MAX_FILES) goto done;
      goto head;
    }
  }
done:

  printf ("Found %d files\n", NFiles);

  times (I, NFiles) {
    if (Files[I].Len<0)
      Files[I].Len = (I+1<NFiles ? Files[I+1].Off : L) - Files[I].Off;

    sprintf(Name, "%05d_%08X.%s", I, Files[I].Off, Files[I].Ext);
    printf ("Extracting %s\n", Name);
    sprintf(Tmp, "%s/%s", Output, Name);
    writeFile(0, Files[I].Len, Tmp, M+Files[I].Off);
  }
}

int scanInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "scan";
  F->Description = "Scans any file for known format patterns";
  F->Decompile = scanDecompile;
  return 1;
}

