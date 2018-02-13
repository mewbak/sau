//Credits go to NVIDIA for piblishing original source code
#include "common.h"


#define MAKEFOURCC(ch0, ch1, ch2, ch3) \
  (((u4)(u1)(ch0)      ) | ((u4)(u1)ch1 <<  8) \
  |((u4)(u1)(ch2) << 16) | ((u4)(u1)ch3 << 24))


#define FOURCC_DDS  MAKEFOURCC('D', 'D', 'S', ' ')
#define FOURCC_DXT1 MAKEFOURCC('D', 'X', 'T', '1')
#define FOURCC_DXT2 MAKEFOURCC('D', 'X', 'T', '2')
#define FOURCC_DXT3 MAKEFOURCC('D', 'X', 'T', '3')
#define FOURCC_DXT4 MAKEFOURCC('D', 'X', 'T', '4')
#define FOURCC_DXT5 MAKEFOURCC('D', 'X', 'T', '5')
#define FOURCC_RXGB MAKEFOURCC('R', 'X', 'G', 'B')
#define FOURCC_ATI1 MAKEFOURCC('A', 'T', 'I', '1')
#define FOURCC_ATI2 MAKEFOURCC('A', 'T', 'I', '2')
#define FOURCC_A2XY MAKEFOURCC('A', '2', 'X', 'Y')
#define FOURCC_DX10 MAKEFOURCC('D', 'X', '1', '0')

// 32 bit RGB formats.
#define D3DFMT_R8G8B8       20
#define D3DFMT_A8R8G8B8     21
#define D3DFMT_X8R8G8B8     22
#define D3DFMT_R5G6B5       23
#define D3DFMT_X1R5G5B5     24
#define D3DFMT_A1R5G5B5     25
#define D3DFMT_A4R4G4B4     26
#define D3DFMT_R3G3B2       27
#define D3DFMT_A8           28
#define D3DFMT_A8R3G3B2     29
#define D3DFMT_X4R4G4B4     30
#define D3DFMT_A2B10G10R10  31
#define D3DFMT_A8B8G8R8     32
#define D3DFMT_X8B8G8R8     33
#define D3DFMT_G16R16       34
#define D3DFMT_A2R10G10B10  35
#define D3DFMT_A16B16G16R16 36

// Palette formats.
#define D3DFMT_A8P8         40
#define D3DFMT_P8           41

// Luminance formats.
#define D3DFMT_L8           50
#define D3DFMT_A8L8         51
#define D3DFMT_A4L4         52
#define D3DFMT_L16          81

// Floating point formats
#define D3DFMT_R16F          111
#define D3DFMT_G16R16F       112
#define D3DFMT_A16B16G16R16F 113
#define D3DFMT_R32F          114
#define D3DFMT_G32R32F       115
#define D3DFMT_A32B32G32R32F 116

#define DDSD_CAPS          0x00000001U
#define DDSD_PIXELFORMAT   0x00001000U
#define DDSD_WIDTH         0x00000004U
#define DDSD_HEIGHT        0x00000002U
#define DDSD_PITCH         0x00000008U
#define DDSD_MIPMAPCOUNT   0x00020000U
#define DDSD_LINEARSIZE    0x00080000U
#define DDSD_DEPTH         0x00800000U

#define DDSCAPS_COMPLEX    0x00000008U
#define DDSCAPS_TEXTURE    0x00001000U
#define DDSCAPS_MIPMAP     0x00400000U
#define DDSCAPS2_VOLUME    0x00200000U
#define DDSCAPS2_CUBEMAP   0x00000200U

#define DDSCAPS2_CUBEMAP_POSITIVEX 0x00000400U
#define DDSCAPS2_CUBEMAP_NEGATIVEX 0x00000800U
#define DDSCAPS2_CUBEMAP_POSITIVEY 0x00001000U
#define DDSCAPS2_CUBEMAP_NEGATIVEY 0x00002000U
#define DDSCAPS2_CUBEMAP_POSITIVEZ 0x00004000U
#define DDSCAPS2_CUBEMAP_NEGATIVEZ 0x00008000U
#define DDSCAPS2_CUBEMAP_ALL_FACES 0x0000FC00U

#define DDPF_ALPHAPIXELS     0x00000001U
#define DDPF_ALPHA           0x00000002U
#define DDPF_FOURCC          0x00000004U
#define DDPF_RGB             0x00000040U
#define DDPF_PALETTEINDEXED1 0x00000800U
#define DDPF_PALETTEINDEXED2 0x00001000U
#define DDPF_PALETTEINDEXED4 0x00000008U
#define DDPF_PALETTEINDEXED8 0x00000020U
#define DDPF_LUMINANCE       0x00020000U
#define DDPF_ALPHAPREMULT    0x00008000U
#define DDPF_NORMAL          0x80000000U// @@ Custom nv flag.

// DX10 formats.
#define DXGI_FORMAT_UNKNOWN 0

#define DXGI_FORMAT_R32G32B32A32_TYPELESS 1
#define DXGI_FORMAT_R32G32B32A32_FLOAT 2
#define DXGI_FORMAT_R32G32B32A32_u4 3
#define DXGI_FORMAT_R32G32B32A32_SINT 4

#define DXGI_FORMAT_R32G32B32_TYPELESS 5
#define DXGI_FORMAT_R32G32B32_FLOAT 6
#define DXGI_FORMAT_R32G32B32_u4 7
#define DXGI_FORMAT_R32G32B32_SINT 8

#define DXGI_FORMAT_R16G16B16A16_TYPELESS 9
#define DXGI_FORMAT_R16G16B16A16_FLOAT 10
#define DXGI_FORMAT_R16G16B16A16_UNORM 11
#define DXGI_FORMAT_R16G16B16A16_u4 12
#define DXGI_FORMAT_R16G16B16A16_SNORM 13
#define DXGI_FORMAT_R16G16B16A16_SINT 14

#define DXGI_FORMAT_R32G32_TYPELESS 15
#define DXGI_FORMAT_R32G32_FLOAT 16
#define DXGI_FORMAT_R32G32_u4 17
#define DXGI_FORMAT_R32G32_SINT 18

#define DXGI_FORMAT_R32G8X24_TYPELESS 19
#define DXGI_FORMAT_D32_FLOAT_S8X24_u4 20
#define DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS 21
#define DXGI_FORMAT_X32_TYPELESS_G8X24_u4 22

#define DXGI_FORMAT_R10G10B10A2_TYPELESS 23
#define DXGI_FORMAT_R10G10B10A2_UNORM 24
#define DXGI_FORMAT_R10G10B10A2_u4 25

#define DXGI_FORMAT_R11G11B10_FLOAT 26

#define DXGI_FORMAT_R8G8B8A8_TYPELESS 27
#define DXGI_FORMAT_R8G8B8A8_UNORM 28
#define DXGI_FORMAT_R8G8B8A8_UNORM_SRGB 29
#define DXGI_FORMAT_R8G8B8A8_u4 30
#define DXGI_FORMAT_R8G8B8A8_SNORM 31
#define DXGI_FORMAT_R8G8B8A8_SINT 32

#define DXGI_FORMAT_R16G16_TYPELESS 33
#define DXGI_FORMAT_R16G16_FLOAT 34
#define DXGI_FORMAT_R16G16_UNORM 35
#define DXGI_FORMAT_R16G16_u4 36
#define DXGI_FORMAT_R16G16_SNORM 37
#define DXGI_FORMAT_R16G16_SINT 38

#define DXGI_FORMAT_R32_TYPELESS 39
#define DXGI_FORMAT_D32_FLOAT 40
#define DXGI_FORMAT_R32_FLOAT 41
#define DXGI_FORMAT_R32_u4 42
#define DXGI_FORMAT_R32_SINT 43

#define DXGI_FORMAT_R24G8_TYPELESS 44
#define DXGI_FORMAT_D24_UNORM_S8_u4 45
#define DXGI_FORMAT_R24_UNORM_X8_TYPELESS 46
#define DXGI_FORMAT_X24_TYPELESS_G8_u4 47

#define DXGI_FORMAT_R8G8_TYPELESS 48
#define DXGI_FORMAT_R8G8_UNORM 49
#define DXGI_FORMAT_R8G8_u4 50
#define DXGI_FORMAT_R8G8_SNORM 51
#define DXGI_FORMAT_R8G8_SINT 52

#define DXGI_FORMAT_R16_TYPELESS 53
#define DXGI_FORMAT_R16_FLOAT 54
#define DXGI_FORMAT_D16_UNORM 55
#define DXGI_FORMAT_R16_UNORM 56
#define DXGI_FORMAT_R16_u4 57
#define DXGI_FORMAT_R16_SNORM 58
#define DXGI_FORMAT_R16_SINT 59

#define DXGI_FORMAT_R8_TYPELESS 60
#define DXGI_FORMAT_R8_UNORM 61
#define DXGI_FORMAT_R8_u4 62
#define DXGI_FORMAT_R8_SNORM 63
#define DXGI_FORMAT_R8_SINT 64
#define DXGI_FORMAT_A8_UNORM 65

#define DXGI_FORMAT_R1_UNORM 66

#define DXGI_FORMAT_R9G9B9E5_SHAREDEXP 67

#define DXGI_FORMAT_R8G8_B8G8_UNORM 68
#define DXGI_FORMAT_G8R8_G8B8_UNORM 69

#define DXGI_FORMAT_BC1_TYPELESS 70
#define DXGI_FORMAT_BC1_UNORM 71
#define DXGI_FORMAT_BC1_UNORM_SRGB 72

#define DXGI_FORMAT_BC2_TYPELESS 73
#define DXGI_FORMAT_BC2_UNORM 74
#define DXGI_FORMAT_BC2_UNORM_SRGB 75

#define DXGI_FORMAT_BC3_TYPELESS 76
#define DXGI_FORMAT_BC3_UNORM 77
#define DXGI_FORMAT_BC3_UNORM_SRGB 78

#define DXGI_FORMAT_BC4_TYPELESS 79
#define DXGI_FORMAT_BC4_UNORM 80
#define DXGI_FORMAT_BC4_SNORM 81

#define DXGI_FORMAT_BC5_TYPELESS 82
#define DXGI_FORMAT_BC5_UNORM 83
#define DXGI_FORMAT_BC5_SNORM 84

#define DXGI_FORMAT_B5G6R5_UNORM 85
#define DXGI_FORMAT_B5G5R5A1_UNORM 86
#define DXGI_FORMAT_B8G8R8A8_UNORM 87
#define DXGI_FORMAT_B8G8R8X8_UNORM 88


#define D3D10_RESOURCE_DIMENSION_UNKNOWN 0
#define D3D10_RESOURCE_DIMENSION_BUFFER 1
#define D3D10_RESOURCE_DIMENSION_TEXTURE1D 2
#define D3D10_RESOURCE_DIMENSION_TEXTURE2D 3
#define D3D10_RESOURCE_DIMENSION_TEXTURE3D 4


typedef struct {
  u4 size;
  u4 flags;
  u4 fourcc;
  u4 bitcount;
  u4 rmask;
  u4 gmask;
  u4 bmask;
  u4 amask;
} DDSPixelFormat;

typedef struct {
  u4 caps1;
  u4 caps2;
  u4 caps3;
  u4 caps4;
} DDSCaps;

typedef struct { // DDS file header for DX10.
  u4 dxgiFormat;
  u4 resourceDimension;
  u4 miscFlag;
  u4 arraySize;
  u4 reserved;
} DDSHeader10;

typedef struct { // DDS file header.
  u4 fourcc;
  u4 size;
  u4 flags;
  u4 height;
  u4 width;
  u4 pitch;
  u4 depth;
  u4 mipmapcount;
  u4 reserved[11];
  DDSPixelFormat pf;
  DDSCaps caps;
  u4 notused;
  DDSHeader10 header10;
} DDSHeader;


static char * getDxgiFormatString(int dxgiFormat) {
#define CASE(format) case DXGI_FORMAT_##format: return #format
  switch(dxgiFormat) {
  CASE(UNKNOWN);
  
  CASE(R32G32B32A32_TYPELESS);
  CASE(R32G32B32A32_FLOAT);
  CASE(R32G32B32A32_u4);
  CASE(R32G32B32A32_SINT);
  
  CASE(R32G32B32_TYPELESS);
  CASE(R32G32B32_FLOAT);
  CASE(R32G32B32_u4);
  CASE(R32G32B32_SINT);
  
  CASE(R16G16B16A16_TYPELESS);
  CASE(R16G16B16A16_FLOAT);
  CASE(R16G16B16A16_UNORM);
  CASE(R16G16B16A16_u4);
  CASE(R16G16B16A16_SNORM);
  CASE(R16G16B16A16_SINT);
  
  CASE(R32G32_TYPELESS);
  CASE(R32G32_FLOAT);
  CASE(R32G32_u4);
  CASE(R32G32_SINT);
  
  CASE(R32G8X24_TYPELESS);
  CASE(D32_FLOAT_S8X24_u4);
  CASE(R32_FLOAT_X8X24_TYPELESS);
  CASE(X32_TYPELESS_G8X24_u4);
  
  CASE(R10G10B10A2_TYPELESS);
  CASE(R10G10B10A2_UNORM);
  CASE(R10G10B10A2_u4);
  
  CASE(R11G11B10_FLOAT);
  
  CASE(R8G8B8A8_TYPELESS);
  CASE(R8G8B8A8_UNORM);
  CASE(R8G8B8A8_UNORM_SRGB);
  CASE(R8G8B8A8_u4);
  CASE(R8G8B8A8_SNORM);
  CASE(R8G8B8A8_SINT);
  
  CASE(R16G16_TYPELESS);
  CASE(R16G16_FLOAT);
  CASE(R16G16_UNORM);
  CASE(R16G16_u4);
  CASE(R16G16_SNORM);
  CASE(R16G16_SINT);
  
  CASE(R32_TYPELESS);
  CASE(D32_FLOAT);
  CASE(R32_FLOAT);
  CASE(R32_u4);
  CASE(R32_SINT);
  
  CASE(R24G8_TYPELESS);
  CASE(D24_UNORM_S8_u4);
  CASE(R24_UNORM_X8_TYPELESS);
  CASE(X24_TYPELESS_G8_u4);
  
  CASE(R8G8_TYPELESS);
  CASE(R8G8_UNORM);
  CASE(R8G8_u4);
  CASE(R8G8_SNORM);
  CASE(R8G8_SINT);
  
  CASE(R16_TYPELESS);
  CASE(R16_FLOAT);
  CASE(D16_UNORM);
  CASE(R16_UNORM);
  CASE(R16_u4);
  CASE(R16_SNORM);
  CASE(R16_SINT);
  
  CASE(R8_TYPELESS);
  CASE(R8_UNORM);
  CASE(R8_u4);
  CASE(R8_SNORM);
  CASE(R8_SINT);
  CASE(A8_UNORM);

  CASE(R1_UNORM);
  
  CASE(R9G9B9E5_SHAREDEXP);
  
  CASE(R8G8_B8G8_UNORM);
  CASE(G8R8_G8B8_UNORM);

  CASE(BC1_TYPELESS);
  CASE(BC1_UNORM);
  CASE(BC1_UNORM_SRGB);
  
  CASE(BC2_TYPELESS);
  CASE(BC2_UNORM);
  CASE(BC2_UNORM_SRGB);
  
  CASE(BC3_TYPELESS);
  CASE(BC3_UNORM);
  CASE(BC3_UNORM_SRGB);
  
  CASE(BC4_TYPELESS);
  CASE(BC4_UNORM);
  CASE(BC4_SNORM);
  
  CASE(BC5_TYPELESS);
  CASE(BC5_UNORM);
  CASE(BC5_SNORM);

  CASE(B5G6R5_UNORM);
  CASE(B5G5R5A1_UNORM);
  CASE(B8G8R8A8_UNORM);
  CASE(B8G8R8X8_UNORM);

  default: return "UNKNOWN";
  }
#undef CASE
}
  
static char *getD3d10ResourceDimensionString(int resourceDimension) {
  switch(resourceDimension) {
  default:
  case D3D10_RESOURCE_DIMENSION_UNKNOWN: return "UNKNOWN";
  case D3D10_RESOURCE_DIMENSION_BUFFER: return "BUFFER";
  case D3D10_RESOURCE_DIMENSION_TEXTURE1D: return "TEXTURE1D";
  case D3D10_RESOURCE_DIMENSION_TEXTURE2D: return "TEXTURE2D";
  case D3D10_RESOURCE_DIMENSION_TEXTURE3D: return "TEXTURE3D";
  }
}

static void dumpHeader(DDSHeader *H) {
  printf("Flags: 0x%.8X\n", H->flags);
  if (H->flags & DDSD_CAPS) printf("\tDDSD_CAPS\n");
  if (H->flags & DDSD_PIXELFORMAT) printf("\tDDSD_PIXELFORMAT\n");
  if (H->flags & DDSD_WIDTH) printf("\tDDSD_WIDTH\n");
  if (H->flags & DDSD_HEIGHT) printf("\tDDSD_HEIGHT\n");
  if (H->flags & DDSD_DEPTH) printf("\tDDSD_DEPTH\n");
  if (H->flags & DDSD_PITCH) printf("\tDDSD_PITCH\n");
  if (H->flags & DDSD_LINEARSIZE) printf("\tDDSD_LINEARSIZE\n");
  if (H->flags & DDSD_MIPMAPCOUNT) printf("\tDDSD_MIPMAPCOUNT\n");

  printf("Height: %d\n", H->height);
  printf("Width: %d\n", H->width);
  printf("Depth: %d\n", H->depth);
  if (H->flags & DDSD_PITCH) printf("Pitch: %d\n", H->pitch);
  else if (H->flags & DDSD_LINEARSIZE) printf("Linear size: %d\n", H->pitch);
  printf("Mipmap count: %d\n", H->mipmapcount);
  
  printf("Pixel Format:\n");
  printf("\tFlags: 0x%.8X\n", H->pf.flags);
  if (H->pf.flags & DDPF_RGB) printf("\t\tDDPF_RGB\n");
  if (H->pf.flags & DDPF_LUMINANCE) printf("\t\tDDPF_LUMINANCE\n");
  if (H->pf.flags & DDPF_FOURCC) printf("\t\tDDPF_FOURCC\n");
  if (H->pf.flags & DDPF_ALPHAPIXELS) printf("\t\tDDPF_ALPHAPIXELS\n");
  if (H->pf.flags & DDPF_ALPHA) printf("\t\tDDPF_ALPHA\n");
  if (H->pf.flags & DDPF_PALETTEINDEXED1) printf("\t\tDDPF_PALETTEINDEXED1\n");
  if (H->pf.flags & DDPF_PALETTEINDEXED2) printf("\t\tDDPF_PALETTEINDEXED2\n");
  if (H->pf.flags & DDPF_PALETTEINDEXED4) printf("\t\tDDPF_PALETTEINDEXED4\n");
  if (H->pf.flags & DDPF_PALETTEINDEXED8) printf("\t\tDDPF_PALETTEINDEXED8\n");
  if (H->pf.flags & DDPF_ALPHAPREMULT) printf("\t\tDDPF_ALPHAPREMULT\n");
  if (H->pf.flags & DDPF_NORMAL) printf("\t\tDDPF_NORMAL\n");
  
  printf("\tFourCC: '%c%c%c%c'\n",
    ((H->pf.fourcc >> 0) & 0xFF),
    ((H->pf.fourcc >> 8) & 0xFF),
    ((H->pf.fourcc >> 16) & 0xFF),
    ((H->pf.fourcc >> 24) & 0xFF));
  if ((H->pf.fourcc & DDPF_FOURCC) && (H->pf.bitcount != 0))
  {
    printf("\tSwizzle: '%c%c%c%c'\n", 
      (H->pf.bitcount >> 0) & 0xFF,
      (H->pf.bitcount >> 8) & 0xFF,
      (H->pf.bitcount >> 16) & 0xFF,
      (H->pf.bitcount >> 24) & 0xFF);
  }
  else
  {
    printf("\tBit count: %d\n", H->pf.bitcount);
  }
  printf("\tRed mask: 0x%.8X\n", H->pf.rmask);
  printf("\tGreen mask: 0x%.8X\n", H->pf.gmask);
  printf("\tBlue mask: 0x%.8X\n", H->pf.bmask);
  printf("\tAlpha mask: 0x%.8X\n", H->pf.amask);

  printf("Caps:\n");
  printf("\tCaps 1: 0x%.8X\n", H->caps.caps1);
  if (H->caps.caps1 & DDSCAPS_COMPLEX) printf("\t\tDDSCAPS_COMPLEX\n");
  if (H->caps.caps1 & DDSCAPS_TEXTURE) printf("\t\tDDSCAPS_TEXTURE\n");
  if (H->caps.caps1 & DDSCAPS_MIPMAP) printf("\t\tDDSCAPS_MIPMAP\n");

  printf("\tCaps 2: 0x%.8X\n", H->caps.caps2);
  if (H->caps.caps2 & DDSCAPS2_VOLUME) printf("\t\tDDSCAPS2_VOLUME\n");
  else if (H->caps.caps2 & DDSCAPS2_CUBEMAP)
  {
    printf("\t\tDDSCAPS2_CUBEMAP\n");
    if ((H->caps.caps2 & DDSCAPS2_CUBEMAP_ALL_FACES) == DDSCAPS2_CUBEMAP_ALL_FACES) printf("\t\tDDSCAPS2_CUBEMAP_ALL_FACES\n");
    else {
      if (H->caps.caps2 & DDSCAPS2_CUBEMAP_POSITIVEX) printf("\t\tDDSCAPS2_CUBEMAP_POSITIVEX\n");
      if (H->caps.caps2 & DDSCAPS2_CUBEMAP_NEGATIVEX) printf("\t\tDDSCAPS2_CUBEMAP_NEGATIVEX\n");
      if (H->caps.caps2 & DDSCAPS2_CUBEMAP_POSITIVEY) printf("\t\tDDSCAPS2_CUBEMAP_POSITIVEY\n");
      if (H->caps.caps2 & DDSCAPS2_CUBEMAP_NEGATIVEY) printf("\t\tDDSCAPS2_CUBEMAP_NEGATIVEY\n");
      if (H->caps.caps2 & DDSCAPS2_CUBEMAP_POSITIVEZ) printf("\t\tDDSCAPS2_CUBEMAP_POSITIVEZ\n");
      if (H->caps.caps2 & DDSCAPS2_CUBEMAP_NEGATIVEZ) printf("\t\tDDSCAPS2_CUBEMAP_NEGATIVEZ\n");
    }
  }

  printf("\tCaps 3: 0x%.8X\n", H->caps.caps3);
  printf("\tCaps 4: 0x%.8X\n", H->caps.caps4);

  if (H->pf.fourcc == FOURCC_DX10)
  {
    printf("DX10 Header:\n");
    printf("\tDXGI Format: %u (%s)\n"
          ,H->header10.dxgiFormat
          ,getDxgiFormatString(H->header10.dxgiFormat));
    printf("\tResource dimension: %u (%s)\n"
          ,H->header10.resourceDimension
          ,getD3d10ResourceDimensionString(H->header10.resourceDimension));
    printf("\tMisc flag: %u\n", H->header10.miscFlag);
    printf("\tArray size: %u\n", H->header10.arraySize);
  }
  
  if (H->reserved[9] == MAKEFOURCC('N', 'V', 'T', 'T'))
  {
    int major = (H->reserved[10] >> 16) & 0xFF;
    int minor = (H->reserved[10] >> 8) & 0xFF;
    int revision= H->reserved[10] & 0xFF;
    
    printf("Version:\n");
    printf("\tNVIDIA Texture Tools %d.%d.%d\n", major, minor, revision);
  }
}


static int blockSize(DDSHeader *H) {
  switch(H->pf.fourcc) {
  case FOURCC_DXT1:
  case FOURCC_ATI1:
    return 8;
  case FOURCC_DXT2:
  case FOURCC_DXT3:
  case FOURCC_DXT4:
  case FOURCC_DXT5:
  case FOURCC_RXGB:
  case FOURCC_ATI2:
    return 16;
  };
  return 0; // Not a block image.
}

static int computePitch(int w, int bitsize, int alignment) {
  return ((w * bitsize +  8 * alignment - 1) / (8 * alignment)) * alignment;
}

static int mipmapSize(DDSHeader *H, int mipmap) {
  int m, w = H->width, h = H->height, d = H->depth;

  for (m = 0; m < mipmap; m++) {
    w = max(1U, w / 2);
    h = max(1U, h / 2);
    d = max(1U, d / 2);
  }

  if (H->pf.flags & DDPF_FOURCC) {
    // @@ How are 3D textures aligned?
    w = (w + 3) / 4;
    h = (h + 3) / 4;
    return blockSize(H) * w * h;
  }

  // Asuming 8 bit alignment, which is the same D3DX expects.
  return computePitch(w, H->pf.bitcount, 8) * h * d;
}

static int faceSize(DDSHeader *H) {
  int size = 0, m;
  for (m = 0; m < H->mipmapcount; m++) size += mipmapSize(H, m);
  return size;
}

static int offset(DDSHeader *H, int face, int mipmap) {
  int m;
  int size = 128; // sizeof(DDSHeader);
  if (H->pf.fourcc == FOURCC_DX10) size += 20; // sizeof(DDSHeader10);
  if (face != 0) size += face*faceSize(H);
  for (m = 0; m < mipmap; m++) size += mipmapSize(H, m);
  return size;
}


// Get pixel component shift and size given its mask.
static void maskShiftAndSize(u4 mask, u4 * shift, u4 * size) {
  if (!mask) {
    *shift = 0;
    *size = 0;
    return;
  }

  *shift = 0;
  while((mask & 1) == 0) {
    ++(*shift);
    mask >>= 1;
  }
    
  *size = 0;
  while((mask & 1) == 1) {
    ++(*size);
    mask >>= 1;
  }
}

// Convert component c having inbits to the returned value having outbits.
static int convert(u4 c, u4 inbits, u4 outbits) {
  if (inbits == 0) return 0;
  else if (inbits >= outbits) return c >> (inbits - outbits); // truncate
  // bitexpand
  return (c << (outbits - inbits)) | convert(c, inbits, outbits - inbits);
}

static u1 *decodeColors(u4 *Block, u1 *Q) {
  int i, j, R, G, B, A, RR,GG,BB,AA;
  u1 row[4];
  u4 Cs[4];
  u2 col0 = ru2(Q);
  u2 col1 = ru2(Q);
  *(u4*)row = ru4(Q);

  fromR5G6B5(R,G,B,col0);
  Cs[0] = R8G8B8A8(R, G, B, 0xFF);
  fromR5G6B5(R,G,B,col1);
  Cs[1] = R8G8B8A8(R, G, B, 0xFF);

  fromR8G8B8(R,G,B,Cs[0]);
  fromR8G8B8(RR,GG,BB,Cs[1]);
  if (col0 > col1) { // Four-color block: derive the other two colors.
    Cs[2] = R8G8B8A8((2*R+RR)/3, (2*G+GG)/3, (2*B+BB)/3, 0xFF);
    Cs[3] = R8G8B8A8((2*RR+R)/3, (2*GG+G)/3, (2*BB+B)/3, 0xFF);
  } else { // Three-color block: derive the other color.
    Cs[2] = R8G8B8A8((R+RR)/2, (G+GG)/2, (B+BB)/2, 0xFF);
    Cs[3] = 0; // Set to 0 to match DXT specs.
  }

  times (j, 4) times (i, 4) Block[j*4+i] = Cs[(row[j] >> (2*i))&3];
  return Q;
}


static void decodeAlpha(u1 *P, u1 *Is, u8 Alpha) {
  int I;
  u1 A1 = Alpha&0xFF;
  u1 A2 = (Alpha>>8)&0xFF;
  if (A1 > A2) {
    // 8-alpha block:  derive the other six alphas.
    // Bit code 000 = alpha0, 001 = alpha1, others are interpolated.
    P[0] = A1;
    P[1] = A2;
    P[2] = (6 * P[0] + 1 * P[1]) / 7;  // bit code 010
    P[3] = (5 * P[0] + 2 * P[1]) / 7;  // bit code 011
    P[4] = (4 * P[0] + 3 * P[1]) / 7;  // bit code 100
    P[5] = (3 * P[0] + 4 * P[1]) / 7;  // bit code 101
    P[6] = (2 * P[0] + 5 * P[1]) / 7;  // bit code 110
    P[7] = (1 * P[0] + 6 * P[1]) / 7;  // bit code 111
  }
  else {
    // 6-P block.
    // Bit code 000 = P0, 001 = P1, others are interpolated.
    P[0] = A1;
    P[1] = A2;
    P[2] = (4 * P[0] + 1 * P[1]) / 5;  // Bit code 010
    P[3] = (3 * P[0] + 2 * P[1]) / 5;  // Bit code 011
    P[4] = (2 * P[0] + 3 * P[1]) / 5;  // Bit code 100
    P[5] = (1 * P[0] + 4 * P[1]) / 5;  // Bit code 101
    P[6] = 0x00;                       // Bit code 110
    P[7] = 0xFF;                       // Bit code 111
  }

  Alpha >>= 16;
  times (I, 16) {
    Is[I] = Alpha&0x7;
    Alpha >>= 3;
  }
}


static u4 buildNormal(u1 x, u1 y) {
  float nx = 2 * (x / 255.0f) - 1;
  float ny = 2 * (y / 255.0f) - 1;
  float nz = 0.0f;
  if (1 - nx*nx - ny*ny > 0) nz = sqrtf(1 - nx*nx - ny*ny);
  return R8G8B8A8(x, y, min(max((int)(255.0f*(nz+1)/2.0f), 0), 255), 0xFF);
}

static u1 *readBlock(u4 *Block, DDSHeader *H, u1 *Q) {
  int i, j, R, G, B, A, RR,GG,BB,AA;
  u1 Palette[8];
  u1 Indices[16];
  if (H->pf.fourcc == FOURCC_DXT1) Q = decodeColors(Block, Q);
  else if (H->pf.fourcc == FOURCC_DXT2 || H->pf.fourcc == FOURCC_DXT3) {
    u8 Alpha = ru8(Q);
    Q = decodeColors(Block, Q);
    times (i, 16) {
      fromR8G8B8(R,G,B,Block[i]);
      Block[i] = R8G8B8A8(R,G,B,((Alpha&0xF)<<4)|0xF);
      Alpha >>= 4;
    }
  } else if (H->pf.fourcc == FOURCC_DXT4 ||
             H->pf.fourcc == FOURCC_DXT5 ||
             H->pf.fourcc == FOURCC_RXGB) {
    decodeAlpha(Palette, Indices, ru8(Q));
    Q = decodeColors(Block, Q);
    times (i, 16) {
      fromR8G8B8(R,G,B,Block[i]);
      Block[i] = R8G8B8A8(R,G,B,Palette[Indices[i]]);
    }
    if (H->pf.fourcc == FOURCC_RXGB) times (i, 16) {
      fromR8G8B8A8(R,G,B,A,Block[i]);
      Block[i] = R8G8B8A8(A,G,B,R);
    }
  } else if (H->pf.fourcc == FOURCC_ATI1) {
    decodeAlpha(Palette, Indices, ru8(Q));
    times (i, 16) {
      R=G=B=Palette[Indices[i]];
      Block[i] = R8G8B8A8(R,G,B,0xFF);
    }
  } else if (H->pf.fourcc == FOURCC_ATI2) {
    decodeAlpha(Palette, Indices, ru8(Q));
    times (i, 16) {
      fromR8G8B8A8(R,G,B,A,Block[i]);
      Block[i] = R8G8B8A8(Palette[Indices[i]],G,B,A);
    }
    decodeAlpha(Palette, Indices, ru8(Q));
    times (i, 16) {
      fromR8G8B8A8(R,G,B,A,Block[i]);
      Block[i] = R8G8B8A8(R,Palette[Indices[i]],0,0xFF);
    }
  } else {
    printf("  cant handle this DDS\n");
    abort();
  }

  // If normal flag set, convert to normal.
  if (H->pf.flags & DDPF_NORMAL) {
    if (H->pf.fourcc == FOURCC_ATI2) {
      times (i, 16) {
        fromR8G8B8A8(R,G,B,A,Block[i]);
        Block[i] = buildNormal(R, G);
      }
    } else if (H->pf.fourcc == FOURCC_DXT5) {
      times (i, 16) {
        fromR8G8B8A8(R,G,B,A,Block[i]);
        Block[i] = buildNormal(A, G);
      }
    }
  }

  return Q;
}


static pic *ddsLoad(char *Input) {
  pic *P;
  int i, j, bx, by, bw, bh, x, y, R,G,B,A,HasAlpha;
  int L = fileSize(Input);
  u1 *Q, *Z, *M;
  DDSHeader header;

  Q = M = readFile(0, L, Input);

  header.fourcc = ru4(Q);

  if (header.fourcc != FOURCC_DDS) {
    printf("  Not a DDS file\n");
    abort();
  }

  header.size = ru4(Q);
  header.flags = ru4(Q);
  header.height = ru4(Q);
  header.width = ru4(Q);
  header.pitch = ru4(Q);
  header.depth = ru4(Q);
  header.mipmapcount = ru4(Q);
  times (i, 11) header.reserved[i] = ru4(Q);

  header.pf.size = ru4(Q);
  header.pf.flags = ru4(Q);
  header.pf.fourcc = ru4(Q);
  header.pf.bitcount = ru4(Q);
  header.pf.rmask = ru4(Q);
  header.pf.gmask = ru4(Q);
  header.pf.bmask = ru4(Q);
  header.pf.amask = ru4(Q);

  header.caps.caps1 = ru4(Q);
  header.caps.caps2 = ru4(Q);
  header.caps.caps3 = ru4(Q);
  header.caps.caps4 = ru4(Q);

  header.notused = ru4(Q);

  if (header.pf.fourcc == FOURCC_DX10) {
    header.header10.dxgiFormat = ru4(Q);
    header.header10.resourceDimension = ru4(Q);
    header.header10.miscFlag = ru4(Q);
    header.header10.arraySize = ru4(Q);
    header.header10.reserved = ru4(Q);
  }


  dumpHeader(&header);

  Q = M+offset(&header, 0, 0);

  P = picNew(header.width, header.height, 32);

  if ((header.pf.flags&DDPF_RGB)||(header.pf.flags&DDPF_LUMINANCE)) { // linear?
    int byteCount = (header.pf.bitcount + 7) / 8;
    u4 rshift, rsize,  gshift, gsize, bshift, bsize, ashift, asize;
    maskShiftAndSize(header.pf.rmask, &rshift, &rsize);
    maskShiftAndSize(header.pf.gmask, &gshift, &gsize);
    maskShiftAndSize(header.pf.bmask, &bshift, &bsize);
    maskShiftAndSize(header.pf.amask, &ashift, &asize);

    times (y, P->H) {
      times (x, P->W) {
        u4 c = 0;
        times (j, byteCount) c |= *Q++ << (8*j);
        R = convert((c & header.pf.rmask) >> rshift, rsize, 8);
        G = convert((c & header.pf.gmask) >> gshift, gsize, 8);
        B = convert((c & header.pf.bmask) >> bshift, bsize, 8);
        // set image format: RGB or ARGB
        // alpha channel exists if and only if the alpha mask is non-zero
        if (header.pf.amask==0) A = 0xFF;
        else A = convert((c & header.pf.amask) >> ashift, asize, 8);
        picPut32(P, x, y, R8G8B8A8(R,G,B,A));
      }
    }
  } else if (header.pf.flags & DDPF_FOURCC) { //compressed?
    HasAlpha = 0;
    if (header.pf.fourcc == FOURCC_RXGB || header.pf.fourcc == FOURCC_ATI1 ||
        header.pf.fourcc == FOURCC_ATI2 || header.pf.flags & DDPF_NORMAL)
      HasAlpha = 1;
    bw = (P->W + 3) / 4;
    bh = (P->H + 3) / 4;
    times (by, bh) times (bx, bw) {
      u4 Block[4*4];
      Q = readBlock(Block, &header, Q);
      times (y, min(4, P->H - 4*by)) times (x, min(4, P->W - 4*bx)) {
        unless(HasAlpha) Block[y*4+x] |= 0xff000000;
        picPut32(P, 4*bx+x, 4*by+y, Block[y*4+x]);
      }
    }
  } else {
    printf("  cant handle this DDS\n");
    abort();
  }

  //abort();
  return P;
}

int ddsInit(format *F) {
  F->Type = FMT_IMAGE;
  F->Name = "dds";
  F->Description = "Direct Draw Surface (many games)";
  F->Load = ddsLoad;
  return 1;
}

