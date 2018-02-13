// Credits go to Erich Simon, who published original IlbMaker source code

#include "common.h"

#define SHOW_OPAQUE      0
#define SHOW_TRANSPARENT 1 /* 50% transparent */
#define SHOW_BLENDED     2

// these modes use Alpha field, which prescribes amount of effect
// we should really save our BMPs in 32-bit, instead of 24-bit
#define BLEND_USER      0 /* unused */
#define BLEND_ALPHA     1 /* Alpha transparent */
#define BLEND_BRIGHTEN  2 /* adds image color to the background */
#define BLEND_INTENSITY 3 /* same as BLEND_BRIGHTEN, but beef colors to %175 */
#define BLEND_SHADOW    4 /* blend as shadow */
#define BLEND_LINEAR    5 /* unused */

// load modes
#define LOAD_INSTANT       0
#define LOAD_ON_USE        1
#define LOAD_ON_DEMAND     2
#define LOAD_ON_REFERENCE  3

typedef struct {
  //u4 Id;    // 0x424C4904 (present only for AoW1 ILBs)
  u4 Unk;     // unknown value
  u4 Version; // float value (0x40400000=3.0, 0x40800000=4.0)
  u4 Size;    // 16 for v3.0, 24 for v4.0
  u4 Off;     // offset to image data
  u4 Len;     // size of whole ILB file
  u4 NPals;   // number of palettes
  //pal Pal[NPals]; // for v4.0 headers
} __attribute__ ((packed)) header;

typedef struct {
  u2 BitsPerChannel; // RGBA tetrads
  u1 Unknown;
  u1 BPP; // bites per pixel of each entry
  u1 Entries[256*4];
} __attribute__ ((packed)) pal;


typedef struct {
  u1 Unknown1[15];
  u2 BitsPerChannel; // RGBA tetrads
  u1 Unknown2;
  u1 BPP; // bites per pixel of each entry
  u1 Entries[256*4];
} __attribute__ ((packed)) pal2;


typedef struct {
  u4 Tag; // 0x0A - unknown
          // 0x0B - max number of elements
          // 0x0C - palettes
          // 0x32-0xFFFFFFFF - image id
  u4 Off;
} __attribute__ ((packed)) frame;


static u1 Grayscale[256*4];

static pic *decode(u1 *Z, int L, int Width, int Height, int BPP, int RLE
                  ,int ClipX, int ClipY, int ClipW, int ClipH, int ColorKey
                  ,u1 *Pal) {
  int I, X, Y, C=0, R, G, B, Pad, ClipEmpty=(!ClipW||!ClipH), EX;
  u1 *E;
  pic *P;

  if (!ClipW) ClipW = Width;
  if (!ClipH) ClipH = Height;

  //writeFile(0, L, "out/1.bin", Z);
  if (BPP==8) {
    P = picNew(Width, Height, 8);
    memcpy(P->P, Pal, 4*256);
    picClear(P, ColorKey);

    if (RLE == 1) {
      times (Y, ClipH) {
        E = Z + *(u4*)Z; Z+=4;
        for (X=0; Z < E; ) {
          C = *Z++;
          if (C == ColorKey) {
            C = *Z++;
            while (C--) picPut(P, ClipX+X++, ClipY+Y, ColorKey);
          } else {
            picPut(P, ClipX+X++, ClipY+Y, C);
          }
        }
      }
    } else if (RLE == 2) {
      //ClipW -= 1;
      if (ClipEmpty) return P;
      if (ClipW >= 0x20) EX = (1 + (ClipW - 1) / 8) * 8;
      else EX = (1 + (ClipW - 1) / 4) * 4;
      for (C=Y=0; Y<ClipH; Y++) {
        for (X=0; X<EX; ) {
          if (C > 0) {
            picPut(P, ClipX+X++, ClipY+Y, ColorKey);
            C--;
            continue;
          }
          C = *Z++;
          if (C == ColorKey) C = *Z++;
          else {
            picPut(P, ClipX+X++, ClipY+Y, C);
            C = 0;
          }
        }
      }
    } else {
      times(Y, ClipH) times(X, ClipW) picPut(P, ClipX+X, ClipY+Y, *Z++);
    }
  } else {
    P = picNew(Width, Height, 24);
    fromR5G6B5(R,G,B,ColorKey);
    picClear(P, R8G8B8(B,G,R));
    if (RLE == 1) {
      times (Y, ClipH) {
        //printf("%d\n", Y);
        Pad = *(u4*)Z / 2 % 2; // every RLE-line is padded to 4-bytes
        E = Z + *(u4*)Z; Z+=4;
        for (X=0; Z < E; ) {
          C = *(u2*)Z; Z+=2;
          if (C == ColorKey) {
            C = *(u2*)Z/2; Z+=2;
            fromR5G6B5(R,G,B,ColorKey);
            while (C--) picPut24(P, ClipX+X++, ClipY+Y, R8G8B8(B,G,R));
          } else {
            fromR5G6B5(R,G,B,C);
            picPut24(P, ClipX+X++, ClipY+Y, R8G8B8(B,G,R));
          }
        }
        if (Pad) Z += 2;
      }
    } else if (RLE == 2) {
      times (Y, ClipH) {
        Z += ClipW; // skip mask
        times(X, ClipW) {
          C = *(u2*)Z; Z+=2;
          fromR5G6B5(R,G,B,C);
          picPut24(P, ClipX+X, ClipY+Y, R8G8B8(B,G,R));
        }
      }
    } else {
      times (Y, ClipH) times(X, ClipW) {
        C = *(u2*)Z; Z+=2;
        fromR5G6B5(R,G,B,C);
        picPut24(P, ClipX+X, ClipY+Y, R8G8B8(B,G,R));
      }
    }
  }
  return P;
}

// FIXME:
//  Images/Flags.ILB (garbled palette)
static void ilbDecompile2(char *Output, char *Input, u1 *M, int L, char *Tmp) {
  pic *P;
  int I, J, C, NFrames, NPals=0, NTags,  Id, Type, SubType, LoadMode, Composite;
  int Width, Height, XO, YO, SubId, Len, Off, Off2, VWidth, VHeight, PalId;
  int ClipW, ClipH, ClipX, ClipY, HotX, HotY;
  int BPP, RLE;
  int Show, Blend, BlendArg, Alpha, Format, ColorKey;
  char Name[256];
  pal2 *Pals = 0;
  int Tags[256];
  frame *F = ns(frame, 0x100000);
  u1 *Pal, *Z, *Q = M;
  C = *Q++;
  if (C <= 0x80) {I = C; C = 0;}
  else {
    I = C-0x80;
    C = *(u4*)Q; Q+=4;
  }

  for (NFrames = 0; I--; NFrames++) {
    F[NFrames].Tag = *Q++;
    F[NFrames].Off = *Q++;
  }

  for (; C--; NFrames++) {
    F[NFrames].Tag = *(u4*)Q; Q+=4;
    F[NFrames].Off = *(u4*)Q; Q+=4;
  }

  times (I, NFrames) F[I].Off += Q-M; // normalize offsets

  for (I=0; F[I].Tag < 0x32; I++) {
    Q = M + F[I].Off;
    //printf("  Tag=%d Off=#%x\n", F[I].Tag, F[I].Off);
    if (F[I].Tag == 0x0A) continue; // always 1?
    if (F[I].Tag == 0x0B) continue; // number of tags
    if (F[I].Tag == 0x0C) {
      Q += 6; // skip unknown bytes
      NPals = *Q++;
      if (NPals) Pals = (pal2*)Q;
      continue;
    }
    if (F[I].Tag < 0x32) {
      printf("  Unsupported tag (%x)\n", F[I].Tag);
      continue;
    }
  }

  Off = L;
  for (I=NFrames-1; F[I].Tag >= 0x32; I--) {
    Q = M + F[I].Off;
    Id = F[I].Tag - 0x32;
    Type = *(u2*)Q; Q+=2;
    Q += 2; // always 0x0030

    RLE = 0;
    if (Type == 1) { // 8-bit image
      BPP = 8;
    } else if (Type == 2) { // 8-bit RLE sprite
      RLE = 1;
      BPP = 8;
    } else if (Type == 3) { // 8-bit RLE2 image
      RLE = 2;
      BPP = 8;
    } else if (Type == 16) { // 16-bit image
      BPP = 16;
    } else if (Type == 17) { // 16-bit RLE sprite
      RLE = 1;
      BPP = 16;
    } else if (Type == 18) { // 16-bit 50%-transparent RLE sprite
      RLE = 1;
      BPP = 16;
    } else if (Type == 19) { // bitmask
      BPP = 1;
      printf("  Unsupported type (%d)\n", Type);
      abort();
    } else if (Type == 20) { // same as Type==22
      BPP = 16;
      //printf("  Unsupported type (%d)\n", Type);
      //abort();
    } else if (Type == 21) { // 16-bit RLE2 sprite
      BPP = 16;
      RLE = 2;
      //printf("  Unsupported type (%d)\n", Type);
      //abort();
    } else if (Type == 22) { // grayscale alpha-channel
      BPP = 8;
    } else {
      printf("  Unsupported type (%d)\n", Type);
      abort();
    }

    if (BPP == 16) Format = 0x56509310;
    else Format = 8;

    times (J, 256) Tags[J] = -1;
    NTags = *Q++;
    times (J, NTags) {
      Tags[Q[0]] = Q[1];
      Q += 2;
    }

    Width = Height = XO = YO = SubId = LoadMode = SubType = Len = 0;
    Composite = PalId = 0;
    HotX = HotY = -1;
    ClipW = Width;
    ClipH = Height;
    ClipX = ClipY = 0;
    ColorKey = 0; // Important: RLE expects, that we default it to 0
    Show = 1;
    Blend = 0;
    Alpha = 0;
    BlendArg = -1;
    Name[0] = 0;
    Z = Q;
    Off2 = -1;
    times (J, 256) {
      if (Tags[J] == -1) continue;
      Q = Z + Tags[J];
      C = *(u4*)Q; Q+=4;
      switch (J) {
      case 0x00: Q-=4; break;
      case 0x0A: Q-=4;
                 C = *Q++;
                 memcpy(Tmp, Q, C);
                 Tmp[C] = 0;
                 pathParts(0, Name, 0, Tmp);
                 Q += C;
                 break;
      case 0x0B: Width = C; break;
      case 0x0C: Height = C; break;
      case 0x0D: XO = C; break;
      case 0x0E: YO = C; break;
      case 0x0F: SubId = C; break;
      case 0x10: C-=4; LoadMode = *Q++; break;
      case 0x11: Len = C; break;
      case 0x12: VWidth = C; break;
      case 0x13: VHeight = C; break;
      case 0x14: break; // Offset, relative to start of first image
      case 0x16: HotX = C; break;
      case 0x17: HotY = C; break;
      case 0x18: Q-=4; C = *Q++; Q += C; break; // description
      case 0x19: Q-=4;
                 SubType = *Q++;
                 Q += 4; // unknown
                 if (SubType == 3) {
                   BlendArg = *Q++;
                   Q++; // unknown
                 }
                 Q++; // unknown
                 Blend = *Q++;
                 if (SubType == 3) {
                   Alpha = (int)(*(f4*)Q * 100);
                   Q += 4;
                 }
                 break;
      case 0x32: if (BPP == 8 && Type!=22) PalId=C-1; else Format=C; break;
      case 0x3C: ClipW = C; break;
      case 0x3D: ClipH = C; break;
      case 0x3E: ClipX = C; break;
      case 0x3F: ClipY = C; break;
      case 0x40: if (BPP != 8) ColorKey = C;
                 else {Q-=4; ColorKey = *Q++;}
                 break;
      case 0x41: Off2 = C; break;
      default:
        printf("  Unsupported field (#%02x=#%08x)\n", J, C); break;
      }
    }

    Off -= Len;

#if 0
    printf("  Type=%02d:%d Id=%03d,%02d Name=%s\n"
           "  Off=#%08x:#%08x Len=%d Composite=%d X=%d Y=%d W=%d H=%d Show=%d\n"
           "  Blend=%d Alpha=%d PalId=%d ColorKey=#%x ClipX=%d:%d ClipY=%d:%d\n"
           "  HotX=%d HotY=%d RLE=%d Format=#%08x\n"
          ,Type, SubType, Id, SubId, Name
          ,Off, Off2, Len, Composite, XO, YO, Width, Height ,Show
          ,Blend, Alpha, PalId, ColorKey, ClipX, ClipW, ClipY, ClipH
          ,HotX, HotY, RLE, Format);
    printf("\n\n");
#endif

    if (Pals) Pal = Pals[PalId].Entries;
    else Pal = Grayscale;

    P = decode(M+Off, Len, Width, Height, BPP, RLE, ClipX, ClipY, ClipW, ClipH
              ,ColorKey, Pal);
    sprintf(Tmp, "%s/%s_%03d_%02d.bmp", Output, Name, Id, SubId);
    bmpSave(Tmp, P); // GIMP apparently has problem opening 8-bit BMPs
    picDel(P);
  }

  free(F);
}

static void ilbDecompile(char *Output, char *Input) {
  pic *P;
  int I, J, C, Id, Type, SubType, Composite;
  int Width, Height, XO, YO, SubId, Len, Off, VWidth, VHeight, PalId;
  int ClipW, ClipH, ClipX, ClipY;
  int BPP, Sprite, RLE;
  int Show, Blend, Alpha, Format, ColorKey;
  char Tmp[1024], Name[256];
  u1 *Q, *Z, *Pal;
  int L = fileSize(Input);
  u1 *M = readFile(0, L, Input);
  header *H;
  pal *Pals = 0; // palettes

  times (I, 256) {
    Grayscale[I*4+0] = I;
    Grayscale[I*4+1] = I;
    Grayscale[I*4+2] = I;
    Grayscale[I*4+3] = 0;
  }

  if (M[0] == 4 && M[1] == 'I' && M[2] == 'L' && M[3] == 'B') { // AoW1?
    H = (header*)(M+4);
  } else {
    H = (header*)M;
    ilbDecompile2(Output, Input, M, L, Tmp);
    return;
  }

  if (H->Size == 16 && H->Version == 0x40400000) {
    Q = (u1*)H + H->Size;
    //printf("  U=#%08x Version=%f Size=%d Off=#%08x\n"
    //      ,H->Unk, *(f4*)&H->Version, H->Size, H->Off);
  } else if(H->Size == 24 && H->Version == 0x40800000) {
    Pals = (pal*)((u1*)H + H->Size);
    Q = (u1*)(Pals+H->NPals);
    //printf("  U=#%08x Version=%f Size=%d Off=#%08x Len=%08x NPals=%d\n"
    //      ,H->Unk, *(f4*)&H->Version, H->Size, H->Off, H->Len, H->NPals);
  } else {
    printf("  Not an ILB file.\n");
    abort();
  }
  //printf("\n\n");

  Composite = 0;
  while (*(u4*)Q != 0xFFFFFFFF) {
    //printf("%x\n", Q-M);
    unless (Composite) {Id = *(u4*)Q; Q+=4;} // parts of composite have same id
    // Note: Composites are used for dynamically animating sprite.
    //       Example: rotating cannon tower on static tank frame.
    Type = *(u4*)Q; Q+=4;
    if (Type == 0x100) {
      Composite++;
      Type = *(u4*)Q; Q+=4;
    }

    if (Type == 0) {
      //printf("  End of Composite Frame\n\n\n");
      Composite = 0;
      Q += 4; // skip unknown field
      continue;
    }
    SubType = *(u1*)Q; Q+=1;
    C = *(u4*)Q; Q+=4;
    memcpy(Tmp, Q, C);
    Tmp[C] = 0;
    pathParts(0, Name, 0, Tmp);
    Q += C;

    Width = *(u4*)Q; Q+=4;
    Height = *(u4*)Q; Q+=4;
    XO = *(s4*)Q; Q+=4;
    YO = *(s4*)Q; Q+=4;
    SubId = *(u4*)Q; Q+=4; // id of animation sequence
    Q += 1; // skip unknown byte
    Len = *(u4*)Q; Q+=4;
    if (SubType != 1) {
      Off = H->Off + *(u4*)Q;
      Q+=4;
    }
    VWidth = *(u4*)Q; Q+=4;
    VHeight = *(u4*)Q; Q+=4;

    Sprite = 0; // only sprites have clipping info
    RLE = 0;
    if (Type == 1) { // 8-bit image
      BPP = 8;
    } else if (Type == 2) { // 8-bit RLE sprite
      RLE = 1;
      Sprite = 1;
      BPP = 8;
      Q += 1; // skip unknown byte
    } else if (Type == 3) { // 8-bit RLE2 image
      RLE = 2;
      Sprite = 1;
      BPP = 8;
      Q += 1; // skip unknown byte
    } else if (Type == 16) { // 16-bit image
      BPP = 16;
    } else if (Type == 17) { // 16-bit RLE sprite
      RLE = 1;
      Sprite = 1;
      BPP = 16;
    } else if (Type == 18) { // 16-bit 50%-transparent RLE sprite
      RLE = 1;
      Sprite = 1;
      BPP = 16;
    } else if (Type == 19) { // bitmask
      BPP = 1;
      printf("  Unsupported type (%d)\n", Type);
      abort();
    } else if (Type == 20) { // shadow
      BPP = 16;
    } else if (Type == 21) { // 16-bit RLE2 sprite
      RLE = 2;
      //Sprite = 1;
      BPP = 16;
    } else if (Type == 22) { // 16-bit sprite
      Sprite = 1;
      BPP = 16;
    } else {
      printf("  Unsupported type (%d)\n", Type);
      abort();
    }

    if (SubType == 3) {
      Show = *(u4*)Q & 0xFF;
      Blend = (*(u4*)Q>>8) & 0xFF;
      Q+=4;
      Alpha = *(u4*)Q; Q+=4;
    } else {
      Show = 1;
      Blend = 0;
      Alpha = 0;
    }

    if (BPP == 16) {
      Format = *(u4*)Q; Q+=4;
      // 0x55509310=R5G5B5A0 = 16-bit
      // 0x56509310=R5G6B5A0 = 16-bit with 6-bit green
      // 0x88809318=R8G8B8A0 = 24-bit
      // 0x88889320=R8G8B8A8 = 32-bit
      // ILB files use only 0x56509310
    } else {
      Format = 8;
      PalId = *(u4*)Q; Q+=4;
    }

    if (Sprite) {
      ClipW = *(s4*)Q; Q+=4;
      ClipH = *(s4*)Q; Q+=4;
      ClipX = *(s4*)Q; Q+=4;
      ClipY = *(s4*)Q; Q+=4;
      ColorKey = *(u4*)Q; Q+=4;
    } else {
      ClipW = Width;
      ClipH = Height;
      ClipX = 0;
      ClipY = 0;
      //ColorKey = 0xFFFFFFFF;
      ColorKey = 0;
    }

    if (Sprite && Type != 22) Q+=4; // skip unknown field

    if (SubType == 1) {
      Off = Q-M;
      Q += Len;
    }

    unless (Composite) Q += 4; // skip ending word, which is always 0xFFFFFFFF

#if 0
    printf("  Type=%02d:%d Id=%03d,%02d Name=%s Off=#%08x Len=%d\n"
           "  Composite=%d X=%d Y=%d W=%d H=%d Show=%d Blend=%d Alpha=%d PalId=%d\n"
           "  ColorKey=#%x ClipX=%d:%d ClipY=%d:%d Sprite=%d RLE=%d Format=#%08x\n"
          ,Type, SubType, Id, SubId, Name, Off, Len
          ,Composite, XO, YO, Width, Height ,Show, Blend, Alpha, PalId
          ,ColorKey, ClipX, ClipW, ClipY, ClipH ,Sprite, RLE, Format);
    printf("\n\n");
#endif

    if (Pals) Pal = Pals[PalId].Entries;
    else Pal = Grayscale;
    P = decode(M+Off, Len, Width, Height, BPP, RLE, ClipX, ClipY, ClipW, ClipH
              ,ColorKey, Pal);
    sprintf(Tmp, "%s/%s_%03d_%02d_%d.bmp", Output, Name, Id, SubId, Composite);
    bmpSave(Tmp, P);
    picDel(P);
  }
}

int ilbInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "ilb";
  F->Description = "Age of Wonders 1-2 image archives";
  F->Decompile = ilbDecompile;
  return 1;
}

