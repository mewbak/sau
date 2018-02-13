//Credits go to Jon Rafkind (http://code.google.com/p/masterofmagicclone/)

#include "common.h"
#include "mom_pal.h"

typedef struct {
  u2 NFiles;  // LBX archives cant hold more than 126 files.
  u2 Magic;   // \AD\FE
  u2 U1;      // \00\00
  u2 U2;      // 5=TextTable
  //u4 Offs[126];       // offsets, including end of file
  //name Names[NFiles]; // filenames
} __attribute__ ((packed)) header;


typedef struct {
  u1 Dir[9];   // directory
  u1 Name[23]; // filename
} __attribute__ ((packed)) name;

typedef struct {
  u2 Width;
  u2 Height;
  u2 U1;
  u2 NFrames;
  u2 U2;
  u2 U3;
  u2 U4;
  u2 PIOff; // palette information offset
  u2 U5;
  //u4 Offs[NFrames+1]; // frame offsets, including end of file
} __attribute__ ((packed)) gfxHeader;

typedef struct {
  u2 Off;
  u2 Start;
  u2 Len;
  u2 Unknown;
} __attribute__ ((packed)) gfxPaletteInfo;


typedef struct {
  u2 Height;  // always 20
  u2 Width; // always 18
  u1 Padding[12];
  u1 Pixels[20*18];
  u1 Footer[8];
} __attribute__ ((packed)) tile;


static void saveGfx (char *Output, u1 *M, int L) {
  char Tmp[1024];
  int I, J, K, B, C, YE, X, Y, LongData;
  int RLE;
  u1 *Q, *E, *E2, *E3;
  pic *P, *R;
  gfxPaletteInfo PI;
  gfxHeader *H = (gfxHeader*)M;
  u4 *Offs = (u4*)(M+sizeof(gfxHeader));
  //printf("    %dx%dx%d PI=%x\n", H->NFrames, H->Width, H->Height, H->PIOff);

  P = picNew(H->Width, H->Height, 8);
  if (H->PIOff) {
    memcpy(&PI, M+H->PIOff, sizeof(PI));
    Q = M+PI.Off;
    times (I, PI.Len) {
      P->P[PI.Start + I*4+0] = Q[I*3+0]<<2;
      P->P[PI.Start + I*4+1] = Q[I*3+1]<<2;
      P->P[PI.Start + I*4+2] = Q[I*3+2]<<2;
    }
  } else {
    PI.Start = 0;
    PI.Len = 255;
  }

  RLE = PI.Start + PI.Len;

  P->K = 0xFF;
  memcpy(P->P, mom_pal, 4*256);
  P->P[P->K*4+0] = 0x00;
  P->P[P->K*4+1] = 0xFF;
  P->P[P->K*4+2] = 0xFF;
  picClear(P, P->K);

  times (I, H->NFrames) {
    Q = M+Offs[I];
    E = Q + Offs[I+1] - Offs[I];
    if (*Q++ == 1) picClear(P, P->K); // this frame wants clean buffer
    for (X=0; Q<E; X++) {
      C = *Q++;
      if (C == 0x00) RLE = PI.Start + PI.Len; // threshold reset
      else if (C == 0x80) RLE = 0xE0; // low threshold
      else if (C == 0xFF) { // reset and skip one X-column
        RLE = PI.Start + PI.Len;
        continue;
      } else {
        printf("      Invalid control byte: #%02X\n", C);
        //abort();
        continue;
      }
      E2 = Q + *Q;
      Q++;

      for (Y=0; Q < E2; ) {
        E3 = Q;
        E3 += *Q++ + 2;
        Y += *Q++;
        while (Q < E3) {
          C = *Q++;
          if (C < RLE) picPut(P, X, Y++, C);
          else {
            C = C - RLE + 1;
            while (C--) picPut(P, X, Y++, *Q);
            Q++;
          }
        }
      }
    }

    sprintf(Tmp, "%s_%03d.png", Output, I);
    pngSave(Tmp, P);
  }
  picDel(P);
}

static void saveTerra (char *Output, u1 *M, int L) {
  pic *P = picNew(1280, 640, 8);
  int I, X=0, Y=0, TX, TY, NTiles = (L-0xC0)/sizeof(tile);
  tile *T = (tile*)(M+0xC0);
  times (I, NTiles) {
    if (X+T->Width > P->W) {X=0; Y+=T->Height;}
    times (TX, T->Width) times (TY, T->Height)
      picPut(P, X+TX,Y+TY, T->Pixels[TY*T->Width + TX]);
    X += T->Width;
    T++;
  }
  memcpy(P->P, mom_pal, 4*256);
  strcat(Output, ".png");
  pngSave(Output, P);
  picDel(P);
}

// CONQUEST.LBX, LOSE.LBX, MOODWIZ.LBX, SCROLL.LBX, SPELLOSE.LBX, WIZARDS.LBX, WIZLAB.LBX and INTRO.LBX got wrong palette
static void lbxDecompile (char *Output, char *Input) {
  int I, J, Sz;
  char Tmp[1024], Tmp2[1024], Tmp3[1024], Tmp4[1024], Name[256], Dir[16];
  int L = fileSize(Input);
  char LBXName[256];
  u1 *M = readFile(0, L, Input);
  header *H = (header*)M;
  u4 *Offs = (u4*)(M+sizeof(header));
  name *Names = (name*)(M+Offs[0] - H->NFiles*sizeof(name));

  if ((u1*)Names <= (u1*)(Offs+H->NFiles)) Names = ns(name, H->NFiles);

  pathParts(0, LBXName, 0, Input);
  downcase(LBXName);

  printf("  NFiles=%d Magic=#%04X U1=#%04X U2=#%04X\n"
        ,H->NFiles, H->Magic, H->U1, H->U2);

  times (I, H->NFiles) {
    sprintf(Dir, "%s_%03d", *Names[I].Dir ? Names[I].Dir : "noname", I);
    strcpy(Name, *Names[I].Name ? Names[I].Name : "");
    for (J = 0; Name[J]; J++) if (Name[J] == '/') Name[J] = '_';
    printf("  Extracting: %s_%s\n", Dir, Name);
    Sz = Offs[I+1]-Offs[I];
    //printf("    %03d: Offs=#%08X Size=#%08X %s\n"
    //      ,I, Offs[I], Sz, Name);

    sprintf(Tmp, "%s/%s_%s", Output, Dir, Name);
    if (Sz &&
       (!strncmp(LBXName, "figure", 6)
     || !strcmp(LBXName, "items")
     || !strcmp(LBXName, "backgrnd")
     || !strcmp(LBXName, "book")
     || !strcmp(LBXName, "armylist")
     || !strcmp(LBXName, "cityscap")
     || !strcmp(LBXName, "chriver")
     || !strcmp(LBXName, "citywall")
     || !strcmp(LBXName, "cmbdesrc")
     || !strcmp(LBXName, "cmbdesrt")
     || !strcmp(LBXName, "cmbgrasc")
     || !strcmp(LBXName, "cmbgrass")
     || !strcmp(LBXName, "cmbmagic")
     || !strcmp(LBXName, "cmbmounc")
     || !strcmp(LBXName, "cmbmount")
     || !strcmp(LBXName, "cmbtcity")
     || !strcmp(LBXName, "cmbtundc")
     || !strcmp(LBXName, "cmbtundr")
     || !strcmp(LBXName, "cmbtwall")
     || !strcmp(LBXName, "combat")
     || !strcmp(LBXName, "compix")
     || !strcmp(LBXName, "cmbtfx")
     || !strcmp(LBXName, "conquest")
     || !strcmp(LBXName, "desert")
     || !strcmp(LBXName, "diplomac")
     || !strcmp(LBXName, "diplomsg")
     || !strcmp(LBXName, "events")
     || !strcmp(LBXName, "halofam")
     || !strcmp(LBXName, "hire")
     || !strcmp(LBXName, "intro")
     || !strcmp(LBXName, "itemisc")
     || !strcmp(LBXName, "lilwiz")
     || !strcmp(LBXName, "load")
     || !strcmp(LBXName, "lose")
     || !strcmp(LBXName, "magic")
     || !strcmp(LBXName, "main")
     || !strcmp(LBXName, "mainscrn")
     || !strcmp(LBXName, "mapback")
     || !strcmp(LBXName, "monster")
     || !strcmp(LBXName, "moodwiz")
     || !strcmp(LBXName, "newgame")
     || !strcmp(LBXName, "portrait")
     || !strcmp(LBXName, "reload")
     || !strcmp(LBXName, "resource")
     || !strcmp(LBXName, "scroll")
     || !strcmp(LBXName, "specfx")
     || !strcmp(LBXName, "special")
     || !strcmp(LBXName, "special2")
     || !strcmp(LBXName, "spellose")
     || !strcmp(LBXName, "spells")
     || !strcmp(LBXName, "spellscr")
     || !strcmp(LBXName, "splmastr")
     || !strcmp(LBXName, "tundra")
     || !strcmp(LBXName, "units1")
     || !strcmp(LBXName, "units2")
     || !strcmp(LBXName, "unitview")
     || !strcmp(LBXName, "vortex")
     || !strcmp(LBXName, "wallrise")
     || !strcmp(LBXName, "win")
     || !strcmp(LBXName, "wizards")
     || !strcmp(LBXName, "wizlab"))) {
      saveGfx (Tmp, M+Offs[I], Sz);
    } else if (!strcmp(LBXName, "terrain") && I == 0) {
      saveTerra (Tmp, M+Offs[I], Sz);
    } else if (!strcmp(LBXName, "cmbtsnd")
            || !strcmp(LBXName, "introsfx")
            || !strcmp(LBXName, "newsound")
            || !strcmp(LBXName, "soundfx")) {
      strcat(Tmp, ".voc");
      writeFile(0, Sz-16, Tmp, M+Offs[I]+16); // LBX adds own header
    } else if (!strcmp(LBXName, "introsnd")
            || !strcmp(LBXName, "music")) {
      strcat(Tmp, ".xmi");
      writeFile(0, Sz-16, Tmp, M+Offs[I]+16); // LBX adds own header
    } else {
      // Text Tables: buildat, buildesc, cityname, desc, diplomsg, eventmsg, help, listdat, message, names, spelldat
      writeFile(0, Sz, Tmp, M+Offs[I]);
    }
  }
}

int lbxInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "lbx";
  F->Description = "Master of Magic archives";
  F->Decompile = lbxDecompile;
  return 1;
}

