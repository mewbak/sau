//Credits go to SNV

#include "common.h"


#define TILE_PLAIN 1
#define TILE_RLE   3

typedef struct {
  u4 Magic1; //CYLB (cyberlore library)
  u4 Magic2; //PC\20\20
  u4 Version; // 32-bit float
  u4 NTypes; // number of directories
  u4 FTSize; // size of file table (excluding header and directories)
} __attribute__ ((packed)) header;

typedef struct {
  u1 Id[4]; //IMAG, TILE, etc...
  u4 Off; // relative to start of file
} __attribute__ ((packed)) dir;

typedef struct {
  u1 DataID[4];
  u1 Name[16];
  u4 Off;
  u4 Len;
} __attribute__ ((packed)) entry;

// Palettes are at the end of file.
// Usual size of one palette is 1032 (#408).
typedef struct { // sizeof is 26
  u2 Type; // 1=Plain, 2=???, 3=RLE
  u2 High;
  u2 Wide;
  u2 Pitch; // Width*Bits+Padding
  u2 Flags; // #00003 - Bits: 0=8, 1=16, 2=24, 3=32
            // #0000C - Pixel Format: 0=R5G5B5A1, 4=R5G6B5, 8=A4R4G4B4
            //                        12=uses_some_precalc_remapper
            // #00020 - only when NPalettes = 0, load palette from CAM using
            //           Palette as DataID
            // #01F00 - blitter options
            // #10000 - controls memory manager behaviour
  s2 X; // X/Y offset
  s2 Y;
  u2 BlitType;
  u4 ColorKey;
  u2 NPalettes; // each palette is 1032 (#408) bytes in length
                // all palettes are at the end of file
  u4 Palette; // SPLT offset or 0 if image is 16-bit.
              // #00020 of Flags is true, it holds DataID of SPLT.
  //u1 Data[FileLen-sizeof(tile)-NPalettes*1032];
} __attribute__ ((packed)) tile;


typedef struct {
  u2 U1;
  u2 NColors;
  u2 U2;
  u2 U3;
  //Colors[NColors*4];
} __attribute__ ((packed)) pal;



static u1 **SPLT;

static void saveTile(char *Output, char *Name, u1 *M, int L) {
  char Tmp[1024], Txt[1024];
  int I, J, C, X, Y, A, R, G, B;
  int End, Len, Code, Run;
  u4 *Ys;
  pic *P;
  tile *H = (tile*)M;
  int Bits = ((H->Flags&3)+1)*8;
  int PF = H->Flags&0xC;
  int Blit = (H->Flags&0x1F00)>>8;
  int Mem = (H->Flags>>16)&1;
  char *PFName, *TypeName;
  u1 *Q = M+sizeof(tile);

  if (PF == 0) PFName = "R5G5B5A1";
  else if (PF == 4) PFName = "R5G6B5";
  else if (PF == 8) PFName = "A4R4G4B4";
  else PFName = "Unknown";

  if (H->Type==1) TypeName = "Plain";
  else if (H->Type==3) TypeName = "RLE";
  else TypeName = "Unknown";

  printf("    %dx%dx%d:%d,%d:%s:%d Type=%d=%s Blit=#%x Mem=%d Pal=%08x\n"
        , H->Wide, H->High, Bits, H->X, H->Y, PFName, H->Pitch, H->Type
        , TypeName, Blit, Mem, H->Palette);


  if (H->Type != TILE_PLAIN && H->Type != TILE_RLE) {
    printf("    Unsupported TILE type\n");
    return;
  }

  if (Bits == 8) {
    P = picNew(H->Wide, H->High, 8);
    if (H->Flags&0x20) {
      if (H->Palette>=(1<<16) || !SPLT[H->Palette]) {
        printf("    Sprite references missing SPLT (%d)\n", H->Palette);
        abort();
      }
      memcpy(P->P, SPLT[H->Palette]+8, 256*4);
    } else {
      memcpy(P->P, M+H->Palette+8, 256*4);
    }
    if (H->Type == TILE_PLAIN) memcpy(P->D, M+sizeof(tile), P->W*P->H);
    else {
      Ys = (u4*)Q;
      times (Y, P->H) {
        Q = M + *Ys++ + sizeof(tile);
        //printf("  %04d: #%08x\n", Y, Q-M);
        X = 0;
        do {
          //printf("    %04d: %04d %04d\n", X, *(u2*)Q, *(u2*)(Q+2) & 0x7FFF);
          Run = *(u2*)Q;
          Q += 2;
          Code = *(u2*)Q;
          Len = *(u2*)Q & 0x7FFF;
          Q += 2;

          //X += Run-(X+Len);
          Run = Run-(X+Len);
          while (Run--) picPut(P, X++, Y, 0xFF);
          while (Len--) picPut(P, X++, Y, *Q++);
        } while (!(Code&0x8000));
        while (X<P->W) picPut(P, X++, Y, 0xFF);
      }
    }
  } else if (Bits == 16) {
    if (PF != 0 && PF != 4) {
      printf("    Cant handle %s TILEs\n", PFName);
      abort();
    }

    P = picNew(H->Wide, H->High, 24);
    if (H->Type == TILE_PLAIN) {
      times (Y, P->H) {
        times (X, P->W) {
          if (PF == 0) {
            fromR5G6B5(R,G,B,*(u2*)Q); Q+=2;
            picPut24(P, X, Y, R8G8B8(R,G,B));
          } else {
            fromR5G5B5A1(A,R,G,B,*(u2*)Q); Q+=2;
            picPut24(P, X, Y, R8G8B8(R,G,B));
          }
        }
      }
    } else {
      Ys = (u4*)Q;
      times (Y, P->H) {
        Q = M + *Ys++ + sizeof(tile);
        //printf("  %04d: #%08x\n", Y, Q-M);
        X = 0;
        do {
         //printf("    %04d: %04d %04d\n", X, *(u2*)Q, *(u2*)(Q+2) & 0x7FFF);
          Run = *(u2*)Q;
          Q += 2;
          Code = *(u2*)Q;
          Len = *(u2*)Q & 0x7FFF;
          Q += 2;

          X += Run-(X+Len);
          while (Len--) {
            if (PF == 0) {
              fromR5G6B5(R,G,B,*(u2*)Q); Q+=2;
              picPut24(P, X, Y, R8G8B8(R,G,B));
            } else {
              fromR5G5B5A1(A,R,G,B,*(u2*)Q); Q+=2;
              picPut24(P, X, Y, R8G8B8(R,G,B));
            }
            X++;
          }
        } while (!(Code&0x8000));
      }
    }
  } else {
    printf("    Cant handle %d-bit TILEs\n", Bits);
    abort();
  }
  sprintf(Tmp, "%s/%s.png", Output, Name);
  pngSave(Tmp, P);

  sprintf(Tmp, "%s/%s.txt", Output, Name);
  sprintf(Txt, "%d %d", H->X, H->Y);
  writeFile(0, strlen(Txt), Tmp, Txt);
}


static void camDecompile(char *Output, char *Input) {
  char Tmp[1024], Type[5], Name[128], Ext[16], DataID[32];
  int Off, Len, Wide, High, Scan=1;
  int I, J, K, C, X, Y, L = fileSize(Input), NFiles, Id, SubType;
  u1 *Q, *M, *Z;
  header *H;
  dir *D;
  entry *E;

  H = readFile(0, sizeof(header), Input);
  M = readFile(0, 2*H->FTSize, Input);
  D = (dir*)(M+sizeof(header));

  printf("  NTypes=%d FTSize=#%x\n", H->NTypes, H->FTSize);

  SPLT = ns(u1*,1<<16);

again:
  times (I, H->NTypes) {
    Q = M+D[I].Off;
    NFiles = *(u4*)Q; Q += 4;
    SubType = *(u4*)Q; Q += 4;
    memcpy(Type, D[I].Id, 4);
    Type[4] = 0;
    downcase(Type);
    //printf("  %s: Off=#%05x NFiles=%d\n", Type, D[I].Off, NFiles);
    times (J, NFiles) {
      E = (entry*)Q; Q+=sizeof(entry);

      memcpy(DataID, E->DataID, 4);
      DataID[4] = 0;

      if (Scan) {
        if (!strcmp(Type, "splt")) {
          if (*(u4*)E->DataID >= (1<<16)) {
            printf("  Invalid SPLT Id (%d)\n", *(u4*)E->DataID);
            abort();
          }
          SPLT[*(u4*)E->DataID] = readFile(E->Off, E->Len, Input);
        }
        continue;
      }

      if (DataID[3]<0x20) {
        sprintf(DataID, "%05d", *(u4*)E->DataID);
      } else {
        times (K, 4) if (DataID[K] == ' ') {DataID[K]=0; break;}
        downcase(DataID);
      }

      if (*E->Name) sprintf(Name, "%s/%s/%s", Type, E->Name, DataID);
      else sprintf(Name, "%s/unnamed/%s", Type, DataID);


      printf("  Extracting: %s\n", Name);
      //printf("    Off=#%08x Len=%d\n", E->Off, E->Len);

      Z = readFile(E->Off, E->Len, Input);
      Ext[0] = 0;
      if (!strcmp(Type, "wave")) strcpy(Ext,".wav");
      else if (!strcmp(Type, "tile")) {
        saveTile(Output, Name, Z, E->Len);
        goto next;
      }
      sprintf(Tmp, "%s/%s%s", Output, Name, Ext);
      writeFile(0, E->Len, Tmp, Z);
next:
      free(Z);
    }
  }
  if (Scan) {
    Scan = 0;
    goto again;
  }
}

int camInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "cam";
  F->Description = "Cyberlore Library Manager (Majesty, Playboy, etc..)";
  F->Decompile = camDecompile;
  return 1;
}

