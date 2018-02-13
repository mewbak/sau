//Credits go to SNV

#include "common.h"

typedef struct {
  u1 Id[8]; //SPRCRYO\x20
  u4 Type;
} __attribute__ ((packed)) header;

typedef struct {
  u2 W;
  u2 H;
  u2 X;
  u2 Y;
} __attribute__ ((packed)) frame;

typedef struct {
  int A;
  int B;
  int C;
  int D;
  int X;
  int Y;
} base;


typedef struct {
  u1 *Data;
  int W;
  int SY;
  int EY;
  int SX;
  int EX;
} clip;


#define _BYTE u1
#define _WORD u2
#define _DWORD u4
#define __int8 char
#define __int16 short
#define __int32 int
#define bool int

// Some convenience macros to make partial accesses nicer
// first unsigned macros:
#define LOBYTE(x)   (*((_BYTE*)&(x)))   // low byte
#define LOWORD(x)   (*((_WORD*)&(x)))   // low word
#define LODWORD(x)  (*((_DWORD*)&(x)))  // low dword
#define HIBYTE(x)   (*((_BYTE*)&(x)+1))
#define HIWORD(x)   (*((_WORD*)&(x)+1))
#define HIDWORD(x)  (*((_DWORD*)&(x)+1))
#define BYTEn(x, n)   (*((_BYTE*)&(x)+n))
#define WORDn(x, n)   (*((_WORD*)&(x)+n))
#define BYTE1(x)   BYTEn(x,  1)         // byte 1 (counting from 0)
#define BYTE2(x)   BYTEn(x,  2)
#define BYTE3(x)   BYTEn(x,  3)
#define BYTE4(x)   BYTEn(x,  4)
#define BYTE5(x)   BYTEn(x,  5)
#define BYTE6(x)   BYTEn(x,  6)
#define BYTE7(x)   BYTEn(x,  7)
#define BYTE8(x)   BYTEn(x,  8)
#define BYTE9(x)   BYTEn(x,  9)
#define BYTE10(x)  BYTEn(x, 10)
#define BYTE11(x)  BYTEn(x, 11)
#define BYTE12(x)  BYTEn(x, 12)
#define BYTE13(x)  BYTEn(x, 13)
#define BYTE14(x)  BYTEn(x, 14)
#define BYTE15(x)  BYTEn(x, 15)
#define WORD1(x)   WORDn(x,  1)
#define WORD2(x)   WORDn(x,  2)         // third word of the object, unsigned
#define WORD3(x)   WORDn(x,  3)
#define WORD4(x)   WORDn(x,  4)
#define WORD5(x)   WORDn(x,  5)
#define WORD6(x)   WORDn(x,  6)
#define WORD7(x)   WORDn(x,  7)

#define __SETS__(T) \
  (sizeof(T) == 1 ? (s1)(T) < 0 \
  :sizeof(T) == 2 ? (s2)(T) < 0 \
  :sizeof(T) == 4 ? (s4)(T) < 0 \
  :                 (s8)(T) < 0)

// overflow flag of subtraction (x-y)
#define __OFSUB__(X,Y) ((__SETS__(X) ^ __SETS__(Y)) & (__SETS__(X) ^ __SETS__((X)-(Y))))

// overflow flag of addition (x+y)
#define __OFADD__(X,Y) (((1 ^ __SETS__(X)) ^ __SETS__(Y)) & (__SETS__(X) ^ __SETS__((X)+(Y))))

static u1 TypeLUT[] = {
  0x00, 0x01, 0x02, 0x02, 0x03, 0x03, 0x03, 0x03, 0x0C, 0x0D, 0x0E, 0x0E, 0x0F, 0x0F, 0x0F, 0x0F,
  0x18, 0x19, 0x1A, 0x1A, 0x1B, 0x1B, 0x1B, 0x1B, 0x24, 0x25, 0x26, 0x26, 0x27, 0x27, 0x27, 0x27,
  0x00, 0x01, 0x02, 0x02, 0x03, 0x03, 0x03, 0x03, 0x0C, 0x0D, 0x0E, 0x0E, 0x0F, 0x0F, 0x0F, 0x0F,
  0x18, 0x19, 0x1A, 0x1A, 0x1B, 0x1B, 0x1B, 0x1B, 0x24, 0x25, 0x26, 0x26, 0x27, 0x27, 0x27, 0x27,
  0x04, 0x05, 0x06, 0x06, 0x07, 0x07, 0x07, 0x07, 0x10, 0x11, 0x12, 0x12, 0x13, 0x13, 0x13, 0x13,
  0x1C, 0x1D, 0x1E, 0x1E, 0x1F, 0x1F, 0x1F, 0x1F, 0x28, 0x29, 0x2A, 0x2A, 0x2B, 0x2B, 0x2B, 0x2B,
  0x04, 0x05, 0x06, 0x06, 0x07, 0x07, 0x07, 0x07, 0x10, 0x11, 0x12, 0x12, 0x13, 0x13, 0x13, 0x13,
  0x1C, 0x1D, 0x1E, 0x1E, 0x1F, 0x1F, 0x1F, 0x1F, 0x28, 0x29, 0x2A, 0x2A, 0x2B, 0x2B, 0x2B, 0x2B,
  0x08, 0x09, 0x0A, 0x0A, 0x0B, 0x0B, 0x0B, 0x0B, 0x14, 0x15, 0x16, 0x16, 0x17, 0x17, 0x17, 0x17,
  0x20, 0x21, 0x22, 0x22, 0x23, 0x23, 0x23, 0x23, 0x2C, 0x2D, 0x2E, 0x2E, 0x2F, 0x2F, 0x2F, 0x2F,
  0x08, 0x09, 0x0A, 0x0A, 0x0B, 0x0B, 0x0B, 0x0B, 0x14, 0x15, 0x16, 0x16, 0x17, 0x17, 0x17, 0x17,
  0x20, 0x21, 0x22, 0x22, 0x23, 0x23, 0x23, 0x23, 0x2C, 0x2D, 0x2E, 0x2E, 0x2F, 0x2F, 0x2F, 0x2F,
  0x08, 0x09, 0x0A, 0x0A, 0x0B, 0x0B, 0x0B, 0x0B, 0x14, 0x15, 0x16, 0x16, 0x17, 0x17, 0x17, 0x17,
  0x20, 0x21, 0x22, 0x22, 0x23, 0x23, 0x23, 0x23, 0x2C, 0x2D, 0x2E, 0x2E, 0x2F, 0x2F, 0x2F, 0x2F,
  0x08, 0x09, 0x0A, 0x0A, 0x0B, 0x0B, 0x0B, 0x0B, 0x14, 0x15, 0x16, 0x16, 0x17, 0x17, 0x17, 0x17,
  0x20, 0x21, 0x22, 0x22, 0x23, 0x23, 0x23, 0x23, 0x2C, 0x2D, 0x2E, 0x2E, 0x2F, 0x2F, 0x2F, 0x2F,
  0x30, 0x31, 0x32, 0x32, 0x33, 0x33, 0x33, 0x33, 0x30, 0x31, 0x32, 0x32, 0x33, 0x33, 0x33, 0x33,
  0x30, 0x31, 0x32, 0x32, 0x33, 0x33, 0x33, 0x33, 0x30, 0x31, 0x32, 0x32, 0x33, 0x33, 0x33, 0x33,
  0x30, 0x31, 0x32, 0x32, 0x33, 0x33, 0x33, 0x33, 0x30, 0x31, 0x32, 0x32, 0x33, 0x33, 0x33, 0x33,
  0x30, 0x31, 0x32, 0x32, 0x33, 0x33, 0x33, 0x33, 0x30, 0x31, 0x32, 0x32, 0x33, 0x33, 0x33, 0x33,
  0x34, 0x35, 0x36, 0x36, 0x37, 0x37, 0x37, 0x37, 0x34, 0x35, 0x36, 0x36, 0x37, 0x37, 0x37, 0x37,
  0x34, 0x35, 0x36, 0x36, 0x37, 0x37, 0x37, 0x37, 0x34, 0x35, 0x36, 0x36, 0x37, 0x37, 0x37, 0x37,
  0x34, 0x35, 0x36, 0x36, 0x37, 0x37, 0x37, 0x37, 0x34, 0x35, 0x36, 0x36, 0x37, 0x37, 0x37, 0x37,
  0x34, 0x35, 0x36, 0x36, 0x37, 0x37, 0x37, 0x37, 0x34, 0x35, 0x36, 0x36, 0x37, 0x37, 0x37, 0x37,
  0x38, 0x39, 0x3A, 0x3A, 0x3B, 0x3B, 0x3B, 0x3B, 0x38, 0x39, 0x3A, 0x3A, 0x3B, 0x3B, 0x3B, 0x3B,
  0x38, 0x39, 0x3A, 0x3A, 0x3B, 0x3B, 0x3B, 0x3B, 0x38, 0x39, 0x3A, 0x3A, 0x3B, 0x3B, 0x3B, 0x3B,
  0x38, 0x39, 0x3A, 0x3A, 0x3B, 0x3B, 0x3B, 0x3B, 0x38, 0x39, 0x3A, 0x3A, 0x3B, 0x3B, 0x3B, 0x3B,
  0x38, 0x39, 0x3A, 0x3A, 0x3B, 0x3B, 0x3B, 0x3B, 0x38, 0x39, 0x3A, 0x3A, 0x3B, 0x3B, 0x3B, 0x3B,
  0x38, 0x39, 0x3A, 0x3A, 0x3B, 0x3B, 0x3B, 0x3B, 0x38, 0x39, 0x3A, 0x3A, 0x3B, 0x3B, 0x3B, 0x3B,
  0x38, 0x39, 0x3A, 0x3A, 0x3B, 0x3B, 0x3B, 0x3B, 0x38, 0x39, 0x3A, 0x3A, 0x3B, 0x3B, 0x3B, 0x3B,
  0x38, 0x39, 0x3A, 0x3A, 0x3B, 0x3B, 0x3B, 0x3B, 0x38, 0x39, 0x3A, 0x3A, 0x3B, 0x3B, 0x3B, 0x3B,
  0x38, 0x39, 0x3A, 0x3A, 0x3B, 0x3B, 0x3B, 0x3B, 0x38, 0x39, 0x3A, 0x3A, 0x3B, 0x3B, 0x3B, 0x3B};

static _DWORD GlbFormat;
static int SprFlags, Init1, Init2, Init3, Init4, Init5, Init6;
static void *(*UserMalloc)(_DWORD);

static u4 PalA[0x2000], PalB[0x2000], PalC[0x2000];

static void initPalsAB()
{
  unsigned __int32 I; // [sp+4Ch] [bp-18h]@4
  unsigned __int32 J; // [sp+4Ch] [bp-18h]@17
  unsigned __int32 K; // [sp+50h] [bp-14h]@2
  unsigned __int32 L; // [sp+50h] [bp-14h]@15
  unsigned __int32 A; // [sp+5Ch] [bp-8h]@7
  unsigned __int32 B; // [sp+5Ch] [bp-8h]@20

  if ( SprFlags == 1 )
  {
    for ( K = 0; K < 0x20; ++K )
    {
      for ( I = 0; I < 0x100; ++I )
      {
        if ( ((0x1F - K) * 8 * (I & 3) >> 4) & 1 )
          A = ((0x1F - K) * 8 * (I & 3) >> 5) + 1;
        else
          A = (0x1F - K) * 8 * (I & 3) >> 5;
        if ( A > 0x1F )
          A = 31;
        *(_DWORD *)&PalB[2 * (I + (K << 8))] = 0x20 * A + ((31 - K) * ((I & 0x7C) >> 2) >> 5 << 10);
        *(_DWORD *)&PalA[2 * (I + (K << 8))] = ((31 - K) * (I & 0x1F) >> 5)
                                             + 0x20 * ((0x1F - K) * ((I & 0xE0) >> 5) >> 5);
      }
    }
    GlbFormat = 0x7BDEu;
  }
  else
  {
    if ( SprFlags )
    {
      GlbFormat = 0xFEFEFEu;
    }
    else
    {
      for ( L = 0; L < 0x20; ++L )
      {
        for ( J = 0; J < 0x100; ++J )
        {
          if ( ((31 - L) * 8 * (J & 7) >> 4) & 1 )
            B = ((31 - L) * 8 * (J & 7) >> 5) + 1;
          else
            B = (31 - L) * 8 * (J & 7) >> 5;
          if ( B > 0x3F )
            B = 63;
          *(_DWORD *)&PalB[2 * (J + (L << 8))] = 0x20 * B + ((0x1F - L) * ((J & 0xF8) >> 3) >> 5 << 11);
          *(_DWORD *)&PalA[2 * (J + (L << 8))] = ((0x1F - L) * (J & 0x1F) >> 5)
                                               + 0x20 * ((0x1F - L) * ((J & 0xE0) >> 5) >> 5);
        }
      }
      GlbFormat = 0xF7DEu;
    }
  }
}


static void  initPalsC()
{
  unsigned __int8 C; // [sp+4Ch] [bp-Ch]@5
  int J; // [sp+50h] [bp-8h]@3
  int I; // [sp+54h] [bp-4h]@1

  for ( I = 0; I < 32; ++I )
  {
    for ( J = 0; J < 256; ++J )
    {
      C = I * (J & 0x1F) / 31;
      if ( SprFlags >= 2 )
        PalC[J + (I << 8)] = C;
      else
        PalC[J + (I << 8)] = C << 8;
    }
  }
}


static int SprDll_Init(char *CPU, _BYTE *O, void *(*allocator)(_DWORD))
{
  int R; // eax@3

  if ( strcmp(CPU, "PentiumII") && strcmp(CPU, "PentiumPro") )
  {
    R = 1;
  }
  else
  {
    SprFlags = *O;
    Init1 = O[1];
    Init2 = O[2];
    Init3 = O[3];
    Init4 = O[4];
    Init5 = O[5];
    Init6 = O[6];
    initPalsAB();
    initPalsC();
    UserMalloc = allocator;
    R = 0;
  }
  return R;
}

static int decode0x3C(_BYTE *Pal, base *Base, clip *Clip, frame *F)
{
  int S;
  int XD; // ebx@1
  int YD; // eax@1
  int H; // ecx@1
  clip *C; // ebp@1
  _DWORD *LineOffs; // edi@1
  _BYTE *v9; // esi@1
  int YDD; // eax@2
  bool OF2; // zf@2
  bool OF3; // sf@2
  unsigned __int8 OF1; // of@2
  int v14; // edx@4
  int XDD; // ebx@7
  int v16; // edx@9
  _DWORD *Dst; // edi@12
  unsigned int v18; // eax@15
  _BYTE *v19; // esi@15
  unsigned int v20; // ebx@15
  unsigned int v21; // eax@15
  int v22; // edi@15
  int v23; // ebx@15
  int v24; // edx@15
  unsigned int v25; // eax@15
  unsigned int v26; // ecx@15
  unsigned int v27; // ecx@16
  char v28; // cf@16
  unsigned int v29; // ecx@16
  unsigned int v30; // ecx@17
  unsigned int v31; // eax@21
  unsigned int v32; // ebx@21
  unsigned int v33; // ecx@22
  unsigned int v34; // ecx@23
  unsigned int v35; // edx@27
  int v36; // esi@27
  unsigned int v37; // edx@27
  unsigned int v38; // ebx@28
  int v39; // edi@28
  int v40; // eax@28
  unsigned int v41; // ecx@29
  unsigned int v42; // ecx@29
  unsigned int v43; // ecx@30
  unsigned int v44; // ecx@34
  unsigned int v45; // ecx@40
  unsigned int v46; // ecx@41
  int v47; // eax@46
  int v48; // ecx@46
  int v49; // esi@47
  unsigned int v50; // eax@47
  unsigned int v51; // eax@47
  unsigned int v52; // eax@48
  unsigned int v53; // eax@50
  int v54; // eax@54
  int v55; // esi@55
  unsigned int v56; // eax@55
  unsigned int v57; // eax@56
  int v58; // eax@61
  int v59; // esi@63
  unsigned int v60; // eax@63
  unsigned int v61; // eax@63
  unsigned int v62; // eax@66
  unsigned int v63; // eax@71
  unsigned int v64; // eax@77
  unsigned int v65; // eax@78
  int v66; // ecx@83
  int v67; // ebx@83
  unsigned int v68; // eax@85
  int v69; // esi@85
  unsigned int v70; // ebx@85
  unsigned int v71; // eax@85
  int v72; // edi@85
  int v73; // ebx@85
  int v74; // edx@85
  unsigned int v75; // eax@85
  _BYTE *v76; // esi@85
  int v77; // ebx@86
  unsigned int v78; // ecx@87
  unsigned int v79; // ecx@87
  unsigned int v80; // ecx@88
  unsigned int v81; // ecx@89
  int v82; // ecx@92
  int v83; // ebx@92
  unsigned int v84; // eax@94
  unsigned int v85; // ebx@94
  unsigned int v86; // ecx@96
  unsigned int v87; // ecx@97
  int v88; // ecx@101
  unsigned int v89; // edx@101
  int v90; // esi@101
  unsigned int v91; // edx@101
  int v92; // ebx@101
  unsigned int v93; // ebx@103
  int v94; // edi@103
  int v95; // eax@103
  unsigned int v96; // ecx@105
  unsigned int v97; // ecx@105
  unsigned int v98; // ecx@106
  int v99; // ecx@110
  unsigned int v100; // ecx@111
  _DWORD *v101; // eax@116
  _BYTE *v102; // esi@116
  unsigned int v103; // ecx@117
  unsigned int v104; // ecx@118
  int v105; // eax@123
  int v106; // ecx@123
  int v107; // esi@124
  unsigned int v108; // eax@124
  unsigned int v109; // eax@124
  unsigned int v110; // eax@125
  unsigned int v111; // eax@127
  int v112; // eax@131
  int v113; // esi@132
  unsigned int v114; // eax@132
  unsigned int v115; // eax@133
  int v116; // eax@138
  int v117; // esi@140
  unsigned int v118; // eax@140
  unsigned int v119; // eax@140
  unsigned int v120; // eax@143
  unsigned int v121; // eax@148
  unsigned int v122; // eax@153
  unsigned int v123; // eax@154
  int v125; // [sp-Ch] [bp-68h]@83
  int v126; // [sp-Ch] [bp-68h]@92
  int v127; // [sp-Ch] [bp-68h]@101
  _BYTE *v128; // [sp-8h] [bp-64h]@117
  _DWORD *v129; // [sp-4h] [bp-60h]@40
  _DWORD *SavedDst; // [sp-4h] [bp-60h]@117
  int v131; // [sp+0h] [bp-5Ch]@1
  _DWORD *L; // [sp+4h] [bp-58h]@4
  int W; // [sp+Ch] [bp-50h]@1
  int Pitch; // [sp+14h] [bp-48h]@12
  __int16 v136; // [sp+58h] [bp-4h]@12
  char v137; // [sp+5Ah] [bp-2h]@1

  XD = Base->X - F->X;
  YD = Base->Y - F->Y;
  W = F->W;
  H = F->H;
  C = Clip;
  LineOffs = (_DWORD*)(F+1);
  v131 = 0;
  v137 = 0;
  v9 = (char *)&F[1] + 4 * H;
  if ( YD < C->SY )
  {
    YDD = YD - C->SY;
    OF1 = __OFADD__(YDD, H);
    OF2 = YDD + H == 0;
    OF3 = YDD + H < 0;
    H += YDD;
    if ( (unsigned __int8)(OF3 ^ OF1) | OF2 )
      return 1;
    LineOffs -= YDD;
    YD = C->SY;
    v9 += *LineOffs;
  }
  L = LineOffs;
  v14 = H + YD - C->EY;
  if ( H + YD <= C->EY || (OF1 = __OFSUB__(H, v14), H -= v14, !((unsigned __int8)(H < 0 ^ OF1) | H == 0)) )
  {
    if ( XD < C->SX )
    {
      XDD = XD - C->SX;
      OF1 = __OFADD__(XDD, W);
      OF2 = XDD + W == 0;
      OF3 = XDD + W < 0;
      W += XDD;
      if ( (unsigned __int8)(OF3 ^ OF1) | OF2 )
        return 1;
      v137 = 1;
      v131 = -XDD;
      XD = C->SX;
    }
    v16 = W + XD - C->EX;
    if ( W + XD > C->EX )
    {
      OF1 = __OFSUB__(W, v16);
      OF2 = W == v16;
      OF3 = W - v16 < 0;
      W -= v16;
      if ( (unsigned __int8)(OF3 ^ OF1) | OF2 )
        return 1;
      v137 |= 2u;
    }
    Dst = (_DWORD*)(C->Data + 4*(C->W * YD + XD));
    v136 = H;
    Pitch = C->W;
    if ( v137 & 3 )
    {
      if ( v137 & 1 )
        goto LABEL_76;
LABEL_117:
      v77 = W;
      SavedDst = Dst;
      v128 = v9;
      v103 = *v9;
      v76 = v9 + 1;
      v28 = v103 & 1;
      v81 = v103 >> 1;
      if ( v28 )
        goto LABEL_110;
      v28 = v81 & 1;
      v104 = v81 >> 1;
      if ( v28 )
      {
        v28 = v104 & 1;
        v81 = v104 >> 1;
        if ( v28 )
          goto LABEL_101;
        while ( 1 )
        {
LABEL_83:
          v66 = v81 + 1;
          v67 = v77 - v66;
          v125 = v67;
          if ( v67 < 0 )
            v66 += v67;
          do
          {
            v68 = *v76++;
            S = *v76++;
            //v71 = v68 >> 1 << 8;
            //v73 = v71 | Dst[0];
            //v74 = v71 | Dst[1];
            //v75 = (unsigned int)(unsigned __int16)(GlbFormat & LOWORD(Z[S])) >> 1;
            //*(P - 1) = LOWORD(PalA[v73]) + LOWORD(PalB[v74]) + v75;
            *Dst++ = R8G8B8A8(Pal[S*4+0],Pal[S*4+1],Pal[S*4+2],0xFF);
            --v66;
          }
          while ( v66 );
          v77 = v125;
          if ( v125 <= 0 )
            goto LABEL_115;
          v78 = *v76++;
          v28 = v78 & 1;
          v79 = v78 >> 1;
          if ( v28 )
            break;
          v28 = v79 & 1;
          v80 = v79 >> 1;
          if ( v28 )
            goto LABEL_115;
          v28 = v80 & 1;
          v81 = v80 >> 1;
          if ( !v28 )
            goto LABEL_101;
        }
        v28 = v79 & 1;
        v81 = v79 >> 1;
        if ( !v28 )
          goto LABEL_110;
      }
      else
      {
        v28 = v104 & 1;
        v81 = v104 >> 1;
        if ( !v28 )
          goto LABEL_115;
      }
      while ( 1 )
      {
LABEL_92:
        v82 = v81 + 1;
        v83 = v77 - v82;
        v126 = v83;
        if ( v83 < 0 )
          v82 += v83;
        do
        {
          S = *v76++;
          --v82;
          //*Dst++ = ((GlbFormat & *Dst) >> 1) + ((u4)(u2)(GlbFormat & LOWORD(Z[S])) >> 1);
          *Dst++ = R8G8B8A8(Pal[S*4+0],Pal[S*4+1],Pal[S*4+2],0xFF);
        }
        while ( v82 );
        v77 = v126;
        if ( v126 <= 0 )
          break;
        v86 = *v76++;
        v28 = v86 & 1;
        v81 = v86 >> 1;
        if ( v28 )
          goto LABEL_83;
        v28 = v81 & 1;
        v87 = v81 >> 1;
        if ( v28 )
        {
          v28 = v87 & 1;
          v81 = v87 >> 1;
          if ( !v28 )
            goto LABEL_101;
        }
        else
        {
          v28 = v87 & 1;
          v81 = v87 >> 1;
          if ( !v28 )
            break;
          do
          {
            while ( 1 )
            {
LABEL_110:
              v99 = v81 + 1;
              OF1 = __OFSUB__(v77, v99);
              v77 -= v99;
              if ( (u1)(v77 < 0 ^ OF1) | v77 == 0 )
                goto LABEL_115;
              Dst += v99;
              v100 = *v76++;
              v28 = v100 & 1;
              v81 = v100 >> 1;
              if ( v28 )
                goto LABEL_83;
              v28 = v81 & 1;
              v81 >>= 1;
              if ( !v28 )
                break;
              while ( 1 )
              {
LABEL_101:
                v88 = v81 + 1;
                S = v89 = *v76;
                v90 = (int)(v76 + 1);
                v91 = v89 << 8;
                v92 = v77 - v88;
                v127 = v92;
                if ( v92 < 0 )
                  v88 += v92;
                do
                {
                  //v93 = *(_BYTE *)P;
                  //v94 = (int)((char *)P + 1);
                  //v95 = PalB[v91 | *(_BYTE *)v94];
                  //P = (_WORD *)(v94 + 1);
                  --v88;
                  //*(P - 1) = LOWORD(PalA[v91 | v93]) + v95;

                  *Dst++ = R8G8B8A8(Pal[S*4+0],Pal[S*4+1],Pal[S*4+2],0xFF);
                }
                while ( v88 );
                v77 = v127;
                if ( v127 <= 0 )
                  goto LABEL_115;
                v96 = *(_BYTE *)v90;
                v76 = (_BYTE *)(v90 + 1);
                v28 = v96 & 1;
                v97 = v96 >> 1;
                if ( v28 )
                  break;
                v28 = v97 & 1;
                v98 = v97 >> 1;
                if ( v28 )
                  goto LABEL_115;
                v28 = v98 & 1;
                v81 = v98 >> 1;
                if ( !v28 )
                  goto LABEL_92;
              }
              v28 = v97 & 1;
              v81 = v97 >> 1;
              if ( !v28 )
                goto LABEL_83;
            }
            v28 = v81 & 1;
            v81 >>= 1;
          }
          while ( v28 );
        }
      }
LABEL_115:
      while ( 1 )
      {
        --v136;
        if ( !v136 )
          return 1;
        Dst = &SavedDst[Pitch];
        v101 = L;
        v102 = &v128[L[1]];
        ++L;
        v9 = &v102[-*v101];
        if ( !(v137 & 1) )
          goto LABEL_117;
LABEL_153:
        v106 = v131;
        v77 = W;
        SavedDst = Dst;
        v128 = v9;
        v122 = *v9;
        v76 = v9 + 1;
        v28 = v122 & 1;
        v110 = v122 >> 1;
        if ( v28 )
          goto LABEL_147;
        v28 = v110 & 1;
        v123 = v110 >> 1;
        if ( v28 )
        {
          v28 = v123 & 1;
          v110 = v123 >> 1;
          if ( !v28 )
            goto LABEL_123;
          while ( 1 )
          {
LABEL_139:
            v106 -= v110 + 1;
            if ( v106 < 0 )
            {
              v81 = ~v106;
              goto LABEL_101;
            }
            v117 = (int)(v76 + 1);
            v118 = *(_BYTE *)v117;
            v76 = (_BYTE *)(v117 + 1);
            v28 = v118 & 1;
            v119 = v118 >> 1;
            if ( v28 )
              break;
            v28 = v119 & 1;
            v120 = v119 >> 1;
            if ( v28 )
              goto LABEL_115;
            v28 = v120 & 1;
            v110 = v120 >> 1;
            if ( !v28 )
              goto LABEL_131;
          }
          v28 = v119 & 1;
          v110 = v119 >> 1;
          if ( v28 )
            goto LABEL_147;
          while ( 1 )
          {
LABEL_123:
            v105 = v110 + 1;
            v106 -= v105;
            if ( v106 < 0 )
            {
              v76 += 2 * (v106 + v105);
              v81 = ~v106;
              goto LABEL_83;
            }
            v107 = (int)&v76[2 * v105];
            v108 = *(_BYTE *)v107;
            v76 = (_BYTE *)(v107 + 1);
            v28 = v108 & 1;
            v109 = v108 >> 1;
            if ( v28 )
              break;
            v28 = v109 & 1;
            v111 = v109 >> 1;
            if ( v28 )
              goto LABEL_115;
            v28 = v111 & 1;
            v110 = v111 >> 1;
            if ( !v28 )
              goto LABEL_139;
          }
          v28 = v109 & 1;
          v110 = v109 >> 1;
          if ( !v28 )
            goto LABEL_147;
          while ( 1 )
          {
LABEL_131:
            v112 = v110 + 1;
            v106 -= v112;
            if ( v106 < 0 )
            {
              v116 = v106 + v112;
              v81 = ~v106;
              v76 += v116;
              goto LABEL_92;
            }
            v113 = (int)&v76[v112];
            v114 = *(_BYTE *)v113;
            v76 = (_BYTE *)(v113 + 1);
            v28 = v114 & 1;
            v110 = v114 >> 1;
            if ( v28 )
              goto LABEL_123;
            v28 = v110 & 1;
            v115 = v110 >> 1;
            if ( !v28 )
              break;
            v28 = v115 & 1;
            v110 = v115 >> 1;
            if ( !v28 )
              goto LABEL_139;
          }
          v28 = v115 & 1;
          v110 = v115 >> 1;
          if ( !v28 )
            continue;
LABEL_147:
          while ( 1 )
          {
            v106 -= v110 + 1;
            if ( v106 < 0 )
              break;
            v121 = *v76++;
            v28 = v121 & 1;
            v110 = v121 >> 1;
            if ( v28 )
              goto LABEL_123;
            v28 = v110 & 1;
            v110 >>= 1;
            if ( v28 )
              goto LABEL_139;
            v28 = v110 & 1;
            v110 >>= 1;
            if ( !v28 )
              goto LABEL_131;
          }
          v81 = ~v106;
          goto LABEL_110;
        }
        v28 = v123 & 1;
        v110 = v123 >> 1;
        if ( v28 )
          goto LABEL_131;
      }
    }
LABEL_40:
    v129 = Dst;
    v45 = *v9++;
    v28 = v45 & 1;
    v26 = v45 >> 1;
    if ( v28 )
      goto LABEL_34;
    v28 = v26 & 1;
    v46 = v26 >> 1;
    if ( !v28 )
    {
      v28 = v46 & 1;
      v26 = v46 >> 1;
      if ( v28 )
        goto LABEL_21;
LABEL_38:
      while ( 1 )
      {
        --v136;
        if ( !v136 )
          return 1;
        Dst = Pitch + v129;
        if ( !(v137 & 1) )
          goto LABEL_40;
LABEL_76:
        v48 = v131;
        if ( v137 & 2 )
          goto LABEL_153;
        v129 = Dst;
        v64 = *v9++;
        v28 = v64 & 1;
        v52 = v64 >> 1;
        if ( v28 )
          goto LABEL_70;
        v28 = v52 & 1;
        v65 = v52 >> 1;
        if ( v28 )
        {
          v28 = v65 & 1;
          v52 = v65 >> 1;
          if ( v28 )
            goto LABEL_62;
          while ( 1 )
          {
LABEL_46:
            v47 = v52 + 1;
            v48 -= v47;
            if ( v48 < 0 )
            {
              v9 += 2 * (v48 + v47);
              v26 = ~v48;
              goto LABEL_15;
            }
            v49 = (int)&v9[2 * v47];
            v50 = *(_BYTE *)v49;
            v9 = (_BYTE *)(v49 + 1);
            v28 = v50 & 1;
            v51 = v50 >> 1;
            if ( v28 )
              break;
            v28 = v51 & 1;
            v53 = v51 >> 1;
            if ( v28 )
              goto LABEL_38;
            v28 = v53 & 1;
            v52 = v53 >> 1;
            if ( !v28 )
              goto LABEL_62;
          }
          v28 = v51 & 1;
          v52 = v51 >> 1;
          if ( !v28 )
            goto LABEL_70;
          do
          {
LABEL_54:
            v54 = v52 + 1;
            v48 -= v54;
            if ( v48 < 0 )
            {
              v58 = v48 + v54;
              v26 = ~v48;
              v9 += v58;
              goto LABEL_21;
            }
            v55 = (int)&v9[v54];
            v56 = *(_BYTE *)v55;
            v9 = (_BYTE *)(v55 + 1);
            v28 = v56 & 1;
            v52 = v56 >> 1;
            if ( v28 )
              goto LABEL_46;
            v28 = v52 & 1;
            v57 = v52 >> 1;
            if ( !v28 )
            {
              v28 = v57 & 1;
              v52 = v57 >> 1;
              if ( v28 )
                goto LABEL_70;
              goto LABEL_38;
            }
            v28 = v57 & 1;
            v52 = v57 >> 1;
          }
          while ( v28 );
          while ( 1 )
          {
LABEL_62:
            v48 -= v52 + 1;
            if ( v48 < 0 )
            {
              v26 = ~v48;
              goto LABEL_27;
            }
            v59 = (int)(v9 + 1);
            v60 = *(_BYTE *)v59;
            v9 = (_BYTE *)(v59 + 1);
            v28 = v60 & 1;
            v61 = v60 >> 1;
            if ( v28 )
              break;
            v28 = v61 & 1;
            v62 = v61 >> 1;
            if ( v28 )
              goto LABEL_38;
            v28 = v62 & 1;
            v52 = v62 >> 1;
            if ( !v28 )
              goto LABEL_54;
          }
          v28 = v61 & 1;
          v52 = v61 >> 1;
          if ( !v28 )
            goto LABEL_46;
LABEL_70:
          while ( 1 )
          {
            v48 -= v52 + 1;
            if ( v48 < 0 )
              break;
            v63 = *v9++;
            v28 = v63 & 1;
            v52 = v63 >> 1;
            if ( v28 )
              goto LABEL_46;
            v28 = v52 & 1;
            v52 >>= 1;
            if ( v28 )
              goto LABEL_62;
            v28 = v52 & 1;
            v52 >>= 1;
            if ( !v28 )
              goto LABEL_54;
          }
          v26 = ~v48;
          goto LABEL_34;
        }
        v28 = v65 & 1;
        v52 = v65 >> 1;
        if ( v28 )
          goto LABEL_54;
      }
    }
    v28 = v46 & 1;
    v26 = v46 >> 1;
    if ( v28 )
      goto LABEL_27;
    while ( 1 )
    {
      do
      {
LABEL_15:
        v18 = *v9++;
        S = *v9++;
        //v20 = *(_BYTE *)P;
        //v21 = v18 >> 1 << 8;
        //v22 = (int)((char *)P + 1);
        //v23 = v21 | v20;
        //v24 = v21 | *(_BYTE *)v22;
        //v25 = (unsigned int)(unsigned __int16)(GlbFormat & LOWORD(Z[S])) >> 1;
        //*(P - 1) = LOWORD(PalA[v23]) + LOWORD(PalB[v24]) + v25;
        *Dst++ = R8G8B8A8(Pal[S*4+0],Pal[S*4+1],Pal[S*4+2],0xFF);
        --v26;
      }
      while ( (v26 & 0x80000000u) == 0 );
      v27 = *v9++;
      v28 = v27 & 1;
      v29 = v27 >> 1;
      if ( v28 )
        break;
      v28 = v29 & 1;
      v30 = v29 >> 1;
      if ( v28 )
        goto LABEL_38;
      v28 = v30 & 1;
      v26 = v30 >> 1;
      if ( !v28 )
        goto LABEL_27;
    }
    v28 = v29 & 1;
    v26 = v29 >> 1;
    if ( !v28 )
      goto LABEL_34;
    while ( 1 )
    {
      while ( 1 )
      {
        do
        {
LABEL_21:
          S = *v9++;
          //v32 = *P++;

          //*(P - 1) = ((GlbFormat & v32) >> 1) + ((unsigned int)(unsigned __int16)(GlbFormat & LOWORD(Z[v31])) >> 1);
          *Dst++ = R8G8B8A8(Pal[S*4+0],Pal[S*4+1],Pal[S*4+2],0xFF);
          --v26;
        }
        while ( (v26 & 0x80000000u) == 0 );
        v33 = *v9++;
        v28 = v33 & 1;
        v26 = v33 >> 1;
        if ( v28 )
          goto LABEL_15;
        v28 = v26 & 1;
        v34 = v26 >> 1;
        if ( v28 )
          break;
        v28 = v34 & 1;
        v26 = v34 >> 1;
        if ( !v28 )
          goto LABEL_38;
        do
        {
LABEL_34:
          Dst += v26 + 1;
          v44 = *v9++;
          v28 = v44 & 1;
          v26 = v44 >> 1;
          if ( v28 )
            goto LABEL_15;
          v28 = v26 & 1;
          v26 >>= 1;
          if ( v28 )
            goto LABEL_27;
          v28 = v26 & 1;
          v26 >>= 1;
        }
        while ( v28 );
      }
      v28 = v34 & 1;
      v26 = v34 >> 1;
      if ( !v28 )
      {
LABEL_27:
        while ( 1 )
        {
          S = v35 = *v9;
          v36 = (int)(v9 + 1);
          v37 = v35 << 8;
          do
          {
            //v38 = *(_BYTE *)P;
            //v39 = (int)((char *)P + 1);
            //v40 = PalB[v37 | *(_BYTE *)v39];
            //P = (_WORD *)(v39 + 1);
            //*(P - 1) = LOWORD(PalA[v37 | v38]) + v40;
             *Dst++ = R8G8B8A8(Pal[S*4+0],Pal[S*4+1],Pal[S*4+2],0xFF);
            --v26;
          }
          while ( (v26 & 0x80000000u) == 0 );
          v41 = *(_BYTE *)v36;
          v9 = (_BYTE *)(v36 + 1);
          v28 = v41 & 1;
          v42 = v41 >> 1;
          if ( v28 )
            break;
          v28 = v42 & 1;
          v43 = v42 >> 1;
          if ( v28 )
            goto LABEL_38;
          v28 = v43 & 1;
          v26 = v43 >> 1;
          if ( !v28 )
            goto LABEL_21;
        }
        v28 = v42 & 1;
        v26 = v42 >> 1;
        if ( !v28 )
          goto LABEL_15;
        goto LABEL_34;
      }
    }
  }
  return 1;
}

static int decode0x13C(pic *P, frame *F, u1 *Pal) {
  int S, R,G,B,A;
  int X, Y;
  _BYTE *Src; // esi@1
  char Bit; // cf@16
  u4 U; // ecx@39

  Src = (char *)&F[1] + 4 * F->H;

  times (Y, F->H) {
    X = 0;
    U = *Src++;
    Bit = U & 1;
    U = U >> 1;
    if ( Bit ) goto transparent;
    Bit = U & 1;
    U = U >> 1;
    if ( Bit ) {
      Bit = U & 1;
      U = U >> 1;
      if ( Bit ) goto shadow;
      while ( 1 ) {
        do {
blend:
          A = *Src++;
          S = *Src++;

          // following is used for antialiased border
#if 0
          v21 = A >> 1 << 8;
          v23 = v21 | Dst[0];
          v24 = v21 | Dst[1];
#endif
          //v25 = (unsigned int)(unsigned __int16)(GlbFormat & LOWORD(Z[S])) >> 1;
          //*(Dst - 1) = LOWORD(PalA[v23]) + LOWORD(PalB[v24]) + v25;
          picPut32(P,X++,Y,R8G8B8A8(Pal[S*4+0],Pal[S*4+1],Pal[S*4+2],0xFF));
          --U;
        } while ( (U & 0x80000000u) == 0 );
        U = *Src++;
        Bit = U & 1;
        U = U >> 1;
        if ( Bit ) break;
        Bit = U & 1;
        U = U >> 1;
        if ( Bit ) goto nextY;
        Bit = U & 1;
        U = U >> 1;
        if ( !Bit ) {
          while ( 1 ) {
            do {
shadow:
              picPut32(P,X++,Y,R8G8B8A8(0,0,0,0x80));
              --U;
            } while ( (U & 0x80000000u) == 0 );
            U = *Src++;
            Bit = U & 1;
            U = U >> 1;
            if ( Bit ) break;
            Bit = U & 1;
            U = U >> 1;
            if ( Bit ) goto nextY;
            Bit = U & 1;
            U = U >> 1;
            if ( !Bit ) goto simple;
          }
          Bit = U & 1;
          U = U >> 1;
          if ( !Bit ) goto blend;
          goto transparent;
        }
      }
      Bit = U & 1;
      U = U >> 1;
      if ( !Bit ) goto transparent;
    } else {
      Bit = U & 1;
      U = U >> 1;
      if ( !Bit ) goto nextY;
    }

    while ( 1 ) {
      while ( 1 ) {
        do {
simple:
          S = *Src++;
           picPut32(P,X++,Y,R8G8B8A8(Pal[S*4+0],Pal[S*4+1],Pal[S*4+2],0xFF));
          --U;
        } while ( (U & 0x80000000u) == 0 );
        U = *Src++;
        Bit = U & 1;
        U = U >> 1;
        if ( Bit ) goto blend;
        Bit = U & 1;
        U = U >> 1;
        if ( !Bit ) break;
        Bit = U & 1;
        U = U >> 1;
        if ( !Bit ) goto shadow;
      }
      Bit = U & 1;
      U = U >> 1;
      if ( !Bit ) break;
      do {
transparent:
        X += U + 1;
        U = *Src++;
        Bit = U & 1;
        U = U >> 1;
        if ( Bit ) goto blend;
        Bit = U & 1;
        U >>= 1;
        if ( Bit ) goto shadow;
        Bit = U & 1;
        U >>= 1;
      } while ( Bit );
    }
nextY:;
  }
  return 1;
}


#define BUFSZ (1024*1024*5)
static void bmcDecompile(char *Output, char *Input) {
  char Tmp[1024];
  int I, J, X, Y, Off, Len, Wide, High, R, G, B, A, T, C, U, UU;
  int L = fileSize(Input);
  u1 *Q;
  pic *P;
  u1 *M = readFile(0, L, Input);
  header *H = (header*)M;
  u4 *O = (u4*)(H+1);
  int NFrames = *O++;
  u4 *Ys;
  u1 Pal[1024];
  frame *F;
  clip Clip;
  base Base;
  u1 *Buf = ns(u1, BUFSZ); //should be enough

  u1 Junk[8];
  memset(Junk, 0, 8);
  SprDll_Init("PentiumII", Junk, 0);

  if (H->Type&1) {
    printf("  ignoring this SPR\n");
    abort();
  }

  if (H->Type&0xC) { //8-bit image
    Q = (u1*)(H+1);
    if (H->Type&8) {
      times (J, 256) {
        Pal[J*4+0] = Q[J*4+2];
        Pal[J*4+1] = Q[J*4+1];
        Pal[J*4+2] = Q[J*4+0];
        Pal[J*4+3] = 0;
      }
      O = (u4*)(Q+256*4);
    } else {
      times (J, 256) {
        fromA1R5G5B5(A,R,G,B,ru2(Q));
        Pal[J*4+0] = R;
        Pal[J*4+1] = G;
        Pal[J*4+2] = B;
        Pal[J*4+3] = 0;
      }
      O = (u4*)Q;
    }
    NFrames = *O++;
  }

  T = (H->Type&0x3FF)>>1;
  printf("  NFrames=%d Type=%04X (#%x %d)\n", NFrames, H->Type, T, TypeLUT[T]);

  times (I, NFrames) {
    F = (frame*)((u1*)O + O[I] + ((H->Type&0x800) ? 4 : 0));
    Ys = (u4*)(F+1);

    printf("  %dx%d\n", F->W, F->H);

    P = picNew(F->W, F->H, 32);

    memset(Buf, 0xFF, BUFSZ);
    memset(&Clip, 0, sizeof(clip));
    memset(&Base, 0, sizeof(base));
    // this should point to the center of sprite
    Base.X = F->W/2;
    Base.Y = F->H/2;
    Clip.Data = Buf;
    Clip.W = F->W;
    Clip.SX = 0;
    Clip.SY = 0;
    Clip.EX = F->W;
    Clip.EY = F->H;

    if (T == 0x58) { //16-bit
      times (Y, F->H) {
        Off = (u1*)(Ys+F->H+1) + Ys[Y] - M;
        //printf("  %08X\n", Off);
        Q = M+Off;
        times (X, F->W) {
          fromA1R5G5B5(A,R,G,B,ru2(Q));
          picPut32(P, X, Y, R8G8B8A8(R,G,B,0xFF));
        }
      }
    } else if (T == 0x3C) {
      decode0x3C(Pal, &Base, &Clip, F);
      memcpy(P->D, Clip.Data, 4*F->W*F->H);
    } else if (T == 0x13C) { //8-bit compressed
      decode0x13C(P, F, Pal);
    } else {
      printf("  unsupported SPR type (#%X)\n", T);
      abort();
    }
    sprintf(Tmp, "%s/%04d.png", Output, I);
    pngSave(Tmp, P);
    picDel(P);
  }
}


int bmcInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "bmc";
  F->Description = "Black Moon Chronicles, Persian Wars (use on *.SPR files)";
  F->Decompile = bmcDecompile;
  return 1;
}

