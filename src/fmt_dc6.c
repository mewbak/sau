#include "common.h"
#include "diablo2_pal.h"



// Credits go to Paul Siramy for his outstanding work documenting D2 formats.


typedef struct
{
   int version;
   int unknown1;
   int unknown2;
   int termination[4];
   int directions;
   int frames_per_dir;
} DC6HEADER;

typedef struct
{
	int unknown1;
	int width;
	int height;
	int offset_x;
	int offset_y;
	int unknown2;
	int next_block;
	int length;
} DC6BLOCKHEADER;

typedef struct
{
   char           filename[257];
   DC6HEADER      dc6header;
   DC6BLOCKHEADER * dc6blockheader;
   int           * block_ptr;
   pic            * bmp;
} HEADERS_ELEMENTS;

typedef struct
{
   char * palette;
   char * background;
   int    verbose;
   int    skill;
   int    blur;
   int    from;
   int    to;
} PARAMS;

typedef struct
{
   pic  * bmp;
   int    width;
   int    height;
   int    min_x;
   int    min_y;
   int    max_x;
   int    max_y;
} BOX;

static HEADERS_ELEMENTS body, skill;
static PARAMS           glb_prm;
static BOX              box;


// ==========================================================================
static int read_dc6_header(FILE * in, HEADERS_ELEMENTS * elm)
{
   DC6HEADER * ptr = & elm->dc6header;
   int      * lptr;
   int       i, nb;
   
   fread(&ptr->version,        4, 1, in);
   fread(&ptr->unknown1,       4, 1, in);
   fread(&ptr->unknown2,       4, 1, in);
   fread(&ptr->termination[0], 1, 1, in);
   fread(&ptr->termination[1], 1, 1, in);
   fread(&ptr->termination[2], 1, 1, in);
   fread(&ptr->termination[3], 1, 1, in);
   fread(&ptr->directions,     4, 1, in);
   fread(&ptr->frames_per_dir, 4, 1, in);

   if (glb_prm.verbose)
   {
      printf("version     : %li\n", ptr->version);
      printf("unknown1    : %li\n", ptr->unknown1);
      printf("unknown2    : %li\n", ptr->unknown2);
      printf("termination : %02X %02X %02X %02X\n",
         ptr->termination[0],
         ptr->termination[1],
         ptr->termination[2],
         ptr->termination[3]);
      printf("directions     : %li\n",   ptr->directions);
      printf("frames_per_dir  : %li\n\n", ptr->frames_per_dir);
   }

   nb = ptr->frames_per_dir;
   
   elm->block_ptr = (int *) malloc(sizeof(int) * nb);
   if (elm->block_ptr == NULL)
   {
      printf("not enough memory for %li block pointers\n", nb);
      return 1;
   }
   
   elm->dc6blockheader = (DC6BLOCKHEADER *) malloc(sizeof(DC6BLOCKHEADER) * nb);
   if (elm->dc6blockheader == NULL)
   {
      printf("not enough memory for %li block headers\n", nb);
      return 1;
   }
   
   lptr = elm->block_ptr;
   for (i=0; i<nb; i++)
   {
      fread(lptr, 4, 1, in);
      if (glb_prm.verbose)
         printf("   block pointer %3i : %li\n", i, * lptr);
      lptr++;
   }
   return 0;
}

// ==========================================================================
static void read_dc6_blockheader(FILE * in, int i, HEADERS_ELEMENTS * elm)
{
   DC6BLOCKHEADER * bh = elm->dc6blockheader + i, * bh_old;
   int tmp_off, hole;
   int  x;
   int old_seek;
   
   fseek(in, * (elm->block_ptr + i), SEEK_SET);

   fread(&bh->unknown1,   4, 1, in);
   fread(&bh->width,      4, 1, in);
   fread(&bh->height,     4, 1, in);
   fread(&bh->offset_x,   4, 1, in);
   fread(&bh->offset_y,   4, 1, in);
   fread(&bh->unknown2,   4, 1, in);
   fread(&bh->next_block, 4, 1, in);
   fread(&bh->length,     4, 1, in);
   
   if (i > 0)
   {
      tmp_off =  * (elm->block_ptr + i - 1);
      tmp_off += 4 * 8;
      bh_old  =  elm->dc6blockheader + i - 1;
      tmp_off += bh_old->length;
      hole    =  * (elm->block_ptr + i) - tmp_off;
      if (hole && glb_prm.verbose)
      {
         printf("\n      HOLE       : %li ( ", hole);
         old_seek = ftell(in);
         fseek(in, * (elm->block_ptr + i) - hole, SEEK_SET);
         for (x=0; x<hole; x++)
            printf("%02X ", fgetc(in));
         printf(")\n");
         fseek(in, old_seek, SEEK_SET);
      }
   }
   if (glb_prm.verbose)
   {
      printf("\n   block header %03i (offset %li in file):\n", i, * (elm->block_ptr+i));
      printf("      unknown1   : %li\n", bh->unknown1);
      printf("      width      : %li\n", bh->width);
      printf("      height     : %li\n", bh->height);
      printf("      offset_x   : %li\n", bh->offset_x);
      printf("      offset_y   : %li\n", bh->offset_y);
      printf("      unknown2   : %li\n", bh->unknown2);
      printf("      next_block : %li\n", bh->next_block);
      printf("      length     : %li\n", bh->length);
   }
}

// ==========================================================================
static void search_box(void)
{
   DC6BLOCKHEADER * bh, * bh_ski;
   int           nb, i, min_x, max_x, min_y, max_y, x1, y1, x2, y2;

   min_x = 2000000L;
   max_x = -2000000L;
   min_y = 2000000L;
   max_y = -2000000L;

   nb = body.dc6header.frames_per_dir;
   if (glb_prm.verbose)
      printf("\nanalyse dc6 \"body\" file\n");
   bh = body.dc6blockheader;
   for (i=0; i<nb; i++)
   {
      if (glb_prm.verbose)
         printf("frame %3li : %3li, %3li (%4li, %4li) ---> ",
            i, bh->width, bh->height, bh->offset_x, bh->offset_y);
      x1 = bh->offset_x;
      y1 = bh->offset_y;
      x2 = x1 + bh->width;
      y2 = y1 - bh->height;
      
      if (x1<min_x) min_x = x1;
      if (x2>max_x) max_x = x2;
      if (y1<min_y) min_y = y1;
      if (y2>max_y) max_y = y2;
      
      if (x2<min_x) min_x = x2;
      if (x1>max_x) max_x = x1;
      if (y2<min_y) min_y = y2;
      if (y1>max_y) max_y = y1;
      
      if (glb_prm.verbose)
         printf("(%4li, %4li) TO (%4li, %4li)\n", x1, y1, x2, y2);
      bh++;
   }

   if (glb_prm.skill)
   {
      nb = skill.dc6header.frames_per_dir;
      if (glb_prm.verbose)
         printf("\nanalyse dc6 \"skill\" file\n");
      bh = body.dc6blockheader;
      bh_ski = skill.dc6blockheader;
      for (i=0; i<nb; i++)
      {
         if (glb_prm.verbose)
            printf("frame %3li : %3li, %3li (%4li, %4li) ---> ",
               i, bh_ski->width, bh_ski->height, bh_ski->offset_x, bh_ski->offset_y);

//      off_x = bod_bh->offset_x - box.min_x;
//      off_y = bod_bh->offset_y - box.min_y;

//      off_x = ski_bh->offset_x - box.min_x ;
//      off_y = bod_bh->height + ski_bh->offset_y - ski_bh->height;

         x1 = bh_ski->offset_x;
         y1 = bh_ski->offset_y;
//         y1 = bh->height + bh_ski->offset_y - bh_ski->height;
         x2 = x1 + bh_ski->width;
         y2 = y1 - bh_ski->height;

         if (x1<min_x) min_x = x1;
         if (x2>max_x) max_x = x2;
         if (y1<min_y) min_y = y1;
         if (y2>max_y) max_y = y2;
      
         if (x2<min_x) min_x = x2;
         if (x1>max_x) max_x = x1;
         if (y2<min_y) min_y = y2;
         if (y1>max_y) max_y = y1;
      
         if (glb_prm.verbose)
            printf("(%4li, %4li) TO (%4li, %4li)\n", x1, y1, x2, y2);
         bh++;
         bh_ski++;
      }
   }

   box.width  = max_x - min_x + 1;
   box.height = max_y - min_y + 1;
   box.min_x  = min_x;
   box.min_y  = min_y;
   box.max_x  = max_x;
   box.max_y  = max_y;
   if (glb_prm.verbose)
   {
      printf("\nRESULT :\n");
      printf("min x, y = %4li, %4li\n", min_x, min_y);
      printf("max x, y = %4li, %4li\n", max_x, max_y);
      printf(" ==> box = %4li, %4li\n", box.width, box.height);
   }
}

// ==========================================================================
static int decompress_dc6(FILE * in, int frame, HEADERS_ELEMENTS * elm)
{
   DC6BLOCKHEADER * bh;
   int           i, i2;
   int            c, c2, x, y, color;
   
   bh = elm->dc6blockheader + frame;
   elm->bmp = picNew(bh->width, bh->height, 8);
   picClear(elm->bmp, 0);
   fseek(in, (* (elm->block_ptr + frame)) + 32, SEEK_SET);
   x = 0;
   y = bh->height - 1;

   for (i=0; i<bh->length; i++)
   {
      c = fgetc(in);
      if (c == 0x80)
      {
         x = 0;
         y--;
      }
      else if (c & 0x80)
         x += c & 0x7F;
      else
      {
         for (i2=0; i2<c; i2++)
         {
            picPut(elm->bmp, x, y, fgetc(in));
            x++;
         }
      }
   }
   return 0;
}


// ==========================================================================
static void make_frame(int frame)
{
   DC6BLOCKHEADER * bod_bh, * ski_bh;
   int            off_x, off_y;
   int            x, y, s, d, sr, sg ,sb ,dr ,dg ,db, fr, fg, fb, pink;

   picClear(box.bmp, 0); // BLACK
   
   if (frame < body.dc6header.frames_per_dir)
   {
      bod_bh = body.dc6blockheader  + frame;
      off_x = bod_bh->offset_x - box.min_x;
      off_y = bod_bh->offset_y - box.min_y - bod_bh->height;
      picBlt(box.bmp, body.bmp, 0, off_x, off_y, 0, 0, body.bmp->W, body.bmp->H);
   }
}



static spr *dc6Load(char *Input) {
  FILE * body_f;
  int I, N;
  spr *S;
  pic *P;

  memset(&body,  0, sizeof (body));
  memset(&skill, 0, sizeof (skill));
  memset(&glb_prm, 0, sizeof (glb_prm));

  body_f = fopen(Input, "rb");  strcpy(body.filename, Input);
   
  if (!read_dc6_header(body_f, &body))
    times (I, body.dc6header.frames_per_dir)
      read_dc6_blockheader(body_f, I, &body);

  search_box();

  box.bmp = picNew(box.width, box.height, 8);

  S = sprNew();
  S->Palette = ns(u1,4*256);
  memcpy(S->Palette, diablo2_pal_units, 4*256);

  S->Palette[0*4+0] = 0x00;
  S->Palette[0*4+1] = 0xFF;
  S->Palette[0*4+2] = 0xFF;


  // FIXME: some dc6 files (like Mephisto) have more than 1 direction
  S->NAnis = 1;
  S->Anis = ns(ani, S->NAnis);
  S->Anis[0].NFacs = 1;
  S->Anis[0].Facs = ns(fac,S->Anis[0].NFacs);
  S->Anis[0].Facs[0].NPics = body.dc6header.frames_per_dir;
  S->Anis[0].Facs[0].Pics = ns(pic*, S->Anis[0].Facs[0].NPics);

  times (I, S->Anis[0].Facs[0].NPics) {
    decompress_dc6(body_f, I, &body);
    make_frame(I);
    picDel(body.bmp);
    P = S->Anis[0].Facs[0].Pics[I] = picDup(box.bmp);
    P->SharedPalette = 1;
    free(P->P);
    P->P = S->Palette;
  }

  picDel(box.bmp);
  free(body.block_ptr);
  free(body.dc6blockheader);
  return S;
}

int dc6Init(format *F) {
  F->Type = FMT_SPRITE;
  F->Name = "dc6";
  F->Description = "Diablo 2 sprites";
  F->Load = dc6Load;
  return 1;
}



