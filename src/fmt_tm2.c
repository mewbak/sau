//Credits go to SCE


#include "common.h"

typedef struct {
    s4 fileid ;
    s1 version ;
    s1 format ;
    s2 n_pictures ;
    s1 zero[ 8 ] ;
} __attribute__ ((packed)) Tm2Header ;

typedef struct {
    s4 total_size ;
    s4 clut_size ;
    s4 image_size ;
    s2 header_size ;
    s2 clut_colors ;
    s1 pict_format ;
    s1 n_mipmaps ;
    s1 clut_type ;
    s1 image_type ;
    s2 image_width ;
    s2 image_height ;
    s4 gs_tex0[ 2 ] ;
    s4 gs_tex1[ 2 ] ;
    s4 gs_regs ;
    s4 gs_texclut ;
} __attribute__ ((packed)) Tm2Pict ;

typedef struct {
    s4 gs_miptbp1[ 2 ] ;
    s4 gs_miptbp2[ 2 ] ;
    s4 sizes[ 8 ] ;
} __attribute__ ((packed)) Tm2MipMap ;

enum {
    SCEGIM_FORMAT_RGBA5650        = 0,
    SCEGIM_FORMAT_RGBA5551        = 1,
    SCEGIM_FORMAT_RGBA4444        = 2,
    SCEGIM_FORMAT_RGBA8888        = 3,
    SCEGIM_FORMAT_INDEX4        = 4,
    SCEGIM_FORMAT_INDEX8        = 5,
    SCEGIM_FORMAT_INDEX16        = 6,
    SCEGIM_FORMAT_INDEX32        = 7,
    SCEGIM_FORMAT_DXT1        = 8,
    SCEGIM_FORMAT_DXT3        = 9,
    SCEGIM_FORMAT_DXT5        = 10,
    SCEGIM_FORMAT_DXT1EXT        = 264,
    SCEGIM_FORMAT_DXT3EXT        = 265,
    SCEGIM_FORMAT_DXT5EXT        = 266,

    /* obsolete */
    SCEGIM_RGBA_5650        = 0,
    SCEGIM_RGBA_5551        = 1,
    SCEGIM_RGBA_4444        = 2,
    SCEGIM_RGBA_8888        = 3,
    SCEGIM_INDEX_4            = 4,
    SCEGIM_INDEX_8            = 5,
    SCEGIM_INDEX_16            = 6,
    SCEGIM_INDEX_32            = 7
} ;

enum {
    TM2_FORMAT_RGBA5551    = 1,
    TM2_FORMAT_RGBA8880    = 2,
    TM2_FORMAT_RGBA8888    = 3,
    TM2_FORMAT_INDEX4    = 4,
    TM2_FORMAT_INDEX8    = 5,
} ;

static int PixelFormats[] = {
    -1,
    SCEGIM_FORMAT_RGBA5551,
    SCEGIM_FORMAT_RGBA8888,
    SCEGIM_FORMAT_RGBA8888,
    SCEGIM_FORMAT_INDEX4,
    SCEGIM_FORMAT_INDEX8,
} ;

static void copy_pixels( void *dst, const void *src, int width, int height, int type ) {
    int i, j, k, R, G, B, A;
    u1 *dp = (u1 *)dst ;
    u1 *sp = (u1 *)src ;

    switch ( type ) {
        case TM2_FORMAT_RGBA5551 : {
        for ( i = 0 ; i < height ; i ++ ) {
            for ( j = 0 ; j < width ; j ++ ) {
                fromR5G5B5A1(R,G,B,A, ru2(sp));
                *dp++ = R;
                *dp++ = G;
                *dp++ = B;
                *dp++ = A*0xFF;
            }
        }
        break ;
        }
        case TM2_FORMAT_RGBA8880 : {
        for ( i = 0 ; i < height ; i ++ ) {
            for ( j = 0 ; j < width ; j ++ ) {
                *dp++ = sp[ 0 ] ;
                *dp++ = sp[ 1 ] ;
                *dp++ = sp[ 2 ] ;
                *dp++ = 0xff ;
                sp += 3 ;
            }
        }
        break ;
        }
        case TM2_FORMAT_RGBA8888 : {
        for ( i = 0 ; i < height ; i ++ ) {
            for ( j = 0 ; j < width ; j ++ ) {
                int a = sp[ 3 ] * 255 / 128 ;
                if ( a > 255 ) a = 255 ;
                *dp++ = sp[ 0 ] ;
                *dp++ = sp[ 1 ] ;
                *dp++ = sp[ 2 ] ;
                *dp++ = a ;
                sp += 4 ;
            }
        }
        break ;
        }
        case TM2_FORMAT_INDEX4 : {
        for ( i = 0 ; i < height ; i ++ ) {
            for ( j  = k = 0 ; j < (width+1)/2; j++, *sp++) {
                if (k++<width) *dp++ = *sp&0xf;
                if (k++<width) *dp++ = (*sp>>4)&0xf;
            }
        }
        break ;
        }
        case TM2_FORMAT_INDEX8 : {
        for ( i = 0 ; i < height ; i ++ ) {
            for ( j = 0 ; j < width ; j ++ ) {
                *dp++ = *sp++;
            }
        }
        break ;
        }
    }
}

static void sort_clut( void *clut, int format, int width ) {
    int i, j;
    if ( width != 256 ) return ;

    u4 *ptr = (u4*)clut ;
    switch ( format ) {
        case SCEGIM_FORMAT_RGBA5551 : {
        for ( i = 0 ; i < 8 ; i ++ ) {
            for ( j = 0 ; j < 4 ; j ++ ) {
                u4 tmp = ptr[ 4 + j ] ;
                ptr[ 4 + j ] = ptr[ 8 + j ] ;
                ptr[ 8 + j ] = tmp ;
            }
            ptr += 16 ;
        }
        break ;
        }
        case SCEGIM_FORMAT_RGBA8888 : {
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                u4 tmp = ptr[8+j];
                ptr[8+j] = ptr[16+j];
                ptr[16+j] = tmp;
            }
            ptr += 32;
        }
        break ;
        }
    }
}


static pic *sceGimPictureLoadTm2( const void *buf, int size, int index ) {
    int i, j;
    pic *picture = 0;
    Tm2Header *header = (Tm2Header *)buf ;
    if ( index < 0 || index >= header->n_pictures ) return 0 ;
    Tm2Pict *pict = (Tm2Pict *)( header + 1 ) ;
    if ( header->format ) pict = (Tm2Pict *)( (u1 *)header + 128 ) ;
    for (i = 0; i < index; i++) pict = (Tm2Pict*)((u1*)pict + pict->total_size);

    picture = picNew(1,1,32); //some pics can hold just color map

    int clut_type = 7 & pict->clut_type ;
    if ( pict->n_mipmaps != 0 ) {
        int format = PixelFormats[pict->image_type];
        int width = pict->image_width ;
        int height = pict->image_height ;
        int level = pict->n_mipmaps ;
        int BPP = (format == SCEGIM_FORMAT_INDEX4 || format == SCEGIM_FORMAT_INDEX8) ? 8 : 32;
        picture = picNew(width,height,BPP);
        printf("  %dx%d Format=%d Pal=%d NColors=%d\n"
              ,width, height, format, PixelFormats[clut_type], pict->clut_colors);

        Tm2MipMap *mipmap = 0 ;
        if ( pict->n_mipmaps > 1 ) mipmap = (Tm2MipMap *)( pict + 1 ) ;
        void *ptr = (u1 *)pict + pict->header_size ;

        for ( j = 0 ; j < 1 /*pict->n_mipmaps*/ ; j ++ ) {
            copy_pixels(picture->D, ptr, width, height, pict->image_type);
            /*if ( mipmap != 0 ) {
                width = ( width + 1 ) / 2 ;
                height = ( height + 1 ) / 2 ;
                ptr = (char *)ptr + mipmap->sizes[ j ] ;
            }*/
        }
    }

    if ( clut_type != 0 ) {
        int clut_format = PixelFormats[clut_type];
        int clut_width = pict->clut_colors;
        void *ptr = (u1*)pict + pict->header_size + pict->image_size;
        free(picture->P);
        void *ptr2 = picture->P = ns(u1, clut_width*4);
        if (!(pict->clut_type&0x80)) {
          sort_clut(ptr, clut_format, clut_width);
          pict->clut_type |= 0x80;
        }
        copy_pixels(ptr2, ptr, clut_width, 1, clut_type);
    }
    return picture;
}

static void tm2Decompile(char *Output, char *Input) {
  char Tmp[1024], Name[256];
  int I, J, L=fileSize(Input);
  u1 *Q, *M = readFile(0, L, Input);
  pic *P;

  pathParts(0, Name, 0, Input);

  for (I=0; ((P = sceGimPictureLoadTm2(M, L, I))); I++) {
    sprintf(Tmp, "%s/%s_%03d.png", Output, Name, I);
    pngSave(Tmp, P);
  }

}

int tm2Init(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "tm2";
  F->Description = "PSP Sprite";
  F->Decompile = tm2Decompile;
  return 1;
}


