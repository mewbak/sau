//Credits go to SNV

#include "common.h"
#include "minilzo.h"


//entry into A* files
typedef struct {
  u4 N;
  u4 W;
  u4 H;
  u4 A; // 0
  u4 B; // 0
  u4 C; // -1
  s2 X;
  s2 Y;
  s2 X2;
  s2 Y2;
} __attribute__ ((packed)) entryA;


//entry ino C* files
typedef struct {
  u4 Off; // offset into corresponding data file
  u4 W;
  u4 H;
  u4 RLE;
  u4 SX;
  u4 SY;
  u4 EX;
  u4 EY;
  s2 CX;
  s2 CY;
  // I think some frames are composite from several other frames
  u4 C;
  u4 D;
  u4 E;
  u4 F;
  u4 G;
} __attribute__ ((packed)) entry;

typedef struct {
  u4 Size; //size of whole sprite
  u4 Data; //offset to pixels, from start of file
  u2 W;
  u2 H;
  //u2 Offs[(Data-12)/4];
  //u2 Data[Size-Data];
} __attribute__ ((packed)) header;

// FIXME: main/startup and objects.000/objects.000 hold some names
static struct {
  int A;
  int B;
  char *Name;
} Names[] = {
  {     0,   107, "NPC01"},
  {   108,   393, "Cat1"},
  {   394,   660, "Lizard"},
  {   661,  1145, "Assasin1"},
  {  1146,  1554, "Assasin2"},
  {  1555,  1968, "Assasin3"},
  {  1969,  2442, "Sand_Demon"},
  {  2443,  2606, "Bat"},
  {  2607,  2788, "Wasp1"},
  {  2789,  2860, "Bird1"},
  {  2861,  3231, "Deathknight1"},
  {  3232,  3508, "Snake1"},
  {  3509,  3785, "Boar"},
  {  3786,  4152, "Orc_Crossbow"},
  {  4153,  4511, "Skeleton_Archer"},
  {  4512,  4619, "Butterfly"},
  {  4620,  4950, "Cat2"},
  {  4951,  5244, "Skeleton_Fallen_Warrior"},
  {  5245,  5486, "Cow"},
  {  5487,  5616, "Grim_Reaper"},
  {  5617,  5999, "Deathknight2"},
  {  6000,  6325, "Boss_Josephina"},
  {  6326,  6482, "NPC02"},
  {  6483,  6554, "Bird2"},
  {  6555,  6711, "NPC03"},
  {  6712,  6719, "Wisp"},
  {  6720,  6859, "Frog"},
  {  6860,  7126, "NPC04"},
  {  7127,  7415, "Ghoul"},
  {  7416,  7796, "Spider_Giant"},
  {  7797,  8207, "Imp"},
  {  8208,  8539, "Imp_Chieftain"},
  {  8540,  8831, "Imp_Mage"},
  {  8832,  9123, "Imp_Shaman"},
  {  9124,  9445, "Boss_Iona"},
  {  9446,  9682, "NPC05"},
  {  9683,  9959, "NPC06"},
  {  9960, 10226, "NPC07"},
  { 10227, 10588, "Lizard_Dragon_Assasin"},
  { 10589, 10870, "Lizard_Dragon_Warrior"},
  { 10871, 11052, "Bee1"},
  { 11053, 11304, "NPC08"},
  { 11305, 11412, "NPC09"},
  { 11413, 11669, "Thug_Sword"},
  { 11670, 11921, "Thug_Scythe"},
  { 11922, 12185, "Thug_Crossbow"},
  { 12186, 12540, "Mummy"},
  { 12541, 12862, "Orc_Berserk_Warrior"},
  { 12863, 13149, "Orc_Giant"},
  { 13150, 13426, "NPC10"},
  { 13427, 13803, "Pet"},
  { 13804, 13913, "Pig"},
  { 13914, 14637, "Elf_Archer1"},
  { 14638, 14999, "Elf_Archer2"},
  { 15000, 15361, "Elf_Archer3"},
  { 15362, 15723, "Elf_Archer4"},
  { 15724, 16085, "Elf_Archer5"},
  { 16086, 16447, "Elf_Archer6"},
  { 16448, 16676, "Rabbit"},
  { 16677, 16970, "Rat"},
  { 16971, 17353, "Rogue"},
  { 17354, 18382, "Guard1"},
  { 18383, 18644, "Scorpion"},
  { 18645, 18921, "Scorpion_Steel"},
  { 18922, 19213, "Lobster"},
  { 19214, 19502, "Skeleton_Fallen_Warlord"},
  { 19503, 19789, "Snake2"},
  { 19790, 20011, "Spider_Small"},
  { 20012, 20369, "Toad"},
  { 20370, 20728, "Troll1"},
  { 20729, 21217, "Vampire"},
  { 21218, 21433, "NPC11"},
  { 21434, 21667, "NPC12"},
  { 21668, 22017, "Wraith"},
  { 22018, 22179, "NPC13"},
  { 22180, 22433, "Zombie1"},
  { 22434, 22895, "Imp_Raider"},
  { 22896, 23251, "Dragon"},
  { 23252, 23368, "NPC14"},
  { 23369, 23476, "NPC15"},
  { 23477, 23584, "NPC16"},
  { 23585, 23701, "NPC17"},
  { 23702, 23809, "NPC18"},
  { 23810, 23917, "NPC19"},
  { 23918, 24196, "NPC20"},
  { 24197, 24325, "NPC21"},
  { 24326, 24460, "NPC22"},
  { 24461, 24577, "NPC23"},
  { 24578, 24841, "NPC24"},
  { 24842, 24964, "NPC25"},
  { 24965, 25081, "NPC26"},
  { 25082, 25759, "Guard2"},
  { 25760, 25926, "NPC27"},
  { 25927, 26043, "NPC28"},
  { 26044, 26151, "NPC29"},
  { 26152, 26259, "NPC30"},
  { 26260, 26662, "Succubus"},
  { 26663, 27007, "Skeleton_Insane1"},
  { 27008, 27352, "Skeleton_Insane2"},
  { 27353, 27634, "Skeleton_Steel_Guardian"},
  { 27635, 27916, "Skeleton_Steel_Warrior"},
  { 27917, 28218, "Skeleton_Guardian_Warrior"},
  { 28219, 28335, "NPC31"},
  { 28336, 29385, "Knight1"},
  { 29386, 29714, "Knight2"},
  { 29715, 30061, "Skeleton_Guardian_Archer"},
  { 30062, 30408, "Skeleton_Steel_Marksman"},
  { 30409, 30755, "Skeleton_Steel_Archer"},
  { 30756, 31082, "Orc_Shaman"},
  { 31083, 31325, "NPC32"},
  { 31326, 31458, "NPC33"},
  { 31459, 31692, "NPC34"},
  { 31593, 31817, "NPC35"},
  { 31818, 32094, "Skeleton_Infernal_Conjurer"},
  { 32095, 32371, "Skeleton_Steel_Conjurer"},
  { 32372, 32643, "Skeleton_Lord"},
  { 32644, 32920, "Skeleton_Steel_Lord"},
  { 32921, 33197, "Skeleton_Infernal_Lord"},
  { 33198, 33599, "Mummy2"},
  { 33600, 33861, "Zombie2"},
  { 33862, 34104, "Zombie3"},
  { 34105, 34347, "Zombie_Infernal1"},
  { 34348, 34610, "Zombie_Infernal2"},
  { 34611, 34853, "Zombie4"},
  { 34854, 35125, "Ghoul_Greater"},
  { 35126, 35402, "Ghoul_Vampiric"},
  { 35403, 35708, "Ghost"},
  { 35709, 36280, "Dwarf_Mage"},
  { 36281, 36684, "Witch"},
  { 36685, 36851, "NPC36"},
  { 36852, 36984, "NPC37"},
  { 36985, 37101, "NPC38"},
  { 37102, 37209, "NPC39"},
  { 37210, 37317, "NPC40"},
  { 37318, 37630, "Dragonrider_Green"},
  { 37631, 38001, "Deathlord1"},
  { 38002, 38372, "Deathlord2"},
  { 38373, 38872, "NPC41"},
  { 38873, 39238, "Engeneer"},
  { 39239, 39725, "Lizard_Lord"},
  { 39726, 40264, "Evil_Mage1"},
  { 40265, 40650, "Gargoyle_Crystal"},
  { 40651, 41036, "Gargoyle_Crystal_Guardian"},
  { 41037, 41422, "Gargoyle_Gold"},
  { 41423, 42194, "Gargoyle_Gold_Guardian"},
  { 42195, 42997, "Assasin4"},
  { 42998, 43452, "Dwarf_Warrior1"},
  { 43453, 43867, "Dwarf_Warrior2"},
  { 43868, 44204, "Dwarf_Girl1"},
  { 44205, 44541, "Dwarf_Girl2"},
  { 44542, 44981, "Dwarf_Captain"},
  { 44982, 45355, "Dwarf_King"},
  { 45356, 45850, "Dwarf_Lieutenant"},
  { 45851, 46127, "Boar_Mutated"},
  { 46128, 46503, "Spider_Mutated"},
  { 46504, 46861, "Toad_Mutated"},
  { 46862, 47351, "Dwarf_Warrior3"},
  { 47352, 47825, "Sand_Devil"},
  { 47826, 48245, "Sand_Horror"},
  { 48246, 48665, "Sand_Minion"},
  { 48666, 49095, "Dwarf_Warrior4"},
  { 49096, 49461, "Lich"},
  { 49462, 49767, "Spectre_Lord"},
  { 49768, 50055, "Undead_Lord"},
  { 50056, 50479, "Lizard_Lightning"},
  { 50480, 50865, "Gargoyle_Stone"},
  { 50866, 51539, "Lizard_Archmage"},
  { 51540, 52077, "Lizard_Fanatic"},
  { 52078, 52501, "Lizard_Thunder"},
  { 52502, 55901, "Wizard"},
  { 55902, 56018, "NPC42"},
  { 56019, 56135, "NPC43"},
  { 56136, 56252, "NPC44"},
  { 56253, 56369, "NPC45"},
  { 56370, 56486, "NPC46"},
  { 56487, 56594, "NPC47"},
  { 56595, 56711, "NPC48"},
  { 56712, 57298, "Orc_Leader"},
  { 57299, 57620, "Orc_Heavy"},
  { 57621, 58143, "Orc_Drummer"},
  { 58144, 58470, "Orc_Warlord"},
  { 58471, 58652, "Bee2"},
  { 58653, 58834, "Bee3"},
  { 58835, 59726, "Dog1"},
  { 59727, 60138, "Chicken1"},
  { 60139, 60550, "Chicken2"},
  { 60551, 60950, "Dog2"},
  { 60951, 61630, "Paladin1"},
  { 61631, 62139, "Paladin2"},
  { 62140, 62492, "Dragonrider_Blue"},
  { 62493, 62845, "Dragonrider_White"},
  { 62846, 63198, "Dragonrider_Red"},
  { 63199, 63350, "Dragonfly"},
  { 63351, 63467, "NPC49"},
  { 63468, 63620, "Eyeball"},
  { 63621, 63845, "NPC50"},
  { 63846, 64295, "NPC51"},
  { 64296, 64619, "NPC52"},
  { 64620, 64736, "NPC53"},
  { 64737, 64853, "NPC54"},
  { 64854, 64934, "NPC55"},
  { 64935, 65051, "NPC56"},
  { 65052, 65168, "NPC57"},
  { 65169, 66527, "Brigand1"},
  { 66528, 66644, "NPC58"},
  { 66645, 66761, "NPC59"},
  { 66762, 67283, "NPC60"},
  { 67284, 67391, "NPC61"},
  { 67392, 67400, "NPC62"},
  { 67401, 67517, "NPC63"},
  { 67518, 67634, "NPC64"},
  { 67635, 67751, "NPC65"},
  { 67752, 67868, "NPC66"},
  { 67869, 67985, "NPC67"},
  { 67986, 68246, "NPC68"},
  { 68247, 68363, "NPC69"},
  { 68364, 68489, "NPC70"},
  { 68490, 68606, "NPC71"},
  { 68607, 68723, "NPC72"},
  { 68724, 68840, "NPC73"},
  { 68841, 68957, "NPC74"},
  { 68958, 69452, "Knight3"},
  { 69453, 69569, "NPC75"},
  { 69570, 70055, "Knight4"},
  { 70056, 70172, "NPC76"},
  { 70173, 70289, "NPC77"},
  { 70290, 70406, "NPC78"},
  { 70407, 70523, "NPC79"},
  { 70524, 70640, "NPC80"},
  { 70641, 70838, "NPC81"},
  { 70839, 70955, "NPC82"},
  { 70956, 71072, "NPC83"},
  { 71073, 71270, "NPC84"},
  { 71271, 71387, "NPC85"},
  { 71388, 71837, "Brigand2"},
  { 71838, 71954, "NPC87"},
  { 71955, 72071, "NPC88"},
  { 72072, 72197, "NPC89"},
  { 72198, 72315, "NPC90"},
  { 72315, 72647, "Brigand3"},
  { 72648, 73034, "Troll2"},
  { 73035, 73232, "NPC91"},
  { 73233, 73430, "NPC92"},
  { 73431, 73628, "NPC93"},
  { 73629, 74078, "Brigand4"},
  { 74079, 74276, "NPC94"},
  { 74277, 74384, "NPC95"},
  { 74385, 74915, "NPC96"},
  { 74916, 75230, "NPC97"},
  { 75231, 75602, "Archer1"},
  { 75603, 75874, "Executioner"},
  { 75875, 76071, "NPC98"},
  { 76072, 76188, "NPC99"},
  { 76189, 76305, "NPC100"},
  { 76306, 76422, "NPC101"},
  { 76423, 76549, "NPC102"},
  { 76550, 76676, "NPC103"},
  { 76677, 76793, "NPC104"},
  { 76794, 76910, "NPC105"},
  { 76911, 77135, "NPC106"},
  { 77136, 77475, "Thug_Sword2"},
  { 77476, 77729, "Wasp2"},
  { 77730, 78036, "Beetle"},
  { 78037, 78313, "Ghoul_Lord"},
  { 78314, 78852, "Evil_Mage2"},

  {0,  0, 0}};



// sub_5FB5E0 is LZO decoder
// sub_55BF60 is RLE decoder

//FIXME: boar has some holes inside him
//       7296 (ghoul) is damaged
//       10871 (bee) is damaged
//       21218 is damaged
//       21676, 21686 other wraith sprites are damaged
//       25936 is damaged
//       34854 is damaged
//       37318 (dragonrider green) is damaged
//       Ghost should be transparent
//       Bee should have transparent wings


//convert to HSV to get rid of black borders
//convert yoba.jpg -alpha set -channel RGBA -fuzz 5% -fill none -floodfill +0+0 black yoba_apha.png
static void divinityDecompile(char *Output, char *Input) {
  char Tmp[1024], Path[1024], ArDir[1024], ArName[256], ArExt[32], Name[256];
  int I, J, K, C, N, U, Off, Len, ULen, NFiles, L, X, Y, Wide, High, A, R, G, B;
  u1 *M, *Q, *O, *Z;
  header *H;
  pic *P;
  entry *E;

  pathParts(ArDir, ArName, ArExt, Input);

  K = atoi(ArExt);
  if (K==11) {
    printf("  cant unpack this paticular archive\n");
    abort();
  }

  sprintf(Tmp, "%s/CPackedi.%dc", ArDir, K);
  E = (entry*)readFile(0, fileSize(Tmp), Tmp);
  NFiles = fileSize(Tmp)/sizeof(entry);


  sprintf(Tmp, "%s/CPackedb.%dc", ArDir, K);
  L = fileSize(Tmp);

  times(I, NFiles) {
    //if (I < 3357 || I > 3389) goto skip;
    //if (I < 58471 || I > 58652) goto skip;
    //if (I != 58523) goto skip;

    if (K == 1) {
      for(J = 0; Names[J].Name; J++) {
        if (I < Names[J].A || I > Names[J].B) continue;
        //unless(!strcmp(Names[J].Name, "Dragonrider_Green")) goto skip;
        sprintf(Name, "%s/%06d", Names[J].Name, I);
        goto found_name;
        //goto skip;
      }
    }

    sprintf(Name, "unsorted/%06d", I);

found_name:

    Off = E->Off;
    Len = ((I == NFiles-1) ? L : (E+1)->Off)-E->Off;
    printf("  %05d:%08X:%05X:%03dx%03d:%d:%03d,%03d->%03d,%03d:%d,%d: %X %X %X %X %X\n"
          ,I,Off,Len,E->W,E->H,E->RLE,E->SX,E->SY,E->EX,E->EY,E->CX,E->CY
          ,E->C,E->D,E->E,E->F,E->G);

    M = readFile(Off, Len, Tmp);
    ULen = *(u4*)M;
    Q = ns(u1,ULen);
    lzo1x_decompress(M+4,Len-4,Q,&ULen,0);
    free(M);
    M=Q;
    Len = ULen;

    //sprintf(Path, "%s/%02d/%05d", Output, K, I);
    //writeFile(0, Len, Path, M);

    if (E->RLE) {
      H = (header*)M;

      //printf("    %05d:%06X:%04X:%03dx%03d\n",I,H->Size,H->Data,H->W,H->H);
      unless (H->Size) {
        printf("    cant handle zero-sized sprites.\n");
        abort();
      }
      Wide=H->W;
      High=H->H;

      P = picNew(Wide, High, 32);
      //picClear(P,R8G8B8(0xFF,0xFF,0xFF));

      O = M+sizeof(header);
      Z = M+H->Data;
      times (Y, H->H) {
        N = ru2(O); // number of opaque chunks for this scanline
        if (!N) { // empty line
          Off = ru4(O);
          U = ru2(O);
          //printf("  %03d:%03X:%04X:%03X\n", Y, N, Off, U);
          continue;
        }

        Off = ru4(O); // offset to the start of scanline pixels
        Q = Z+Off;
        times (J, N) {
          X = ru2(O);
          C = ru2(O);
          //printf("    %02d:%02d\n", X, C);
          times (U, C) {
            fromR5G6B5(R, G, B, ru2(Z));
            picPut32(P,X++,Y,R8G8B8A8(R,G,B,0xFF));
          }
        }
        U = ru2(O); //copy of N, if drawing backward
        //printf("  %03d:%03X:%04X:%03X\n", Y, N, Off, U);
      }
    } else {
      P = picNew(E->W, E->H, 24);
      Q = M;
      times (Y,E->H) {
        times (X,E->W) {
          fromR5G6B5(R, G, B, ru2(Q));
          picPut24(P,X,Y,R8G8B8(R,G,B));
        }
      }
    }

    if (K==0) sprintf(Path, "%s/objects/%s.png", Output, Name);
    else if (K==1) sprintf(Path, "%s/units/%s.png", Output, Name);
    else if (K==2) sprintf(Path, "%s/floor/%s.png", Output, Name);
    else if (K==3) sprintf(Path, "%s/gui1/%s.png", Output, Name);
    else if (K==4) sprintf(Path, "%s/missiles/%s.png", Output, Name);
    else if (K==5) sprintf(Path, "%s/item_icons/%s.png", Output, Name);
    else if (K==6) sprintf(Path, "%s/gui_editor/%s.png", Output, Name);
    else if (K==7) sprintf(Path, "%s/magic/%s.png", Output, Name);
    else if (K==8) sprintf(Path, "%s/gui2/%s.png", Output, Name);
    else if (K==9) sprintf(Path, "%s/gui3/%s.png", Output, Name);
    else if (K==10) sprintf(Path, "%s/units_icons/%s.png", Output, Name);
    else if (K==12) sprintf(Path, "%s/gui_startup/%s.png", Output, Name);
    else sprintf(Path, "%s/%02d/%s.png", Output, K, Name);
    pngSave(Path, P);
    picDel(P);

    free(M);

skip:
    E++;
  }
}

int divinityInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "divinity";
  F->Description = "Divine Divinity sprite archive (use on CPackedi.*c files)";
  F->Decompile = divinityDecompile;
  return 1;
}

