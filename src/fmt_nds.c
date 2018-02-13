//Credits go to SNV

#include "common.h"

typedef struct {
  // 0x000 Reserved region for system use
  u1 reserved_A[32]; // Reserved for system A

  // 0x020 b) Parameter for static module
  // ARM9
  u4 main_rom_offset; // Source ROM offset
  u4 main_entry_address; // Execution start address (not implemented)
  u4 main_ram_address; // Destination RAM address
  u4 main_size; // Size
  // ARM7
  u4 sub_rom_offset; // Source ROM offset
  u4 sub_entry_address; // Execution start address (not implemented)
  u4 sub_ram_address; // Destination RAM address
  u4 sub_size; // Size

  // 0x040 c) Parameter for file name table
  u4 fnt_offset; // Top ROM offset (points to ROM_FNTDir)
  u4 fnt_size; // Table size

  // 0x048 e) Parameter for file allocation table
  u4 fat_offset; // Top ROM offset (points to ROM_FAT)
  u4 fat_size; // Table size

  // 0x0050 d) Parameter for overlay header table
  // ARM9
  u4 main_ovt_offset; // Top ROM offset (points to ROM_OVT)
  u4 main_ovt_size; // Table size

  // ARM7
  u4 sub_ovt_offset; // Top ROM offset (points to ROM_OVT)
  u4 sub_ovt_size; // Table size

  // 0x0060 - 0x0067 Reserved region for system use
  u1 reserved_B1[8]; // Reserved for system B1

  // 0x0068 g) Banner file offset
  u4 banner_offset; // Top ROM offset

  // 0x006c - 0x006f Reserved for system
  u1 reserved_B2[4]; // Reserved for system B2

  // 0x0070 Static module parameter 2 (for debugging)
  u4 main_autoload_done; // ARM9 AUTOLOAD complete CALLBACK
  u4 sub_autoload_done; // ARM7 AUTOLOAD complete CALLBACK

  // 0x0078 - 0x03fff Reserved for system
  u1 reserved_C[4*1024-0x78]; // Reserved for system C
  u1 reserved_D[12*1024]; // Reserved for system D
} __attribute__ ((packed)) header; // 16KB


typedef struct {
  u4 Off;     // Offset into filename table
  u2 Start;   // Index of directory's first file in entry table
  u2 Parent;  // Index of parent directory
} __attribute__ ((packed)) dir;

typedef struct {
  u4 Head; // Top ROM address of file
  u4 Tail; // Bottom ROM address of file
} __attribute__ ((packed)) file;


typedef struct {
  char *Name;
  int Id;     // this entry's id
  int Parent; // id of parent directory
  int Off;    // offset in bytes or id of first directory entry
  int Len;    // length in butyes of number of sub entries
} entry;

struct {
  int End;    // index of last frame
  int Corpse; // if animation has a corpse, it is stored outside of main range
  int PalS;   // first palette in range (single character can have several palettes)
  int PalE;   // last palette in range
  const char *Name;
} MKAnims[] = {
    {  53, 2322,  1,  6, "archer"},
    { 101, 2323,  7, 11, "cultist"},
    { 158, 2324, 13, 13, "deathspeaker"},
    { 236, 2325, 14, 22, "delphana_master"},
    { 374, 2326, 28, 29, "disciple"},
    { 443, 2327, 39, 39, "drummer"},
    { 500, 2328, 42, 45, "dwarf"},
    { 551, 2329, 47, 50, "elven_warrior"},
    { 602, 2330, 51, 54, "golem"},
    { 665, 2331, 55, 57, "griffin"},
    { 737, 2332, 58, 58, "guardian"},
    { 788, 2333, 59, 63, "hound"},
    { 860, 2334, 64, 66, "magna_champion"},
    { 929, 2335, 67, 70, "mauler"},
    { 998, 2336, 71, 76, "oak_warrior"},
    {1076, 2337, 78, 81, "oracle"},
    {1133, 2338, 82, 82, "pathis_arcana"},
    {1211, 2339, 83, 88, "priest"},
    {1289, 2340, 89, 94, "priestess"},
    {1337, 2341, 95, 98, "shade"},
    {1415, 2342, 99,103, "shaman"},
    {1469, 2343,104,107, "skymage"},
    {1514, 2344,108,110, "soulshredder"},
    {1580, 2345,111,115, "steam_knight"},
    {1631, 2346,116,120, "trance_warrior"},
    {1688, 2347,121,125, "troll"},
    {1736, 2348,126,126, "varatrix"},
    {1781, 2349,128,131, "veteran"},
    {1853, 2350,132,132, "vorgoth"},
    {1898, 2351,133,135, "warbeast"},
    {1982,    0,136,139, "npc01"},
    {2003,    0,140,140, "npc02"},
    {2018,    0,141,141, "npc03"},
    {2039,    0,142,142, "npc04"},
    {2060,    0,143,143, "npc05"},
    {2081,    0,144,144, "npc06"},
    {2102,    0,146,146, "npc07"},
    {2123,    0,145,145, "npc08"},
    {2144,    0,147,150, "npc09"},
    {2165,    0,151,154, "npc10"},
    {2170,    0, -1, -1, "fx01"},
    {2171,    0, -1, -1, "fx02"},
    {2189,    0, -1, -1, "fx03"},
    {2204,    0, -1, -1, "fx04"},
    {2216,    0, -1, -1, "fx05"},
    {2230,    0, -1, -1, "fx06"},
    {2245,    0, -1, -1, "fx07"},
    {2290,    0, -1, -1, "fx08"},
    {2301,    0, -1, -1, "fx09"},
    {2313,    0, -1, -1, "fx10"},
    {2321,    0, -1, -1, "fx11"},
    {0,0,0,0,0}
};



typedef struct {
  int N; // number of entries
  u2 D[1]; // color entries
} __attribute__ ((packed)) mkpal;

static u1 *MKPals;
static int MKPalsLen;

static void mk_dump_anims(char *Output, u1 *M, int L) {
  char Tmp[1024], Name[256], LastName[256];
  pic *P, **T;
  mkpal **Pals;
  int I, J, K, C, X, Y, W, H, TPL=8, Len, A, R, G, B;
  int NTiles = *(u4*)M;
  u4 *Offs = (u4*)(M+4);

  T = ns(pic*,NTiles);
  for (I=0; I<NTiles; I++) {
    T[I] = picNew(64, 64, 8);
    memcpy(T[I]->D,  M+Offs[I], 64*64);
  }


  Pals = ns(mkpal*, *(u4*)MKPals);
  times (K, *(u4*)MKPals) {
    Pals[K] = (mkpal*)(MKPals + *((u4*)MKPals+1+K));
    //printf("%d: %d\n", K, Pals[K]->N);
  }


  C = 0;
  for (I=0; MKAnims[I].Name; I++) {
    printf("    assembling %s.png\n", MKAnims[I].Name);
    Len =  MKAnims[I].End+1 - C;
    H = (Len+TPL-1)/TPL;
    W = (Len+H-1)/H;
    P = picNew(64*W, 64*(H + (MKAnims[I].Corpse ? 1 : 0)), 8);
    X = Y = 0;
    times (K, Len) {
      if (K == 0) memset(P->D, T[C]->D[0], P->W*P->H);
      picBlt(P, T[C++], 0, X++*64, Y*64, 0, 0, 64, 64);
      if (X==W) {
        X = 0;
        Y++;
      }
    }

    if (MKAnims[I].Corpse) picBlt(P, T[MKAnims[I].Corpse], 0, 0, H*64, 0, 0, 64, 64);

    if (MKAnims[I].PalS == -1) { //no palette
      times (K, 256) {
        P->P[K*4+0] = K;
        P->P[K*4+1] = K;
        P->P[K*4+2] = K;
      }
      sprintf(Tmp, "%s/%s.png", Output, MKAnims[I].Name);
      pngSave(Tmp, P);
    } else {
      // save sprite sheet with various palettes
      for (J=MKAnims[I].PalS; J <= MKAnims[I].PalE; J++) {
        times (K, 32) {
          fromA1R5G5B5(A, R, G, B, Pals[J]->D[K]);
          P->P[(224+K)*4+0] = B;
          P->P[(224+K)*4+1] = G;
          P->P[(224+K)*4+2] = R;
        }
        sprintf(Tmp, "%s/%s_%d.png", Output, MKAnims[I].Name, J-MKAnims[I].PalS);
        pngSave(Tmp, P);
      }
    }

#if 0
    // save sprite sheet with all palettes
    for (J=0; J < *(u4*)MKPals; J++) {
      times (K, 32) {
        fromA1R5G5B5(A, R, G, B, Pals[J]->D[K]);
        P->P[(224+K)*4+0] = B;
        P->P[(224+K)*4+1] = G;
        P->P[(224+K)*4+2] = R;
      }
      sprintf(Tmp, "%s/%s/%d.png", Output, MKAnims[I].Name, J);
      pngSave(Tmp, P);
    }
#endif

    picDel(P);
  }
}

// generic nitro header
typedef struct {
  u1 Id[4];     // Format Id (NTBG/NTFP/NTFT/NTFS)
  u4 U;         // Always 0xFFFE0001?
  u4 Len;       // File length including this header
  u2 HeaderLen; // Length of this header
  u2 NChunks;   // Total number of sub sections
} __attribute__ ((packed)) nitro;


typedef struct {
  u1 Id[4];
  u4 Len; // chunk length including this header
  u4 NColors;
  //u2 Colors[NColors];
} __attribute__ ((packed)) palt;

// no idea how to use following chunk
typedef struct {
  u1 Id[4];
  u4 Len; // chunk length including this header
  u4 LenColors;
  //u1 Colors[LenColors];
} __attribute__ ((packed)) dfpl;

typedef struct {
  u1 Id[4];
  u4 Len; // chunk length including this header
  u4 U; //0x00030306 = 8bit, alpha, 8color (arc)
        //0x00070003 = 4bit, alpha, 16color (font)
        //0x00050006 = 8bit, 8color (grad)
        //0x00030306 = 8bit, 8color (shadow)
        //0x00020201 = 8bit, 32color (dice)
        //0x00030306 = 8bit, 8color (ring)
        //0x00030306 = 8bit, 8color (hexgrid_overlay_tile)
        //0x00030301 = 8bit, 32color (hexgrid_overlay_tile3)
  u2 W;
  u2 H;
  u4 DataLen;
  //u1 Data[DataLen];
} __attribute__ ((packed)) imge;

typedef struct {
  u1 Id[4];
  u4 Len; // chunk length including this header
  u4 U;   // {ColorKey?, U, BPP, NFrames}
  u4 MapLen; // MapW*MapH*4
  u2 MapW;   // width in 8x8-tiles
  u2 MapH;   // height in 8x8-tiles
  u2 X; // sprites also have X and Y
  u2 Y;
  u2 W;
  u2 H;
  u4 TilesLen;
  //u4 Map[MapLen/4];
  //u1 Tiles[NPixels/(8*8)][8*8];
} __attribute__ ((packed)) objd;


typedef struct {
  u1 Id[4];
  u4 Len; // chunk length including this header
  u4 U;
  u4 MapLen; // MapW*MapH*2
  u2 MapW;   // width in 8x8-tiles
  u2 MapH;   // height in 8x8-tiles
  u2 W;
  u2 H;
  u4 TilesLen;
  //u2 Map[MapLen/2];
  //u1 Tiles[TilesLen/(8*8)][8*8];
} __attribute__ ((packed)) bgdt;


// format of anim_5pl.bin and anim_5tx.bin
//typedef struct {
//  u4 Len;
//  u4 Data[Len/4];
//} __attribute__ ((packed)) binHeader;




//for i in input/unpacked/worldmap/*.5bg; do ./sau nitro="$i" ./output/; done
//FIXME: it seems all sprites are 4bit

//FIXME: MK-Help_SpeedTypes_Upper.png should have some tiles flipped

// NTBG is basically an array of tiles plus a tile map,
// which maps tiles into final image
int handleNTBG(char *Output, char *Name, char *Ext, nitro *H, int L) {
  pic *P, **T;
  char Tmp[1024];
  int I, J, X, Y, NTiles, Bits=4, R, G, B, A, TI, Flip, PI, Flags;
  palt *PALT = (palt*)(H+1);
  u2 *Pal = (u2*)(PALT+1);
  u1 *Q = (u1*)(Pal+PALT->NColors);
  u1 *Z;

  memcpy(Tmp, Q, 4);
  Tmp[4] = 0;
  if (!strncmp(Tmp, "BGDT", 4)) {
    bgdt *BG = (bgdt*)Q;
    u2 *Map = (u2*)(BG+1);

    NTiles = 0;
    times (Y, BG->MapH) {
      times (X, BG->MapW) {
        NTiles = max(NTiles, Map[Y*BG->MapW + X]&0x03FF);
      }
    }
    ++NTiles;
    //Bits = (8*BG->TilesLen)/NTiles/8/8;

    //printf("  BGDT: %dx%dx%d(%d)\n", BG->MapW*8, BG->MapH*8, Bits, PALT->NColors);

    // first read all 8x8 tiles
    T = ns(pic*,NTiles);
    Q = (u1*)Map + BG->MapLen;
    if (Bits == 8) {
      times (I, NTiles) {
        Z = Q + I*8*8;
        T[I] = picNew(8, 8, 8);
        memcpy(T[I]->D, Q, 8*8);
      }
    } else if (Bits == 4) {
      times (I, NTiles) {
        Z = Q + I*8*8/2;
        T[I] = picNew(8, 8, 8);
        times (J, 8*8/2) {
          T[I]->D[J*2+0] = *Z&0xF;
          T[I]->D[J*2+1] = *Z>>4;
          Z++;
        }
      }
    } else {
      printf("  unsupported BPP (%d)\n", Bits);
      return 0;
    }

    // then map and blit them into final image
    P = picNew(BG->MapW*8, BG->MapH*8, 8);
    times (Y, BG->MapH) {
      times (X, BG->MapW) {
        TI = Map[Y*BG->MapW + X];
        Flags = 0;
        if ((TI>>10)&0x1) Flags |= PIC_FLIP_X;
        if ((TI>>10)&0x2) Flags |= PIC_FLIP_Y;
        PI = (TI>>12)&0xF; // palette number
        TI &= 0x03FF; // Tile Index
        //printf("%x/%x/%d/%d\n", TI, NTiles,PI);
        if (Bits==4) {
          times (I, 8*8) T[TI]->D[I] = (T[TI]->D[I]&0xF)|PI<<4;
        }
        picBlt(P, T[TI], Flags, X*8, Y*8, 0, 0, T[TI]->W, T[TI]->H);
      }
    }
    times (I, PALT->NColors) {
      fromA1R5G5B5(A, R, G, B, Pal[I]);
      P->P[I*4+0] = B;
      P->P[I*4+1] = G;
      P->P[I*4+2] = R;
      P->P[I*4+3] = 0;
    }
    sprintf(Tmp, "%s.png", Output, Name);
    pngSave(Tmp, P);
    picDel(P);
  } else if (!strncmp(Tmp, "OBJD", 4)) {
    //most OBJD files are NxNx4bit images
    objd *BG = (objd*)Q;
    u4 *Map = (u4*)(BG+1);

    Bits = 4;
    NTiles = BG->TilesLen/(8*Bits);
    //BG->MapW=(int)sqrt(NTiles);
    //BG->MapH=BG->MapW;
    BG->MapW=(int)sqrt(BG->MapW*BG->TilesLen*2/BG->MapH);
    BG->MapH=(BG->TilesLen*2)/BG->MapW;
    BG->MapW/=8;
    BG->MapH/=8;

    //printf("  OBJD: %dx%dx%dx(%d)\n", BG->MapW*8, BG->MapH*8, Bits, PALT->NColors);

    // first read all 8x8 tiles
    T = ns(pic*,NTiles);
    Q = (u1*)Map + BG->MapLen;
    if (Bits == 8) {
      times (I, NTiles) {
        Z = Q + I*8*8;
        T[I] = picNew(8, 8, 8);
        memcpy(T[I]->D, Q, 8*8);
      }
    } else if (Bits == 4) {
      times (I, NTiles) {
        Z = Q + I*8*8/2;
        T[I] = picNew(8, 8, 8);
        times (J, 8*8/2) {
          T[I]->D[J*2+0] = *Z&0xF;
          T[I]->D[J*2+1] = *Z>>4;
          Z++;
        }
      }
    } else {
      printf("  unsupported BPP (%d)\n", Bits);
      return 0;
    }

    // then map and blit them into final image
    P = picNew(BG->MapW*8, BG->MapH*8, 8);

    TI = 0;
    times (Y, BG->MapH) {
      times (X, BG->MapW) {
        picBlt(P, T[TI], 0, X*8, Y*8, 0, 0, T[TI]->W, T[TI]->H);
        TI++;
      }
    }
    times (I, PALT->NColors) {
      fromA1R5G5B5(A, R, G, B, Pal[I]);
      P->P[I*4+0] = B;
      P->P[I*4+1] = G;
      P->P[I*4+2] = R;
      P->P[I*4+3] = 0;
    }
    sprintf(Tmp, "%s.png", Output, Name);
    pngSave(Tmp, P);
    picDel(P);
  } else {
    printf("  unsupported NTBG format (%s)\n", Tmp);
    return 0;
  }
  return 1;
}

int handleNTTX(char *Output, char *Name, char *Ext, nitro *H, int L) {
  pic *P;
  char Tmp[1024];
  int I, J, X, Y, Bits=8, R, G, B, A;
  palt *PALT = (palt*)(H+1);
  u2 *Pal = (u2*)(PALT+1);
  imge *BG = (imge*)(Pal+PALT->NColors);
  u1 *Q = (u1*)(BG+1);

  //printf("  NTTX: %dx%dx%d(%d), U=0x%08X\n", BG->W, BG->H, Bits, PALT->NColors, BG->U);

  P = picNew(BG->W, BG->H, 8);
  times (Y, BG->H) {
    times (X, BG->W) {
      // upper half probably selects some transformation
      P->D[Y*BG->W+X] = *Q++&0xF;
    }
  }

  times (I, 256) {
    P->P[I*4+0] = I;
    P->P[I*4+1] = I;
    P->P[I*4+2] = I;
    P->P[I*4+3] = 0;
  }

  times (I, PALT->NColors) {
    fromA1R5G5B5(A, R, G, B, Pal[I]);
    P->P[I*4+0] = B;
    P->P[I*4+1] = G;
    P->P[I*4+2] = R;
    P->P[I*4+3] = 0;
  }
  sprintf(Tmp, "%s.png", Output, Name);
  pngSave(Tmp, P);
  picDel(P);
  return 1;
}

static struct {
  char *Id;
  void *f;
} Handlers[] = {
  {"NTBG", handleNTBG},
  {"NTTX", handleNTTX},
  {0,0}
};


// decompiles a nintendo nitro files or returns 0
// nitro formats are various grpahics and audio formats found inside of NDS ROMs
static int handleNitro(char *Output, u1 *M, int L) {
  char Tmp[1024], Name[128], Ext[32];
  int I;
  nitro *H = (nitro*)M;
  pathParts(0, Name, Ext, Output);

  if (L < sizeof(nitro) || H->U != 0x0100FEFF) return 0;

  memcpy(Tmp, H->Id, 4);
  Tmp[4] = 0;

  for (I=0; Handlers[I].f; I++) {
    if (strncmp(H->Id, Handlers[I].Id, 4)) continue;
    return ((int(*)(char*,char*,char*,nitro*,int))(Handlers[I].f))
       (Output, Name, Ext, H, L);
  }
  return 0;
}


static void ndsDecompile(char *Output, char *Input) {
  char Tmp[1024], Name[256], Path[256];
  u1 Tag[128];
  int I, J, Off, Len, Id, DF;
  entry *E = ns(entry,0x10000); // entries below OxF000 are files, other are dirs
  int L = fileSize(Input);
  u1 *M = readFile(0, L, Input);
  u1 *Q;
  header *H = (header*)M;
  file *F = (file*)(M+H->fat_offset); // table of filenames (File Alloc Table)
  int NFiles = H->fat_size/sizeof(file);
  dir *D = (dir*)(M+H->fnt_offset); // table of directories (FileName Table)
  int NDirs = H->fnt_size/sizeof(dir);

  E[0xF000].Name = strdup("");
  E[0xF000].Id = 0xF000;
  E[0xF000].Parent = -1;
  //E[0xF000].Len = D[0].Parent; // D[0].Parent holds number of files

  times (I, NDirs) {
    E[I+0xF000].Parent = I==0 ? -1 : D[I].Parent;
    J = E[I+0xF000].Off = D[I].Start;
    Q = M+H->fnt_offset + D[I].Off;
    // each directory is terminated by a zerosized filename
    while (Q < M+H->fnt_offset+H->fnt_size && *Q) {
      DF = *Q & 0x80;
      Len = *Q & 0x7F;
      memcpy(Name, Q+1, Len);
      Name[Len] = 0;
      Q += Len+1;

      if (DF) {
        Id = (Q[1]<<8) | Q[0];
        Q += 2;
      } else {
        Id = J++;
        E[Id].Off = F[Id].Head;
        E[Id].Len = F[Id].Tail - F[Id].Head;
      }
      E[Id].Id = Id;
      E[Id].Name = strdup(Name);
      E[Id].Parent = I+0xF000;
      E[J+0xF000].Len++;
    }
  }

  memset(Tag, 0, 128);
  memcpy(Tag, M+E[I].Off, min(4, E[I].Len));

  times (I, 0xF000) {
    unless (E[I].Name) continue;
    strcpy(Path, E[I].Name);
    J = E[I].Parent;
    while(*E[J].Name) {
      sprintf(Tmp, "%s/%s", E[J].Name, Path);
      strcpy(Path, Tmp);
      J = E[J].Parent;
    }

    printf("  Extracting: %s\n", Path);
    //printf("    Off=%08X Len=%08X\n", E[I].Off, E[I].Len);

    sprintf(Tmp, "%s/%s", Output, Path);
    if (handleNitro(Tmp, M+E[I].Off, E[I].Len)) {
    } else if (!strcmp(Path, "anim_new/anim_5tx.bin")) { // Mage Knight anims
      mk_dump_anims(Tmp, M+E[I].Off, E[I].Len);
    } else if (!strcmp(Path, "anim_new/anim_5pl.bin")) { // Mage Knight palettes
      MKPals = M+E[I].Off;
      MKPalsLen = E[I].Len;
    } else  {
      writeFile(0, E[I].Len, Tmp, M+E[I].Off);
    }
  }
}

int ndsInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "nds";
  F->Description = "Nintendo Nitro Filesystem and Formats (use on NDS roms)";
  F->Decompile = ndsDecompile;
  return 1;
}


