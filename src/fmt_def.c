#include "common.h"

// Credits go to VCMI team

#define TYPE_CREATURE 0x42
#define TYPE_HERO     0x44

typedef struct {
  u4 Type;  // type of DEF file (see TypeNames)
  u4 Width;
  u4 Height;
  u4 NumGroups; // number of animation groups
  u1 ColorMap[3*256];
} __attribute__ ((packed)) Header;

typedef struct {
  u4 Id;
  u4 Size; // number of frames in this animation group
  u4 U1;
  u4 U2;
} __attribute__ ((packed)) GroupHeader;

typedef struct {
  u4 U1;
  u4 Type;
  u4 VirtualWidth;
  u4 VirtualHeight;
  u4 Width;
  u4 Height;
  u4 X; // displacement X/Y
  u4 Y;
} __attribute__ ((packed)) FrameHeader;

static u1 SpecialColors[] = {
  0x00, 0xFF, 0xFF, // background
  0xFF, 0xFF, 0x00, // selection
  0xFF, 0x96, 0xFF, // shadow light
  0xFF, 0x00, 0xFF, // shadow gray
  0xB4, 0x00, 0xFF, // shadow dark
  0x00, 0xFF, 0x00  // selection/shadow intersection
};


// Types inclusive between 0x40 and 0x49 are DEF files. 0x16, 0x17 - PCX files
typedef struct {int Id; char *Name;} typeName;
typeName TypeNames[] =
 {{0x01, "campaign_file"}
 ,{0x02, "text_file"}
 ,{0x10, "image_8bit"}
 ,{0x11, "image_24bit"}
 ,{0x40, "sprite_spell"}
 ,{0x41, "sprite"}
 ,{0x42, "creature"}
 ,{0x43, "adventure_map_object"}
 ,{0x44, "adventure_map_hero"}
 ,{0x45, "terrain"}
 ,{0x46, "cursor"}
 ,{0x47, "interface"}
 ,{0x48, "sprite_frame"}
 ,{0x49, "combat_hero"}
 ,{0x4F, "mask"}
 ,{0, 0}
  };


static char *CreatureAnimNames[] =
 {"moving"
 ,"mouse_over"
 ,"standing"
 ,"getting_hit"
 ,"defending"
 ,"death"
 ,"unused_death"
 ,"turn_left"
 ,"turn_right"
 ,"turn_left"
 ,"turn_right"
 ,"attack_up"
 ,"attack_straight"
 ,"attack_down"
 ,"shoot_up"
 ,"shoot_straight"
 ,"shoot_down"
 ,"attack_2_hex_up"
 ,"attack_2_hex_straight"
 ,"attack_2_hex_down"
 ,"start_moving"
 ,"stop_moving"
 };

static char *HeroAnimNames[] =
 {"up"
 ,"up_right"
 ,"right"
 ,"down_right"
 ,"down"
 ,"move_up"
 ,"move_up_right"
 ,"move_right"
 ,"move_down_right"
 ,"move_down"
 };

static char *findTypeName(int Id) {
  int I;
  for(I = 0; TypeNames[I].Name; I++)
    if(TypeNames[I].Id == Id)
      return TypeNames[I].Name;
  return 0;
}


// read little-endian number
static u4 lsb(int Size, u1 *Buf) {
  u4 R=0, P=1;
    int I;
  for (I=0; I<Size; I++) {
    R += Buf[I]*P;
    P <<= 8;
  }
  return R;
}

typedef struct {
  int Width;
  int Height;
  u1 *Pixels;
  int Pitch;
} UnpackResult;

static void unpackFrame(UnpackResult *R, u1 *D, u4 Offset) {
  u1 fbuffer[800];
  int BaseOffset, BaseOffsetor,
    SpriteWidth, SpriteHeight,
    LeftMargin, RightMargin, TopMargin,BottomMargin,
    i, j, k, h, add, FullHeight,FullWidth,
    TotalRowLength,
    NextSpriteOffset, RowAdd,
        prSize, defType2;
  u1 SegmentType, SegmentLength, BL, BR;
  u1 *TempDef, *Pixels, *P;
  u4 *RWEntries = NULL;
  int *RLEntries = NULL;

  i=Offset;
  prSize = lsb(4,D+i);i+=4;
  defType2 = lsb(4,D+i);i+=4;
  FullWidth = lsb(4,D+i);i+=4;
  FullHeight = lsb(4,D+i);i+=4;
  SpriteWidth = lsb(4,D+i);i+=4;
  SpriteHeight = lsb(4,D+i);i+=4;
  LeftMargin = lsb(4,D+i);i+=4;
  TopMargin = lsb(4,D+i);i+=4;
  RightMargin = FullWidth - SpriteWidth - LeftMargin;
  BottomMargin = FullHeight - SpriteHeight - TopMargin;

  add = 4 - FullWidth%4;

  P = Pixels = malloc(FullHeight*(FullWidth+add));

  BaseOffsetor = BaseOffset = i;

  for (i=0; i<800; i++) fbuffer[i]=0;

  if (defType2==0)
  {
    if (add==4)
    add=0;

    if (TopMargin>0)
    {
      for (i=0;i<TopMargin;i++)
      {
        for (j=0;j<FullWidth+add;j++)
          *P++ = fbuffer[j];
      }
    }
    for (i=0;i<SpriteHeight;i++)
    {
      if (LeftMargin>0)
      {
        for (j=0;j<LeftMargin;j++)
          *P++ = fbuffer[j];
      }
      for (j=0; j<SpriteWidth;j++)
        *P++ = D[BaseOffset++];
      if (RightMargin>0)
      {
        for (j=0;j<add;j++)
          *P++ = fbuffer[j];
      }
    }
    if (BottomMargin>0)
    {
      for (i=0;i<BottomMargin;i++)
      {
        for (j=0;j<FullWidth+add;j++)
          *P++ = fbuffer[j];
      }
    }

  }
  if (defType2==1)
  {
    if (add==4) add=0;
    if (TopMargin>0)
    {
      for (i=0;i<TopMargin;i++)
      {
        for (j=0;j<FullWidth+add;j++)
          *P++ = fbuffer[j];
      }
    }
    RLEntries = malloc(SpriteHeight*sizeof(int));
    for (i=0;i<SpriteHeight;i++)
    {
      RLEntries[i]=lsb(4,D+BaseOffset);BaseOffset+=4;
    }
    for (i=0;i<SpriteHeight;i++)
    {
      BaseOffset=BaseOffsetor+RLEntries[i];
      if (LeftMargin>0)
      {
        for (j=0;j<LeftMargin;j++)
          *P++ = fbuffer[j];
      }
      TotalRowLength=0;
      do
      {
        SegmentType=D[BaseOffset++];
        SegmentLength=D[BaseOffset++];
        if (SegmentType==0xFF)
        {
          for (k=0;k<=SegmentLength;k++)
          {
            *P++ = D[BaseOffset+k];
            if ((TotalRowLength+k+1)>=SpriteWidth)
              break;
          }
          BaseOffset+=SegmentLength+1;
          TotalRowLength+=SegmentLength+1;
        }
        else
        {
          for (k=0;k<SegmentLength+1;k++)
          {
            *P++ = SegmentType;
          }
          TotalRowLength+=SegmentLength+1;
        }
      }while(TotalRowLength<SpriteWidth);
      RowAdd=SpriteWidth-TotalRowLength;
      if (RightMargin>0)
      {
        for (j=0;j<RightMargin;j++)
          *P++ = fbuffer[j];
      }
      if (add>0)
      {
        for (j=0;j<add+RowAdd;j++)
          *P++ = fbuffer[j];
      }
    }
    free(RLEntries);
    RLEntries = NULL;
    if (BottomMargin>0)
    {
      for (i=0;i<BottomMargin;i++)
      {
        for (j=0;j<FullWidth+add;j++)
          *P++ = fbuffer[j];
      }
    }
  }
  if (defType2==2)
  {
    if (add==4)
      add=0;
    if (TopMargin>0)
    {
      for (i=0;i<TopMargin;i++)
      {
        for (j=0;j<FullWidth+add;j++)
          *P++ = fbuffer[j];
      }
    }
    RWEntries = malloc(SpriteHeight*sizeof(u4));
    for (i=0;i<SpriteHeight;i++)
    {
      BaseOffset=BaseOffsetor+i*2*(SpriteWidth/32);
      RWEntries[i] = lsb(2,D+BaseOffset);
    }
    BaseOffset = BaseOffsetor+RWEntries[0];
    for (i=0;i<SpriteHeight;i++)
    {
      if (LeftMargin>0)
      {
        for (j=0;j<LeftMargin;j++)
          *P++ = fbuffer[j];
      }
      TotalRowLength=0;
      do
      {
        SegmentType=D[BaseOffset++];
        u1 code = SegmentType / 32;
        u1 value = (SegmentType & 31) + 1;
        if(code==7)
        {
          for(h=0; h<value; ++h)
          {
            *P++ = D[BaseOffset++];
          }
        }
        else
        {
          for(h=0; h<value; ++h)
          {
            *P++ = code;
          }
        }
        TotalRowLength+=value;
      } while(TotalRowLength<SpriteWidth);
      if (RightMargin>0)
      {
        for (j=0;j<RightMargin;j++)
          *P++ = fbuffer[j];
      }
      if (add>0)
      {
        for (j=0;j<add+RowAdd;j++)
          *P++ = fbuffer[j];
      }
    }
    free(RWEntries);
    RWEntries = NULL;
    if (BottomMargin>0)
    {
      for (i=0;i<BottomMargin;i++)
      {
        for (j=0;j<FullWidth+add;j++)
          *P++ = fbuffer[j];
      }
    }
  }
  if (defType2==3)
  {
    if (add==4)
      add=0;
    if (TopMargin>0)
    {
      for (i=0;i<TopMargin;i++)
      {
        for (j=0;j<FullWidth+add;j++)
          *P++ = fbuffer[j];
      }
    }
    RWEntries = malloc(SpriteHeight*sizeof(u4));
    for (i=0;i<SpriteHeight;i++)
    {
      BaseOffset=BaseOffsetor+i*2*(SpriteWidth/32);
      RWEntries[i] = lsb(2,D+BaseOffset);
    }
    for (i=0;i<SpriteHeight;i++)
    {
      BaseOffset = BaseOffsetor+RWEntries[i];
      if (LeftMargin>0)
      {
        for (j=0;j<LeftMargin;j++)
          *P++ = fbuffer[j];
      }
      TotalRowLength=0;
      do
      {
        SegmentType=D[BaseOffset++];
        u1 code = SegmentType / 32;
        u1 value = (SegmentType & 31) + 1;
        if(code==7)
        {
          for(h=0; h<value; ++h)
          {
            *P++ = D[BaseOffset++];
          }
        }
        else
        {
          for(h=0; h<value; ++h)
          {
            *P++ = code;
          }
        }
        TotalRowLength+=value;
      }while(TotalRowLength<SpriteWidth);
      if (RightMargin>0)
      {
        for (j=0;j<RightMargin;j++)
          *P++ = fbuffer[j];
      }
      if (add>0)
      {
        for (j=0;j<add+RowAdd;j++)
          *P++=fbuffer[j];
      }
    }
    free(RWEntries);
    RWEntries=NULL;
    if (BottomMargin>0)
    {
      for (i=0;i<BottomMargin;i++)
      {
        for (j=0;j<FullWidth+add;j++)
          *P++ = fbuffer[j];
      }
    }
  }

  R->Width = FullWidth;
  R->Height = FullHeight;
  R->Pitch = FullWidth+add;
  R->Pixels = Pixels;
}



spr *defLoad(char *Input) {
  spr *S;
  pic *P;
  u1 *M;
  int I, J;
  Header *H;
  GroupHeader *GH;
  u1 Palette[256*4];
  char Tmp[1024];
  char *TypeName;
  u4 *Offs;
  u4 Offset = 0x310;

  unless(H = readFile(0, sizeof(Header), Input)) {
    printf("Invalid input file: %s\n", Input);
    abort();
  }

  unless(TypeName = findTypeName(H->Type)) {
    printf("Unrecognized DEF Type (%x): %s\n", H->Type, Input);
    abort();
  }

  /*printf("Type: %s\n", TypeName);
  printf("Width: %d\n", H->Width);
  printf("Height: %d\n", H->Height);
  printf("Groups: %d\n", H->NumGroups);
  printf("\n");*/

  S = sprNew();
  S->Palette = ns(u1, 4*256);

  // normalize palette to 256 32bit colors
  times (I, 256) {
    S->Palette[I*4+0] = H->ColorMap[I*3+0];
    S->Palette[I*4+1] = H->ColorMap[I*3+1];
    S->Palette[I*4+2] = H->ColorMap[I*3+2];
    S->Palette[I*4+3] = 0;
  }
  S->Palette[0+3] = 255;

  S->NAnis = H->NumGroups;
  S->Anis = ns(ani, S->NAnis);

  M = loadFile(Input);
  times (I, H->NumGroups) {
    char *Names;
    u4 *Offs;
    // map heroes can face different direction, but we dont care
    S->Anis[I].NFacs = 1;
    S->Anis[I].Facs = ns(fac,S->Anis[I].NFacs);

    if(H->Type == TYPE_CREATURE && I<23) {
      S->Anis[I].Name = strdup(CreatureAnimNames[I]);
    } else if(H->Type == TYPE_HERO && 9) {
      S->Anis[I].Name = strdup(HeroAnimNames[I]);
    }

    GH = readFile(Offset, sizeof(GroupHeader), Input);
    Offset += sizeof(GroupHeader);

    Names = readFile(Offset, 13*GH->Size, Input);
    Offset += 13*GH->Size;

    Offs = readFile(Offset, 4*GH->Size, Input);
    Offset += 4*GH->Size;

    S->Anis[I].Facs[0].NPics = GH->Size;
    S->Anis[I].Facs[0].Pics = ns(pic*, S->Anis[I].Facs[0].NPics);
    times (J, GH->Size) {
      UnpackResult R;
      unpackFrame(&R, M, Offs[J]);
      P = S->Anis[I].Facs[0].Pics[J] = picNew(R.Width, R.Height, 8);
      P->SharedPalette = 1;
      free(P->P);
      P->P = S->Palette;
      free(P->D);
      P->D = R.Pixels;
      P->I = R.Pitch;
      memcpy(Tmp, Names+13*J, 13);
      Tmp[13] = 0;
      P->N = strdup(Tmp);
    }

    free(Offs);
    free(Names);
  }

  return S;
}



int defInit(format *F) {
  F->Type = FMT_SPRITE;
  F->Name = "def";
  F->Description = "Heroes of Might & Magic 3 sprites";
  //F->Save = defSave;
  F->Load = defLoad;
  return 1;
}

