//Credits go to SNV

#include "common.h"


#define LOD_IMAGE        0x010
#define LOD_ZLIB_NORMAL  0x100
#define LOD_ZLIB_IMAGE   0x200

typedef struct {
  u1 ID[4];    // LOD\0
  u4 U1;       // version?
  u4 NFiles;
  u1 u1[0x50]; // zeroes
} __attribute__ ((packed)) H3LodHeader;


typedef struct {
  u1 Name[16]; // zero terminated
  u4 Offset;
  u4 ULen;     // uncompressed size
  u4 Type;     // type, like adventure_map_object or adventure_map_hero
  u4 Len;      // conpressed size
} __attribute__ ((packed)) H3LodEntry;

typedef struct {
  u4 Size;   // size of Pixels array
  u4 Width;
  u4 Height;
  //u1 Pixels[Size];
  //u1 Palette[256*3]; // if 8-bit image
} __attribute__ ((packed)) H3ImageHeader;


typedef struct {
  u1 ID[4]; // LOD\0
  u1 Game[80]; // either "MMVI" or "GameMMVI" (for games.lod)
               // "MMVIII" used for EnglishT.lod and EnglishD.lod in MM8
               // else it's HOMM3 LOD
  u1 Description[172];
} __attribute__ ((packed)) LodHeader;

typedef struct {
  u1 Name[16];
  u4 Offset;
  u4 Length;
  u4 U1; // unknown
  u4 SubEntries; // 0 for files
} __attribute__ ((packed)) LodEntry;


// icons.lod and bitmaps.lod
typedef struct {
  u1 Name[16];
  u4 ILen;    // uncompressed length for images, zero otherwise
  u4 Len;     // compressed size
  u2 Width;   // zero for non-image files
  u2 Height;
  u4 U1;

  u4 U2;
  u4 U3;
  u4 ULen; // zero for non-compressed
  u4 Type; // 0x010 - uncompressed image
           // 0x100 - compressed data
           // 0x200 - compressed image

  // Most files in LODs are LZ-compressed (like ZIP archives).
  // u1 ZlibStream[Len]; // #78 #9C ...

  // u1 Palette[256*3]; // it us uncompressed
} __attribute__ ((packed)) FileHeader;


// AFAIK, MMVI sprite-system originaly used DEFs (HOMM3 format),
// so it uses DEF-like RLE-copying
typedef struct {
  u2 SX;  // where to copy pixels (if 0xffff, scanline is empty)
  u2 EX;  // when to end copying (if 0xffff, scanline is empty)
  u4 Off; // from where to copy
} __attribute__ ((packed)) scan;


// sprites.lod zlib header
typedef struct {
  u1 Name[16];
  u2 Width;
  u2 Height;
  u2 Palette; // number of pal* file in bitmaps.lod
  u2 U1; // 0
  u2 U2; // 0
  u2 U3; // 0xFFFF
  u4 ULen; // uncompressed length (zero if uncompressed)

  //scan Scans[Height];

  // u1 ZlibStream[Len];
  // u1 Palette[256*3]; // it us uncompressed
} __attribute__ ((packed)) FrameHeader;


// games.lod zlib header for MM6
typedef struct {
  u4 Len;
  u4 ULen;
  //u1 ZlibStream[Len];
} __attribute__ ((packed)) MM6_BLVHeader;

// games.lod zlib header for MM7 and MM8
typedef struct {
  u1 Id1[4]; // \41\67\01\00
  u1 Id2[4]; // mvii
  u4 Len;
  u4 ULen;
  //u1 ZlibStream[Len];
} __attribute__ ((packed)) MM7_BLVHeader;


// MM8 uses different LOD format for EnglishT.lod and EnglishD.lod files
// it extends Name field from 16 to 64 bytes
typedef struct { // sizeof = 76
  u1 Name[64];
  u4 Offset;
  u4 Length;
  u4 SubEntries; // should be 0 for files
} __attribute__ ((packed)) MM8LodEntry;

typedef struct {
  u1 Name[64];
  u4 ILen;    // uncompressed length for images, zero otherwise
  u4 Len;     // compressed size
  u2 Width;   // zero for non-image files
  u2 Height;
  u4 U1;

  u4 U2;
  u4 U3;
  u4 ULen;
  u4 Type;
  // u1 Palette[256*3]; // it us uncompressed
} __attribute__ ((packed)) MM8FileHeader;



static u1 **Pals;

// FIXME: we are missing some files, if name in file table doesnt equal name in file header, try seeking futurestatic void extractFile(char *Output, char *Input, u4 Offset, u4 Length) {
  char Name[32], Tmp[1024];
  int I;
  pic *P;
  u1 *Out, *Palette;
  u1 *In = readFile(Offset, Length, Input);
  FileHeader *H = (FileHeader*)In;
  char Ext[32];

  memcpy(Name, H->Name, 16);
  Name[16] = 0;

  I = strlen(Name);
  if (I < 15 && Name[I+1]) {
    Name[I] = '.';
    strcpy(Ext, Name+I+1);
    downcase(Ext);
  } else {
    Ext[0] = 0;
  }

  /*printf("  Type=#%03x W=%04d H=%04d U1=%03d, U2=#%x, U3=#%x %d/%d:%d (%s)\n"
        , H->Type, H->Width, H->Height, H->U1, H->U2, H->U3
        , H->Len, H->ULen, H->ILen, H->Name);*/
  //sprintf(Tmp, "%s.bin", Output);
  //writeFile(0, Length, Tmp, In);

  if (H->ULen) {
    Out = inflateBuf(H->ULen, H->Len, In + sizeof(FileHeader));
  } else {
    if (!H->Len) H->Len = max(Length-0x30, 0); // *.pal files dont have H->Len
    Out = ns(u1, H->Len);
    memcpy(Out, In + sizeof(FileHeader), H->Len);
    H->ULen = H->Len;
  }

  if (!strcmp(Ext, "pcx") || !strcmp(Ext, "tga")) {
    P = picNew(H->Width, H->Height, 8);
    memcpy(P->D, Out, H->Width*H->Height);
    Palette = In + sizeof(FileHeader) + H->Len;
    times (I, 256) {
      P->P[I*4+0] = Palette[I*3+0];
      P->P[I*4+1] = Palette[I*3+1];
      P->P[I*4+2] = Palette[I*3+2];
      P->P[I*4+3] = 0;
    }
    I = strlen(Output);
    if (Output[I-4] != '.') {
      Output[I] = '.';
      strcpy(Output+I+1, Ext);
    }
    if (!strcmp(Ext, "pcx")) pcxSave(Output, P);
    else tgaSave(Output, P);
    picDel(P);
  } else {
    writeFile(0, H->ULen, Output, Out);
  }

  free(In);
  free(Out);
}

static void extractFileMM8(char *Output, char *Input, u4 Offset, u4 Length) {
  char Name[32], Tmp[1024];
  int I;
  pic *P;
  u1 *Out, *Palette;
  u1 *In = readFile(Offset, Length, Input);
  MM8FileHeader *H = (MM8FileHeader*)In;
  char Ext[32];

  memcpy(Name, H->Name, 16);
  Name[16] = 0;

  I = strlen(Name);
  if (I < 15 && Name[I+1]) {
    Name[I] = '.';
    strcpy(Ext, Name+I+1);
    downcase(Ext);
  } else {
    Ext[0] = 0;
  }

  if (H->ULen) {
    Out = inflateBuf(H->ULen, H->Len, In + sizeof(MM8FileHeader));
  } else {
    if (!H->Len) H->Len = max(Length-0x30, 0); // *.pal files dont have H->Len
    Out = ns(u1, H->Len);
    memcpy(Out, In + sizeof(MM8FileHeader), H->Len);
    H->ULen = H->Len;
  }

  if (!strcmp(Ext, "pcx") || !strcmp(Ext, "tga")) {
    P = picNew(H->Width, H->Height, 8);
    memcpy(P->D, Out, H->Width*H->Height);
    Palette = In + sizeof(MM8FileHeader) + H->Len;
    times (I, 256) {
      P->P[I*4+0] = Palette[I*3+0];
      P->P[I*4+1] = Palette[I*3+1];
      P->P[I*4+2] = Palette[I*3+2];
      P->P[I*4+3] = 0;
    }
    I = strlen(Output);
    if (Output[I-4] != '.') {
      Output[I] = '.';
      strcpy(Output+I+1, Ext);
    }
    if (!strcmp(Ext, "pcx")) pcxSave(Output, P);
    else tgaSave(Output, P);
    picDel(P);
  } else {
    writeFile(0, H->ULen, Output, Out);
  }

  free(In);
  free(Out);
}

static void extractFileH3(char *Output, char *Input, u4 Off, u4 ULen, u4 Len, u4 Type) {
  int I;
  pic *P;
  u1 *Palette;
  H3ImageHeader *IH;
  u1 *In = readFile(Off, Len, Input);
  u1 *Out = inflateBuf(ULen, Len, In);
  IH = (H3ImageHeader*)Out;
  if (Type == 0x10) { // 8-bit image
    P = picNew(IH->Width, IH->Height, 8);
    memcpy(P->D, Out+sizeof(H3ImageHeader), IH->Width*IH->Height);
    Palette = Out+ULen-256*3;
    times (I, 256) {
      P->P[I*4+0] = Palette[I*3+0];
      P->P[I*4+1] = Palette[I*3+1];
      P->P[I*4+2] = Palette[I*3+2];
      P->P[I*4+3] = 0;
    }
    pcxSave(Output, P);
    picDel(P);
  } else if (Type == 0x11) { // 24-bit image
    P = picNew(IH->Width, IH->Height, 24);
    memcpy(P->D, Out+sizeof(H3ImageHeader), IH->Width*IH->Height*3);
    pcxSave(Output, P);
    picDel(P);
  } else { // plain file
    writeFile(0, ULen, Output, Out);
  }
  free(In);
  free(Out);
}

static void extractSprite(char *Output, char *Input, u4 Offset, u4 Length) {
  char Name[32], Tmp[1024];
  int I, J, K = 0;
  pic *P;
  u1 *Out, *Palette;
  u1 *In = readFile(Offset, Length, Input);
  FrameHeader *H = (FrameHeader*)In;
  scan *S = (scan*)(In+sizeof(FrameHeader));
  int CLen = Length-(sizeof(FrameHeader)+sizeof(scan)*H->Height);
  u1 *Z = In + Length-CLen;

  //sprintf(Tmp, "%s.bin", Output);
  //writeFile(0, Length, Tmp, In);

  //printf("  W=%d H=%d P=%d U1=#%x U2=#%x U3=#%x (%s)\n"
  //      , H->Width, H->Height, H->Palette, H->U1, H->U2, H->U3, H->Name);


  Palette = Pals[H->Palette];
  unless (Palette) {
    printf ("  Missing palette: image wont be extracted.\n");
    free (In);
    return;
  }

  if (H->ULen) {
    Out = inflateBuf(H->ULen, CLen, Z);
  } else {
    H->ULen = CLen;
    Out = ns(u1, H->ULen);
    memcpy(Out, Z, H->ULen);
  }
  //sprintf(Tmp, "%s.raw", Output);
  //writeFile(0, Length, Tmp, Out);

  P = picNew(H->Width, H->Height, 8);

  times (I, 256) {
    P->P[I*4+0] = Palette[I*3+0];
    P->P[I*4+1] = Palette[I*3+1];
    P->P[I*4+2] = Palette[I*3+2];
    P->P[I*4+3] = 0;
  }

  times (I, P->H)
    for (J = S[I].SX, K=S[I].Off; J <= S[I].EX; J++, K++)
      if (J<P->W && K<H->ULen)
        P->D[I*P->I + J] = Out[K];

  sprintf(Tmp, "%s.pcx", Output);
  pcxSave(Tmp, P);

  //abort();
  picDel(P);
  free(In);
  free(Out);
}

static void extractMap(char *Output, char *Input, u4 Offset, u4 Length) {
  u1 *In = readFile(Offset, Length, Input);
  u1 *Out;
  MM6_BLVHeader *H6 = (MM6_BLVHeader*)In;
  MM7_BLVHeader *H7 = (MM7_BLVHeader*)In;
  if (!strncmp(H7->Id2, "mvii", 4)) { // MM7, MM8
    Out = inflateBuf(H7->ULen, H7->Len, In+sizeof(MM7_BLVHeader));
    writeFile(0, H7->ULen, Output, Out);
  } else { // MM6
    Out = inflateBuf(H6->ULen, H6->Len, In+sizeof(MM6_BLVHeader));
    writeFile(0, H6->ULen, Output, Out);
  }
  free(In);
  free(Out);
}

static void plainCopy(char *Output, char *Input, u4 Offset, u4 Length) {
  u1 *In = readFile(Offset, Length, Input);
  writeFile(0, Length, Output, In);
  free(In);
}

static void lodDecompile (char *Output, char *Input) {
  char Tmp1[1024];
  char Tmp2[1024];
  char Ext[32];
  LodEntry *Dir;
  LodEntry *Stack[256];
  MM8LodEntry *E8;
  H3LodHeader *HH;
  H3LodEntry *HE;
  char *P;
  void *B;
  u4 O;
  int I, SP = 0;
  int NumFiles;
  char LodName[128];
  char LodDir[1024];
  pathParts(LodDir, LodName, 0, Input);
  downcase(LodName);

  LodHeader *H = readFile(0, sizeof(LodHeader), Input);
  if(!H || strncmp(H->ID, "LOD", 4)) {
    printf("Not a LOD file: %s\n", Input);
    abort();
  }

  if (!strncmp(H->Game, "MMVIII", 6)) { // MM8
    Dir = readFile(sizeof(LodHeader), sizeof(LodEntry), Input);
    E8 = readFile(sizeof(LodHeader)+sizeof(LodEntry)
                 ,Dir->SubEntries*sizeof(MM8LodEntry), Input);
    times (I, Dir->SubEntries) {
      E8[I].Offset += sizeof(LodHeader)+sizeof(LodEntry);
      sprintf(Tmp1, "%s/%s", Dir->Name, E8[I].Name);
      sprintf(Tmp2, "%s/%s", Output, Tmp1);
      printf("Extracting: %s\n", Tmp1);
      //printf("  Sub=%d, Off=#%x, Len=#%x\n"
      //      ,E8[I].SubEntries, E8[I].Offset, E8[I].Length);
      extractFileMM8(Tmp2, Input, E8[I].Offset, E8[I].Length);
    }
    return;
  }

  // check for HOMM3 LOD
  if (strncmp(H->Game, "MMVI", 4) && strncmp(H->Game, "GameMMVI", 8)) {
    HH = (H3LodHeader*)H;
    HE = readFile(sizeof(H3LodHeader), HH->NFiles*sizeof(H3LodEntry), Input);
    printf("%d\n", sizeof(H3LodHeader));
    times (I, HH->NFiles) {
      printf("Extracting: %s\n", HE[I].Name);
      //printf("  Type=#%02x, Off=#%x, Len=#%d/%d\n"
      //     ,HE[I].Type, HE[I].Offset, HE[I].Len, HE[I].ULen);

      sprintf(Tmp1, "%s/%s", Output, HE[I].Name);
      extractFileH3(Tmp1, Input, HE[I].Offset, HE[I].ULen, HE[I].Len, HE[I].Type);
    }
    return;
  }

  // sprites.lod requires palettes from bitmaps.lod
  if (!strcmp(LodName, "sprites")) {
     sprintf(Tmp1, "%s/Bitmaps.lod", LodDir);
     if (!fileP(Tmp1)) {
       sprintf(Tmp1, "%s/BITMAPS.LOD", LodDir);
       if (!fileP(Tmp1)) {
         sprintf(Tmp1, "%s/bitmaps.lod", LodDir);
         if (!fileP(Tmp1)) {
           printf("Sprites.lod requires %s, which is missing.\n", Tmp1);
           abort();
         }
       }
     }
     Pals = ns(u1*,0x10000);
     lodDecompile (0, Tmp1);
  }

  Stack[0] = readFile(sizeof(LodHeader), sizeof(LodEntry), Input);

  while(SP>=0) {
    if(Stack[SP]->SubEntries == 0xFFFFFFFF) {
      free(Stack[SP--]);
      continue;
    }
    if(Stack[SP]->SubEntries > 0) {
      Stack[SP+1] = readFile(Stack[SP]->Offset, sizeof(LodEntry), Input);
      Stack[SP]->Offset += sizeof(LodEntry);
      if(--Stack[SP]->SubEntries == 0) Stack[SP]->SubEntries = 0xFFFFFFFF;
      ++SP;
      //printf("U1=%d, Off=#%x, Len=#%x\n"
      //      ,Stack[SP]->U1, Stack[SP]->Offset, Stack[SP]->Length);
    } else {
      if (!Output) {
        strcpy(Tmp1, Stack[SP]->Name);
        downcase(Tmp1);
        if (!strncmp(Tmp1, "pal", 3) && strlen(Tmp1)==6) {
          I = atoi(Tmp1+3);
          O = Stack[SP]->Offset+sizeof(LodHeader)+32;
          Pals[I] = readFile(O+sizeof(FileHeader), 3*256, Input);
        }
        goto next;
      }

      P = Tmp1;
      for (I = 0; I<SP+1; I++) {
        if (I>0) *P++ = '/';
        strcpy(P, Stack[I]->Name);
        P += strlen(Stack[I]->Name);
      }

      printf("Extracting: %s\n", Tmp1);

      sprintf(Tmp2, "%s/%s", Output, Tmp1);

      Ext[0] = 0;
      P = strrchr(Stack[SP]->Name, '.');
      if (P) {
         strcpy(Ext, P+1);
         downcase(Ext);
      }
      O = Stack[SP]->Offset+sizeof(LodHeader)+32;

      if (!strcmp(LodName, "games") || !strcmp(LodName, "new")) {
        if (!strcmp(Ext, "blv") || !strcmp(Ext, "dlv")
        ||  !strcmp(Ext, "odm") || !strcmp(Ext, "ddm")) {
          extractMap(Tmp2, Input, O, Stack[SP]->Length);
        } else {
          plainCopy(Tmp2, Input, O, Stack[SP]->Length);
        }
      } else if (!strcmp(LodName, "sprites")) {
        extractSprite(Tmp2, Input, O, Stack[SP]->Length);
      } else {
        extractFile(Tmp2, Input, O, Stack[SP]->Length);
      }
    next:
      Stack[SP]->SubEntries = 0xFFFFFFFF;
    }
  }
}

int lodInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "lod";
  F->Description = "LOD archives (Might & Magic 6-8, Heroes of Might & Magic 3)";
  F->Decompile = lodDecompile;
  return 1;
}

