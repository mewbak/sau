//Credits go to Imagination Technologies 

#include "common.h"


typedef struct {
  u4 Size;          // size of the structure
  u4 H;             // height of surface to be created
  u4 W;             // width of input surface
  u4 Mips;          // number of mip-map levels requested
  u4 Flags;         // pixel format flags
  u4 DataSize;      // Total size of texture data in bytes
  u4 Bits;          // number of bits per pixel
  u4 RMask;         // mask for red bit
  u4 GMask;         // mask for green bits
  u4 BMask;         // mask for blue bits
  u4 AMask;         // mask for alpha channel
  u4 PVR;           // magic number identifying pvr file
  u4 NumSurfs;      // the number of surfaces present in the pvr
} __attribute__ ((packed)) header;


enum PixelType
{
  MGLPT_ARGB_4444 = 0x00,
  MGLPT_ARGB_1555,
  MGLPT_RGB_565,
  MGLPT_RGB_555,
  MGLPT_RGB_888,
  MGLPT_ARGB_8888,
  MGLPT_ARGB_8332,
  MGLPT_I_8,
  MGLPT_AI_88,
  MGLPT_1_BPP,
  MGLPT_VY1UY0,
  MGLPT_Y1VY0U,
  MGLPT_PVRTC2,
  MGLPT_PVRTC4,
  MGLPT_PVRTC2_2,
  MGLPT_PVRTC2_4,

  OGL_RGBA_4444 = 0x10,
  OGL_RGBA_5551,
  OGL_RGBA_8888,
  OGL_RGB_565,
  OGL_RGB_555,
  OGL_RGB_888,
  OGL_I_8,
  OGL_AI_88,
  OGL_PVRTC2,
  OGL_PVRTC4,

  // OGL_BGRA_8888 extension
  OGL_BGRA_8888,
  OGL_A_8,

  D3D_DXT1 = 0x20,
  D3D_DXT2,
  D3D_DXT3,
  D3D_DXT4,
  D3D_DXT5,

  D3D_RGB_332,
  D3D_AI_44,
  D3D_LVU_655,
  D3D_XLVU_8888,
  D3D_QWVU_8888,

  //10 bits per channel
  D3D_ABGR_2101010,
  D3D_ARGB_2101010,
  D3D_AWVU_2101010,

  //16 bits per channel
  D3D_GR_1616,
  D3D_VU_1616,
  D3D_ABGR_16161616,

  //HDR formats
  D3D_R16F,
  D3D_GR_1616F,
  D3D_ABGR_16161616F,

  //32 bits per channel
  D3D_R32F,
  D3D_GR_3232F,
  D3D_ABGR_32323232F,

  // Ericsson
  ETC_RGB_4BPP,
  ETC_RGBA_EXPLICIT,
  ETC_RGBA_INTERPOLATED,

  // DX10
  ePT_DX10_R32G32B32A32_FLOAT= 0x50,
  ePT_DX10_R32G32B32A32_UINT ,
  ePT_DX10_R32G32B32A32_SINT,

  ePT_DX10_R32G32B32_FLOAT,
  ePT_DX10_R32G32B32_UINT,
  ePT_DX10_R32G32B32_SINT,

  ePT_DX10_R16G16B16A16_FLOAT ,
  ePT_DX10_R16G16B16A16_UNORM,
  ePT_DX10_R16G16B16A16_UINT ,
  ePT_DX10_R16G16B16A16_SNORM ,
  ePT_DX10_R16G16B16A16_SINT ,

  ePT_DX10_R32G32_FLOAT ,
  ePT_DX10_R32G32_UINT ,
  ePT_DX10_R32G32_SINT ,

  ePT_DX10_R10G10B10A2_UNORM ,
  ePT_DX10_R10G10B10A2_UINT ,

  ePT_DX10_R11G11B10_FLOAT ,

  ePT_DX10_R8G8B8A8_UNORM ,
  ePT_DX10_R8G8B8A8_UNORM_SRGB ,
  ePT_DX10_R8G8B8A8_UINT ,
  ePT_DX10_R8G8B8A8_SNORM ,
  ePT_DX10_R8G8B8A8_SINT ,

  ePT_DX10_R16G16_FLOAT ,
  ePT_DX10_R16G16_UNORM ,
  ePT_DX10_R16G16_UINT ,
  ePT_DX10_R16G16_SNORM ,
  ePT_DX10_R16G16_SINT ,

  ePT_DX10_R32_FLOAT ,
  ePT_DX10_R32_UINT ,
  ePT_DX10_R32_SINT ,

  ePT_DX10_R8G8_UNORM ,
  ePT_DX10_R8G8_UINT ,
  ePT_DX10_R8G8_SNORM ,
  ePT_DX10_R8G8_SINT ,

  ePT_DX10_R16_FLOAT ,
  ePT_DX10_R16_UNORM ,
  ePT_DX10_R16_UINT ,
  ePT_DX10_R16_SNORM ,
  ePT_DX10_R16_SINT ,

  ePT_DX10_R8_UNORM,
  ePT_DX10_R8_UINT,
  ePT_DX10_R8_SNORM,
  ePT_DX10_R8_SINT,

  ePT_DX10_A8_UNORM,
  ePT_DX10_R1_UNORM,
  ePT_DX10_R9G9B9E5_SHAREDEXP,
  ePT_DX10_R8G8_B8G8_UNORM,
  ePT_DX10_G8R8_G8B8_UNORM,

  ePT_DX10_BC1_UNORM,
  ePT_DX10_BC1_UNORM_SRGB,

  ePT_DX10_BC2_UNORM,
  ePT_DX10_BC2_UNORM_SRGB,

  ePT_DX10_BC3_UNORM,
  ePT_DX10_BC3_UNORM_SRGB,

  ePT_DX10_BC4_UNORM,
  ePT_DX10_BC4_SNORM,

  ePT_DX10_BC5_UNORM,
  ePT_DX10_BC5_SNORM,

  ePT_DX10_B5G6R5_UNORM,      // defined but obsolete - won't actually load in DX10
  ePT_DX10_B5G5R5A1_UNORM,
  ePT_DX10_B8G8R8A8_UNORM,
  ePT_DX10_B8G8R8X8_UNORM,

  // OpenVG

  /* RGB{A,X} channel ordering */
  ePT_VG_sRGBX_8888  = 0x90,
  ePT_VG_sRGBA_8888,
  ePT_VG_sRGBA_8888_PRE,
  ePT_VG_sRGB_565,
  ePT_VG_sRGBA_5551,
  ePT_VG_sRGBA_4444,
  ePT_VG_sL_8,
  ePT_VG_lRGBX_8888,
  ePT_VG_lRGBA_8888,
  ePT_VG_lRGBA_8888_PRE,
  ePT_VG_lL_8,
  ePT_VG_A_8,
  ePT_VG_BW_1,

  /* {A,X}RGB channel ordering */
  ePT_VG_sXRGB_8888,
  ePT_VG_sARGB_8888,
  ePT_VG_sARGB_8888_PRE,
  ePT_VG_sARGB_1555,
  ePT_VG_sARGB_4444,
  ePT_VG_lXRGB_8888,
  ePT_VG_lARGB_8888,
  ePT_VG_lARGB_8888_PRE,

  /* BGR{A,X} channel ordering */
  ePT_VG_sBGRX_8888,
  ePT_VG_sBGRA_8888,
  ePT_VG_sBGRA_8888_PRE,
  ePT_VG_sBGR_565,
  ePT_VG_sBGRA_5551,
  ePT_VG_sBGRA_4444,
  ePT_VG_lBGRX_8888,
  ePT_VG_lBGRA_8888,
  ePT_VG_lBGRA_8888_PRE,

  /* {A,X}BGR channel ordering */
  ePT_VG_sXBGR_8888,
  ePT_VG_sABGR_8888 ,
  ePT_VG_sABGR_8888_PRE,
  ePT_VG_sABGR_1555,
  ePT_VG_sABGR_4444,
  ePT_VG_lXBGR_8888,
  ePT_VG_lABGR_8888,
  ePT_VG_lABGR_8888_PRE,

  // max cap for iterating
  END_OF_PIXEL_TYPES,

  MGLPT_NOTYPE = 0xff
};

#define PVRTEX_MIPMAP         (1<< 8)    /* has mip map levels */
#define PVRTEX_TWIDDLE        (1<< 9)    /* is twiddled */
#define PVRTEX_BUMPMAP        (1<<10)    /* has normals encoded for a bump map */
#define PVRTEX_TILING         (1<<11)    /* is bordered for tiled pvr */
#define PVRTEX_CUBEMAP        (1<<12)    /* is a cubemap/skybox */
#define PVRTEX_FALSEMIPCOL    (1<<13)    /* are there false coloured MIP levels */
#define PVRTEX_VOLUME         (1<<14)    /* is this a volume texture */
#define PVRTEX_ALPHA          (1<<15)    /* v2.1 is there transparency info in the texture */
#define PVRTEX_VERTICAL_FLIP  (1<<16)    /* v2.1 is the texture vertically flipped */

#define PVRTEX_PIXELTYPE       0xff        /*pixel type is always in the last 16bits of the flags*/
#define PVRTEX_IDENTIFIER      0x21525650  /*the pvr identifier is the characters 'P','V','R'*/

#define PVRTEX_V1_HEADER_SIZE  44          /* old header size was 44 for identification purposes */

#define PVRTC2_MIN_TEXWIDTH    16
#define PVRTC2_MIN_TEXHEIGHT   8
#define PVRTC4_MIN_TEXWIDTH    8
#define PVRTC4_MIN_TEXHEIGHT   8
#define ETC_MIN_TEXWIDTH       4
#define ETC_MIN_TEXHEIGHT      4
#define DXT_MIN_TEXWIDTH       4
#define DXT_MIN_TEXHEIGHT      4

#define PT_INDEX    2  /* The Punch-through index */
#define BLK_Y_SIZE  4  /* always 4 for all 2D block types */
#define BLK_X_MAX   8  /*  Max X dimension for blocks */
#define BLK_X_2BPP  8  /* dimensions for the two formats */
#define BLK_X_4BPP  4

#define WRAP_COORD(Val, Size) ((Val) & ((Size)-1))
#define POWER_OF_2(X)   util_number_is_power_2(X)


#define PVRT_MIN(a,b)            (((a) < (b)) ? (a) : (b))
#define PVRT_MAX(a,b)            (((a) > (b)) ? (a) : (b))
#define PVRT_CLAMP(x, l, h)      (PVRT_MIN((h), PVRT_MAX((x), (l))))

//  either wrap or clamp large or small vals to the legal range
#define LIMIT_COORD(Val, Size, AssumeImageTiles) \
      ((AssumeImageTiles)? WRAP_COORD((Val), (Size)): PVRT_CLAMP((Val), 0, (Size)-1))


typedef struct {u4 PackedData[2];} AMTC_BLOCK_STRUCT; // Uses 64 bits pre block


static void Decompress(AMTC_BLOCK_STRUCT *pCompressedData,
             int Do2bitMode,
             int XDim,
             int YDim,
             int AssumeImageTiles,
             u1* pResultImage);

/*!***********************************************************************
 @Function    PVRTDecompressPVRTC
 @Input      pCompressedData The PVRTC texture data to decompress
 @Input      Do2bitMode Signifies whether the data is PVRTC2 or PVRTC4
 @Input      XDim X dimension of the texture
 @Input      YDim Y dimension of the texture
 @Modified    pResultImage The decompressed texture data
 @Description  Decompresses PVRTC to RGBA 8888
*************************************************************************/
void PVRTDecompressPVRTC(void *pCompressedData, int Do2bitMode, int XDim, int YDim, u1* pResultImage)
{
  Decompress((AMTC_BLOCK_STRUCT*)pCompressedData,Do2bitMode,XDim,YDim,1,pResultImage);
}

 /*!***********************************************************************
  @Function    util_number_is_power_2
  @Input    input A number
  @Returns    TRUE if the number is an integer power of two, else FALSE.
  @Description  Check that a number is an integer power of two, i.e.
               1, 2, 4, 8, ... etc.
        Returns FALSE for zero.
*************************************************************************/
int util_number_is_power_2( unsigned  input )
{
  unsigned minus1;

  if( !input ) return 0;

  minus1 = input - 1;
  return ( (input | minus1) == (input ^ minus1) ) ? 1 : 0;
}


/*!***********************************************************************
 @Function    Unpack5554Colour
 @Input      pBlock
 @Input      ABColours
 @Description  Given a block, extract the colour information and convert
         to 5554 formats
*************************************************************************/
static void Unpack5554Colour(AMTC_BLOCK_STRUCT *pBlock, int ABColours[2][4])
{
  u4 RawBits[2];

  int i;

  // Extract A and B
  RawBits[0] = pBlock->PackedData[1] & (0xFFFE); // 15 bits (shifted up by one)
  RawBits[1] = pBlock->PackedData[1] >> 16;     // 16 bits

  // step through both colours
  for(i = 0; i < 2; i++)
  {
    // If completely opaque
    if(RawBits[i] & (1<<15))
    {
      // Extract R and G (both 5 bit)
      ABColours[i][0] = (RawBits[i] >> 10) & 0x1F;
      ABColours[i][1] = (RawBits[i] >>  5) & 0x1F;

      /*
        The precision of Blue depends on  A or B. If A then we need to
        replicate the top bit to get 5 bits in total
      */
      ABColours[i][2] = RawBits[i] & 0x1F;
      if(i==0)
      {
        ABColours[0][2] |= ABColours[0][2] >> 4;
      }

      // set 4bit alpha fully on...
      ABColours[i][3] = 0xF;
    }
    else // Else if colour has variable translucency
    {
      /*
        Extract R and G (both 4 bit).
        (Leave a space on the end for the replication of bits
      */
      ABColours[i][0] = (RawBits[i] >>  (8-1)) & 0x1E;
      ABColours[i][1] = (RawBits[i] >>  (4-1)) & 0x1E;

      // replicate bits to truly expand to 5 bits
      ABColours[i][0] |= ABColours[i][0] >> 4;
      ABColours[i][1] |= ABColours[i][1] >> 4;

      // grab the 3(+padding) or 4 bits of blue and add an extra padding bit
      ABColours[i][2] = (RawBits[i] & 0xF) << 1;

      /*
        expand from 3 to 5 bits if this is from colour A, or 4 to 5 bits if from
        colour B
      */
      if(i==0)
      {
        ABColours[0][2] |= ABColours[0][2] >> 3;
      }
      else
      {
        ABColours[0][2] |= ABColours[0][2] >> 4;
      }

      // Set the alpha bits to be 3 + a zero on the end
      ABColours[i][3] = (RawBits[i] >> 11) & 0xE;
    }
  }
}

/*!***********************************************************************
 @Function    UnpackModulations
 @Input      pBlock
 @Input      Do2bitMode
 @Input      ModulationVals
 @Input      ModulationModes
 @Input      StartX
 @Input      StartY
 @Description  Given the block and the texture type and it's relative
         position in the 2x2 group of blocks, extract the bit
         patterns for the fully defined pixels.
*************************************************************************/
static void  UnpackModulations(AMTC_BLOCK_STRUCT *pBlock,
                int Do2bitMode,
                int ModulationVals[8][16],
                int ModulationModes[8][16],
                int StartX,
                int StartY)
{
  int BlockModMode;
  u4 ModulationBits;

  int x, y;

  BlockModMode= pBlock->PackedData[1] & 1;
  ModulationBits  = pBlock->PackedData[0];

  // if it's in an interpolated mode
  if(Do2bitMode && BlockModMode)
  {
    /*
      run through all the pixels in the block. Note we can now treat all the
      "stored" values as if they have 2bits (even when they didn't!)
    */
    for(y = 0; y < BLK_Y_SIZE; y++)
    {
      for(x = 0; x < BLK_X_2BPP; x++)
      {
        ModulationModes[y+StartY][x+StartX] = BlockModMode;

        // if this is a stored value...
        if(((x^y)&1) == 0)
        {
          ModulationVals[y+StartY][x+StartX] = ModulationBits & 3;
          ModulationBits >>= 2;
        }
      }
    }
  }
  else if(Do2bitMode) // else if direct encoded 2bit mode - i.e. 1 mode bit per pixel
  {
    for(y = 0; y < BLK_Y_SIZE; y++)
    {
      for(x = 0; x < BLK_X_2BPP; x++)
      {
        ModulationModes[y+StartY][x+StartX] = BlockModMode;

        // double the bits so 0=> 00, and 1=>11
        if(ModulationBits & 1)
        {
          ModulationVals[y+StartY][x+StartX] = 0x3;
        }
        else
        {
          ModulationVals[y+StartY][x+StartX] = 0x0;
        }
        ModulationBits >>= 1;
      }
    }
  }
  else // else its the 4bpp mode so each value has 2 bits
  {
    for(y = 0; y < BLK_Y_SIZE; y++)
    {
      for(x = 0; x < BLK_X_4BPP; x++)
      {
        ModulationModes[y+StartY][x+StartX] = BlockModMode;

        ModulationVals[y+StartY][x+StartX] = ModulationBits & 3;
        ModulationBits >>= 2;
      }
    }
  }

  // make sure nothing is left over
  assert(ModulationBits==0);
}

/*!***********************************************************************
 @Function    InterpolateColours
 @Input      ColourP
 @Input      ColourQ
 @Input      ColourR
 @Input      ColourS
 @Input      Do2bitMode
 @Input      x
 @Input      y
 @Modified    Result
 @Description  This performs a HW bit accurate interpolation of either the
        A or B colours for a particular pixel.

        NOTE: It is assumed that the source colours are in ARGB 5554
        format - This means that some "preparation" of the values will
        be necessary.
*************************************************************************/
static void InterpolateColours(int ColourP[4],
              int ColourQ[4],
              int ColourR[4],
              int ColourS[4],
              int Do2bitMode,
              int x,
              int y,
              int Result[4])
{
  int u, v, uscale;
  int k;

  int tmp1, tmp2;

  int P[4], Q[4], R[4], S[4];

  // Copy the colours
  for(k = 0; k < 4; k++)
  {
    P[k] = ColourP[k];
    Q[k] = ColourQ[k];
    R[k] = ColourR[k];
    S[k] = ColourS[k];
  }

  // put the x and y values into the right range
  v = (y & 0x3) | ((~y & 0x2) << 1);

  if(Do2bitMode)
    u = (x & 0x7) | ((~x & 0x4) << 1);
  else
    u = (x & 0x3) | ((~x & 0x2) << 1);

  // get the u and v scale amounts
  v  = v - BLK_Y_SIZE/2;

  if(Do2bitMode)
  {
    u = u - BLK_X_2BPP/2;
    uscale = 8;
  }
  else
  {
    u = u - BLK_X_4BPP/2;
    uscale = 4;
  }

  for(k = 0; k < 4; k++)
  {
    tmp1 = P[k] * uscale + u * (Q[k] - P[k]);
    tmp2 = R[k] * uscale + u * (S[k] - R[k]);

    tmp1 = tmp1 * 4 + v * (tmp2 - tmp1);

    Result[k] = tmp1;
  }

  // Lop off the appropriate number of bits to get us to 8 bit precision
  if(Do2bitMode)
  {
    // do RGB
    for(k = 0; k < 3; k++)
    {
      Result[k] >>= 2;
    }

    Result[3] >>= 1;
  }
  else
  {
    // do RGB  (A is ok)
    for(k = 0; k < 3; k++)
    {
      Result[k] >>= 1;
    }
  }

  // sanity check
  for(k = 0; k < 4; k++)
  {
    assert(Result[k] < 256);
  }


  /*
    Convert from 5554 to 8888

    do RGB 5.3 => 8
  */
  for(k = 0; k < 3; k++)
  {
    Result[k] += Result[k] >> 5;
  }

  Result[3] += Result[3] >> 4;

  // 2nd sanity check
  for(k = 0; k < 4; k++)
  {
    assert(Result[k] < 256);
  }

}

/*!***********************************************************************
 @Function    GetModulationValue
 @Input      x
 @Input      y
 @Input      Do2bitMode
 @Input      ModulationVals
 @Input      ModulationModes
 @Input      Mod
 @Input      DoPT
 @Description  Get the modulation value as a numerator of a fraction of 8ths
*************************************************************************/
static void GetModulationValue(int x,
                 int y,
                 int Do2bitMode,
                 int ModulationVals[8][16],
                 int ModulationModes[8][16],
                 int *Mod,
                 int *DoPT)
{
  static int RepVals0[4] = {0, 3, 5, 8};
  static int RepVals1[4] = {0, 4, 4, 8};

  int ModVal;

  // Map X and Y into the local 2x2 block
  y = (y & 0x3) | ((~y & 0x2) << 1);

  if(Do2bitMode)
    x = (x & 0x7) | ((~x & 0x4) << 1);
  else
    x = (x & 0x3) | ((~x & 0x2) << 1);

  // assume no PT for now
  *DoPT = 0;

  // extract the modulation value. If a simple encoding
  if(ModulationModes[y][x]==0)
  {
    ModVal = RepVals0[ModulationVals[y][x]];
  }
  else if(Do2bitMode)
  {
    // if this is a stored value
    if(((x^y)&1)==0)
      ModVal = RepVals0[ModulationVals[y][x]];
    else if(ModulationModes[y][x] == 1) // else average from the neighbours if H&V interpolation..
    {
      ModVal = (RepVals0[ModulationVals[y-1][x]] +
            RepVals0[ModulationVals[y+1][x]] +
            RepVals0[ModulationVals[y][x-1]] +
            RepVals0[ModulationVals[y][x+1]] + 2) / 4;
    }
    else if(ModulationModes[y][x] == 2) // else if H-Only
    {
      ModVal = (RepVals0[ModulationVals[y][x-1]] +
            RepVals0[ModulationVals[y][x+1]] + 1) / 2;
    }
    else // else it's V-Only
    {
      ModVal = (RepVals0[ModulationVals[y-1][x]] +
            RepVals0[ModulationVals[y+1][x]] + 1) / 2;
    }
  }
  else // else it's 4BPP and PT encoding
  {
    ModVal = RepVals1[ModulationVals[y][x]];

    *DoPT = ModulationVals[y][x] == PT_INDEX;
  }

  *Mod =ModVal;
}

/*!***********************************************************************
 @Function    TwiddleUV
 @Input      YSize  Y dimension of the texture in pixels
 @Input      XSize  X dimension of the texture in pixels
 @Input      YPos  Pixel Y position
 @Input      XPos  Pixel X position
 @Returns    The twiddled offset of the pixel
 @Description  Given the Block (or pixel) coordinates and the dimension of
         the texture in blocks (or pixels) this returns the twiddled
         offset of the block (or pixel) from the start of the map.

        NOTE the dimensions of the texture must be a power of 2
*************************************************************************/
static int DisableTwiddlingRoutine = 0;

static u4 TwiddleUV(u4 YSize, u4 XSize, u4 YPos, u4 XPos)
{
  u4 Twiddled;

  u4 MinDimension;
  u4 MaxValue;

  u4 SrcBitPos;
  u4 DstBitPos;

  int ShiftCount;

  assert(YPos < YSize);
  assert(XPos < XSize);

  assert(POWER_OF_2(YSize));
  assert(POWER_OF_2(XSize));

  if(YSize < XSize)
  {
    MinDimension = YSize;
    MaxValue   = XPos;
  }
  else
  {
    MinDimension = XSize;
    MaxValue   = YPos;
  }

  // Nasty hack to disable twiddling
  if(DisableTwiddlingRoutine)
    return (YPos* XSize + XPos);

  // Step through all the bits in the "minimum" dimension
  SrcBitPos = 1;
  DstBitPos = 1;
  Twiddled  = 0;
  ShiftCount = 0;

  while(SrcBitPos < MinDimension)
  {
    if(YPos & SrcBitPos)
    {
      Twiddled |= DstBitPos;
    }

    if(XPos & SrcBitPos)
    {
      Twiddled |= (DstBitPos << 1);
    }


    SrcBitPos <<= 1;
    DstBitPos <<= 2;
    ShiftCount += 1;

  }

  // prepend any unused bits
  MaxValue >>= ShiftCount;

  Twiddled |=  (MaxValue << (2*ShiftCount));

  return Twiddled;
}

/*!***********************************************************************
 @Function    Decompress
 @Input      pCompressedData The PVRTC texture data to decompress
 @Input      Do2BitMode Signifies whether the data is PVRTC2 or PVRTC4
 @Input      XDim X dimension of the texture
 @Input      YDim Y dimension of the texture
 @Input      AssumeImageTiles Assume the texture data tiles
 @Modified    pResultImage The decompressed texture data
 @Description  Decompresses PVRTC to RGBA 8888
*************************************************************************/
static void Decompress(AMTC_BLOCK_STRUCT *pCompressedData,
        int Do2bitMode,
        int XDim,
        int YDim,
        int AssumeImageTiles,
        u1* pResultImage)
{
  int x, y;
  int i, j;

  int BlkX, BlkY;
  int BlkXp1, BlkYp1;
  int XBlockSize;
  int BlkXDim, BlkYDim;

  int StartX, StartY;

  int ModulationVals[8][16];
  int ModulationModes[8][16];

  int Mod, DoPT;

  unsigned int uPosition;

  // local neighbourhood of blocks
  AMTC_BLOCK_STRUCT *pBlocks[2][2];

  AMTC_BLOCK_STRUCT *pPrevious[2][2] = {{NULL, NULL}, {NULL, NULL}};

  // Low precision colours extracted from the blocks
  struct
  {
    int Reps[2][4];
  }Colours5554[2][2];

  // Interpolated A and B colours for the pixel
  int ASig[4], BSig[4];

  int Result[4];

  if(Do2bitMode)
    XBlockSize = BLK_X_2BPP;
  else
    XBlockSize = BLK_X_4BPP;

  // For MBX don't allow the sizes to get too small
  BlkXDim = PVRT_MAX(2, XDim / XBlockSize);
  BlkYDim = PVRT_MAX(2, YDim / BLK_Y_SIZE);

  /*
    Step through the pixels of the image decompressing each one in turn

    Note that this is a hideously inefficient way to do this!
  */
  for(y = 0; y < YDim; y++)
  {
    for(x = 0; x < XDim; x++)
    {
      // map this pixel to the top left neighbourhood of blocks
      BlkX = (x - XBlockSize/2);
      BlkY = (y - BLK_Y_SIZE/2);

      BlkX = LIMIT_COORD(BlkX, XDim, AssumeImageTiles);
      BlkY = LIMIT_COORD(BlkY, YDim, AssumeImageTiles);


      BlkX /= XBlockSize;
      BlkY /= BLK_Y_SIZE;

      // compute the positions of the other 3 blocks
      BlkXp1 = LIMIT_COORD(BlkX+1, BlkXDim, AssumeImageTiles);
      BlkYp1 = LIMIT_COORD(BlkY+1, BlkYDim, AssumeImageTiles);

      // Map to block memory locations
      pBlocks[0][0] = pCompressedData +TwiddleUV(BlkYDim, BlkXDim, BlkY, BlkX);
      pBlocks[0][1] = pCompressedData +TwiddleUV(BlkYDim, BlkXDim, BlkY, BlkXp1);
      pBlocks[1][0] = pCompressedData +TwiddleUV(BlkYDim, BlkXDim, BlkYp1, BlkX);
      pBlocks[1][1] = pCompressedData +TwiddleUV(BlkYDim, BlkXDim, BlkYp1, BlkXp1);


      /*
        extract the colours and the modulation information IF the previous values
        have changed.
      */
      if(memcmp(pPrevious, pBlocks, 4*sizeof(void*)) != 0)
      {
        StartY = 0;
        for(i = 0; i < 2; i++)
        {
          StartX = 0;
          for(j = 0; j < 2; j++)
          {
            Unpack5554Colour(pBlocks[i][j], Colours5554[i][j].Reps);

            UnpackModulations(pBlocks[i][j],
                      Do2bitMode,
                      ModulationVals,
                      ModulationModes,
                      StartX, StartY);

            StartX += XBlockSize;
          }

          StartY += BLK_Y_SIZE;
        }

        // make a copy of the new pointers
        memcpy(pPrevious, pBlocks, 4*sizeof(void*));
      }

      // decompress the pixel.  First compute the interpolated A and B signals
      InterpolateColours(Colours5554[0][0].Reps[0],
                 Colours5554[0][1].Reps[0],
                 Colours5554[1][0].Reps[0],
                 Colours5554[1][1].Reps[0],
                 Do2bitMode, x, y,
                 ASig);

      InterpolateColours(Colours5554[0][0].Reps[1],
                 Colours5554[0][1].Reps[1],
                 Colours5554[1][0].Reps[1],
                 Colours5554[1][1].Reps[1],
                 Do2bitMode, x, y,
                 BSig);

      GetModulationValue(x,y, Do2bitMode, (int (*)[16])ModulationVals, (int (*)[16])ModulationModes,
                 &Mod, &DoPT);

      // compute the modulated colour
      for(i = 0; i < 4; i++)
      {
        Result[i] = ASig[i] * 8 + Mod * (BSig[i] - ASig[i]);
        Result[i] >>= 3;
      }

      if(DoPT)
        Result[3] = 0;

      // Store the result in the output image
      uPosition = (x+y*XDim)<<2;
      pResultImage[uPosition+0] = (u1)Result[0];
      pResultImage[uPosition+1] = (u1)Result[1];
      pResultImage[uPosition+2] = (u1)Result[2];
      pResultImage[uPosition+3] = (u1)Result[3];
    }
  }
}

/****************************
**  ETC Compression
****************************/

/*****************************************************************************
Macros
*****************************************************************************/
#define _CLAMP_(X,Xmin,Xmax) (  (X)<(Xmax) ?  (  (X)<(Xmin)?(Xmin):(X)  )  : (Xmax)    )

/*****************************************************************************
Constants
******************************************************************************/
static unsigned int ETC_FLIP =  0x01000000;
static unsigned int ETC_DIFF = 0x02000000;
static int mod[8][4]={{2, 8,-2,-8},
          {5, 17, -5, -17},
          {9, 29, -9, -29},
          {13, 42, -13, -42},
          {18, 60, -18, -60},
          {24, 80, -24, -80},
          {33, 106, -33, -106},
          {47, 183, -47, -183}};

 /*!***********************************************************************
 @Function    modifyPixel
 @Input      red    Red value of pixel
 @Input      green  Green value of pixel
 @Input      blue  Blue value of pixel
 @Input      x  Pixel x position in block
 @Input      y  Pixel y position in block
 @Input      modBlock  Values for the current block
 @Input      modTable  Modulation values
 @Returns    Returns actual pixel colour
 @Description  Used by ETCTextureDecompress
*************************************************************************/
unsigned long modifyPixel(int red, int green, int blue, int x, int y, unsigned long modBlock, int modTable)
{
  int index = x*4+y, pixelMod;
  unsigned long mostSig = modBlock<<1;

  if (index<8)
    pixelMod = mod[modTable][((modBlock>>(index+24))&0x1)+((mostSig>>(index+8))&0x2)];
  else
    pixelMod = mod[modTable][((modBlock>>(index+8))&0x1)+((mostSig>>(index-8))&0x2)];

  red = _CLAMP_(red+pixelMod,0,255);
  green = _CLAMP_(green+pixelMod,0,255);
  blue = _CLAMP_(blue+pixelMod,0,255);

  return ((red<<16) + (green<<8) + blue)|0xff000000;
}

 /*!***********************************************************************
 @Function    ETCTextureDecompress
 @Input      pSrcData The ETC texture data to decompress
 @Input      x X dimension of the texture
 @Input      y Y dimension of the texture
 @Modified    pDestData The decompressed texture data
 @Input      nMode The format of the data
 @Returns    The number of bytes of ETC data decompressed
 @Description  Decompresses ETC to RGBA 8888
*************************************************************************/
int ETCTextureDecompress(void *pSrcData, int x, int y, void *pDestData, int nMode)
{
  unsigned long blockTop, blockBot, *input = (unsigned long*)pSrcData, *output;
  unsigned char red1, green1, blue1, red2, green2, blue2;
  int i, j, k,  m, bFlip, bDiff, modtable1, modtable2;

  for(i=0;i<y;i+=4)
  {
    for(m=0;m<x;m+=4)
    {
        blockTop = *(input++);
        blockBot = *(input++);

      output = (unsigned long*)pDestData + i*x +m;

      // check flipbit
      bFlip = (blockTop & ETC_FLIP) != 0;
      bDiff = (blockTop & ETC_DIFF) != 0;

      if(bDiff)
      {  // differential mode 5 colour bits + 3 difference bits
        // get base colour for subblock 1
        blue1 = (unsigned char)((blockTop&0xf80000)>>16);
        green1 = (unsigned char)((blockTop&0xf800)>>8);
        red1 = (unsigned char)(blockTop&0xf8);

        // get differential colour for subblock 2
        signed char blues = (signed char)(blue1>>3) + ((signed char) ((blockTop & 0x70000) >> 11)>>5);
        signed char greens = (signed char)(green1>>3) + ((signed char)((blockTop & 0x700) >>3)>>5);
        signed char reds = (signed char)(red1>>3) + ((signed char)((blockTop & 0x7)<<5)>>5);

        blue2 = (unsigned char)blues;
        green2 = (unsigned char)greens;
        red2 = (unsigned char)reds;

        red1 = red1 +(red1>>5);  // copy bits to lower sig
        green1 = green1 + (green1>>5);  // copy bits to lower sig
        blue1 = blue1 + (blue1>>5);  // copy bits to lower sig

        red2 = (red2<<3) +(red2>>2);  // copy bits to lower sig
        green2 = (green2<<3) + (green2>>2);  // copy bits to lower sig
        blue2 = (blue2<<3) + (blue2>>2);  // copy bits to lower sig
      }
      else
      {  // individual mode 4 + 4 colour bits
        // get base colour for subblock 1
        blue1 = (unsigned char)((blockTop&0xf00000)>>16);
        blue1 = blue1 +(blue1>>4);  // copy bits to lower sig
        green1 = (unsigned char)((blockTop&0xf000)>>8);
        green1 = green1 + (green1>>4);  // copy bits to lower sig
        red1 = (unsigned char)(blockTop&0xf0);
        red1 = red1 + (red1>>4);  // copy bits to lower sig

        // get base colour for subblock 2
        blue2 = (unsigned char)((blockTop&0xf0000)>>12);
        blue2 = blue2 +(blue2>>4);  // copy bits to lower sig
        green2 = (unsigned char)((blockTop&0xf00)>>4);
        green2 = green2 + (green2>>4);  // copy bits to lower sig
        red2 = (unsigned char)((blockTop&0xf)<<4);
        red2 = red2 + (red2>>4);  // copy bits to lower sig
      }
      // get the modtables for each subblock
      modtable1 = (blockTop>>29)&0x7;
      modtable2 = (blockTop>>26)&0x7;

      if(!bFlip)
      {  // 2 2x4 blocks side by side

        for(j=0;j<4;j++)  // vertical
        {
          for(k=0;k<2;k++)  // horizontal
          {
            *(output+j*x+k) = modifyPixel(red1,green1,blue1,k,j,blockBot,modtable1);
            *(output+j*x+k+2) = modifyPixel(red2,green2,blue2,k+2,j,blockBot,modtable2);
          }
        }

      }
      else
      {  // 2 4x2 blocks on top of each other
        for(j=0;j<2;j++)
        {
          for(k=0;k<4;k++)
          {
            *(output+j*x+k) = modifyPixel(red1,green1,blue1,k,j,blockBot,modtable1);
            *(output+(j+2)*x+k) = modifyPixel(red2,green2,blue2,k,j+2,blockBot,modtable2);
          }
        }
      }
    }
  }

  return x*y/2;
}

/*!***********************************************************************
@Function    PVRTDecompressETC
@Input      pSrcData The ETC texture data to decompress
@Input      x X dimension of the texture
@Input      y Y dimension of the texture
@Modified    pDestData The decompressed texture data
@Input      nMode The format of the data
@Returns    The number of bytes of ETC data decompressed
@Description  Decompresses ETC to RGBA 8888
*************************************************************************/
int PVRTDecompressETC(void *pSrcData, int x, int y, void *pDestData, int nMode)
{
  int i, j, i32read;

  if(x<ETC_MIN_TEXWIDTH || y<ETC_MIN_TEXHEIGHT)
  {  // decompress into a buffer big enough to take the minimum size
    char* pTempBuffer =  (char*)malloc(PVRT_MAX(x,ETC_MIN_TEXWIDTH)*PVRT_MAX(y,ETC_MIN_TEXHEIGHT)*4);
    i32read = ETCTextureDecompress(pSrcData,PVRT_MAX(x,ETC_MIN_TEXWIDTH),PVRT_MAX(y,ETC_MIN_TEXHEIGHT),pTempBuffer,nMode);

    for(i=0;i<y;i++)
    {  // copy from larger temp buffer to output data
      memcpy((char*)(pDestData)+i*x*4,pTempBuffer+PVRT_MAX(x,ETC_MIN_TEXWIDTH)*4*i,x*4);
    }

    if(pTempBuffer) free(pTempBuffer);
  }
  else  // decompress larger MIP levels straight into the output data
    i32read = ETCTextureDecompress(pSrcData,x,y,pDestData,nMode);

  // swap r and b channels
  unsigned char* pSwap = (unsigned char*)pDestData, swap;

  for(i=0;i<y;i++)
    for(j=0;j<x;j++)
    {
      swap = pSwap[0];
      pSwap[0] = pSwap[2];
      pSwap[2] = swap;
      pSwap+=4;
    }

  return i32read;
}

#define typeret(x) if(Type == x) return #x;

static char *typeAsString(int Type) {
  typeret(MGLPT_ARGB_4444)
  else typeret(MGLPT_ARGB_1555)
  else typeret(MGLPT_RGB_565)
  else typeret(MGLPT_RGB_555)
  else typeret(MGLPT_RGB_888)
  else typeret(MGLPT_ARGB_8888)
  else typeret(MGLPT_ARGB_8332)
  else typeret(MGLPT_I_8)
  else typeret(MGLPT_AI_88)
  else typeret(MGLPT_1_BPP)
  else typeret(MGLPT_VY1UY0)
  else typeret(MGLPT_Y1VY0U)
  else typeret(MGLPT_PVRTC2)
  else typeret(MGLPT_PVRTC4)
  else typeret(MGLPT_PVRTC2_2)
  else typeret(MGLPT_PVRTC2_4)
  else typeret(OGL_RGBA_4444)
  else typeret(OGL_RGBA_5551)
  else typeret(OGL_RGBA_8888)
  else typeret(OGL_RGB_565)
  else typeret(OGL_RGB_555)
  else typeret(OGL_RGB_888)
  else typeret(OGL_I_8)
  else typeret(OGL_AI_88)
  else typeret(OGL_PVRTC2)
  else typeret(OGL_PVRTC4)
  else typeret(OGL_BGRA_8888)
  else typeret(OGL_A_8)
  else typeret(D3D_DXT1)
  else typeret(D3D_DXT2)
  else typeret(D3D_DXT3)
  else typeret(D3D_DXT4)
  else typeret(D3D_DXT5)
  else typeret(D3D_RGB_332)
  else typeret(D3D_AI_44)
  else typeret(D3D_LVU_655)
  else typeret(D3D_XLVU_8888)
  else typeret(D3D_QWVU_8888)
  else typeret(D3D_ABGR_2101010)
  else typeret(D3D_ARGB_2101010)
  else typeret(D3D_AWVU_2101010)
  else typeret(D3D_GR_1616)
  else typeret(D3D_VU_1616)
  else typeret(D3D_ABGR_16161616)
  else typeret(D3D_R16F)
  else typeret(D3D_GR_1616F)
  else typeret(D3D_ABGR_16161616F)
  else typeret(D3D_R32F)
  else typeret(D3D_GR_3232F)
  else typeret(D3D_ABGR_32323232F)
  else typeret(ETC_RGB_4BPP)
  else typeret(ETC_RGBA_EXPLICIT)
  else typeret(ETC_RGBA_INTERPOLATED)
  else typeret(ePT_DX10_R32G32B32A32_FLOAT)
  else typeret(ePT_DX10_R32G32B32A32_UINT)
  else typeret(ePT_DX10_R32G32B32A32_SINT)
  else typeret(ePT_DX10_R32G32B32_FLOAT)
  else typeret(ePT_DX10_R32G32B32_UINT)
  else typeret(ePT_DX10_R32G32B32_SINT)
  else typeret(ePT_DX10_R16G16B16A16_FLOAT)
  else typeret(ePT_DX10_R16G16B16A16_UNORM)
  else typeret(ePT_DX10_R16G16B16A16_UINT)
  else typeret(ePT_DX10_R16G16B16A16_SNORM)
  else typeret(ePT_DX10_R16G16B16A16_SINT)
  else typeret(ePT_DX10_R32G32_FLOAT)
  else typeret(ePT_DX10_R32G32_UINT)
  else typeret(ePT_DX10_R32G32_SINT)
  else typeret(ePT_DX10_R10G10B10A2_UNORM)
  else typeret(ePT_DX10_R10G10B10A2_UINT)
  else typeret(ePT_DX10_R11G11B10_FLOAT)
  else typeret(ePT_DX10_R8G8B8A8_UNORM)
  else typeret(ePT_DX10_R8G8B8A8_UNORM_SRGB)
  else typeret(ePT_DX10_R8G8B8A8_UINT)
  else typeret(ePT_DX10_R8G8B8A8_SNORM)
  else typeret(ePT_DX10_R8G8B8A8_SINT)
  else typeret(ePT_DX10_R16G16_FLOAT)
  else typeret(ePT_DX10_R16G16_UNORM)
  else typeret(ePT_DX10_R16G16_UINT)
  else typeret(ePT_DX10_R16G16_SNORM)
  else typeret(ePT_DX10_R16G16_SINT)
  else typeret(ePT_DX10_R32_FLOAT)
  else typeret(ePT_DX10_R32_UINT)
  else typeret(ePT_DX10_R32_SINT)
  else typeret(ePT_DX10_R8G8_UNORM)
  else typeret(ePT_DX10_R8G8_UINT)
  else typeret(ePT_DX10_R8G8_SNORM)
  else typeret(ePT_DX10_R8G8_SINT)
  else typeret(ePT_DX10_R16_FLOAT)
  else typeret(ePT_DX10_R16_UNORM)
  else typeret(ePT_DX10_R16_UINT)
  else typeret(ePT_DX10_R16_SNORM)
  else typeret(ePT_DX10_R16_SINT)
  else typeret(ePT_DX10_R8_UNORM)
  else typeret(ePT_DX10_R8_UINT)
  else typeret(ePT_DX10_R8_SNORM)
  else typeret(ePT_DX10_R8_SINT)
  else typeret(ePT_DX10_A8_UNORM)
  else typeret(ePT_DX10_R1_UNORM)
  else typeret(ePT_DX10_R9G9B9E5_SHAREDEXP)
  else typeret(ePT_DX10_R8G8_B8G8_UNORM)
  else typeret(ePT_DX10_G8R8_G8B8_UNORM)
  else typeret(ePT_DX10_BC1_UNORM)
  else typeret(ePT_DX10_BC1_UNORM_SRGB)
  else typeret(ePT_DX10_BC2_UNORM)
  else typeret(ePT_DX10_BC2_UNORM_SRGB)
  else typeret(ePT_DX10_BC3_UNORM)
  else typeret(ePT_DX10_BC3_UNORM_SRGB)
  else typeret(ePT_DX10_BC4_UNORM)
  else typeret(ePT_DX10_BC4_SNORM)
  else typeret(ePT_DX10_BC5_UNORM)
  else typeret(ePT_DX10_BC5_SNORM)
  else typeret(ePT_DX10_B5G6R5_UNORM)
  else typeret(ePT_DX10_B5G5R5A1_UNORM)
  else typeret(ePT_DX10_B8G8R8A8_UNORM)
  else typeret(ePT_DX10_B8G8R8X8_UNORM)
  else typeret(ePT_VG_sRGBX_8888)
  else typeret(ePT_VG_sRGBA_8888)
  else typeret(ePT_VG_sRGBA_8888_PRE)
  else typeret(ePT_VG_sRGB_565)
  else typeret(ePT_VG_sRGBA_5551)
  else typeret(ePT_VG_sRGBA_4444)
  else typeret(ePT_VG_sL_8)
  else typeret(ePT_VG_lRGBX_8888)
  else typeret(ePT_VG_lRGBA_8888)
  else typeret(ePT_VG_lRGBA_8888_PRE)
  else typeret(ePT_VG_lL_8)
  else typeret(ePT_VG_A_8)
  else typeret(ePT_VG_BW_1)
  else typeret(ePT_VG_sXRGB_8888)
  else typeret(ePT_VG_sARGB_8888)
  else typeret(ePT_VG_sARGB_8888_PRE)
  else typeret(ePT_VG_sARGB_1555)
  else typeret(ePT_VG_sARGB_4444)
  else typeret(ePT_VG_lXRGB_8888)
  else typeret(ePT_VG_lARGB_8888)
  else typeret(ePT_VG_lARGB_8888_PRE)
  else typeret(ePT_VG_sBGRX_8888)
  else typeret(ePT_VG_sBGRA_8888)
  else typeret(ePT_VG_sBGRA_8888_PRE)
  else typeret(ePT_VG_sBGR_565)
  else typeret(ePT_VG_sBGRA_5551)
  else typeret(ePT_VG_sBGRA_4444)
  else typeret(ePT_VG_lBGRX_8888)
  else typeret(ePT_VG_lBGRA_8888)
  else typeret(ePT_VG_lBGRA_8888_PRE)
  else typeret(ePT_VG_sXBGR_8888)
  else typeret(ePT_VG_sABGR_8888 )
  else typeret(ePT_VG_sABGR_8888_PRE)
  else typeret(ePT_VG_sABGR_1555)
  else typeret(ePT_VG_sABGR_4444)
  else typeret(ePT_VG_lXBGR_8888)
  else typeret(ePT_VG_lABGR_8888)
  else typeret(ePT_VG_lABGR_8888_PRE)
  return "UNKNOWN";
}


static void swapShit(pic *P) {
  int I, T;
  times (I, P->W*P->H) {
    T=P->D[I*4+0];
    P->D[I*4+0] = P->D[I*4+2];
    P->D[I*4+2] = T;
  }
}

static void pvrDecompile(char *Output, char *Input) {
  char Tmp[1024], Name[256], Ext[32];
  int I, J, Type, X,Y,R,G,B,A;
  pic *P;
  int L = fileSize(Input);
  u1 *M = (u1*)readFile(0, L, Input);
  u1 *Q = M;
  header *H;

  // Scan for PVRs
  I=0;
  while (Q < M+L) {
    if (Q-M > L-8) break;
    if (Q-M < 0x2C || *(u4*)Q != PVRTEX_IDENTIFIER ) {
      Q++;
      continue;
    }
    H = (header*)(Q-0x2C);
    Q = (u1*)H + H->Size + H->DataSize;

    Type = H->Flags & PVRTEX_PIXELTYPE;

    printf("  %02d: %dx%dx%d (%s) MipMaps=%d NumSurfs=%d\n"
          ,I,H->W, H->H, H->Bits, typeAsString(Type), H->Mips, H->NumSurfs);


    P = picNew(H->W, H->H, 32);

    Q = (u1*)H + H->Size;

    if (Type == OGL_PVRTC2) {
      PVRTDecompressPVRTC(Q, 1, H->W, H->H, P->D);
      swapShit(P);
    } else if (Type == OGL_PVRTC4) {
      PVRTDecompressPVRTC(Q, 0, H->W, H->H, P->D);
      swapShit(P);
    } else if (Type == OGL_RGBA_4444) {
      times (Y,P->H) times(X, P->W) {
        fromA4R4G4B4(R,G,B,A, *((u2*)Q+Y*P->W+X));
        picPut32(P,X,P->H-Y-1, R8G8B8A8(R,G,B,A));
      }
    } else if (Type == OGL_RGBA_5551) {
      times (Y,P->H) times(X, P->W) {
        fromR5G5B5A1(A,R,G,B, *((u2*)Q+Y*P->W+X));
        picPut32(P,X,P->H-Y-1, R8G8B8A8(R,G,B,A*0xFF));
      }
    } else if (Type == OGL_RGB_888) {
      times (Y,P->H) times(X, P->W) {
        R = Q[(Y*P->W+X)*3+0];
        G = Q[(Y*P->W+X)*3+1];
        B = Q[(Y*P->W+X)*3+2];
        picPut32(P,X,P->H-Y-1, R8G8B8A8(R,G,B,0xFF));
      }
    }  else if (Type == OGL_RGBA_8888) {
      times (Y,P->H) times(X, P->W) {
        fromR8G8B8A8(R,G,B,A, *((u4*)Q+Y*P->W+X));
        picPut32(P,X,Y, R8G8B8A8(B,G,R,A));
      }
    } else {
      printf(" Unsupported PVR type (%d=%s)\n", Type,typeAsString(Type));
      abort();
    }
    if (H->Flags & PVRTEX_VERTICAL_FLIP) times (Y,P->H/2) times(X, P->W) {
      u4 T = picGet32(P,X,Y);
      picPut32(P,X,Y, picGet32(P,X,P->H-Y-1));
      picPut32(P,X,P->H-Y-1, T);
    }
    pathParts(0, Name, Ext, Input);
    sprintf(Tmp, "%s/%s_%d.png", Output, Name, I);
    pngSave(Tmp, P);
    picDel(P);
    I++;
  }

  if (!I) {
    printf("  File has no PVR textures\n");
    abort();
  }
}

int pvrInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "pvr";
  F->Description = "PowerVR textures (many iOS games)";
  F->Decompile = pvrDecompile;
  return 1;
}


