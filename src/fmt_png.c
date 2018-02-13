#include "libpng/png.h"
#include "common.h"

void pngSave(char *Output, pic *P) {
  png_structp Png;
  png_infop Info;
  int I, X, Y, BPR;
  u1 *Q, **Rows;
  FILE *F;
  png_color Pal[256];

  writeFile(0, 0, Output, Output);
  F = fopen(Output, "wb");
  if (!F) {
    printf("cant create %s\n", Output);
    abort();
  }
  Png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  Info = png_create_info_struct(Png);
  png_set_IHDR(Png,
               Info,
               P->W,
               P->H,
               P->B<8 ? P->B : 8, // depth of color channel
               P->B <= 8  ? PNG_COLOR_TYPE_PALETTE :
               P->B == 32 ? PNG_COLOR_TYPE_RGB_ALPHA :
                            PNG_COLOR_TYPE_RGB,
               PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_DEFAULT,
               PNG_FILTER_TYPE_DEFAULT);

  BPR = (P->W*P->B + 7)/8;
  Rows = png_malloc(Png, P->H * sizeof(png_byte *));
  if (P->B==8) {
    times (I, 256) {
      Pal[I].red = P->P[I*4+0];
      Pal[I].green = P->P[I*4+1];
      Pal[I].blue = P->P[I*4+2];
    }
    png_set_PLTE(Png, Info, Pal, 256);
    if (P->K!=-1) { // that is little ticky
      png_color_16 CK;
      CK.index = P->K; //just in case png uses it
      u1 Alpha[256];
      times (I, 256) Alpha[I] = I==P->K ? 0 : 0xFF;
      png_set_tRNS(Png, Info, Alpha, 256, &CK);
    }
    times (Y, P->H) {
      Rows[Y] = Q = png_malloc(Png, BPR);
      times (X, P->W) *Q++ = picGet(P,X,Y);
    }
  } else if (P->B==24) {
    times (Y, P->H) {
      Rows[Y] = Q = png_malloc(Png, BPR);
      times (X, P->W) {
        fromR8G8B8(Q[0],Q[1],Q[2], picGet24(P,X,Y));
        Q += 3;
      }
    }
  } else if (P->B==32) {
    times (Y, P->H) {
      Rows[Y] = Q = png_malloc(Png, BPR);
      times (X, P->W) {
        fromR8G8B8A8(Q[0],Q[1],Q[2],Q[3], picGet32(P,X,Y));
        Q += 4;
      }
    }
  } else {
    printf("  Cant save %d-bit PNGs\n", P->B);
    abort();
  }

  png_init_io(Png, F);
  png_set_rows(Png, Info, Rows);
  png_write_png(Png, Info, PNG_TRANSFORM_IDENTITY, NULL);

  times (Y, P->H) png_free(Png, Rows[Y]);
  png_free(Png, Rows);

  png_destroy_write_struct(&Png, &Info);
  fclose(F);
}


int pngInit(format *F) {
  F->Type = FMT_IMAGE;
  F->Name = "png";
  F->Description = "PNG images";
  F->Save = pngSave;
  return 1;
}

