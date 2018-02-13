#include "common.h"
#include "diablo2_pal.h"



// Credits go to Paul Siramy for his outstanding work documenting D2 formats.

#define DCC_MAX_DIR    32
#define DCC_MAX_FRAME 256

#define DCC_MAX_PB_ENTRY 85000


typedef struct {
   s4 xmin;
   s4 ymin;
   s4 xmax;
   s4 ymax;
   s4 width;
   s4 height;
} DCC_BOX_S;

typedef struct {
   int  x0, y0;  // for frame cells in stage 2
   int  w, h;

   int last_w, last_h;   // width & size of the last frame cell that used
                            // this buffer cell (for stage 2)
   int last_x0, last_y0;
   
   pic * bmp;   // sub-bitmap in the buffer bitmap
} DCC_CELL_S; // maybe I'll make 2 kind of cells, 1 for frame-buffer cells,
              // the other for the frame cells...

typedef struct {
   u4 variable0;
   u4 width;
   u4 height;
   s4 xoffset;
   s4 yoffset;
   u4 optional_bytes;
   u4 coded_bytes;

   u1  * optional_bytes_data;

   // next var : u1 should be enough
   //    but my bitstream reading function need a 32 bits wide variable
   u4 bottom_up;

   // cells infos
   DCC_CELL_S * cell;
   int        nb_cells_w;
   int        nb_cells_h;

   // not in file, for my own purpose
   DCC_BOX_S box;

   // final bitmap
   pic * bmp;
} DCC_FRAME_S;

typedef struct {
   u1  * data;      // pointer to the start of the bitstream
   u4 size;        // size of the bitstream, in bits
   u4 cur_byte;    // byte cursor
   u1  cur_bit;     // bit cursor in the byte of current byte cursor
                       //    (from 0 to 7 : lowest bit to highest bit)
   u4 cur_bit_num; // bit cursor in the bitstream (not in the byte)
                       //    this also indicate the # of bits already read
} DCC_BITSTREAM_S;

typedef struct // PixelBuffer entry
{
   u1      val[4];
   int     frame;
   int     frame_cell_index;
} DCC_PB_ENTRY_S;

typedef struct
{
   u4 outsize_coded;
   u4 compression_flag;

   // next var : u1 should be enough
   //    but my bitstream reading function need a 32 bits wide variable
   u4          variable0_bits;
   u4          width_bits;
   u4          height_bits;
   u4          xoffset_bits;
   u4          yoffset_bits;
   u4          optional_bytes_bits;
   u4          coded_bytes_bits;

   // bitstreams
   u4          equal_cell_bitstream_size;
   u4          pixel_mask_bitstream_size;
   u4          encoding_type_bitstream_size;
   u4          raw_pixel_bitstream_size;
   DCC_BITSTREAM_S equal_cell_bitstream;
   DCC_BITSTREAM_S pixel_mask_bitstream;
   DCC_BITSTREAM_S encoding_type_bitstream;
   DCC_BITSTREAM_S raw_pixel_bitstream;
   DCC_BITSTREAM_S pixel_code_and_displacment_bitstream;
   
   // frame buffer size, in # of cells, used in stage 2
   int             nb_cells_w;
   int             nb_cells_h;
   pic          * bmp;        // frame buffer bitmap
   DCC_CELL_S      * buffer_ptr; // frame buffer cells
   
   // pixel buffer (1 entry = 4 pixels code & some other datas)
   DCC_PB_ENTRY_S  * pixel_buffer;
   int             pb_nb_entry;

   // colors present in the frame
   u1              pixel_values[256]; // for pixel code color conversion

   // not in file, for my own purpose
   DCC_BOX_S       box;
} DCC_DIRECTION_S;

typedef struct
{
   u1 file_signature;
   u1 version;
   u1 directions;
   s4  frames_per_dir;
   s4  tag; // always 1. A data's presence bitfield maybe,
              //    like 0x01 = "final_dc6_size data present" ?
   s4  final_dc6_size;
   s4  dir_offset [DCC_MAX_DIR];

   int   already_decoded;
} DCC_HEADER_S;

typedef struct
{
   u1            * ptr; // copy of the dcc in mem
   s4              size;  // size of the dcc in mem (in bytes)
   DCC_HEADER_S    header;
   DCC_DIRECTION_S direction [DCC_MAX_DIR];
   DCC_FRAME_S     frame     [DCC_MAX_DIR] [DCC_MAX_FRAME];
   DCC_BOX_S       box;
} DCC_S;

// if an error is found while using a function of this
// dcc "library", this variable contain a string that
// explain what happened. You should display it somewhere
// to inform the user
static char dcc_error[512];

static int dcc_bits_width_table[16] =
  {0, 1, 2, 4, 6, 8, 10, 12, 14, 16, 20, 24, 26, 28, 30, 32};


static void dcc_init();
static int dcc_read_bytes(DCC_BITSTREAM_S * bs, int bytes_number, void * dest);
static int dcc_read_bits(DCC_BITSTREAM_S * bs, int bits_number, int is_signed, u4 * dest);
static int dcc_frame_header_bitstream(DCC_S * dcc, DCC_BITSTREAM_S * bs, int d, int f);
static void dcc_init_dir_bitstream(DCC_S * dcc, DCC_BITSTREAM_S * bs, int d);
static int dcc_optional_data(DCC_S * dcc, DCC_BITSTREAM_S * bs, int d);
static int dcc_other_bitstream_size(DCC_S * dcc, DCC_BITSTREAM_S * bs, int d);
static int dcc_pixel_values_key(DCC_S * dcc, DCC_BITSTREAM_S * bs, int d);
static int dcc_dir_bitstream(DCC_S * dcc, int d);
static int dcc_file_header(DCC_S * dcc);
static int dcc_decode(DCC_S * dcc, s4 dir_bitfield);
static void dcc_debug(DCC_S * dcc);
static DCC_S *dcc_disk_load(char * dcc_name);
static DCC_S *dcc_mem_load(void * mem_ptr, int mem_size);
static void  dcc_destroy(DCC_S * dcc);


static pic *create_sub_bitmap(pic *S, int X, int Y, int W, int H) {
  // dunno why, allegro allows null-pointers as S
  if (S) return picProxy(S, X, Y, W, H);
  return picNew(W, H, 8);
}

static pic *create_bitmap(int W, int H) {
  return picNew(W, H, 8);
}

static void destroy_bitmap(pic *P) {}

// ==========================================================================
// copy 'bytes_number' bytes into 'dest' from the current byte position
//    in the bitstream
// return 0 on success, non-zero if error
static int dcc_read_bytes(DCC_BITSTREAM_S * bs, int bytes_number, void * dest)
{
   int i;

   // in case
   if (bs->cur_bit)
   {
      sprintf(dcc_error, "dcc_read_bytes() : bytes reading must be aligned "
                         "on a byte boundary (cur_bit = %i)\n", bs->cur_bit);
      return 1;
   }
   
   // copy bytes
   for (i=0; i < bytes_number; i++)
   {
      if (bs->cur_bit_num >= bs->size)
      {
         sprintf(dcc_error, "dcc_read_bytes() : want to read behind "
                            "the end (%li >= %li)\n",
                            bs->cur_bit_num, bs->size);
         return 1;
      }
      * (((u1 *) dest) + i) = bs->data[bs->cur_byte];
      bs->cur_byte ++;
      bs->cur_bit_num += 8;
   }

   // end
   return 0;
}


// ==========================================================================
// copy 'bits_number' bits into 'dest' from the current byte & bit position
//    in the bitstream
// is_signed = 1 / 0
// the dest variable MUST BE 32 bits width
// return 0 on success, non-zero if error
static int dcc_read_bits(DCC_BITSTREAM_S * bs, int bits_number, int is_signed,
                  u4 * dest )
{
   int b, dest_bit = 0, dest_byte = 0;


   // init to zero
   * dest = 0;
   
/*
   // no bits to read
   if (bits_number == 0)
      return 0; // reading 0 bits is NOT an error, this is to avoid some
                // checks if the data is 0 bit size before reading them,
                // this check is done here

   TODO ? : if want to read a bitstream which don't exists (size = 0),
      return a 0. Seems a good idea. Might remove many further checks...
*/

   // in case
   if (bits_number < 0)
   {
      sprintf(dcc_error, "dcc_read_bits() : can't read %i bits\n",
         bits_number);
      return 1;
   }

   if (bits_number > 32)
   {
      sprintf(dcc_error, "dcc_read_bits() : can't read more than 32 bits "
                         "at once\n");
      return 1;
   }

   // copy all necessary bits
   for (b=0; b < bits_number; b++)
   {
      if (bs->cur_bit_num >= bs->size)
      {
         sprintf(dcc_error, "dcc_read_bits() : want to read behind "
                            "the end (%li >= %li)\n",
                            bs->cur_bit_num, bs->size);
         return 1;
      }

      // copy 1 bit
      if ( bs->data[bs->cur_byte] & (1 << bs->cur_bit) )
         * (((u1 *) dest) + dest_byte) |= (1 << dest_bit);

      // prepare next bit to read
      dest_bit++;
      if (dest_bit >= 8)
      {
         dest_bit = 0;
         dest_byte++;
      }
      bs->cur_bit++;
      bs->cur_bit_num++;
      if (bs->cur_bit >= 8)
      {
         bs->cur_bit = 0;
         bs->cur_byte++;
      }
   }

   // signed value handle
   if (!is_signed) return 0;
      
   // else it's a signed value (1 bit value will be 0 or -1, NOT 1)
   // so check if we have read a negative value
   if ((* dest) & (1 << (bits_number-1)) )
   {
      // negative : extend its sign
      (* dest) |= ~ ((1 << bits_number) - 1);
   }

   return 0;
}


// ==========================================================================
// read 1 frame header, from a direction bitstream
// return 0 on success, non-zero if error
static int dcc_frame_header_bitstream(DCC_S *dcc, DCC_BITSTREAM_S *bs, int d, int f)
{
   DCC_DIRECTION_S * dir = & dcc->direction[d];
   DCC_FRAME_S     * frm = & dcc->frame[d][f];
   int             sign, width, i;
   void            * ptr;
   char            add_error[80], str[30];

   
   // read frame datas
   for (i=0; i<8; i++)
   {
      sign = 0;
      switch(i)
      {
         case 0 :
            width = dir->variable0_bits;
            ptr   = & frm->variable0;
            strcpy(str, "variable0");
            break;
            
         case 1 :
            width = dir->width_bits;
            ptr   = & frm->width;
            strcpy(str, "width");
            break;
            
         case 2 :
            width = dir->height_bits;
            ptr   = & frm->height;
            strcpy(str, "height");
            break;
            
         case 3 :
            sign  = 1;
            width = dir->xoffset_bits;
            ptr   = & frm->xoffset;
            strcpy(str, "xoffset");
            break;
            
         case 4 :
            sign  = 1;
            width = dir->yoffset_bits;
            ptr   = & frm->yoffset;
            strcpy(str, "yoffset");
            break;
            
         case 5 :
            width = dir->optional_bytes_bits;
            ptr   = & frm->optional_bytes;
            strcpy(str, "optional_bytes");
            break;
            
         case 6 :
            width = dir->coded_bytes_bits;
            ptr   = & frm->coded_bytes;
            strcpy(str, "coded_bytes");
            break;
            
         case 7 :
            width = 1;
            ptr   = & frm->bottom_up;
            strcpy(str, "bottom_up");
            break;
      }
      if (dcc_read_bits(bs, dcc_bits_width_table[width], sign, ptr))
      {
         sprintf(add_error, "frame header %i, %s\n", f, str);
         strcat(dcc_error, add_error);
         return 1;
      }
   }
      
   // frame box
   dcc->frame[d][f].box.xmin = dcc->frame[d][f].xoffset;
   dcc->frame[d][f].box.xmax =
      dcc->frame[d][f].box.xmin + dcc->frame[d][f].width - 1;

   if (dcc->frame[d][f].bottom_up) // bottom-up
   {
      dcc->frame[d][f].box.ymin = dcc->frame[d][f].yoffset;
      dcc->frame[d][f].box.ymax =
         dcc->frame[d][f].box.ymin + dcc->frame[d][f].height - 1;
   }
   else // top-down
   {
      dcc->frame[d][f].box.ymax = dcc->frame[d][f].yoffset;
      dcc->frame[d][f].box.ymin =
         dcc->frame[d][f].box.ymax - dcc->frame[d][f].height + 1;
   }
   dcc->frame[d][f].box.width =
      dcc->frame[d][f].box.xmax - dcc->frame[d][f].box.xmin + 1;
   dcc->frame[d][f].box.height =
      dcc->frame[d][f].box.ymax - dcc->frame[d][f].box.ymin + 1;

   // end
   return 0;
}


// ==========================================================================
// init the bitstreams for this direction
static void dcc_init_dir_bitstream(DCC_S * dcc, DCC_BITSTREAM_S * bs, int d)
{
   DCC_BITSTREAM_S * ec, * pm, * et, * rp, * pcd;
   u4          glb_bit = (bs->cur_byte * 8) + bs->cur_bit;


   // init temporary bitstream pointers
   ec  = & dcc->direction[d].equal_cell_bitstream;
   pm  = & dcc->direction[d].pixel_mask_bitstream;
   et  = & dcc->direction[d].encoding_type_bitstream;
   rp  = & dcc->direction[d].raw_pixel_bitstream;
   pcd = & dcc->direction[d].pixel_code_and_displacment_bitstream;
   
   // equal cell bitstream
   if (dcc->direction[d].compression_flag & 0x02)
   {
      ec->data        = bs->data + (glb_bit / 8);
      ec->cur_bit     = (u1) (glb_bit % 8);
      ec->cur_bit_num = 0;
      ec->cur_byte    = 0;
      ec->size        = dcc->direction[d].equal_cell_bitstream_size;
      glb_bit        += ec->size;
   }

   // pixel mask bitstream
   pm->data        = bs->data + (glb_bit / 8);
   pm->cur_bit     = (u1) (glb_bit % 8);
   pm->cur_bit_num = 0;
   pm->cur_byte    = 0;
   pm->size        = dcc->direction[d].pixel_mask_bitstream_size;
   glb_bit        += pm->size;
   
   if (dcc->direction[d].compression_flag & 0x01)
   {
      // encoding type bitstream
      et->data        = bs->data + (glb_bit / 8);
      et->cur_bit     = (u1) (glb_bit % 8);
      et->cur_bit_num = 0;
      et->cur_byte    = 0;
      et->size        = dcc->direction[d].encoding_type_bitstream_size;
      glb_bit        += et->size;

      // raw pixel bitstream
      rp->data        = bs->data + (glb_bit / 8);
      rp->cur_bit     = (u1) (glb_bit % 8);
      rp->cur_bit_num = 0;
      rp->cur_byte    = 0;
      rp->size        = dcc->direction[d].raw_pixel_bitstream_size;
      glb_bit        += rp->size;
   }

   // pixel code and displacment bitstream
   pcd->data        = bs->data + (glb_bit / 8);
   pcd->cur_bit     = (u1) (glb_bit % 8);
   pcd->cur_bit_num = 0;
   pcd->cur_byte    = 0;
   pcd->size        = bs->size - glb_bit;

// debug
/*
printf("current bitstream\n");
printf("   data        = %p\n",  bs->data);
printf("   size        = %lu\n", bs->size);
printf("   cur_byte    = %lu\n", bs->cur_byte);
printf("   cur_bit     = %i\n",  bs->cur_bit);
printf("   cur_bit_num = %lu\n", bs->cur_bit_num);

printf("equal cell bitstream\n");
printf("   data        = %p\n",  ec->data);
printf("   size        = %lu\n", ec->size);
printf("   cur_byte    = %lu\n", ec->cur_byte);
printf("   cur_bit     = %i\n",  ec->cur_bit);
printf("   cur_bit_num = %lu\n", ec->cur_bit_num);

printf("pixel mask bitstream\n");
printf("   data        = %p\n",  pm->data);
printf("   size        = %lu\n", pm->size);
printf("   cur_byte    = %lu\n", pm->cur_byte);
printf("   cur_bit     = %i\n",  pm->cur_bit);
printf("   cur_bit_num = %lu\n", pm->cur_bit_num);

printf("encoding type cell bitstream\n");
printf("   data        = %p\n",  et->data);
printf("   size        = %lu\n", et->size);
printf("   cur_byte    = %lu\n", et->cur_byte);
printf("   cur_bit     = %i\n",  et->cur_bit);
printf("   cur_bit_num = %lu\n", et->cur_bit_num);

printf("raw_pixel bitstream\n");
printf("   data        = %p\n",  rp->data);
printf("   size        = %lu\n", rp->size);
printf("   cur_byte    = %lu\n", rp->cur_byte);
printf("   cur_bit     = %i\n",  rp->cur_bit);
printf("   cur_bit_num = %lu\n", rp->cur_bit_num);

printf("pixel code & displacment cell bitstream\n");
printf("   data        = %p\n",  pcd->data);
printf("   size        = %lu\n", pcd->size);
printf("   cur_byte    = %lu\n", pcd->cur_byte);
printf("   cur_bit     = %i\n",  pcd->cur_bit);
printf("   cur_bit_num = %lu\n", pcd->cur_bit_num);
*/
}


// ==========================================================================
// read the optional datas (if any) from 1 direction bitstream
// return 0 on success, non-zero if error
static int dcc_optional_datas(DCC_S * dcc, DCC_BITSTREAM_S * bs, int d)
{
   u4 size = 0;
   int    f;
   char   add_error[80];

   
   // check if any optional datas
   for (f=0; f < dcc->header.frames_per_dir; f++)
      size += dcc->frame[d][f].optional_bytes;
   if (size == 0)
      return 0; // nothing to read
      
   // byte boundary
   if (bs->cur_bit)
   {
      // skip alignment bits
      bs->cur_bit_num += 8 - bs->cur_bit;
      bs->cur_bit = 0;
      bs->cur_byte ++;
   }

   // read the datas
   for (f=0; f < dcc->header.frames_per_dir; f++)
   {
      size = dcc->frame[d][f].optional_bytes;
      if (size)
      {
         // malloc
         dcc->frame[d][f].optional_bytes_data = (u1 *) malloc (size);
         if (dcc->frame[d][f].optional_bytes_data == NULL)
         {
            sprintf(dcc_error,
               "dcc_optional_datas() : can't allocate "
               "%li bytes for optional_byte_data\n"
               "direction %i, frame %i\n",
               size, d, f);
            return 1;
         }

         // get them
         if (dcc_read_bytes(bs, size, dcc->frame[d][f].optional_bytes_data))
         {
            sprintf(add_error, "direction %i, frame %i\n", d, f);
            strcat(dcc_error, add_error);
            return 1;
         }
      }
   }

   // end
   return 0;
}


// ==========================================================================
// read the size of the other bitstreams from 1 direction bitstream
// return 0 on success, non-zero if error
static int dcc_other_bitstream_size(DCC_S * dcc, DCC_BITSTREAM_S * bs, int d)
{
   char add_error[80];

   
   // equal cell bitstream
   if (dcc->direction[d].compression_flag & 0x02)
   {
      if (dcc_read_bits(bs, 20, 0,
             & dcc->direction[d].equal_cell_bitstream_size))
      {
         sprintf(add_error,
                 "direction %i, equal_cell_bitstream_size\n", d);
         strcat(dcc_error, add_error);
         return 1;
      }
   }
   
   // pixel mask bitstream
   if (dcc_read_bits(bs, 20, 0,
          & dcc->direction[d].pixel_mask_bitstream_size))
   {
      sprintf(add_error,
              "direction %i, pixel_mask_bitstream_size\n", d);
      strcat(dcc_error, add_error);
      return 1;
   }
   
   if (dcc->direction[d].compression_flag & 0x01)
   {
      // encoding type bitstream
      if (dcc_read_bits(bs, 20, 0,
             & dcc->direction[d].encoding_type_bitstream_size))
      {
         sprintf(add_error,
                 "direction %i, encoding_type_bitstream_size\n", d);
         strcat(dcc_error, add_error);
         return 1;
      }

      // raw pixel bitstream
      if (dcc_read_bits(bs, 20, 0,
             & dcc->direction[d].raw_pixel_bitstream_size))
      {
         sprintf(add_error,
                 "direction %i, raw_pixel_bitstream_size\n", d);
         strcat(dcc_error, add_error);
         return 1;
      }
   }

   // end
   return 0;
}


// ==========================================================================
// read the 256 bits of the pixel values key from 1 direction bitstream
// TODO : may need big optimization... but that's not a process that is
//   use heavilly in a dcc decoding, so I can also left it in this state
// return 0 on success, non-zero if error
static int dcc_pixel_values_key(DCC_S * dcc, DCC_BITSTREAM_S * bs, int d)
{
   char   add_error[80];
   int    i, idx = 0;
   u4 tmp;

   for (i=0; i<256; i++)
   {
      if (dcc_read_bits(bs, 1, 0, & tmp))
      {
         sprintf(add_error,
                 "direction %i, pixel_values %i\n", d, i);
         strcat(dcc_error, add_error);
         return 1;
      }
      if (tmp)
         dcc->direction[d].pixel_values[idx++] = i;
   }

   // end
   return 0;
}


// ==========================================================================
// make the cells for the frame buffer, to prepare the stage 1 & 2
// return 0 on success, non-zero if error
static int dcc_prepare_buffer_cells(DCC_S * dcc, int d)
{
   DCC_DIRECTION_S * dir = & dcc->direction[d];
   DCC_CELL_S      * cell;
   int             buffer_w, buffer_h, tmp, nb_cell_w, nb_cell_h, nb_cell,
                   size, * cell_w, * cell_h, i, x0, y0, x, y;

   buffer_w = dir->box.width;
   buffer_h = dir->box.height;


   tmp = buffer_w - 1;
   nb_cell_w = 1 + (tmp / 4);
      
   tmp = buffer_h - 1;
   nb_cell_h = 1 + (tmp / 4);

   nb_cell = nb_cell_w * nb_cell_h;
   size = nb_cell * sizeof(DCC_CELL_S);
   dir->buffer_ptr = (DCC_CELL_S *) malloc(size);
   if (dir->buffer_ptr == NULL)
   {
      sprintf(dcc_error, "dcc_prepare_buffer_cells() :\n"
                         "can't allocate %i bytes for dir->buffer_ptr\n",
                         size);
      return 1;
   }
   memset(dir->buffer_ptr, 0, size);

   size   = nb_cell_w * sizeof(int);
   cell_w = (int *) malloc(size);
   if (cell_w == NULL)
   {
      sprintf(dcc_error, "dcc_prepare_buffer_cells() :\n"
                         "can't allocate %i bytes for cell_w\n",
                         size);
      return 1;
   }

   size   = nb_cell_h * sizeof(int);
   cell_h = (int *) malloc(size);
   if (cell_h == NULL)
   {
      free(cell_w);
      sprintf(dcc_error, "dcc_prepare_buffer_cells() :\n"
                         "can't allocate %i bytes for cell_h\n",
                         size);
      return 1;
   }

   if (nb_cell_w == 1)
      cell_w[0] = buffer_w;
   else
   {
      for (i=0; i < (nb_cell_w - 1); i++)
         cell_w[i] = 4;
      cell_w[nb_cell_w - 1] = buffer_w - (4 * (nb_cell_w - 1));
   }

   if (nb_cell_h == 1)
      cell_h[0] = buffer_h;
   else
   {
      for (i=0; i < (nb_cell_h - 1); i++)
         cell_h[i] = 4;
      cell_h[nb_cell_h - 1] = buffer_h - (4 * (nb_cell_h - 1));
   }

   dir->nb_cells_w = nb_cell_w;
   dir->nb_cells_h = nb_cell_h;

   y0 = 0;
   for (y=0; y < nb_cell_h; y++)
   {
      x0 = 0;
      for (x=0; x < nb_cell_w; x++)
      {
         cell      = & dir->buffer_ptr[x + (y * nb_cell_w)];
         cell->w   = cell_w[x];
         cell->h   = cell_h[y];
         cell->bmp = create_sub_bitmap(dir->bmp, x0, y0, cell->w, cell->h);
         x0 += 4;
      }
      y0 += 4;
   }

   free(cell_w);
   free(cell_h);

   return 0;
}


// ==========================================================================
// make the cells of 1 frame, called during stage 1, but used mainly
// for stage 2
// return 0 on success, non-zero if error
static int dcc_prepare_frame_cells(DCC_S * dcc, int d, int f)
{
   DCC_DIRECTION_S * dir   = & dcc->direction[d];
   DCC_FRAME_S     * frame = & dcc->frame[d][f];
   DCC_CELL_S      * cell;
   int             frame_w, frame_h, w, h, tmp,
                   nb_cell_w, nb_cell_h, nb_cell,
                   size, * cell_w, * cell_h, i, x0, y0, x, y;

   frame_w = frame->box.width;
   frame_h = frame->box.height;

   // width (in # of pixels) in 1st column
   w = 4 - ((frame->box.xmin - dir->box.xmin) % 4);
   
   if ((frame_w - w) <= 1) // if 2nd column is 0 or 1 pixel width
      nb_cell_w = 1;
   else
   {
      // so, we have minimum 2 pixels behind 1st column
      tmp = frame_w - w - 1; // tmp is minimum 1, can't be 0
      nb_cell_w = 2 + (tmp / 4);
      if ((tmp % 4) == 0)
         nb_cell_w--;
   }

   h = 4 - ((frame->box.ymin - dir->box.ymin) % 4);
   if ((frame_h - h) <= 1)
      nb_cell_h = 1;
   else
   {
      tmp = frame_h - h - 1;
      nb_cell_h = 2 + (tmp / 4);
      if ((tmp % 4) == 0)
         nb_cell_h--;
   }

   nb_cell = nb_cell_w * nb_cell_h;
   size = nb_cell * sizeof(DCC_CELL_S);
   frame->cell = (DCC_CELL_S *) malloc(size);
   if (frame->cell == NULL)
   {
      sprintf(dcc_error, "dcc_prepare_frame_cells() :\n"
                         "can't allocate %i bytes for frame->cell\n",
                         size);
      return 1;
   }
   memset(frame->cell, 0, size);

   size   = nb_cell_w * sizeof(int);
   cell_w = (int *) malloc(size);
   if (cell_w == NULL)
   {
      sprintf(dcc_error, "dcc_prepare_frame_cells() :\n"
                         "can't allocate %i bytes for cell_w\n",
                         size);
      return 1;
   }

   size   = nb_cell_h * sizeof(int);
   cell_h = (int *) malloc(size);
   if (cell_h == NULL)
   {
      free(cell_w);
      sprintf(dcc_error, "dcc_prepare_frame_cells() :\n"
                         "can't allocate %i bytes for cell_h\n",
                         size);
      return 1;
   }

   if (nb_cell_w == 1)
      cell_w[0] = frame_w;
   else
   {
      cell_w[0] = w;
      for (i=1; i < (nb_cell_w - 1); i++)
         cell_w[i] = 4;
      cell_w[nb_cell_w - 1] = frame_w - w - (4 * (nb_cell_w - 2));
   }

   if (nb_cell_h == 1)
      cell_h[0] = frame_h;
   else
   {
      cell_h[0] = h;
      for (i=1; i < (nb_cell_h - 1); i++)
         cell_h[i] = 4;
      cell_h[nb_cell_h - 1] = frame_h - h - (4 * (nb_cell_h - 2));
   }

   frame->nb_cells_w = nb_cell_w;
   frame->nb_cells_h = nb_cell_h;

   y0 = frame->box.ymin - dir->box.ymin;
   for (y=0; y < nb_cell_h; y++)
   {
      x0 = frame->box.xmin - dir->box.xmin;
      for (x=0; x < nb_cell_w; x++)
      {
         cell = & frame->cell[x + (y * nb_cell_w)];
         cell->x0  = x0;
         cell->y0  = y0;
         cell->w   = cell_w[x];
         cell->h   = cell_h[y];
         cell->bmp = create_sub_bitmap(dir->bmp, x0, y0, cell->w, cell->h);
         x0 += cell->w;
      }
      y0 += cell->h;
   }

   free(cell_w);
   free(cell_h);

   return 0;
}


// ==========================================================================
// stage 1 of the decompression of the direction frames
// return 0 on success, non-zero if error
static int dcc_fill_pixel_buffer(DCC_S * dcc, int d)
{
   DCC_DIRECTION_S * dir = & dcc->direction[d];
   DCC_FRAME_S     * frame;
   DCC_PB_ENTRY_S  * old_entry, * new_entry;
   s4            cell0_x, cell0_y, cell_w, cell_h;
   u4          tmp, pixel_mask, encoding_type, last_pixel, read_pixel[4],
                   pix_displ;
   int             nb_cell, size;
   int             i, f, curr_cell_x, curr_cell_y, curr_cell;
   int             nb_pix_table[16] =
                      {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};
   int             x, y, nb_pix, next_cell, curr_idx,
                   pb_idx = -1; // current entry of pixel_buffer
   DCC_PB_ENTRY_S  ** cell_buffer;
   int             buff_w, buff_h, decoded_pix;
   char            add_error[256];


   // create & init pixel buffer
   size = DCC_MAX_PB_ENTRY * sizeof(DCC_PB_ENTRY_S);
   dir->pixel_buffer = (DCC_PB_ENTRY_S *) malloc(size);
   if (dir->pixel_buffer == NULL)
   {
      sprintf(dcc_error, "dcc_fill_pixel_buffer() : can't allocate "
                         "%i bytes for pixel_buffer (max entry = %i)\n",
                         size, DCC_MAX_PB_ENTRY);
      return 1;
   }
   memset(dir->pixel_buffer, 0, size);
   for (i=0; i < DCC_MAX_PB_ENTRY; i++)
   {
      dir->pixel_buffer[i].frame            = -1;
      dir->pixel_buffer[i].frame_cell_index = -1;
   }

   // frame buffer, will be simply called as "buffer" for now
   if (dcc_prepare_buffer_cells(dcc, d))
      return 1;
   dir->bmp = create_bitmap(dir->box.width, dir->box.height);
   if (dir->bmp == NULL)
   {
      sprintf(dcc_error, "dcc_fill_pixel_buffer() : can't create buffer bitmap "
                         "of %i * %i pixels\n",
                         dir->box.width, dir->box.height);
      return 1;
   }
   picClear(dir->bmp, 0); // all 0

   // create & init cell_buffer (for stage 1 only)
   // this buffer is a table of pointer to the current pixelbuffer entry
   // of 1 buffer cell
   buff_w  = dir->nb_cells_w;
   buff_h  = dir->nb_cells_h;
   nb_cell = buff_w * buff_h;
   size    = nb_cell * sizeof(DCC_PB_ENTRY_S *);
   cell_buffer = (DCC_PB_ENTRY_S **) malloc(size);
   if (cell_buffer == NULL)
   {
      sprintf(dcc_error, "dcc_fill_pixel_buffer() : can't allocate "
                         "%i bytes for cell_buffer (%i cells)\n",
                         size, nb_cell);
      return 1;
   }
   memset(cell_buffer, 0, size);

   // for all frames of this direction
   for (f=0; f < dcc->header.frames_per_dir; f++)
   {
      // make cells of this frame
      if (dcc_prepare_frame_cells(dcc, d, f))
      {
         free(cell_buffer);
         return 1;
      }

      frame   = & dcc->frame[d][f];
      cell_w  = frame->nb_cells_w;
      cell_h  = frame->nb_cells_h;
      cell0_x = (frame->box.xmin - dir->box.xmin) / 4;
      cell0_y = (frame->box.ymin - dir->box.ymin) / 4;
      
      // for each cells of this frame
      for (y=0; y < cell_h; y++)
      {
         curr_cell_y = cell0_y + y;
         for (x=0; x < cell_w; x++)
         {
            curr_cell_x = cell0_x + x;
            curr_cell   = curr_cell_x + (curr_cell_y * buff_w);
            if (curr_cell >= nb_cell)
            {
               sprintf(dcc_error, "dcc_fill_pixel_buffer() : "
                  "can't check the cell %i in cell_buffer,\n"
                  "run-time max cell is %i\n"
                  "dir %i, frm %i, x=%i, y=%i\n",
                  curr_cell, nb_cell,
                  d, f, x, y);
               free(cell_buffer);
               return 1;
            }

            // check if this cell need a new entry in pixel_buffer
            next_cell = 0;
            if (cell_buffer[curr_cell] != NULL)
            {
               if (dir->equal_cell_bitstream_size)
               {
                  if (dcc_read_bits( & dir->equal_cell_bitstream,
                                     1, 0, & tmp))
                  {
                     sprintf(add_error, "dcc_fill_pixel_buffer() : "
                        "equal_cell bitstream\ndirection %i, frame %i, "
                        "curr_cell %i\n",
                         d, f, curr_cell);
                     strcat(dcc_error, add_error);
                     free(cell_buffer);
                     return 1;
                  }
               }
               else
                  tmp = 0;
               if (tmp == 0)
               {
                  if (dcc_read_bits( & dir->pixel_mask_bitstream,
                                     4, 0, & pixel_mask))
                  {
                     sprintf(add_error, "dcc_fill_pixel_buffer() : "
                        "pixel_mask bitstream\n"
                        "direction %i, frame %i, curr_cell %i\n"
                        "x=%i, y=%i\n",
                         d, f, curr_cell, x, y);
                     strcat(dcc_error, add_error);
                     free(cell_buffer);
                     return 1;
                  }
                                 
               }
               else
                  next_cell = 1;
            }
            else
               pixel_mask = 0x0F;

            if (next_cell == 0)
            {
               // decode the pixels

               // read_pixel[] is a stack, where we "push" the pixel code
               read_pixel[0] = read_pixel[1] = 0;
               read_pixel[2] = read_pixel[3] = 0;
               last_pixel    = 0;
               nb_pix        = nb_pix_table[pixel_mask];
               if (nb_pix && dir->encoding_type_bitstream_size)
               {
                  if (dcc_read_bits( & dir->encoding_type_bitstream,
                                     1, 0, & encoding_type))
                  {
                     sprintf(add_error, "dcc_fill_pixel_buffer() :\n   "
                        "encoding_type bitstream, direction %i, frame %i, "
                        "curr_cell %i\n"
                        "   nb_pix = %i, curr_cell_x y = %i %i\n",
                         d, f, curr_cell, nb_pix, curr_cell_x, curr_cell_y);
                     strcat(dcc_error, add_error);
                     free(cell_buffer);
                     return 1;
                  }
               }
               else
                  encoding_type = 0;
               decoded_pix = 0;
               for (i=0; i < nb_pix; i++)
               {
                  if (encoding_type)
                  {
                     if (dcc_read_bits( & dir->raw_pixel_bitstream,
                                        8, 0, & read_pixel[i]))
                     {
                        sprintf(add_error, "dcc_fill_pixel_buffer() :\n   "
                           "raw_pixel bitstream, direction %i, frame %i, "
                           "curr_cell %i\n"
                           "   nb_pix = %i, curr_cell_x y = %i %i\n",
                           d, f, curr_cell, nb_pix, curr_cell_x, curr_cell_y);
                        strcat(dcc_error, add_error);
                        free(cell_buffer);
                        return 1;
                     }
                  }
                  else
                  {
                     read_pixel[i] = last_pixel;
                     if (dcc_read_bits(
                        & dir->pixel_code_and_displacment_bitstream,
                        4, 0, & pix_displ))
                     {
                        sprintf(add_error, "dcc_fill_pixel_buffer() :\n   "
                           "pixel_code_and_displacment bitstream, direction %i, frame %i, "
                           "curr_cell %i\n",
                            d, f, curr_cell);
                        strcat(dcc_error, add_error);
                        free(cell_buffer);
                        return 1;
                     }
                     read_pixel[i] += pix_displ;
                     while (pix_displ == 15)
                     {
                        if (dcc_read_bits(
                           & dir->pixel_code_and_displacment_bitstream,
                           4, 0, & pix_displ))
                        {
                           sprintf(add_error, "dcc_fill_pixel_buffer() :\n   "
                              "pixel_code_and_displacment bitstream, direction %i, frame %i, "
                              "curr_cell %i\n",
                               d, f, curr_cell);
                           strcat(dcc_error, add_error);
                           free(cell_buffer);
                           return 1;
                        }
                        read_pixel[i] += pix_displ;
                     }
                  }
                  if (read_pixel[i] == last_pixel)
                  {
                     read_pixel[i] = 0; // discard this pixel
                     i = nb_pix;        // stop the decoding of pixels
                  }
                  else
                  {
                     last_pixel = read_pixel[i];
                     decoded_pix++;
                  }
               }
               
               // we have the 4 pixels code for the new entry in pixel_buffer
               old_entry = cell_buffer[curr_cell];
               pb_idx++;
               if (pb_idx >= DCC_MAX_PB_ENTRY)
               {
                  sprintf(dcc_error, "dcc_fill_pixel_buffer() : "
                     "can't add a new entry in pixel buffer,\nmax is %i\n"
                     "direction %i, frame %i, curr_cell %i\n"
                     "   nb_pix = %i, curr_cell_x y = %i %i\n",
                     DCC_MAX_PB_ENTRY, d, f, curr_cell, nb_pix,
                     curr_cell_x, curr_cell_y);
                  free(cell_buffer);
                  return 1;
               }
               new_entry = & dir->pixel_buffer[pb_idx];
               curr_idx  = decoded_pix - 1; // we'll "pop" them
               for (i=0; i<4; i++)
               {
                  if (pixel_mask & (1 << i))
                  {
                     if (curr_idx >= 0) // if stack is not empty, pop it
                        new_entry->val[i] = (u1) read_pixel[curr_idx--];
                     else // else pop a 0
                        new_entry->val[i] = 0;
                  }
                  else
                     new_entry->val[i] = old_entry->val[i];
               }
               cell_buffer[curr_cell]      = new_entry;
               new_entry->frame            = f;
               new_entry->frame_cell_index = x + (y * cell_w);
            }
         }
      }
   }

   // prepare the stage 2
   //    replace pixel codes in pixel_buffer by their true values
   for (i=0; i <= pb_idx; i++)
   {
      for (x=0; x<4; x++)
      {
         y = dir->pixel_buffer[i].val[x];
         dir->pixel_buffer[i].val[x] = dir->pixel_values[y];
      }
   }

   // end
   dir->pb_nb_entry = pb_idx + 1;
   free(cell_buffer);
   return 0;
}


// ==========================================================================
static int dcc_save_frame(DCC_S * dcc, int d, int f, pic * frm_bmp)
{
   dcc->frame[d][f].bmp = create_bitmap(frm_bmp->W, frm_bmp->H);
   picBlt(dcc->frame[d][f].bmp, frm_bmp, 0, 0, 0, 0, 0, frm_bmp->W, frm_bmp->H);
   return 0;
}


// ==========================================================================
// stage 2 of the decompression of the direction frames
// return 0 on success, non-zero if error
static int dcc_make_frames(DCC_S * dcc, int d)
{
   DCC_DIRECTION_S * dir = & dcc->direction[d];
   DCC_PB_ENTRY_S  * pbe = dir->pixel_buffer;
   DCC_FRAME_S     * frame;
   DCC_CELL_S      * buff_cell, * cell;
   u4 pix;
   int nb_cell, nb_bit, cell_x, cell_y, cell_idx, x, y, f, c;
   pic *frm_bmp;
   char add_error[256];

   for (c=0; c < dir->nb_cells_w * dir->nb_cells_h; c++)
   {
      dir->buffer_ptr[c].last_w  = -1;
      dir->buffer_ptr[c].last_h  = -1;
   }

   // create the temp frame bitmap (size = current direction box)
   frm_bmp = picNew(dir->box.width, dir->box.height, 8);

   for (f=0; f < dcc->header.frames_per_dir; f++)
   {
      picClear(frm_bmp, 0); // clear the final frame bitmap (to index 0)
      frame = & dcc->frame[d][f];
      nb_cell = frame->nb_cells_w * frame->nb_cells_h;

      // for all cells of this frame
      for (c=0; c < nb_cell; c++)
      {
         // frame cell
         cell = & frame->cell[c];

         // buffer cell
         cell_x    = cell->x0 / 4;
         cell_y    = cell->y0 / 4;
         cell_idx  = cell_x + (cell_y * dir->nb_cells_w);
         buff_cell = & dir->buffer_ptr[cell_idx];

         // equal cell checks
         if ((pbe->frame != f) || (pbe->frame_cell_index != c))
         {
            // this buffer cell have an equalcell bit set to 1
            //    so either copy the frame cell or clear it
            
            if ((cell->w != buff_cell->last_w) || (cell->h != buff_cell->last_h))
            {
               // different sizes
               picClear(cell->bmp, 0); // set all pixels of the frame cell to 0
            }
            else
            {
               // same sizes

               // copy the old frame cell into its new position
               picBlt(dir->bmp, dir->bmp, 0,
                    cell->x0, cell->y0,
                    buff_cell->last_x0, buff_cell->last_y0,
                    cell->w, cell->h
               );

               // copy it again, into the final frame bitmap
               picBlt(frm_bmp, cell->bmp, 0,
                    cell->x0, cell->y0,
                    0, 0,
                    cell->w, cell->h
               );
            }
         }
         else
         {
            // fill the frame cell with pixels
            
            if (pbe->val[0] == pbe->val[1])
            {
               // fill FRAME cell to color val[0]
               picClear(cell->bmp, pbe->val[0]);
            }
            else
            {
               if (pbe->val[1] == pbe->val[2])
                  nb_bit = 1;
               else
                  nb_bit = 2;

               // fill FRAME cell with pixels
               for (y=0; y < cell->h; y++)
               {
                  for (x=0; x < cell->w; x++)
                  {
                     if (dcc_read_bits(
                        & dcc->direction[d].pixel_code_and_displacment_bitstream,
                        nb_bit, 0, & pix))
                     {
                        sprintf(add_error, "dcc_make_frame() :\n   "
                           "pixel_code_and_displacment bitstream, direction %i, frame %i, "
                           "curr_cell %i\n",
                        d, pbe->frame, pbe->frame_cell_index);
                        strcat(dcc_error, add_error);
                        return 1;
                     }
                     picPut(cell->bmp, x, y, pbe->val[pix]);
                  }
               }
            }

            // copy the frame cell into the frame bitmap
            picBlt(frm_bmp, cell->bmp, 0,
                 cell->x0, cell->y0,
                 0, 0,
                 cell->w, cell->h
            );

            // next pixelbuffer entry
            pbe++;
         }

         // for the buffer cell that was used by this frame cell,
         // save the width & size of the current frame cell
         // (needed for further tests about equalcell)
         buff_cell->last_w  = cell->w;
         buff_cell->last_h  = cell->h;

         // and save its origin, for further copy when equalcell
         buff_cell->last_x0 = cell->x0;
         buff_cell->last_y0 = cell->y0;
      }

      // save frame
      dcc_save_frame(dcc, d, f, frm_bmp);
   }

   // end
   picDel(frm_bmp);
   return 0;
}


// ==========================================================================
// read 1 direction bitstream
// return 0 on success, non-zero if error
static int dcc_dir_bitstream(DCC_S * dcc, int d)
{
   DCC_BITSTREAM_S bs;
   int             f;
   char            add_error[80];

   // if already done, return
   if (dcc->direction[d].bmp != NULL)
      return 0;
   
   // in case
   if ((d < 0) || (d >= dcc->header.directions))
   {
      sprintf(dcc_error, "dcc_dir_bitstream() : invalid direction, %i is "
                         "not in the range [0; %i]\n",
                         d, dcc->header.directions);
      return 1;
   }

   // init direction bitstream
   bs.cur_bit     = 0;
   bs.cur_bit_num = 0;
   bs.cur_byte    = 0;
   bs.data        = dcc->ptr + dcc->header.dir_offset[d];
   if (d == dcc->header.directions - 1)
      bs.size = 8 * (dcc->size - dcc->header.dir_offset[d]);
   else
      bs.size = 8 * (dcc->header.dir_offset[d + 1] -
                     dcc->header.dir_offset[d]);
   
   // read direction datas
   if (dcc_read_bits( & bs, 32, 0, & dcc->direction[d].outsize_coded))
   {
      sprintf(add_error, "direction %i, outsize_coded\n", d);
      strcat(dcc_error, add_error);
      return 1;
   }
   
   if (dcc_read_bits( & bs, 2, 0, & dcc->direction[d].compression_flag))
   {
      sprintf(add_error, "direction %i, compression_flag\n", d);
      strcat(dcc_error, add_error);
      return 1;
   }
   
   if (dcc_read_bits( & bs, 4, 0, & dcc->direction[d].variable0_bits))
   {
      sprintf(add_error, "direction %i, variable0_bits\n", d);
      strcat(dcc_error, add_error);
      return 1;
   }
   
   if (dcc_read_bits( & bs, 4, 0, & dcc->direction[d].width_bits))
   {
      sprintf(add_error, "direction %i, width_bits\n", d);
      strcat(dcc_error, add_error);
      return 1;
   }
   
   if (dcc_read_bits( & bs, 4, 0, & dcc->direction[d].height_bits))
   {
      sprintf(add_error, "direction %i, height_bits\n", d);
      strcat(dcc_error, add_error);
      return 1;
   }
   
   if (dcc_read_bits( & bs, 4, 0, & dcc->direction[d].xoffset_bits))
   {
      sprintf(add_error, "direction %i, xoffset_bits\n", d);
      strcat(dcc_error, add_error);
      return 1;
   }
   
   if (dcc_read_bits( & bs, 4, 0, & dcc->direction[d].yoffset_bits))
   {
      sprintf(add_error, "direction %i, yoffset_bits\n", d);
      strcat(dcc_error, add_error);
      return 1;
   }
   
   if (dcc_read_bits( & bs, 4, 0, & dcc->direction[d].optional_bytes_bits))
   {
      sprintf(add_error, "direction %i, optional_bytes_bits\n", d);
      strcat(dcc_error, add_error);
      return 1;
   }
   
   if (dcc_read_bits( & bs, 4, 0, & dcc->direction[d].coded_bytes_bits))
   {
      sprintf(add_error, "direction %i, coded_bytes_bits\n", d);
      strcat(dcc_error, add_error);
      return 1;
   }

   
   // init direction box min & max (NOT ZERO !)
   dcc->direction[d].box.xmin = dcc->direction[d].box.ymin = 0x7FFFFFFFL;
   dcc->direction[d].box.xmax = dcc->direction[d].box.ymax = 0x80000000L;

   
   // read frames header
   for (f=0; f < dcc->header.frames_per_dir; f++)
   {
      if (dcc_frame_header_bitstream(dcc, & bs, d, f))
      {
         sprintf(add_error, "direction %i, frames header\n", d);
         strcat(dcc_error, add_error);
         return 1;
      }

      // direction box
      if (dcc->frame[d][f].box.xmin < dcc->direction[d].box.xmin)
         dcc->direction[d].box.xmin = dcc->frame[d][f].box.xmin;

      if (dcc->frame[d][f].box.ymin < dcc->direction[d].box.ymin)
         dcc->direction[d].box.ymin = dcc->frame[d][f].box.ymin;

      if (dcc->frame[d][f].box.xmax > dcc->direction[d].box.xmax)
         dcc->direction[d].box.xmax = dcc->frame[d][f].box.xmax;

      if (dcc->frame[d][f].box.ymax > dcc->direction[d].box.ymax)
         dcc->direction[d].box.ymax = dcc->frame[d][f].box.ymax;
   }
   dcc->direction[d].box.width =
      dcc->direction[d].box.xmax - dcc->direction[d].box.xmin + 1;
   dcc->direction[d].box.height =
      dcc->direction[d].box.ymax - dcc->direction[d].box.ymin + 1;

   // optional bytes data
   if (dcc_optional_datas(dcc, & bs, d))
      return 1;
   
   // another bitstreams size
   if (dcc_other_bitstream_size(dcc, & bs, d))
      return 1;

   // pixel values key (256 bits)
   if (dcc_pixel_values_key(dcc, & bs, d))
      return 1;

   // init the other bitstreams
   dcc_init_dir_bitstream(dcc, & bs, d);

   // stage 1 : fill the pixelbuffer
   if (dcc_fill_pixel_buffer(dcc, d))
      return 1;

   // stage 2 : build the frames
   if (dcc_make_frames(dcc, d))
      return 1;

   // end
   if (dcc->direction[d].pixel_buffer)
   {
      free(dcc->direction[d].pixel_buffer);
      dcc->direction[d].pixel_buffer = NULL;
   }

   // check for not obvious decoding error
   if (dcc->direction[d].equal_cell_bitstream_size !=
       dcc->direction[d].equal_cell_bitstream.cur_bit_num)
   {
      sprintf(dcc_error, "direction %i, equal_cell bitstream not read completly while it should\n", d);
      strcat(dcc_error, add_error);
      return 1;
   }
   if (dcc->direction[d].pixel_mask_bitstream_size !=
       dcc->direction[d].pixel_mask_bitstream.cur_bit_num)
   {
      sprintf(dcc_error, "direction %i, pixel_mask bitstream not read completly while it should\n", d);
      strcat(dcc_error, add_error);
      return 1;
   }
   if (dcc->direction[d].encoding_type_bitstream_size !=
       dcc->direction[d].encoding_type_bitstream.cur_bit_num)
   {
      sprintf(dcc_error, "direction %i, encoding_type bitstream not read completly while it should\n", d);
      strcat(dcc_error, add_error);
      return 1;
   }
   if (dcc->direction[d].pixel_code_and_displacment_bitstream.cur_bit_num + 7
        < dcc->direction[d].pixel_code_and_displacment_bitstream.size)
   {
      sprintf(dcc_error, "direction %i, pixel_code_and_displacment bitstream not read completly while it should\n", d);
      strcat(dcc_error, add_error);
      return 1;
   }
   
   return 0;
}


// ==========================================================================
// read dcc file header
static int dcc_file_header(DCC_S * dcc)
{
   DCC_BITSTREAM_S bs;
   int             i;
   
   if (dcc->header.already_decoded == 1)
      return 0;
      
   bs.cur_bit     = 0;
   bs.cur_bit_num = 0;
   bs.cur_byte    = 0;
   bs.data        = dcc->ptr;
   bs.size        = 8 * dcc->size;
   
   if (dcc_read_bytes( & bs, 1, & dcc->header.file_signature))
      return 1;
      
   if (dcc_read_bytes( & bs, 1, & dcc->header.version))
      return 1;

   if (dcc_read_bytes( & bs, 1, & dcc->header.directions))
      return 1;

   if (dcc_read_bytes( & bs, 4, & dcc->header.frames_per_dir))
      return 1;

   if (dcc_read_bytes( & bs, 4, & dcc->header.tag))
      return 1;

   if (dcc_read_bytes( & bs, 4, & dcc->header.final_dc6_size))
      return 1;

   for (i=0; i < dcc->header.directions; i++)
      if (dcc_read_bytes( & bs, 4, & dcc->header.dir_offset[i]))
         return 1;

   // end
   dcc->header.already_decoded = 1;
   return 0;
}


// ==========================================================================
// read a dcc and decode it
// dir_bitfield is a bitfield : set the bit(s) for the direction(s) to decode
// return 0 on sucess, non-zero if error
static int dcc_decode(DCC_S * dcc, s4 dir_bitfield)
{
   int d;
   *dcc_error = 0;

   // init direction box min & max (NOT ZERO !)
   dcc->box.xmin = dcc->box.ymin = 0x7FFFFFFFL; // highest s4 number
   dcc->box.xmax = dcc->box.ymax = 0x80000000L; // lowest  s4 number
   
   // file header
   if (dcc_file_header(dcc))
      return 1;
   
   // direction bitstream
   for (d=0; d<dcc->header.directions; d++)
   {
      if (dir_bitfield & (1 << d))
      {
         if (dcc_dir_bitstream(dcc, d))
            return 1;

         // dcc box
         if (dcc->direction[d].box.xmin < dcc->box.xmin)
            dcc->box.xmin = dcc->direction[d].box.xmin;

         if (dcc->direction[d].box.ymin < dcc->box.ymin)
            dcc->box.ymin = dcc->direction[d].box.ymin;

         if (dcc->direction[d].box.xmax > dcc->box.xmax)
            dcc->box.xmax = dcc->direction[d].box.xmax;

         if (dcc->direction[d].box.ymax > dcc->box.ymax)
            dcc->box.ymax = dcc->direction[d].box.ymax;
      }
   }
   dcc->box.width  = dcc->box.xmax - dcc->box.xmin + 1;
   dcc->box.height = dcc->box.ymax - dcc->box.ymin + 1;

   // end
   return 0;
}


// ==========================================================================
// output the dcc datas in a more usable form
static void dcc_debug(DCC_S * dcc)
{
   int d, f, i;
  
   printf("file_signature = %i\n",  dcc->header.file_signature);
   printf("version        = %i\n",  dcc->header.version);
   printf("directions     = %i\n",  dcc->header.directions);
   printf("frames_per_dir = %li\n", dcc->header.frames_per_dir);
   printf("tag            = %li\n", dcc->header.tag);
   printf("final_dc6_size = %li bytes\n", dcc->header.final_dc6_size);

   printf("\nbox                = (%li, %li)  --->  (%li, %li)  =  %li * %li\n",
      dcc->box.xmin, dcc->box.ymin,
      dcc->box.xmax, dcc->box.ymax,
      dcc->box.width, dcc->box.height
   );

   for (d=0; d<dcc->header.directions; d++)
   {
      printf("\ndirection %2i\n", d);
      printf("   outsize_coded       = %li bytes\n", dcc->direction[d].outsize_coded);
      printf("   compression_flag    = %li\n", dcc->direction[d].compression_flag);
      printf("   variable0_bits      = %2lu  (%2i bits)\n", dcc->direction[d].variable0_bits,      dcc_bits_width_table[dcc->direction[d].variable0_bits]);
      printf("   width_bits          = %2lu  (%2i bits)\n", dcc->direction[d].width_bits,          dcc_bits_width_table[dcc->direction[d].width_bits]);
      printf("   height_bits         = %2lu  (%2i bits)\n", dcc->direction[d].height_bits,         dcc_bits_width_table[dcc->direction[d].height_bits]);
      printf("   xoffset_bits        = %2lu  (%2i bits)\n", dcc->direction[d].xoffset_bits,        dcc_bits_width_table[dcc->direction[d].xoffset_bits]);
      printf("   yoffset_bits        = %2lu  (%2i bits)\n", dcc->direction[d].yoffset_bits,        dcc_bits_width_table[dcc->direction[d].yoffset_bits]);
      printf("   optional_bytes_bits = %2lu  (%2i bits)\n", dcc->direction[d].optional_bytes_bits, dcc_bits_width_table[dcc->direction[d].optional_bytes_bits]);
      printf("   coded_bytes_bits    = %2lu  (%2i bits)\n", dcc->direction[d].coded_bytes_bits,    dcc_bits_width_table[dcc->direction[d].coded_bytes_bits]);
      printf("   box                 = (%li, %li)  --->  (%li, %li)  =  %li * %li\n",
         dcc->direction[d].box.xmin,
         dcc->direction[d].box.ymin,
         dcc->direction[d].box.xmax,
         dcc->direction[d].box.ymax,
         dcc->direction[d].box.width,
         dcc->direction[d].box.height
      );
      printf("   cells               = %i * %i = %i\n",
         dcc->direction[d].nb_cells_w,
         dcc->direction[d].nb_cells_h,
         dcc->direction[d].nb_cells_w * dcc->direction[d].nb_cells_h
      );
      printf("   CVDCC.DLL offsets   = %li, %li (in version < 3.0)\n\n",
         dcc->direction[d].box.xmin,
         dcc->direction[d].box.ymax
      );
      printf("   equal_cell_bitstream_size       = %8li bits (read %8li)\n", dcc->direction[d].equal_cell_bitstream_size, dcc->direction[d].equal_cell_bitstream.cur_bit_num);
      printf("   pixel_mask_bitstream_size       = %8li bits (read %8li)\n", dcc->direction[d].pixel_mask_bitstream_size, dcc->direction[d].pixel_mask_bitstream.cur_bit_num);
      printf("   encoding_type_bitstream_size    = %8li bits (read %8li)\n", dcc->direction[d].encoding_type_bitstream_size, dcc->direction[d].encoding_type_bitstream.cur_bit_num);
      printf("   raw_pixel_bitstream_size        = %8li bits (read %8li)\n", dcc->direction[d].raw_pixel_bitstream_size, dcc->direction[d].raw_pixel_bitstream.cur_bit_num);
      printf("   pixel_code_and_displacment.size = %8li bits (read %8li)\n", dcc->direction[d].pixel_code_and_displacment_bitstream.size, dcc->direction[d].pixel_code_and_displacment_bitstream.cur_bit_num);

      printf("\n   pixel_values :\n");
      for (i=0; i < 256; i++)
      {
         printf(" %3i", dcc->direction[d].pixel_values[i]);
         if ( ! ((i+1) % 16))
            printf("\n");
      }

      for (f=0; f<dcc->header.frames_per_dir; f++)
      {
         printf("\n   frame %3i\n", f);

         if (dcc->direction[d].variable0_bits)
            printf("      variable0      = %lu\n", dcc->frame[d][f].variable0);

         if (dcc->direction[d].width_bits)
            printf("      width          = %lu\n", dcc->frame[d][f].width);

         if (dcc->direction[d].height_bits)
            printf("      height         = %lu\n", dcc->frame[d][f].height);

         if (dcc->direction[d].xoffset_bits)
            printf("      xoffset        = %li\n", dcc->frame[d][f].xoffset);

         if (dcc->direction[d].yoffset_bits)
            printf("      yoffset        = %li\n", dcc->frame[d][f].yoffset);

         if (dcc->direction[d].optional_bytes_bits)
            printf("      optional_bytes = %lu\n", dcc->frame[d][f].optional_bytes);

         if (dcc->direction[d].coded_bytes_bits)
            printf("      coded_bytes    = %lu\n", dcc->frame[d][f].coded_bytes);

         printf("      bottom_up      = %li\n", dcc->frame[d][f].bottom_up);
         printf("      box            = (%li, %li)  --->  (%li, %li)  =  %li * %li\n",
            dcc->frame[d][f].box.xmin,
            dcc->frame[d][f].box.ymin,
            dcc->frame[d][f].box.xmax,
            dcc->frame[d][f].box.ymax,
            dcc->frame[d][f].box.width,
            dcc->frame[d][f].box.height
         );

         printf("      box2           = (%li, %li)  --->  (%li, %li)  =  %li * %li\n",
            dcc->frame[d][f].box.xmin - dcc->direction[d].box.xmin,
            dcc->frame[d][f].box.ymin - dcc->direction[d].box.ymin,
            dcc->frame[d][f].box.xmax - dcc->direction[d].box.xmin,
            dcc->frame[d][f].box.ymax - dcc->direction[d].box.ymin,
            dcc->frame[d][f].box.width,
            dcc->frame[d][f].box.height
         );

         printf("      cells          = %i * %i = %i\n",
            dcc->frame[d][f].nb_cells_w,
            dcc->frame[d][f].nb_cells_h,
            dcc->frame[d][f].nb_cells_w * dcc->frame[d][f].nb_cells_h
         );

         printf("      1st cell0_x, y = %li, %li\n",
            dcc->frame[d][f].cell[0].x0,
            dcc->frame[d][f].cell[0].y0
         );

         if (dcc->frame[d][f].optional_bytes)
         {
            printf("      optional datas =");
            for (i=0; i < (int) dcc->frame[d][f].optional_bytes; i++)
               printf(" %02X", dcc->frame[d][f].optional_bytes_data[i]);
            printf("\n");
         }
      }
   }
   printf("\ndcc_error[] :\n%s", dcc_error);
}


// ==========================================================================
// copy the file from disk into mem
// return NULL on error, check dcc_error in this case
// if success, return a pointer on a DCC_S structure
//    it's up to you to free the memory at the end of the program
//    with the help of dcc_destroy()
static DCC_S * dcc_disk_load(char * dcc_name)
{
   DCC_S * dcc;
   FILE  * in;
   int   size;

   // open file
   in = fopen(dcc_name, "rb");
   if (in == NULL)
   {
      sprintf(dcc_error, "dcc_disk_load() : can't open %s\n", dcc_name);
      return NULL;
   }

   // malloc
   size = sizeof(DCC_S);
   dcc = (DCC_S *) malloc(size);
   if (dcc == NULL)
   {
      sprintf(dcc_error, "dcc_disk_load() : can't allocate %i bytes (main struct) "
                         "for %s\n", size, dcc_name);
      fclose(in);
      return NULL;
   }
   memset(dcc, 0, size);

   // get size of the file
   fseek(in, 0, SEEK_END);
   dcc->size = ftell(in);
   if (dcc->size < 0)
   {
      sprintf(dcc_error, "dcc_disk_load() : ftell() have return %li "
                         "for %s\n", dcc->size, dcc_name);
      fclose(in);
      free(dcc);
      return NULL;
   }
   fseek(in, 0, SEEK_SET);

   // buffer for the copy in mem
   dcc->ptr = (u1 *) malloc(dcc->size);
   if (dcc->ptr == NULL)
   {
      sprintf(dcc_error, "dcc_disk_load() : can't allocate %li bytes (buffer) "
                         "for %s\n", dcc->size, dcc_name);
      fclose(in);
      free(dcc);
      return NULL;
   }
   
   // fill buffer
   fread(dcc->ptr, dcc->size, 1, in);

   // end
   fclose(in);
   return dcc;
}


// ==========================================================================
// copy the file from mem into mem
// (this function make a copy of the buffer you give)
// return NULL on error, check dcc_error in this case
// if success, return a pointer on a DCC_S structure
//    it's up to you to free the memory at the end of the program
//    with the help of dcc_destroy()
static DCC_S * dcc_mem_load(void * mem_ptr, int mem_size)
{
   DCC_S * dcc;
   int   size;

   // malloc
   size = sizeof(DCC_S);
   dcc = (DCC_S *) malloc(size);
   if (dcc == NULL)
   {
      sprintf(dcc_error, "dcc_mem_load() : can't allocate %i bytes "
         "(main struct)\n", size);
      return NULL;
   }
   memset(dcc, 0, size);

   // get size of the file
   dcc->size = mem_size;
   if (dcc->size < 0)
   {
      sprintf(dcc_error, "dcc_mem_load() : size is %i bytes\n", mem_size);
      free(dcc);
      return NULL;
   }

   // buffer for the copy in mem
   dcc->ptr = (u1 *) malloc(dcc->size);
   if (dcc->ptr == NULL)
   {
      sprintf(dcc_error, "dcc_mem_load() : can't allocate %li bytes "
         "(buffer)\n", dcc->size);
      free(dcc);
      return NULL;
   }
   
   // fill buffer
   memcpy(dcc->ptr, mem_ptr, mem_size);

   // end
   return dcc;
}


// ==========================================================================
// free a dcc from memory
static void dcc_destroy(DCC_S * dcc)
{
   int d, f, nb_cell, i;

   
   if (dcc == NULL)
      return;

   // free the copy of the file in mem
   if (dcc->ptr != NULL)
   {
      free(dcc->ptr);
      dcc->ptr = NULL;
   }

   // free datas in all directions
   for (d=0; d < dcc->header.directions; d++)
   {
      // free datas in all frames for this direction
      for (f=0; f < dcc->header.frames_per_dir; f++)
      {
         // free optional bytes, if any
         if (dcc->frame[d][f].optional_bytes_data)
         {
            free(dcc->frame[d][f].optional_bytes_data);
            dcc->frame[d][f].optional_bytes_data = NULL;
         }

         // free frame cells & sub-bitmaps
         if (dcc->frame[d][f].cell)
         {
            nb_cell = dcc->frame[d][f].nb_cells_w * dcc->frame[d][f].nb_cells_h;
            for (i=0; i < nb_cell; i++)
               if (dcc->frame[d][f].cell[i].bmp)
                  destroy_bitmap(dcc->frame[d][f].cell[i].bmp);

            free(dcc->frame[d][f].cell);
            dcc->frame[d][f].cell = NULL;
         }

         //destroy frame bitmap
         if (dcc->frame[d][f].bmp)
            destroy_bitmap(dcc->frame[d][f].bmp);
      }

      // free pixels buffer
      if (dcc->direction[d].pixel_buffer)
      {
         free(dcc->direction[d].pixel_buffer);
         dcc->direction[d].pixel_buffer = NULL;
      }

      // destroy buffer cells & sub-bitmaps
      if (dcc->direction[d].buffer_ptr)
      {
         nb_cell = dcc->direction[d].nb_cells_w * dcc->direction[d].nb_cells_h;
         for (i=0; i < nb_cell; i++)
            if (dcc->direction[d].buffer_ptr[i].bmp)
               destroy_bitmap(dcc->direction[d].buffer_ptr[i].bmp);
               
         free(dcc->direction[d].buffer_ptr);
         dcc->direction[d].buffer_ptr = NULL;
      }

      // destroy buffer bitmap
      if (dcc->direction[d].bmp)
         destroy_bitmap(dcc->direction[d].bmp);
   }

   // free general structure of this dcc
   free(dcc);
}




static spr *dccLoad(char *Input) {
  int I, J;
  spr *S ;
  pic *P;
  DCC_S *dcc;

  dcc = dcc_disk_load(Input);
  if (!dcc) {
    printf("  Error loading: %s\n", dcc_error);
    abort();
  }

  if (dcc_decode(dcc, -1)) {
    printf("  Error decoding: %s\n", dcc_error);
    dcc_debug(dcc);
    dcc_destroy(dcc);
    abort();
  }

  S = sprNew();
  S->Palette = ns(u1,4*256);
  memcpy(S->Palette, diablo2_pal_units, 4*256);

  S->Palette[0*4+0] = 0x00;
  S->Palette[0*4+1] = 0xFF;
  S->Palette[0*4+2] = 0xFF;


  S->NAnis = 1;
  S->Anis = ns(ani, S->NAnis);
  S->Anis[0].NFacs = dcc->header.directions;
  S->Anis[0].Facs = ns(fac,S->Anis[0].NFacs);

  times (I, S->Anis[0].NFacs) {
    S->Anis[0].Facs[I].NPics = dcc->header.frames_per_dir;
    S->Anis[0].Facs[I].Pics = ns(pic*, S->Anis[0].Facs[I].NPics);
    times (J, S->Anis[0].Facs[I].NPics) {
       P = S->Anis[0].Facs[I].Pics[J] = dcc->frame[I][J].bmp;
       P->SharedPalette = 1;
       free(P->P);
       P->P = S->Palette;
    }
  }

  return S;
}

int dccInit(format *F) {
  F->Type = FMT_SPRITE;
  F->Name = "dcc";
  F->Description = "Diablo 2 sprites";
  F->Load = dccLoad;
  return 1;
}


/*

each sprite has subdirectories:
  hd - head
  lg - legs
  tr - torso
  s1 - special 1

Monster names can be obtained from monstats.txt.

Here is what I got using:
 cat monstats.txt | awk 'BEGIN { FS = "\t" } ;{ print $8, $3}' | sort | uniq


10 TrappedSoul1
13 TrappedSoul2
17 IzualGhost
1D DeckardCain1
1Z youngdiablo
20 halbu
21 Hydra2
22 izual
2C Act2Child
2D DeckardCain2
2F Act2Female
2M Act2Male
36 Malachai
3D DeckardCain3
4D DeckardCain4
5P TheSmith
66 PrisonFR
67 PrisonFL
68 PrisonBR
69 PrisonBL
9A guardian
9a thingamabob
9A trap1
AE LightningSpire
AH asheara
AL alkor
AN Andariel
AS Atma
B9 bat 
BB BaboonDemon
BG bug
BH BigHead
BK BloodHawk
BL BirdLarge
BN FoulCrowNest
BS BirdSmall
BT BatDemon
BW Bonewall
CH ChaosHorde
CI Charsi
CK chicken
CM Camel
CO Snake
CR CorruptRogue
CR Quest
CS CorpseSpitter
CW Cow
DC DeckardCain
DH SuckerNest
DI Diablo
DM Megademon
DR Drognan
DS scorpion
DU Duriel
EC HellBovine
EH EvilHole
EL Elzix
FA Fallen
FC Fetish
FD FrogDemon
FE Fetish
FI Familiar
FJ Fish
FK FetishSkeleton
FM FireBeast
FR FingerMage
FS FallenShaman
FW FetishShaman
G1 GolemClay
G2 GolemBlood
G3 GolemFire
G4 GolemIron
GE Geglash
GH Gheed
GM Goatmen
GO Gorgon
GR Greiz
GT GargoyleTrap
GU Guard
GY GreaterMummy
GZ Griswold
HP HighPriest
HR hratli
HX Hydra1
HZ Hydra3
IM FireBeast
IW IronWolf
ja Jamella
JE Jerhyn
K9 InvisoSpawner
LS MiniSpider
LV larva
LY Lysander
M1 Act2Merchant1
M2 Act2Merchant2
M3 meshif3
M4 pillar_trap
M6 MephistoSpirit
MA Maggot
MG MummyGenerator
MM Mummy
MO MosquitoDemon
MP Mephisto
MQ MaggotQueen
MS Meshif
N3 Act3Female
N4 Act3Male
OF Fara
OR ormus
PB FireTower
PM PoisonOrb
PN PinHead
PR Parrot
PS Akara
PW PantherWoman
RC Kashya
RD Radament
RG Rogue
RT rat
S7 SpikeGiant
SB SandMaggotBaby
SC ScarabDemon
SD Salamander
SE SandMaggotEgg
SI SpikeFiend
SK Skeleton
SL SandLeaper
SM SandMaggot
SP Spider
SR SandRaider
ST Scimitar
SU Summoner
SW Swarm
TE tentaclehead
TH ThornedHulk
TN tentacle
TX tyrael1
TY tyrael2
TZ Natalya
UM Undead Horror
VA Vampire
VC VileDog
VD VultureDemon
VH WindowRight
VJ WindowLeft
VK Dopplezon
VK Valkyrie
VM VileMother
WA Warriv
WR Wraith
WW WillOWisp
WX Warriv2
YE Yeti
ZM Zombie
ZP Cantor
ZZ Zealot
*/

