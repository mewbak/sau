//Credits go to Chris Butcher for his invaluable PluginViewer sources
//              all magma-faggots for keeping source code closed

#include "common.h"

// NOTE: The games was developed on PPC, so everything is big-endian

#define F_TEXTURES      0x01
#define F_SPRITES       0x02
#define F_PROJECTILES   0x04
#define F_SCENERY       0x08
#define F_ATMOSPHERIC   0x10
#define F_INTERFACE     0x20
#define F_SOUND         0x40

typedef struct {
  u4 U1;
  u1 Name[32];
  u4 U2;
  u4 FTOff;
} __attribute__ ((packed)) header;

typedef struct {
  u1 Name[32];
  u1 Id[8];
  u4 A;
  u4 B;
  u4 Off;
  u4 Len;
  u4 User; //for `core` it holds some ASCII chars
           //for `.256` it determines bitmap type
  u1 Game[4]; // myth or mth2
} __attribute__ ((packed)) entry;

typedef struct {
  u4 Num; // number of entries
  u4 Off; // offset, relative to end of header
  u4 Len; // length of data in bytes
  u4 Pad; // 0
} __attribute__ ((packed)) spriteEntry;

typedef struct {
  u4 U0;
  u4 Type; // 256=colormap, 32=overhead, 12=sprite, 6=sprite_collection
  u1 Junk[0x38];
  spriteEntry Pals;    // palette
  spriteEntry Hues;    // color swapping info, used for on fly recoloring
  spriteEntry Images; // pixels itself
  spriteEntry Frames;  // X,Y for bitmaps
  spriteEntry Anims;   // combine frames into named sequences
  spriteEntry A;     // unknown
  spriteEntry CMaps;   // 65536-byte colormaps
  spriteEntry CMapsI;  // indices for colormaps
  spriteEntry B;
  spriteEntry C;
  spriteEntry D;
  u4 U1;
  u4 U2;
  u4 Off; // data length
  u4 Len; // data offset
} __attribute__ ((packed)) sprite;

typedef struct { // this time palette is 64-bit
  u4 NColors;
  u1 Junk[0x20-4];
  u2 Colors[256*4]; // lower 8-bits sometimes hold additional info
} __attribute__ ((packed)) pal;

// a section of the palette that can be fiddled with by a 'core' tag.
typedef struct {
  u1 Name[64];
  u2 Min;
  u2 Max; // huerange, min and max from 0 - 65535
  u2 Junk1[17];
  u2 Median; // median of huerange
  u4 Junk2[6];
}  __attribute__ ((packed)) hue;

typedef struct {
  u1 Name[0x40];
  u4 Off;
  u4 Len;
  u4 U1;
  u2 W;
  u2 H;
  u1 U2[0x30];
} __attribute__ ((packed)) image;

typedef struct {
  u2 U1[6];
  u2 X; // X/Y offset
  u2 Y;
  u2 U2[6];
  u2 Image; // frame array
  u2 U3[17];
} __attribute__ ((packed)) frame;


typedef struct { // indexes a frame
  u2 U1;      // sometimes the same as Index, sometimes -1
  u2 U2[22];  // timing?
  u2 Frame;   // index into the frames array
} __attribute__ ((packed)) animElem;


typedef struct {
  u1 U1[10];
  u2 NFrames;  // number of animElem following this header
  u1 U2[52];
  //animElem Elems[NFrames];
} __attribute__ ((packed)) animHeader;

// binds number of frames together
typedef struct {
  u1 Name[64];
  u4 Off; // relative to sprite header's Off
  u4 Len; // length of data
  u2 U1;  // flags?
  u2 U2;  // time?
  u4 Pad[13];
} __attribute__ ((packed)) anim;

typedef struct { // Offset for RLE
  u2 SX; //start X
  u2 EX; //end X
} __attribute__ ((packed)) run;


typedef struct {
  u2 W;             // width
  u2 H;             // height
  u2 Pitch;         // 0xFFFF for RLE, else it's width algined by 2
  u2 A;
  u2 Bits; // either 8 or 16
  u2 B;
  u2 C;
  u2 Align;
  u1 Pad[32]; //0
  //u1 Pixels[];
} __attribute__ ((packed)) imageHeader;


//animHeader H;         // header for the list of view elements
//animElem E[H.Len];          // the list of view elements


static void rev(void *X, int N) {
  int T, I;
  u1 *P = (u1*)X;
  times (I, N/2) {T = P[I]; P[I] = P[N-I-1]; P[N-I-1] = T;}
}

static void revEntry(spriteEntry *E) {
  rev(&E->Num, 4);
  rev(&E->Off, 4);
  rev(&E->Len, 4);
  rev(&E->Pad, 4);
}



static void saveSprite(char *Output, u1 *M, int L) {
  char Tmp[1024];
  int I, J, K, N, X, Y, CX, CY, Wide, High;
  u2 C, U, T, SX, EX;
  u1 *Q, *Z;
  pic *P;
  pal *Pal;
  anim *An;
  animHeader *AH;
  animElem *AE;
  image *Im, *Ims;
  frame *F, *Fs;
  imageHeader *IH;
  run *Run;
  sprite *H = (sprite*)M;

  revEntry(&H->Pals);
  revEntry(&H->Hues);
  revEntry(&H->Images);
  revEntry(&H->Frames);
  revEntry(&H->Anims);
  revEntry(&H->A);
  revEntry(&H->CMaps);
  revEntry(&H->CMapsI);

  rev(&H->Off, 4);
  rev(&H->Len, 4);

  Pal = (pal*)(M + H->Off + H->Pals.Off);
  rev(&Pal->NColors, 4);

  Fs = (frame*)(M + H->Off + H->Frames.Off);
  times (I, H->Frames.Num) {
    rev(&Fs[I].X, 2);
    rev(&Fs[I].Y, 2);
    rev(&Fs[I].Image, 2);
  }

  Ims = (image*)(M + H->Off + H->Images.Off);
  times (K, H->Images.Num) {
    rev(&Ims[K].Off, 4);
    rev(&Ims[K].Len, 4);
    rev(&Ims[K].W, 2);
    rev(&Ims[K].H, 2);

    IH = (imageHeader*)(M + H->Off + Ims[K].Off);
    if ((u1*)IH+Ims[K].Len > M+L) continue;
    rev(&IH->W, 2);
    rev(&IH->H, 2);
    rev(&IH->Pitch, 2);
    rev(&IH->Bits, 2);
    rev(&IH->Align, 2);
  }

  times (K, H->Frames.Num) {
    F = Fs+K;
    if (F->Image == 0xFFFF) continue; //missing
    Im = Ims+F->Image;
    IH = (imageHeader*)(M + H->Off + Im->Off);

    if ((u1*)IH+Im->Len > M+L) continue;
    //if (IH->Align) continue; // these have advanced transparency

    Wide = max(IH->W,F->X);
    High = max(IH->H,F->Y);

    if (Wide>2048 || High >2048) continue;
    P = picNew(Wide*2, High*2, 8);
    CX = Wide-F->X;
    CY = High-F->Y;
    times (I, min(Pal->NColors, 256)) {
      P->P[I*4+0] = Pal->Colors[I*4+0]&0xff;
      P->P[I*4+1] = Pal->Colors[I*4+1]&0xff;
      P->P[I*4+2] = Pal->Colors[I*4+2]&0xff;
      P->P[I*4+3] = Pal->Colors[I*4+3]&0xff;
    }
    P->K = 0;

    Q = (u1*)(IH+1);
    Q += IH->H*4; // skip row info
    if (IH->Pitch == 0xFFFF) { // RLE
      times (Y, IH->H) {
        U = ru2(Q); // number of RLE runs
        C = ru2(Q); // total bytes
        rev(&U, 2);
        rev(&C, 2);
        Run = (run*)Q; Q += U*sizeof(run);
        times (I, U) {
          SX = Run->SX;
          EX = Run->EX;
          rev(&SX, 2);
          rev(&EX, 2);
          //printf("  %d %X %X : %X %X\n", K, U, C, SX, EX);
          for (X=SX; X < EX; X++) {
            if (IH->Bits==8) picPut(P,CX+X,CY+Y,*Q++);
            else {
              T = ru2(Q);
              picPut(P,CX+X,CY+Y,T>>8);
            }
          }
          Run++;
        }
        if (C&1 && !IH->Align) Q++; // align to 2
      }
    } else { // uncompressed
      times (Y, IH->H) times (X, IH->W) {
        picPut(P,X,Y,Q[Y*IH->Pitch+X]);
      }
    }

    sprintf(Tmp, "%s/%04d.png", Output, K);
    pngSave(Tmp, P);
    picDel(P);
  }
}


static int IMA_Adjust[16] = {-1, -1, -1, -1, 2,  4,  6,  8
                            ,-1, -1, -1, -1, 2,  4,  6,  8};

static int IMA_Step[89] = {
  7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21, 23, 25, 28, 31, 34, 37, 41, 45,
  50, 55, 60, 66, 73, 80, 88, 97, 107, 118, 130, 143, 157, 173, 190, 209, 230,
  253, 279, 307, 337, 371, 408, 449, 494, 544, 598, 658, 724, 796, 876, 963,
  1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024, 3327,
  3660, 4026, 4428, 4871, 5358, 5894, 6484, 7132, 7845, 8630, 9493, 10442,
  11487, 12635, 13899, 15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794,
  32767};

static u1 *imaDecode(u1 *M, int L, int Chns, int *ULen) { //sub_401990
  int I, J, K, StepIndex, Predictor, Step, Ptr, C, D, Len;
  u2 *O, U;

  Len = L/34/Chns;
  *ULen = Len*64*sizeof(s2)*Chns;

  O = (u2*)ns(u1, *ULen);

  times (I, Len) {
    times (K, Chns) {
      U = ru2(M);
      rev(&U,2);
      Predictor = (s2)(U&0xFF80);
      StepIndex = U&0x7F;
      times (J, 64) {
        if (StepIndex < 0) StepIndex = 0;
        if (StepIndex > 88) StepIndex = 88;
        Step = IMA_Step[StepIndex];

        if (J&1) C = *M++ >> 4;
        else C = *M&0xF;

        D = Step >> 3;
        if (C&1) D += Step >> 2;
        if (C&2) D += Step >> 1;
        if (C&4) D += Step;
        if (C&8) D = -D;
        Predictor += D;
        if (Predictor < -32768) Predictor = -32768;
        else if (Predictor > 32767) Predictor = 32767;
        O[(I*64 + J)*Chns + K] = Predictor;
        StepIndex += IMA_Adjust[C];
      }
    }
  }
  return (u1*)O;
}


typedef struct {
  u4 Flags;
  u1 U1[14];
  u2 SubLine; // subtitle line number
  u4 VarOff;
  u4 VarLen;
  u4 Sub; // subtitle id
  u1 U2[6];
  u2 VarNum; // number of variations
  u4 VarNamesOff;
  u4 VarNamesLen;
  u1 U3[16];
} __attribute__ ((packed)) sounHeader;

typedef struct {
  u4 Chance; // how often to pick this sample (16.16 fixed point number)
  u2 Unknown;
  u1 Name[26];
} __attribute__ ((packed)) varName;

typedef struct {
  u4 U1;
  u2 Bits; // 16
  u2 U2;
  u2 Chns;
  u2 U3;
  u4 Freq; // Sample Rate (16.16 fixed point number)
           // 56220000 hex means 22050.0
  u4 Len; // Number of IMA blocks
          // IMA ADPCM encoded AIFF files have a block length of 34
          // One block decodes to 64 16-bit samples.
          // Each variation has 34*Len bytes of compressed data.
          // stereo channel blocks are interleaved.
  u1 U4[12];
} __attribute__ ((packed)) var;

static void saveSound(char *Output, u1 *M, int L) {
  int I, J, K, Len, ULen;
  u1 *Z, *Out;
  char Tmp[1024];
  varName *N;
  var *V;
  sounHeader *H = (sounHeader*)M;

  rev(&H->SubLine, 2);
  rev(&H->VarNum, 2);
  rev(&H->VarOff, 4);
  rev(&H->VarLen, 4);
  rev(&H->VarNamesOff, 4);
  rev(&H->VarNamesLen, 4);
  rev(&H->Flags, 4);
  rev(&H->Sub, 4);

  N = (varName*)(M+H->VarNamesOff);
  V = (var*)(M+H->VarOff);
  Z = (M+H->VarOff + H->VarNum*sizeof(var));
  times (I, H->VarNum) {
    rev(&V->Bits, 2);
    rev(&V->Chns, 2);
    rev(&V->Freq, 4);
    rev(&V->Len, 4);
    V->Freq >>= 16;
    Len = 34*V->Len;
    printf("    Chns=%d Freq=%d Bits=%d Len=%X: %s\n", V->Chns, V->Freq, V->Bits, Len, N->Name);
    if (!V->Chns || !V->Len) goto skip; //missing

    Out = imaDecode(Z, Len, V->Chns, &ULen);
    sprintf(Tmp, "%s/%s.wav", Output, N->Name);
    wavSave(Tmp, Out, ULen, V->Bits, V->Chns, V->Freq);
    free(Out);
skip:
    Z += Len;
    N++;
    V++;
  }
}

typedef struct { // for standalone tagged files
  u2 Head; // inited after loading
  u1 Flags;
  u1 Type;
  u1 Name[32];
  u1 Id[8];  u4 Off; // valid no matter what kind of file (single file, archive, etc)
  u4 Len;
  u4 User; // depends on tag type
  u2 Version; // depends on tag type
  u1 Archive; //   small install       0
              //   medium install      1
              //   large install       2
              //   small interface     3
              //   large interface     4
  u1 Priority; // used by memory manager
  u1 Game[4]; //mth2
} __attribute__ ((packed)) tag;


typedef struct {
  u2 Type;  // 0=tag 1=plugin 2=patch 3=arhive 4=cache
  u2 Version;
  u1 Name[32];
  u1 Author[64];
  u2 NMeshes;
  u2 NFiles;
  u4 CRC;
  u4 Flags;
  u4 Size; // size of this whole file
  u4 HeaderCRC; // calculated with this field set to zero
  u4 Unused;
  u1 Id[4]; // unique ID of this file
} __attribute__ ((packed)) plgHeader;


// An entry-point structure. Every plugin file has one entrypoint
// structure for every tag of type 'mesh' that it contains.
typedef struct {
  u1 MeshID[4]; // ID of the corresponding mesh tag
  u4 U1;
  u4 U2;
  u4 U3;
  u1 TagName[32];
  u1 Name[64];
} __attribute__ ((packed)) plgMesh;

// myth2 archives are called "plugins"
static void pluginDecompile(char *Output, char *Input) {
  char Tmp[1024], Name[256], Id[128], Dir[512], OutName[256];
  int I, J, K, C, Off, Len, NFiles, L = fileSize(Input);
  u1 *M, *Q, *Z;
  tag *E;
  plgHeader *H = readFile(0, sizeof(plgHeader), Input);

  rev(&H->NFiles, 2);
  rev(&H->NMeshes, 2);

  printf("  NFiles=%d NMeshes=%d\n", H->NFiles, H->NMeshes);

  E = readFile(sizeof(plgHeader)+H->NMeshes*sizeof(plgMesh)
              ,H->NFiles*sizeof(tag)
              ,Input);

  times (I, H->NFiles) {
    rev(&E->Off, 4);
    rev(&E->Len, 4);
    rev(&E->User, 4);

    Len = E->Len;

    memcpy(Name, E->Name, 32);
    memcpy(Id, E->Id, 8);
    Name[32] = 0;
    Id[8] = 0;


    for(Z = Name; *Z; Z++) {
       if (*Z == 0xD5) *Z = '\'';
       else if (*Z == 0xC9) {
         strcpy(Z,"...");
         break;
       }
    }

    //printf("  %08X %08X %08X: %s/%s\n",E->A, E->B, E->Type, Id, Name);
    if (Id[0] == '.') { //Else Linux will hide it. Yeah! Linux sucks that much.
      memcpy(Dir, Id+1, 3);
      Dir[3] = 0;
    } else {
      memcpy(Dir, Id, 4);
      Dir[4] = 0;
    }

    //unless(I==165 /*!strcmp(Name,"19 barrier device")*/) goto skip;
    //printf("%d\n", I);

    if (!memcmp(Id, ".256", 4)) {
      if (E->User&F_SPRITES) strcat(Dir, "/units");
      else if (E->User&F_INTERFACE) strcat(Dir, "/interface");
      else if (E->User&F_SCENERY) strcat(Dir, "/scenery");
      else if (E->User&F_TEXTURES) strcat(Dir, "/textures");
      else strcat(Dir, "/unsorted");
    }

    sprintf(OutName, "%s/%s", Dir, Name);
    printf("  Extracting: %s\n", OutName);

    M = readFile(E->Off, Len, Input);

    sprintf(Tmp, "%s/%s", Output, OutName);
    if (!memcmp(Id, ".256", 4)) saveSprite(Tmp, M, Len);
    else if (!memcmp(Id, "soun", 4)) saveSound(Tmp, M, Len);
    else writeFile(0, Len, Tmp, M);
    free(M);
skip:
    E++;
  }
}

static void gorDecompile(char *Output, char *Input) {
  char Tmp[1024], Name[256], Id[128], Dir[512], OutName[256];
  int I, J, K, C, Off, Len, NFiles, L = fileSize(Input);
  u1 *M, *Q, *Z;
  entry *E;
  header *H = readFile(0, 64, Input);
  tag *T = (tag*)H;

  // check for standalone file
  if (!memcmp(T->Game, "mth2", 4) || !memcmp(T->Game, "myth", 4)) {
    rev(&T->Off, 4);
    rev(&T->Len, 4);
    rev(&T->User, 4);
    rev(&T->Version, 2);

    Len = T->Len;
    memcpy(Id, T->Id, 8);
    Id[8] = 0;

    if (Id[0] == '.') { //Else Linux will hide it. Yeah! Linux sucks that much.
      memcpy(Dir, Id+1, 3);
      Dir[3] = 0;
    } else {
      memcpy(Dir, Id, 4);
      Dir[4] = 0;
    }

    M = readFile(sizeof(tag), Len, Input);

    strcpy(Tmp, Output);
    if (!strcmp(Dir, "256")) saveSprite(Tmp, M, Len);
    else if (!strcmp(Dir, "soun")) saveSound(Tmp, M, Len);
    else writeFile(0, Len, Tmp, M);
    return;
  }
  if (strcmp(H->Name+strlen(H->Name)-4, ".gor")) { // check for myth2 archive
    pluginDecompile(Output, Input);
    return;
  }

  rev(&H->FTOff, 4);
  E = readFile(H->FTOff, L-H->FTOff, Input);
  NFiles = (L-H->FTOff)/sizeof(entry);

  times (I, NFiles) {
    rev(&E->Off, 4);
    rev(&E->Len, 4);
    rev(&E->User, 4);
    rev(&E->A, 4);
    rev(&E->B, 4);

    Len = E->Len;

    memcpy(Name, E->Name, 32);
    memcpy(Id, E->Id, 8);
    Name[32] = 0;
    Id[8] = 0;


    for(Z = Name; *Z; Z++) {
       if (*Z == 0xD5) *Z = '\'';
       else if (*Z == 0xC9) {
         strcpy(Z,"...");
         break;
       }
    }

    //printf("  %08X %08X %08X: %s/%s\n",E->A, E->B, E->Type, Id, Name);
    if (Id[0] == '.') { //Else Linux will hide it. Yeah! Linux sucks that much.
      memcpy(Dir, Id+1, 3);
      Dir[3] = 0;
    } else {
      memcpy(Dir, Id, 4);
      Dir[4] = 0;
    }

    //unless(!strcmp(Name,"skrael")) goto skip;

    if (!memcmp(Id, ".256", 4)) {
      if (E->User&F_SPRITES) strcat(Dir, "/units");
      else if (E->User&F_INTERFACE) strcat(Dir, "/interface");
      else if (E->User&F_SCENERY) strcat(Dir, "/scenery");
      else if (E->User&F_TEXTURES) strcat(Dir, "/textures");
      else strcat(Dir, "/unsorted");
    }

    sprintf(OutName, "%s/%s", Dir, Name);
    printf("  Extracting: %s\n", OutName);

    M = readFile(E->Off, Len, Input);

    sprintf(Tmp, "%s/%s", Output, OutName);
    if (!memcmp(Id, ".256", 4)) saveSprite(Tmp, M, Len);
    else if (!memcmp(Id, "soun", 4)) saveSound(Tmp, M, Len);
    else writeFile(0, Len, Tmp, M);
    free(M);

skip:
    E++;
  }
}

int gorInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "gor";
  F->Description = "Myth Fallen Lords Archives and Tagged Files";
  F->Decompile = gorDecompile;
  return 1;
}

