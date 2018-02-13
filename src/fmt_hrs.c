//Credits go to SNV

#include "common.h"


typedef struct {
  u1 Id[4]; //HRES for root
  u4 Off;
  u4 Len;
} __attribute__ ((packed)) entry;

typedef struct {
  u1 Id[4]; //RIFF
  u4 Len;
  u1 Type[4]; //WAVE

  u1 FmtId[4];  //fmt
  u4 FmtLen;
  u2 Format;  //1=PCM
  u2 Chns; // channels
  u4 Freq; // sample rate
  u4 ByteRate;
  u2 Align;
  u2 Depth; // bits per sample

  u1 DataId[4]; //data
  u4 DataLen;
  //u1 Data[DataLen];
} __attribute__ ((packed)) WAVHeader;

typedef struct {
  u2 Width;
  u2 Height;
  u2 Compression; // 0=uncompressed, 1=RLE
  //u1 Data[];
} __attribute__ ((packed)) imageHeader;

typedef struct {
  u2 Width;
  u2 Height;
  s2 X; // X/Y offset, where characters feet are in sprite frame
  s2 Y; // for flyers it can be well outside of sprite
  //u1 Data[Width*Height];
} __attribute__ ((packed)) frameHeader;


// color schemes are used to recolor sprite parts on fly
typedef struct {
  u1 Part[12]; // indices into transition table
  char Name[32]; // character name
} __attribute__ ((packed)) schm;

static u1 Pal[256*4];

static schm *Schm[1024];
static char *SchmTags[1024];
static int NSchms;


// offsets to recolorable parts.
static u1 RO[] = {
  16, //  0: belt
  24, //  1: hair
  32, //  2: skin
  40, //  3:
  48, //  4: shirt
  56, //  5: sleeves
  64, //  6: pants
  72, //  7: shirt arnament
  80, //  8: boots
  88, //  9: boots arnament
  96, // 10:
 104, // 11:
   0};

// OBJ requires a SCHM, but for some reason misses it
// we have to provide default
schm OBJSchm = {
  {0x51,  // medium shades (normal surfaces)
   0x1,   // metal (like handles or chest frames)
   0x54,  // dark shades (i.e. inside of opened closet)
   0x50,  // light shades (i.e. shere light hits directly)
   0x4b,
   0xBB,
   0x70,
   0x69},
  "default"};


/*
FTA.HRS      - game world
FTADATA.HRS  - object definitions
FTAIMAGE.HRS - UI graphics, tilesets and sprites
FTASOUND.HRS - UI, environment and creature sounds
FTAVOICE.HRS - voice acting for characters
SCRIPTS.HRS  - game logic
*/


struct {
  char *Tag;
  char *Name;
} SPRINames[] = {
  {"BAT", "bat"},
  {"BRD", "bird"},
  {"CAN", "canine"},
  {"BRO", "brother"},
  {"BUM", "bum"},
  {"CHI", "child"},
  {"DEF", "dark_elf_female"},
  {"DEM", "dark_elf_male"},
  {"DWC", "dwarf_civilian"},
  {"DWF", "dwarf_female"},
  {"DWM", "dwarf_male"},
  {"FCM", "male_guard"},
  {"FEF", "forest_elf_female"},
  {"FEM", "forest_elf_male"},
  {"FPD", "female_peasant"},
  {"FPP", "gypsy_witch"},
  {"FRY", "fairies"},
  {"FSW", "female_sword_warrior"},
  {"GHS", "ghoul"},
  {"GIA", "giant"},
  {"GOB", "goblin"},
  {"GOL", "golem"},
  {"GRY", "gryphon"},
  {"GSP", "giant_spider"},
  {"HOB", "hobgoblin"},
  {"IRA", "irastikaan"},
  {"KNI", "knight"},
  {"MEN", "magic_icons"},
  {"MIS", "missiles"},
  {"MNH", "monk"},
  {"MPB", "male_peasant_bandit"},
  {"MPS", "male_priest"},
  {"MRC", "merchant"},
  {"MSW", "male_sword_warrior"},
  {"OBJ083", "objects"},
  {"ONM", "old_nobleman"},
  {"RAT", "rat"},
  {"RBA", "skeleton_bones1"},
  {"RBB", "skeleton_bones2"},
  {"RPM", "reptile_man"},
  {"SHD", "warlord"},
  {"SKL", "skeleton"},
  {"SLM", "slime"},
  {"SNK", "snake"},
  {"SPFX", "spell_effects"},
  {"SWM", "swarm"},
  {"VIN", "vine"},
  {"WPN", "weapons"},
  {"WRK", "wraith"},
  {"WSP", "weapons_spell"},
  {"WYV", "wyvern"},
  {"YNM", "young_nobleman"},
  {"YNW", "young_noblewoman"},
};


static u1 FTAPal[] = {
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x24, 0x59, 0x61, 0xFF, 0xFF,
 0xFF, 0xF3, 0xF3, 0xF3, 0xD3, 0xD7, 0xDF, 0xBA,
 0xC3, 0xCF, 0xA2, 0xAA, 0xBE, 0x8A, 0x96, 0xAA,
 0x75, 0x82, 0x9A, 0x61, 0x6D, 0x8A, 0x4D, 0x5D,
 0x75, 0x3C, 0x4D, 0x65, 0x30, 0x3C, 0x55, 0x20,
 0x2C, 0x45, 0x14, 0x20, 0x30, 0x0C, 0x10, 0x20,
 0x00, 0x00, 0x00, 0xEF, 0xFB, 0xFF, 0xCF, 0xDF,
 0xE7, 0xB2, 0xC7, 0xCF, 0x9A, 0xAA, 0xB6, 0x7D,
 0x92, 0x9E, 0x65, 0x79, 0x86, 0x51, 0x61, 0x6D,
 0x3C, 0x49, 0x55, 0xDB, 0xD7, 0xCF, 0xC7, 0xC3,
 0xB6, 0xB2, 0xAA, 0xA2, 0x9E, 0x96, 0x8E, 0x8A,
 0x82, 0x79, 0x75, 0x6D, 0x65, 0x61, 0x59, 0x51,
 0x51, 0x49, 0x41, 0xFF, 0xFB, 0xF3, 0xEB, 0xE3,
 0xD3, 0xD7, 0xCB, 0xB6, 0xC7, 0xB2, 0x9A, 0xB2,
 0x9E, 0x86, 0x9E, 0x8A, 0x6D, 0x8E, 0x75, 0x59,
 0x79, 0x5D, 0x45, 0x69, 0x4D, 0x34, 0x55, 0x38,
 0x24, 0x41, 0x28, 0x18, 0x30, 0x1C, 0x10, 0x9E,
 0x92, 0x7D, 0x86, 0x7D, 0x65, 0x71, 0x65, 0x4D,
 0x5D, 0x55, 0x3C, 0xFF, 0xC7, 0xAA, 0xEB, 0xAE,
 0x9A, 0xD7, 0x9A, 0x8A, 0xC3, 0x86, 0x75, 0xAE,
 0x75, 0x65, 0x9A, 0x65, 0x55, 0x8A, 0x55, 0x45,
 0x75, 0x45, 0x34, 0x61, 0x38, 0x28, 0x4D, 0x2C,
 0x1C, 0x38, 0x20, 0x14, 0x28, 0x14, 0x0C, 0xD3,
 0xAE, 0xAE, 0xB2, 0x92, 0x92, 0x96, 0x75, 0x79,
 0x79, 0x5D, 0x61, 0xFF, 0xFF, 0xCB, 0xEF, 0xEB,
 0xA2, 0xDF, 0xD7, 0x7D, 0xD3, 0xC3, 0x5D, 0xC3,
 0xAE, 0x41, 0xB6, 0x96, 0x24, 0xA6, 0x7D, 0x0C,
 0x9A, 0x69, 0x00, 0xE7, 0xDF, 0xA6, 0xCF, 0xC7,
 0x8E, 0xBA, 0xAE, 0x75, 0xA6, 0x96, 0x61, 0x8E,
 0x7D, 0x4D, 0x79, 0x69, 0x3C, 0x65, 0x55, 0x2C,
 0x51, 0x41, 0x20, 0xFF, 0xF7, 0x82, 0xFF, 0xDB,
 0x49, 0xE7, 0xBA, 0x38, 0xCF, 0x9E, 0x2C, 0xBA,
 0x82, 0x20, 0xA2, 0x69, 0x18, 0x8E, 0x51, 0x10,
 0x75, 0x3C, 0x08, 0x5D, 0x28, 0x04, 0x49, 0x18,
 0x00, 0x30, 0x0C, 0x00, 0x1C, 0x04, 0x00, 0x9E,
 0x9A, 0x2C, 0x86, 0x7D, 0x08, 0x71, 0x65, 0x08,
 0x55, 0x49, 0x08, 0xE7, 0x9E, 0x69, 0xE7, 0x8E,
 0x55, 0xE7, 0x79, 0x3C, 0xD3, 0x69, 0x34, 0xBE,
 0x59, 0x2C, 0xAA, 0x49, 0x24, 0x9A, 0x3C, 0x1C,
 0x86, 0x30, 0x14, 0x71, 0x24, 0x10, 0x5D, 0x1C,
 0x0C, 0x49, 0x14, 0x08, 0x38, 0x0C, 0x04, 0xE3,
 0x69, 0x00, 0xC3, 0x49, 0x00, 0xA6, 0x30, 0x00,
 0x79, 0x20, 0x00, 0xFF, 0xFF, 0x86, 0xEF, 0xDB,
 0x69, 0xDF, 0xB6, 0x51, 0xCF, 0x8E, 0x38, 0xBE,
 0x69, 0x28, 0xAE, 0x41, 0x14, 0x9E, 0x1C, 0x08,
 0x7D, 0x00, 0x00, 0xFF, 0x59, 0x55, 0xFF, 0x28,
 0x2C, 0xDB, 0x00, 0x00, 0xAE, 0x00, 0x00, 0xB2,
 0x00, 0x59, 0x8E, 0x00, 0x4D, 0x6D, 0x00, 0x41,
 0x4D, 0x00, 0x30, 0xCB, 0x20, 0xBA, 0xAA, 0x10,
 0x9A, 0x8A, 0x04, 0x7D, 0x6D, 0x00, 0x61, 0xBE,
 0x86, 0xAA, 0xA2, 0x6D, 0x8E, 0x8A, 0x55, 0x71,
 0x71, 0x41, 0x59, 0xCF, 0x9E, 0xE7, 0xB6, 0x79,
 0xD3, 0x9E, 0x5D, 0xC3, 0x8A, 0x41, 0xAE, 0x75,
 0x2C, 0x9E, 0x61, 0x18, 0x8A, 0x51, 0x08, 0x79,
 0x41, 0x00, 0x69, 0xAA, 0x7D, 0xFF, 0x9E, 0x59,
 0xFF, 0x96, 0x3C, 0xF3, 0x75, 0x2C, 0xD3, 0x59,
 0x20, 0xB2, 0x41, 0x14, 0x92, 0x2C, 0x0C, 0x75,
 0x1C, 0x00, 0x5D, 0xBE, 0xD3, 0xEB, 0x9A, 0xB2,
 0xCF, 0x7D, 0x96, 0xB2, 0x61, 0x79, 0x9A, 0xBA,
 0xD3, 0xBE, 0x9E, 0xAE, 0x92, 0x8A, 0x8E, 0x6D,
 0x61, 0x69, 0x4D, 0xE7, 0xF7, 0xFF, 0xC3, 0xDF,
 0xEF, 0xA6, 0xCB, 0xE3, 0x8A, 0xB6, 0xD7, 0x6D,
 0x9E, 0xCB, 0x55, 0x8A, 0xBE, 0x41, 0x75, 0xAE,
 0x2C, 0x61, 0xA2, 0x18, 0x4D, 0x96, 0x0C, 0x3C,
 0x8A, 0x9A, 0xA2, 0xFF, 0x8A, 0x8A, 0xFF, 0x82,
 0x69, 0xFF, 0x82, 0x49, 0xFF, 0x69, 0x34, 0xDB,
 0x55, 0x20, 0xBA, 0xF3, 0xFF, 0xFF, 0xCB, 0xEB,
 0xEB, 0xA6, 0xD7, 0xDB, 0x86, 0xC3, 0xCB, 0x69,
 0xAE, 0xBA, 0x55, 0x9E, 0xAE, 0x38, 0x86, 0x96,
 0x24, 0x71, 0x86, 0x14, 0x5D, 0x75, 0x08, 0x4D,
 0x65, 0x49, 0xB2, 0xEF, 0x1C, 0x96, 0xDF, 0x0C,
 0x75, 0xD7, 0x04, 0x5D, 0xB6, 0x00, 0x49, 0x96,
 0x00, 0x38, 0x75, 0xE3, 0xFF, 0xB6, 0xC7, 0xE7,
 0x9A, 0xAA, 0xD3, 0x7D, 0x8E, 0xBE, 0x65, 0x75,
 0xA6, 0x51, 0x5D, 0x92, 0x3C, 0x49, 0x7D, 0x2C,
 0x34, 0x69, 0x20, 0x45, 0xEB, 0x71, 0x30, 0xC7,
 0x61, 0x20, 0xA6, 0x51, 0x00, 0x86, 0x45, 0x08,
 0x6D, 0x41, 0x04, 0x55, 0x3C, 0x00, 0x3C, 0x34,
 0x00, 0x20, 0x20, 0xFF, 0xEF, 0xCB, 0xE7, 0xDB,
 0x9E, 0xD3, 0xCF, 0x79, 0xB6, 0xBE, 0x59, 0x96,
 0xAA, 0x3C, 0xE7, 0xF7, 0x59, 0xC3, 0xE3, 0x18,
 0x9E, 0xCB, 0x18, 0x82, 0xB6, 0x18, 0x69, 0xA2,
 0x18, 0x51, 0x8E, 0x14, 0x3C, 0x79, 0x14, 0x2C,
 0x65, 0x10, 0x20, 0x51, 0x10, 0x14, 0x3C, 0x0C,
 0x00, 0x24, 0x00, 0xFF, 0xD7, 0xC3, 0xFF, 0xE7,
 0xDB, 0xFF, 0xFB, 0xBE, 0x7D, 0x9A, 0x8A, 0x4D,
 0x96, 0xA6, 0x41, 0x3C, 0x34, 0x30, 0x30, 0x28,
 0xEF, 0xDB, 0x69, 0xCF, 0x8E, 0x38, 0xBE, 0x59,
 0x2C, 0x9E, 0x1C, 0x08, 0xFF, 0xAE, 0xAE, 0x00,
 0x00, 0x00, 0xD7, 0xB2, 0xDF, 0xD7, 0xB2, 0xDF,
 0xD7, 0xB2, 0xDF, 0xD7, 0xB2, 0xDF, 0xD7, 0xB2,
 0xDF, 0xD7, 0xB2, 0xDF, 0xD7, 0xB2, 0xDF, 0xD7,
 0xB2, 0xDF, 0xD7, 0xB2, 0xDF, 0xD7, 0xB2, 0xDF};


static u1 DinoPal[] = {
 0x3C, 0x00, 0x3C, 0xFF, 0xFF, 0xFF, 0xF3, 0xF3,
 0xF3, 0xD3, 0xD7, 0xDF, 0xBA, 0xC3, 0xCF, 0xA2,
 0xAA, 0xBE, 0x8A, 0x96, 0xAA, 0x75, 0x82, 0x9A,
 0x61, 0x6D, 0x8A, 0x4D, 0x5D, 0x75, 0x3C, 0x4D,
 0x65, 0x30, 0x3C, 0x55, 0x20, 0x2C, 0x45, 0x14,
 0x20, 0x30, 0x0C, 0x10, 0x20, 0x00, 0x00, 0x00,
 0xEF, 0xFB, 0xFF, 0xCF, 0xDF, 0xE7, 0xB2, 0xC7,
 0xCF, 0x9A, 0xAA, 0xB6, 0x7D, 0x92, 0x9E, 0x65,
 0x79, 0x86, 0x51, 0x61, 0x6D, 0x3C, 0x49, 0x55,
 0xDB, 0xD7, 0xCF, 0xC7, 0xC3, 0xB6, 0xB2, 0xAA,
 0xA2, 0x9E, 0x96, 0x8E, 0x8A, 0x82, 0x79, 0x75,
 0x6D, 0x65, 0x61, 0x59, 0x51, 0x51, 0x49, 0x41,
 0xFF, 0xFB, 0xF3, 0xEB, 0xE3, 0xD3, 0xD7, 0xCB,
 0xB6, 0xC7, 0xB2, 0x9A, 0xB2, 0x9E, 0x86, 0x9E,
 0x8A, 0x6D, 0x8E, 0x75, 0x59, 0x79, 0x5D, 0x45,
 0x69, 0x4D, 0x34, 0x55, 0x38, 0x24, 0x41, 0x28,
 0x18, 0x30, 0x1C, 0x10, 0x9E, 0x92, 0x7D, 0x86,
 0x7D, 0x65, 0x71, 0x65, 0x4D, 0x5D, 0x55, 0x3C,
 0xFF, 0xC7, 0xAA, 0xEB, 0xAE, 0x9A, 0xD7, 0x9A,
 0x8A, 0xC3, 0x86, 0x75, 0xAE, 0x75, 0x65, 0x9A,
 0x65, 0x55, 0x8A, 0x55, 0x45, 0x75, 0x45, 0x34,
 0x61, 0x38, 0x28, 0x4D, 0x2C, 0x1C, 0x38, 0x20,
 0x14, 0x28, 0x14, 0x0C, 0xD3, 0xAE, 0xAE, 0xB2,
 0x92, 0x92, 0x96, 0x75, 0x79, 0x79, 0x5D, 0x61,
 0xFF, 0xFF, 0xCB, 0xEF, 0xEB, 0xA2, 0xDF, 0xD7,
 0x7D, 0xD3, 0xC3, 0x5D, 0xC3, 0xAE, 0x41, 0xB6,
 0x96, 0x24, 0xA6, 0x7D, 0x0C, 0x9A, 0x69, 0x00,
 0xE7, 0xDF, 0xA6, 0xCF, 0xC7, 0x8E, 0xBA, 0xAE,
 0x75, 0xA6, 0x96, 0x61, 0x8E, 0x7D, 0x4D, 0x79,
 0x69, 0x3C, 0x65, 0x55, 0x2C, 0x51, 0x41, 0x20,
 0xFF, 0xF7, 0x82, 0xFF, 0xDB, 0x49, 0xE7, 0xBA,
 0x38, 0xCF, 0x9E, 0x2C, 0xBA, 0x82, 0x20, 0xA2,
 0x69, 0x18, 0x8E, 0x51, 0x10, 0x75, 0x3C, 0x08,
 0x5D, 0x28, 0x04, 0x49, 0x18, 0x00, 0x30, 0x0C,
 0x00, 0x1C, 0x04, 0x00, 0x9E, 0x9A, 0x2C, 0x86,
 0x7D, 0x08, 0x71, 0x65, 0x08, 0x55, 0x49, 0x08,
 0xE7, 0x9E, 0x69, 0xE7, 0x8E, 0x55, 0xE7, 0x79,
 0x3C, 0xD3, 0x69, 0x34, 0xBE, 0x59, 0x2C, 0xAA,
 0x49, 0x24, 0x9A, 0x3C, 0x1C, 0x86, 0x30, 0x14,
 0x71, 0x24, 0x10, 0x5D, 0x1C, 0x0C, 0x49, 0x14,
 0x08, 0x38, 0x0C, 0x04, 0xE3, 0x69, 0x00, 0xC3,
 0x49, 0x00, 0xA6, 0x30, 0x00, 0x79, 0x20, 0x00,
 0xFF, 0xFF, 0x86, 0xEF, 0xDB, 0x69, 0xDF, 0xB6,
 0x51, 0xCF, 0x8E, 0x38, 0xBE, 0x69, 0x28, 0xAE,
 0x41, 0x14, 0x9E, 0x1C, 0x08, 0x7D, 0x00, 0x00,
 0xFF, 0x59, 0x55, 0xFF, 0x28, 0x2C, 0xDB, 0x00,
 0x00, 0xAE, 0x00, 0x00, 0xB2, 0x00, 0x59, 0x8E,
 0x00, 0x4D, 0x6D, 0x00, 0x41, 0x4D, 0x00, 0x30,
 0xCB, 0x20, 0xBA, 0xAA, 0x10, 0x9A, 0x8A, 0x04,
 0x7D, 0x6D, 0x00, 0x61, 0xBE, 0x86, 0xAA, 0xA2,
 0x6D, 0x8E, 0x8A, 0x55, 0x71, 0x71, 0x41, 0x59,
 0xCF, 0x9E, 0xE7, 0xB6, 0x79, 0xD3, 0x9E, 0x5D,
 0xC3, 0x8A, 0x41, 0xAE, 0x75, 0x2C, 0x9E, 0x61,
 0x18, 0x8A, 0x51, 0x08, 0x79, 0x41, 0x00, 0x69,
 0xAA, 0x7D, 0xFF, 0x9E, 0x59, 0xFF, 0x96, 0x3C,
 0xF3, 0x75, 0x2C, 0xD3, 0x59, 0x20, 0xB2, 0x41,
 0x14, 0x92, 0x2C, 0x0C, 0x75, 0x1C, 0x00, 0x5D,
 0xBE, 0xD3, 0xEB, 0x9A, 0xB2, 0xCF, 0x7D, 0x96,
 0xB2, 0x61, 0x79, 0x9A, 0xBA, 0xD3, 0xBE, 0x9E,
 0xAE, 0x92, 0x8A, 0x8E, 0x6D, 0x61, 0x69, 0x4D,
 0xE7, 0xF7, 0xFF, 0xC3, 0xDF, 0xEF, 0xA6, 0xCB,
 0xE3, 0x8A, 0xB6, 0xD7, 0x6D, 0x9E, 0xCB, 0x55,
 0x8A, 0xBE, 0x41, 0x75, 0xAE, 0x2C, 0x61, 0xA2,
 0x18, 0x4D, 0x96, 0x0C, 0x3C, 0x8A, 0x9A, 0xA2,
 0xFF, 0x8A, 0x8A, 0xFF, 0x82, 0x69, 0xFF, 0x82,
 0x49, 0xFF, 0x69, 0x34, 0xDB, 0x55, 0x20, 0xBA,
 0xF3, 0xFF, 0xFF, 0xCB, 0xEB, 0xEB, 0xA6, 0xD7,
 0xDB, 0x86, 0xC3, 0xCB, 0x69, 0xAE, 0xBA, 0x51,
 0x9A, 0xAA, 0x38, 0x86, 0x96, 0x24, 0x71, 0x86,
 0x14, 0x5D, 0x75, 0x08, 0x4D, 0x65, 0x49, 0xB2,
 0xEF, 0x1C, 0x96, 0xDF, 0x0C, 0x75, 0xD7, 0x04,
 0x5D, 0xB6, 0x00, 0x49, 0x96, 0x00, 0x38, 0x75,
 0xE3, 0xFF, 0xB6, 0xC7, 0xE7, 0x9A, 0xAA, 0xD3,
 0x7D, 0x8E, 0xBE, 0x65, 0x75, 0xA6, 0x51, 0x5D,
 0x92, 0x3C, 0x49, 0x7D, 0x2C, 0x34, 0x69, 0x20,
 0x45, 0xEB, 0x71, 0x30, 0xC7, 0x61, 0x20, 0xA6,
 0x51, 0x00, 0x86, 0x45, 0x08, 0x6D, 0x41, 0x04,
 0x55, 0x3C, 0x00, 0x3C, 0x34, 0x00, 0x20, 0x20,
 0xFF, 0xEF, 0xCB, 0xE7, 0xDB, 0x9E, 0xD3, 0xCF,
 0x79, 0xB6, 0xBE, 0x59, 0x96, 0xAA, 0x3C, 0xE7,
 0xF7, 0x59, 0xC3, 0xE3, 0x18, 0x9E, 0xCB, 0x18,
 0x82, 0xB6, 0x18, 0x69, 0xA2, 0x18, 0x51, 0x8E,
 0x14, 0x3C, 0x79, 0x14, 0x2C, 0x65, 0x10, 0x20,
 0x51, 0x10, 0x14, 0x3C, 0x0C, 0x00, 0x24, 0x00,
 0xFF, 0xD7, 0xC3, 0xFF, 0xE7, 0xDB, 0xFF, 0xFB,
 0xBE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x79, 0x96, 0x82, 0x00, 0x00, 0x00,
 0x49, 0x92, 0xA2, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x34, 0x30, 0x30, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0xD7, 0xB2, 0xDF};

// SCHMs transition lower 96 SPRI indices
// some transparent indices probably used for special effects
u1 Trans[256][8] = {
/*   0  0 */ {255, 255, 255, 255, 255, 255, 255, 255},
/*   1  1 */ { 10,  10,  12,  14,  16,  18,  20,  22},
/*   2  2 */ { 10,  12,  14,  16,  18,  20,  22,  24},
/*   3  3 */ { 12,  14,  16,  18,  20,  22,  24,  24},
/*   4  4 */ { 14,  16,  18,  20,  22,  24,  24,  24},
/*   5  5 */ { 10,  10,  11,  12,  13,  14,  15,  16},
/*   6  6 */ { 10,  11,  12,  13,  14,  15,  16,  17},
/*   7  7 */ { 11,  12,  13,  14,  15,  16,  17,  18},
/*   8  8 */ { 12,  13,  14,  15,  16,  17,  18,  19},
/*   9  9 */ { 13,  14,  15,  16,  17,  18,  19,  20},
/*  10  A */ { 14,  15,  16,  17,  18,  19,  20,  21},
/*  11  B */ { 15,  16,  17,  18,  19,  20,  21,  22},
/*  12  C */ { 16,  17,  18,  19,  20,  21,  22,  23},
/*  13  D */ { 17,  18,  19,  20,  21,  22,  23,  24},
/*  14  E */ { 18,  19,  20,  21,  22,  23,  24,  24},
/*  15  F */ { 19,  20,  21,  22,  23,  24,  24,  24},
/*  16 10 */ { 21,  22,  23,  24,  24,  24,  24,  24},
/*  17 11 */ {161, 161, 162, 163, 164,  18,  19,  20},
/*  18 12 */ {161, 162, 163, 164,  18,  19,  20,  21},
/*  19 13 */ {162, 163, 164,  18,  19,  20,  21,  22},
/*  20 14 */ {163, 164,  18,  19,  20,  21,  22,  23},
/*  21 15 */ { 25,  25,  26,  27,  28,  29,  30,  31},
/*  22 16 */ { 25,  26,  27,  28,  29,  30,  31,  32},
/*  23 17 */ { 27,  28,  29,  30,  31,  32,  20,  21},
/*  24 18 */ { 10,  10,  11,  33,  34,  35,  36,  37},
/*  25 19 */ { 10,  11,  33,  34,  35,  36,  37,  38},
/*  26 1A */ { 11,  33,  34,  35,  36,  37,  38,  39},
/*  27 1B */ { 33,  34,  35,  36,  37,  38,  39,  40},
/*  28 1C */ { 34,  35,  36,  37,  38,  39,  40,  40},
/*  29 1D */ { 41,  41,  42,  43,  44,  45,  46,  47},
/*  30 1E */ { 41,  42,  43,  44,  45,  46,  47,  48},
/*  31 1F */ { 42,  43,  44,  45,  46,  47,  48,  49},
/*  32 20 */ { 43,  44,  45,  46,  47,  48,  49,  50},
/*  33 21 */ { 44,  45,  46,  47,  48,  49,  50,  51},
/*  34 22 */ { 45,  46,  47,  48,  49,  50,  51,  52},
/*  35 23 */ { 46,  47,  48,  49,  50,  51,  52,  24},
/*  36 24 */ {217, 217,  81,  82,  83,  84,  85,  86},
/*  37 25 */ {217,  81,  82,  83,  84,  85,  86,  87},
/*  38 26 */ { 81,  82,  83,  84,  85,  86,  87,  88},
/*  39 27 */ { 82,  83,  84,  85,  86,  87,  88,  51},
/*  40 28 */ { 83,  84,  85,  86,  87,  88,  51,  52},
/*  41 29 */ { 84,  85,  86,  87,  88,  51,  52,  24},
/*  42 2A */ {165, 165, 166, 167, 168,  56,  88,  51},
/*  43 2B */ { 33,  33,  34,  35,  53,  54,  55,  56},
/*  44 2C */ { 33,  34,  35,  53,  54,  55,  56,  88},
/*  45 2D */ { 34,  35,  53,  54,  55,  56,  88,  51},
/*  46 2E */ {233,  57,  58,  59,  60,  61,  62,  63},
/*  47 2F */ { 57,  58,  59,  60,  61,  62,  63,  64},
/*  48 30 */ { 58,  59,  60,  61,  62,  63,  64,  65},
/*  49 31 */ { 59,  60,  61,  62,  63,  64,  65,  66},
/*  50 32 */ { 60,  61,  62,  63,  64,  65,  66,  67},
/*  51 33 */ { 61,  62,  63,  64,  65,  66,  67,  68},
/*  52 34 */ { 62,  63,  64,  65,  66,  67,  68,  24},
/*  53 35 */ { 63,  64,  65,  66,  67,  68,  24,  24},
/*  54 36 */ { 73,  73,  73,  74,  75,  76,  77,  78},
/*  55 37 */ { 73,  73,  74,  75,  76,  77,  78,  79},
/*  56 38 */ { 73,  74,  75,  76,  77,  78,  79,  80},
/*  57 39 */ { 74,  75,  76,  77,  78,  79,  80,  95},
/*  58 3A */ { 75,  76,  77,  78,  79,  80,  95,  96},
/*  59 3B */ { 76,  77,  78,  79,  80,  95,  96,  97},
/*  60 3C */ { 77,  78,  79,  80,  95,  96,  97,  98},
/*  61 3D */ { 78,  79,  80,  95,  96,  97,  98,  99},
/*  62 3E */ { 10,  73,  89,  90,  91,  92,  93,  94},
/*  63 3F */ { 73,  89,  90,  91,  92,  93,  94,  95},
/*  64 40 */ { 89,  90,  91,  92,  93,  94,  95,  96},
/*  65 41 */ { 90,  91,  92,  93,  94,  95,  96,  97},
/*  66 42 */ { 91,  92,  93,  94,  95,  96,  97,  98},
/*  67 43 */ { 92,  93,  94,  95,  96,  97,  98,  99},
/*  68 44 */ { 93,  94,  95,  96,  97,  98,  99, 100},
/*  69 45 */ { 94,  95,  96,  97,  98,  99, 100,  24},
/*  70 46 */ { 73, 121, 122, 123, 124, 125, 126, 127},
/*  71 47 */ {121, 122, 123, 124, 125, 126, 127, 128},
/*  72 48 */ {122, 123, 124, 125, 126, 127, 128, 114},
/*  73 49 */ {123, 124, 125, 126, 127, 128, 114, 115},
/*  74 4A */ {124, 125, 126, 127, 128, 114, 115, 116},
/*  75 4B */ {129, 129, 130, 131, 132, 128, 114, 115},
/*  76 4C */ {129, 130, 131, 132, 128, 114, 115, 116},
/*  77 4D */ {130, 131, 132, 128, 114, 115, 116, 116},
/*  78 4E */ {105, 105, 106, 107, 108, 109, 110, 111},
/*  79 4F */ {105, 106, 107, 108, 109, 110, 111, 112},
/*  80 50 */ {106, 107, 108, 109, 110, 111, 112, 113},
/*  81 51 */ {107, 108, 109, 110, 111, 112, 113, 114},
/*  82 52 */ {108, 109, 110, 111, 112, 113, 114, 115},
/*  83 53 */ {109, 110, 111, 112, 113, 114, 115, 116},
/*  84 54 */ {110, 111, 112, 113, 114, 115, 116,  24},
/*  85 55 */ {111, 112, 113, 114, 115, 116,  24,  24},
/*  86 56 */ { 58, 105, 107, 117, 118, 119, 120, 115},
/*  87 57 */ {105, 107, 117, 118, 119, 120, 115, 116},
/*  88 58 */ {107, 117, 118, 119, 120, 115, 116,  24},
/*  89 59 */ {117, 118, 119, 120, 115, 116,  24,  24},
/*  90 5A */ {133, 133, 134, 135, 136, 116,  24,  24},
/*  91 5B */ {133, 134, 135, 136, 116,  24,  24,  24},
/*  92 5C */ {134, 135, 136, 116,  24,  24,  24,  24},
/*  93 5D */ {137, 137, 137, 138, 139, 140, 136, 116},
/*  94 5E */ {137, 137, 138, 139, 140, 136, 116,  24},
/*  95 5F */ {137, 138, 139, 140, 136, 116,  24,  24},
/*  96 60 */ { 69,  69, 141, 142, 143, 144,  66,  67},
/*  97 61 */ {145, 145, 146, 147, 148, 149, 150, 151},
/*  98 62 */ {145, 146, 147, 148, 149, 150, 151, 152},
/*  99 63 */ {146, 147, 148, 149, 150, 151, 152,  22},
/* 100 64 */ {147, 148, 149, 150, 151, 152,  22,  23},
/* 101 65 */ {148, 149, 150, 151, 152,  22,  23,  24},
/* 102 66 */ {149, 150, 151, 152,  22,  23,  24,  24},
/* 103 67 */ {150, 151, 152,  22,  23,  24,  24,  24},
/* 104 68 */ {153, 153, 154, 155, 156, 157, 158, 159},
/* 105 69 */ {153, 154, 155, 156, 157, 158, 159, 160},
/* 106 6A */ {154, 155, 156, 157, 158, 159, 160,  22},
/* 107 6B */ {155, 156, 157, 158, 159, 160,  22,  23},
/* 108 6C */ {156, 157, 158, 159, 160,  22,  23,  24},
/* 109 6D */ {157, 158, 159, 160,  22,  23,  24,  24},
/* 110 6E */ {171, 172, 195, 196, 197, 198, 199, 200},
/* 111 6F */ {172, 195, 196, 197, 198, 199, 200,  21},
/* 112 70 */ {195, 196, 197, 198, 199, 200,  21,  22},
/* 113 71 */ {196, 197, 198, 199, 200,  21,  22,  23},
/* 114 72 */ {197, 198, 199, 200,  21,  22,  23,  24},
/* 115 73 */ {198, 199, 200,  21,  22,  23,  24,  24},
/* 116 74 */ {169, 169, 170, 171, 172, 173, 174, 175},
/* 117 75 */ {169, 170, 171, 172, 173, 174, 175, 176},
/* 118 76 */ {170, 171, 172, 173, 174, 175, 176, 177},
/* 119 77 */ {171, 172, 173, 174, 175, 176, 177, 178},
/* 120 78 */ {172, 173, 174, 175, 176, 177, 178, 158},
/* 121 79 */ {173, 174, 175, 176, 177, 178, 158, 159},
/* 122 7A */ {174, 175, 176, 177, 178, 158, 159, 160},
/* 123 7B */ {175, 176, 177, 178, 158, 159, 160,  22},
/* 124 7C */ {176, 177, 178, 158, 159, 160,  22,  23},
/* 125 7D */ {177, 178, 158, 159, 160,  22,  23,  24},
/* 126 7E */ {178, 158, 159, 160,  22,  23,  24,  24},
/* 127 7F */ {185, 185, 186, 187, 188, 189, 190, 191},
/* 128 80 */ {185, 186, 187, 188, 189, 190, 191, 192},
/* 129 81 */ {186, 187, 188, 189, 190, 191, 192, 193},
/* 130 82 */ {187, 188, 189, 190, 191, 192, 193, 194},
/* 131 83 */ {188, 189, 190, 191, 192, 193, 194, 215},
/* 132 84 */ {189, 190, 191, 192, 193, 194, 215, 216},
/* 133 85 */ {190, 191, 192, 193, 194, 215, 216,  24},
/* 134 86 */ {191, 192, 193, 194, 215, 216,  24,  24},
/* 135 87 */ {179, 179, 180, 181, 182, 183, 184, 158},
/* 136 88 */ {179, 180, 181, 182, 183, 184, 158, 159},
/* 137 89 */ {180, 181, 182, 183, 184, 158, 159, 160},
/* 138 8A */ {181, 182, 183, 184, 158, 159, 160,  22},
/* 139 8B */ {182, 183, 184, 158, 159, 160,  22,  23},
/* 140 8C */ {183, 184, 158, 159, 160,  22,  23,  24},
/* 141 8D */ {184, 158, 159, 160,  22,  23,  24,  24},
/* 142 8E */ {222, 222, 223, 224, 225, 226, 227, 228},
/* 143 8F */ {222, 223, 224, 225, 226, 227, 228, 229},
/* 144 90 */ {223, 224, 225, 226, 227, 228, 229, 230},
/* 145 91 */ {224, 225, 226, 227, 228, 229, 230, 231},
/* 146 92 */ {225, 226, 227, 228, 229, 230, 231, 232},
/* 147 93 */ {226, 227, 228, 229, 230, 231, 232,  24},
/* 148 94 */ {227, 228, 229, 230, 231, 232,  24,  24},
/* 149 95 */ { 10, 201, 202, 203, 204, 205, 206, 207},
/* 150 96 */ {201, 202, 203, 204, 205, 206, 207, 208},
/* 151 97 */ {202, 203, 204, 205, 206, 207, 208, 230},
/* 152 98 */ {203, 204, 205, 206, 207, 208, 230, 231},
/* 153 99 */ {204, 205, 206, 207, 208, 230, 231, 232},
/* 154 9A */ {205, 206, 207, 208, 230, 231, 232,  24},
/* 155 9B */ {206, 207, 208, 230, 231, 232,  24,  24},
/* 156 9C */ {209, 209, 210, 211, 212, 213, 214, 215},
/* 157 9D */ {209, 210, 211, 212, 213, 214, 215, 216},
/* 158 9E */ {210, 211, 212, 213, 214, 215, 216,  24},
/* 159 9F */ {211, 212, 213, 214, 215, 216,  24,  24},
/* 160 A0 */ {217, 217, 218, 219, 220, 221, 206, 207},
/* 161 A1 */ {217, 218, 219, 220, 221, 206, 207, 208},
/* 162 A2 */ {218, 219, 220, 221, 206, 207, 208, 230},
/* 163 A3 */ {219, 220, 221, 206, 207, 208, 230, 231},
/* 164 A4 */ {220, 221, 206, 207, 208, 230, 231, 232},
/* 165 A5 */ {221, 206, 207, 208, 230, 231, 232,  24},
/* 166 A6 */ {218, 218, 219, 220, 101, 102, 103, 104},
/* 167 A7 */ {218, 219, 220, 101, 102, 103, 104, 104},
/* 168 A8 */ {219, 220, 101, 102, 103, 104, 104, 104},
/* 169 A9 */ { 10,  15,  18,  20,  22,  23,  24,  24},
/* 170 AA */ { 12,  16,  19,  21,  22,  23,  24,  24},
/* 171 AB */ { 15,  18,  20,  22,  23,  24,  24,  24},
/* 172 AC */ { 17,  20,  22,  23,  24,  24,  24,  24},
/* 173 AD */ { 10,  44,  47,  49,  50,  51,  52,  24},
/* 174 AE */ { 44,  47,  49,  51,  52,  24,  24,  24},
/* 175 AF */ { 48,  50,  51,  52,  24,  24,  24,  24},
/* 176 B0 */ { 49,  52,  24,  24,  24,  24,  24,  24},
/* 177 B1 */ {217,  82,  85,  87,  88,  51,  52,  24},
/* 178 B2 */ { 82,  84,  87,  88,  51,  52,  24,  24},
/* 179 B3 */ { 85,  87,  51,  24,  24,  24,  24,  24},
/* 180 B4 */ { 57,  60,  63,  66,  67,  68,  24,  24},
/* 181 B5 */ { 59,  62,  64,  66,  67,  68,  24,  24},
/* 182 B6 */ { 62,  64,  67,  68,  24,  24,  24,  24},
/* 183 B7 */ { 10,  90,  93,  96,  98,  99, 100,  24},
/* 184 B8 */ { 89,  91,  94,  97,  98,  99, 100,  24},
/* 185 B9 */ { 91,  93,  95,  98,  99, 100,  24,  24},
/* 186 BA */ { 92,  95,  98,  99, 100,  24,  24,  24},
/* 187 BB */ {222, 225, 227, 229, 231, 232,  24,  24},
/* 188 BC */ {224, 226, 228, 230, 232,  24,  24,  24},
/* 189 BD */ {226, 228, 230, 232,  24,  24,  24,  24},
/* 190 BE */ {228, 230, 232,  24,  24,  24,  24,  24},
/* 191 BF */ {169, 172, 175, 177, 178, 158, 159, 160},
/* 192 C0 */ {169, 173, 177, 158, 160,  22,  23,  24},
/* 193 C1 */ {179, 182, 184, 158, 160,  22,  24,  24},
/* 194 C2 */ { 10,  10, 234, 233,  57,  58,  59,  60},
/* 195 C3 */ { 10, 234, 233,  57,  58,  59,  60,  61},
/* 196 C4 */ {234, 233,  57,  58,  59,  60,  61,  62},
/* 197 C5 */ {188, 187, 186, 185,  47,  46,  45,  44},
/* 198 C6 */ {255, 255,  32,  78,  27,  16, 255, 255},
/* 199 C7 */ {255, 255,   1, 255,  38,  78,  27,  16},
/* 200 C8 */ {255, 255, 255, 255,   2, 255,  40,  78},
/* 201 C9 */ { 27,  16, 255, 255, 255, 255,   3, 255},
/* 202 CA */ { 42,  78,  27,  16, 255, 255, 255, 255},
/* 203 CB */ {  4,   1,  44,  78,  27,  16, 255, 255},
/* 204 CC */ { 65, 102, 114,  97, 105, 100, 255, 255},
/* 205 CD */ {255, 255, 255, 255, 255, 255, 255, 255},
/* 206 CE */ {255, 255, 255, 255, 255, 255, 255, 255},
/* 207 CF */ { 83, 121, 110, 116,  97, 120,  32,  58},
/* 208 D0 */ { 32,  70,  84,  65,  50,  32,  91,  47},
/* 209 D1 */ { 77,  32,  98,  98,  98,  98,  93,  32},
/* 210 D2 */ { 10,  32,  32,  47,  77,  32,  58,  32},
/* 211 D3 */ {104,  97, 118, 101,  32, 116, 104, 101},
/* 212 D4 */ { 32, 103,  97, 109, 101,  32, 114, 117},
/* 213 D5 */ {110,  32, 105, 110,  32,  97,  32, 104},
/* 214 D6 */ {101,  97, 112,  32, 111, 102,  32, 115},
/* 215 D7 */ {105, 122, 101,  32,  39,  98,  98,  98},
/* 216 D8 */ { 98,  39,  32,  40, 105, 110,  32,  98},
/* 217 D9 */ {121, 116, 101, 115,  41,  10, 255, 255},
/* 218 DA */ {255, 255,   8, 255,   1, 255, 255, 255},
/* 219 DB */ {255, 255, 255, 255, 255, 255, 255, 255},
/* 220 DC */ {255, 255, 255, 255, 255, 255, 255, 255},
/* 221 DD */ {255, 255, 255, 255, 255, 255, 255, 255},
/* 222 DE */ {255, 255, 255, 255, 255, 255, 255, 255},
/* 223 DF */ {255, 255, 255, 255, 255, 255, 255, 255},
/* 224 E0 */ {255, 255, 255, 255, 255, 255, 255, 255},
/* 225 E1 */ {255, 255, 255, 255, 255, 255, 255, 255},
/* 226 E2 */ {255, 255, 255, 255, 255, 255, 255, 255},
/* 227 E3 */ {255, 255, 255, 255, 255, 255, 255, 255},
/* 228 E4 */ {255, 255, 255, 255, 255, 255, 255, 255},
/* 229 E5 */ {255, 255, 255, 255, 255, 255, 255, 255},
/* 230 E6 */ {255, 255, 255, 255, 255, 255, 255, 255},
/* 231 E7 */ {255, 255, 255, 255, 255, 255, 255, 255},
/* 232 E8 */ {255, 255, 255, 255, 255, 255, 255, 255},
/* 233 E9 */ {255, 255, 255, 255, 255, 255, 255, 255},
/* 234 EA */ {255, 255, 255, 255, 255, 255, 255, 255},
/* 235 EB */ {255, 255, 255, 255, 255, 255, 255, 255},
/* 236 EC */ {255, 255, 255, 255, 255, 255, 255, 255},
/* 237 ED */ {255, 255, 255, 255, 255, 255, 255, 255},
/* 238 EE */ {255, 255, 255, 255, 255, 255, 255, 255},
/* 239 EF */ {255, 255, 255, 255, 255, 255, 255, 255},
/* 240 F0 */ {255, 255, 255, 255, 255, 255, 255, 255},
/* 241 F1 */ {255, 255, 255, 255, 255, 255, 255, 255},
/* 242 F2 */ {255, 255, 255, 255, 255, 255, 255, 255},
/* 243 F3 */ {255, 255, 255, 255, 255, 255, 255, 255},
/* 244 F4 */ {255, 255, 255, 255, 255, 255, 255, 255},
/* 245 F5 */ {255, 255, 255, 255, 255, 255, 255, 255},
/* 246 F6 */ {255, 255, 255, 255, 255, 255, 255, 255},
/* 247 F7 */ {255, 255, 255, 255, 255, 255, 255, 255},
/* 248 F8 */ {255, 255, 255, 255, 255, 255, 255, 255},
/* 249 F9 */ {255, 255, 255, 255, 255, 255, 255, 255},
/* 250 FA */ {255, 255, 255, 255, 255, 255, 255, 255},
/* 251 FB */ {255, 255, 255, 255,  65, 115, 108, 101},
/* 252 FC */ {101, 112, 255, 255,  80,  97, 114,  97},
/* 253 FD */ {108, 121, 115, 101, 100, 255, 255, 255},
/* 254 FE */ { 66, 108, 105, 110, 100, 255, 255, 255},
/* 255 FF */ { 65, 102, 114,  97, 105, 100, 255, 255},
};




static void saveSPRI (char *Output, char *Path, char *Tag, u1 *M, int L) {
  char Tmp[1024], Dir[16], Anim[16];
  u1 RT[256];
  int I, J, K, C, CX=1, CY=1, W, H, X, Y, N=128;
  pic *P, *S;
  int NFrames = *(u4*)M;
  u4 *Offs = (u4*)(M+4);
  schm *R, *Rs = 0;

  strncpy(Dir, Tag, 3);
  strncpy(Anim, Tag+3, 3);
  Path[strlen(Path)-6] = 0;
  Anim[3] = 0;
  Dir[3] = 0;

  times (I, NFrames) {
    frameHeader *F = (frameHeader*)(M+Offs[I]);
    CX = max(CX,abs(F->X)+(F->Width+1)/2);
    CY = max(CY,abs(F->Y)+(F->Height+1)/2);
  }

  W = CX*2;
  H = CY*2;

  S = picNew(W*NFrames, H, 8);
  memcpy(S->P, Pal, 256*4);
  S->K = 255;


#if 0
  times (I, 0x100) {
    int XY[256][2] = {
     {228,208}, //32
     {242,239}, //33
     {229,208}, //34
     {243,240}, //35
     {241,239}, //36
     {243,242}, //37
     {241,242}, //38
     {225,206}  //39
    };
    printf("/* %3d %2X */ ", I+1, I+1);
    printf("{");
    times(J, 8) {
      sprintf(Tmp, "input/0/%03x.png.pcx", I+1);
      P = pcxLoad(Tmp);
      if (J) printf(", ");
      printf("%3d", picGet(P, XY[J][0],XY[J][1]));
    }
    printf("},\n");
  }
  abort();
#endif


  times (I, NSchms) if (!strncmp(Tag, SchmTags[I], 3)) {
    Rs = Schm[I];
    break;
  }

  if (!strncmp(Tag, "OBJ", 3)) {
    Rs = &OBJSchm;
    N = 1;
  }

  unless (Rs) {
    printf("  sprite has no recolor schemes\n");

    times (I, NFrames) {
      frameHeader *F = (frameHeader*)(M+Offs[I]);
      pic *P = picNew(F->Width, F->Height, 8);
      u1 *Q = (u1*)(F+1);
      //printf("%02d: O=#%06x W=%d H=%d X=%d Y=%d\n"
      //      ,I, Offs[I], F->Width, F->Height, F->X, F->Y);

      memset(P->D, 0xFF, P->H*P->I);
      for (J = 0; J < F->Width*F->Height; ) {
        J += *Q++;
        C = *Q++;
        while (C-- && J < F->Width*F->Height) P->D[J++] = *Q++;
      }
      times (Y,P->H) times (X, P->W)
        picPut(S, I*W+CX+F->X+X, CY+F->Y+Y, picGet(P, X, Y));
      picDel(P);
    }
    sprintf(Tmp, "%s/%s%s/%s.png", Output, Path, Dir, Anim);
    pngSave(Tmp, S);
    goto end;
  }

  times (K, N) {
    R = Rs+K;
    times (I, 32)
     if (R->Name[I] == ' ' || R->Name[I] == '*'|| R->Name[I] == '?'
      || R->Name[I] == '/' || R->Name[I] == '!') {
       if (I == 31 || !R->Name[I+1]) R->Name[I] = 0;
       else R->Name[I] = '_';
     }
    if (!strcmp(R->Name, "Untitled")) continue;

    // calc recolor table
    times (I, 256) RT[I] = I; // is this okay?
    times (I, 8) RT[I] = 8+I*2; // lower 8 indidecs always select grades of gray
    times (I, 12) memcpy(RT+RO[I], Trans[R->Part[I]], 8);


    memset(S->D, 0xFF, S->H*S->I);

    times (I, NFrames) {
      frameHeader *F = (frameHeader*)(M+Offs[I]);
      pic *P = picNew(F->Width, F->Height, 8);
      u1 *Q = (u1*)(F+1);
      //printf("%02d: O=#%06x W=%d H=%d X=%d Y=%d\n"
      //      ,I, Offs[I], F->Width, F->Height, F->X, F->Y);

      memset(P->D, 0xFF, P->H*P->I);
      for (J = 0; J < F->Width*F->Height; ) {
        J += *Q++;
        C = *Q++;
        while (C-- && J < F->Width*F->Height) P->D[J++] = RT[*Q++];
      }
      times (Y,P->H) times (X, P->W)
        picPut(S, I*W+CX+F->X+X, CY+F->Y+Y, picGet(P, X, Y));
      picDel(P);
    }
    sprintf(Tmp, "%s/%s%s/%s_%s.png", Output, Path, Dir, R->Name, Anim);
    pngSave(Tmp, S);
  }
end:
  picDel(S);
}

static void hrsDecompile (char *Output, char *Input) {
  int GotPal = 0;
  pic *P;
  WAVHeader W;
  FILE *OF;
  u4 NFrames, *Offs;
  int X, Y, I, J, C, N, SP=0;
  u1 *M, *Q;
  char HRSName[128], Tmp[1024], Tmp2[1024], Id[5], Name[128], *NStack[256];
  entry *Dir, *E, *Stack[256];

  pathParts(0, HRSName, 0, Input);
  downcase(HRSName);

  if (!strcmp(HRSName, "dinoimag")) M = DinoPal;
  else M = FTAPal;

  times(I, 256) {
    Pal[I*4+0] = M[I*3+0];
    Pal[I*4+1] = M[I*3+1];
    Pal[I*4+2] = M[I*3+2];
  }

  Dir = Stack[SP++] = readFile(0, sizeof(entry), Input);
  while (SP) {
    if (Dir->Len <= 0) {
      Dir = Stack[--SP - 1];
      //free(NStack[SP]);
      continue;
    }
    E = readFile(Dir->Off, Dir->Len, Input);
    Dir->Off += sizeof(entry);
    Dir->Len -= sizeof(entry);

    memcpy(Id, E->Id, 4);
    Id[4] = 0;
    if (!strncmp(Id, "PRO", 3)
     || !strncmp(Id, "SEG", 3) // script
     || !strncmp(Id, "SEH", 3) // script
     || !strncmp(Id, "XMI", 3) // midi
     || !strncmp(Id, "XMI", 3) // midi

     || !strncmp(Id, "TER", 3) // terrain sound
     || !strncmp(Id, "SPL", 3) // spell sound
     || !strncmp(Id, "TAG", 3) // door open, lockpick, etc..
     || !strncmp(Id, "CBT", 3) // creature sounds
     || !strncmp(Id, "SFX", 3) // various other sound effects
     || !strncmp(Id, "GNT", 3) // creature hit sounds
     || !strncmp(Id, "CLK", 3) // mouse click sounds


     || !strncmp(Dir->Id, "PTRL", 4)
     || !strncmp(Dir->Id, "SPEL", 4)
     || !strncmp(Dir->Id, "ITEM", 4)

     || !strncmp(Dir->Id, "AMAP", 4)
     || !strncmp(Dir->Id, "IMAG", 4)
     || !strncmp(Dir->Id, "SPRI", 4)
     || !strncmp(Dir->Id, "TILE", 4)
     || !strncmp(Dir->Id, "FACE", 4)
     || !strncmp(Dir->Id, "BOOK", 4)
     || !strncmp(Dir->Id, "VIDO", 4)
     || !strncmp(Dir->Id, "CONT", 4)

     || !strncmp(Dir->Id, "IMGA", 4)
     || !strncmp(Dir->Id, "DINO", 4)
     || !strncmp(Dir->Id, "DINP", 4)

     || !strncmp(Id, "SRL", 3) || !strncmp(Id, "RNG", 3) || !strncmp(Id, "STA", 3)
     || !strncmp(Id, "BTN", 3) || !strncmp(Id, "BRD", 3) || !strncmp(Id, "PLQ", 3)
     || !strncmp(Id, "SLD", 3) || !strncmp(Id, "OPT", 3)
     || !strncmp(Id, "MAP", 3) || !strncmp(Id, "SUM", 3)

     || !strncmp(Id, "ZON", 3)
     || !strncmp(Id, "RTE", 3)
     || !strncmp(Id, "EFF", 3)
     || !strncmp(Id, "INF", 3)
     ) {
      N = Id[3];
      Id[3] = 0;
      sprintf(Name, "%s%03u", Id, (u1)N);
    } else {
      strcpy(Name, Id);
    }

    //printf("Id=%s Off=#%08X Len=#%08X\n", Name, E->Off, E->Len);
    // tag directories we know about
    if (
     // FTA.HRS
        !strcmp(Id, "LIST") // various files

     // SCRIPTS.HRS
     || !strcmp(Id, "SAGA") // Scripts for Animated Graphic Adventures

     // FTASOUND.HRS
     || !strcmp(Id, "MIHI") // XMI midi files
     || !strcmp(Id, "MILO") // XMI midi files
     || !strcmp(Id, "LOOP") // ambient loops
     || !strcmp(Id, "LOUD") // all other sounds
     || !strcmp(Id, "TALK") // chacracter speeches (compressed)

     // FTADATA.HRS
     || !strcmp(Id, "PTRL") // patrols?
     || !strcmp(Id, "SPEL") // spells
     || !strcmp(Id, "ITEM") // items

     // FTAIMAGE.HRS
     || !strcmp(Id, "AMAP") // automap
     || !strcmp(Id, "BOOK") // book background
     || !strcmp(Id, "CONT") // container and inventory interface
     || !strcmp(Id, "DIAL") // dialogue box interface
     || !strcmp(Id, "FACE") // kevin, julian and phillip faces
     || !strcmp(Id, "IMAG") // use interface
     || !strcmp(Id, "MANA") // magic indicators
     || !strcmp(Id, "PARC") // dunno, maybe palettes?
     || !strcmp(Id, "FRML") // contains OBJS, which full of 4-byte entries
     || !strcmp(Id, "POSE") // animations
     || !strcmp(Id, "SCHM") // color schemes
     || !strcmp(Id, "SCRL") // scroll background
     || !strcmp(Id, "SPRI") // sprites
     || !strcmp(Id, "TILE") // tileset
     || !strcmp(Id, "VIDO") // video border

     // DINOIMAG.HRS
     || !strcmp(Id, "IMGA") // dinotopia interface
     || !strcmp(Id, "DINO") // dinotopia interface
     || !strcmp(Id, "DINP") // dinotopia interface
     ) {
      NStack[SP] = strdup(Name);
      Dir = Stack[SP++] = E;
      continue;
    }


    *Tmp2 = 0;
    times (I, SP) {
      if (I) strncat(Tmp2, NStack[I], 16);
      if (I) strcat(Tmp2, "/");
    }
    strcat(Tmp2, Name);

    //unless (strncmp(Dir->Id, "SPRI", 4) && strncmp(Dir->Id, "SCHM", 4)) continue;
    //if (strcmp(Name, "YNW010")) continue;

    M = readFile(E->Off, E->Len, Input);

    printf("Extracting: %s\n", Tmp2);

    if (!strncmp(Dir->Id, "LOOP", 4)
     || !strncmp(Dir->Id, "LOUD", 4)) {
      strcat(Tmp2, ".wav");

      memcpy(W.Id, "RIFF", 4);
      W.Len = 36 + E->Len;
      memcpy(W.Type, "WAVE", 4);
      memcpy(W.FmtId, "fmt ", 4);
      W.FmtLen = 16;
      W.Format = 1;
      W.Chns = 1;
      W.Freq = 22050;
      W.Depth = 16;
      W.ByteRate = W.Freq*W.Chns*W.Depth/8;
      W.Align = W.Chns*W.Depth/8;
      memcpy(W.DataId, "data", 4);
      W.DataLen = E->Len;

      sprintf(Tmp, "%s/%s", Output, Tmp2);
      writeFile(0, 0, Tmp, M); // create path
      OF = fopen(Tmp, "wb");
      fwrite(&W, 1, sizeof(WAVHeader), OF);
      fwrite(M, 1, E->Len, OF);
      fclose(OF);
    } else if ((!strcmp(HRSName, "ftaimage") || !strcmp(HRSName, "dinoimag") )&&
               (!strncmp(Dir->Id, "FACE", 4)
            ||  !strncmp(Dir->Id, "BOOK", 4)
            ||  !strncmp(Dir->Id, "SCRL", 4)
            ||  !strncmp(Dir->Id, "MANA", 4)
            ||  !strncmp(Dir->Id, "IMAG", 4)
            ||  !strncmp(Dir->Id, "IMGA", 4)
            ||  !strncmp(Dir->Id, "CONT", 4)
            ||  !strncmp(Dir->Id, "DIAL", 4)
            ||  !strncmp(Dir->Id, "AMAP", 4)
            ||  !strncmp(Dir->Id, "VIDO", 4)
            ||  !strncmp(Dir->Id, "DINO", 4)
            ||  !strncmp(Dir->Id, "DINP", 4)
               )) {
      //sprintf(Tmp, "%s/%s.bin", Output, Tmp2);
      //writeFile(0, E->Len, Tmp, M);

      imageHeader *F = (imageHeader*)M;
      //printf("%02d: W=%d H=%d\n", I, F->Width, F->Height);
      P = picNew(F->Width?F->Width:1, F->Height?F->Height:1, 8);
      memcpy(P->P, Pal, 256*4);
      Q = (u1*)(F+1);
      if (F->Compression) {
        for (J=0; J<F->Width*F->Height;) {
          C = *Q++;
          if (C&0x80) {
            C = 0xFF - C + 2;
            while (C-- && J<F->Width*F->Height) P->D[J++] = *Q;
            Q++;
          } else {
            C += 1;
            while (C-- && J<F->Width*F->Height) P->D[J++] = *Q++;
          }
        }
      } else { // uncompressed (but every scanline is aligned by 2)
        X = (P->W&1) ? P->W+1 : P->W;
        times (Y, P->H) memcpy(P->D+Y*P->W, Q+Y*X, P->W);
      }
      sprintf(Tmp, "%s/%s.png", Output, Tmp2);
      pngSave(Tmp, P);
    } else if (!strncmp(Dir->Id, "TILE", 4) && !strncmp(Id, "TIL", 3)) {
      unless (E->Len) continue; // some tilesets are empty
      strcat(Tmp2, ".png");

      P = picNew(64, 1024*23, 8); // FTA2 uses 64x32 tiles
      memcpy(P->P, Pal, 256*4);

      for (J = Y = 0; Y < P->H && J < E->Len; Y++) {
        X = 0;
        C = M[J++];
        while (C-- && X<P->W) P->D[Y*P->I+X++] = 0xFF;
        while (X < P->W) {
          C = M[J++];
          while (C-- && X<P->W) P->D[Y*P->I+X++] = M[J++];

          C = M[J++];
          while (C-- && X<P->W) P->D[Y*P->I+X++] = 0xFF;
        }
      }
      P->H = Y;

      sprintf(Tmp, "%s/%s", Output, Tmp2);
      pngSave(Tmp, P);
      picDel(P);
    } else if (!strncmp(Dir->Id, "SCHM", 4)) {
      Schm[NSchms] = (schm*)malloc(E->Len);
      memcpy(Schm[NSchms], M, E->Len);
      SchmTags[NSchms++] = strndup(Name,3);
    } else if (!strncmp(Dir->Id, "SPRI", 4)) saveSPRI (Output, Tmp2, Name, M, E->Len);
    else {
      if (!strncmp(Dir->Id, "MILO", 4) || !strncmp(Dir->Id, "MIHI", 4))
        strcat(Tmp2, ".xmi");
      sprintf(Tmp, "%s/%s", Output, Tmp2);
      writeFile(0, E->Len, Tmp, M);
    }
    free(M);
  }
}

int hrsInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "hrs";
  F->Description = "Faery Tale Adventure 2 and Dinotopia archives";
  F->Decompile = hrsDecompile;
  return 1;
}



