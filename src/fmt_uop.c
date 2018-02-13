//Credits go to Malganis (author of Mythic Package Editor and filelists)
//              Turley (some animation names were taken from his UoFiddler)
//              SNV (I did all UOMA related inquiry)
//              Author of FrameView, for being a greedy faggot

#include "zlib/zlib.h"
#include "common.h"

typedef struct {
  u1 Id[4]; // MYP\0
  u4 Version; // 4=KR, 5=Stygian
  u4 U2; // #43 #EC #23 #FD
  u8 First; // start of the first data group (can be anywhere inside archive)
  u4 GroupSize; // #64 for Verion=4, or #3E8 for Verion=5
  u4 NBlocks; // total blocks in file
} __attribute__ ((packed)) header;

typedef struct {
  u4 NFiles;   // up to GroupSize
  u8 Next;     // offset of next group or 0 if end
  //entry Files[NFiles];
} __attribute__ ((packed)) grp;

typedef struct {
  u8 Off;        // to the start of data block
  u4 HdrLen;     // size of data block header
  u4 Len;        // compressed length
  u4 ULen;       // uncompressed length
  u1 Hash[8];    // jenkins hash
  u4 CRC;
  u2 Enc;        // encoding: 0=plain, 1=gzip
} __attribute__ ((packed)) entry;

typedef struct dic dic;
struct dic {
  u8 Hash;
  char *Name;
  dic *A, *B;
};

static dic *Dic;


static void addRec(dic **R, dic *N) {
  if (N->Hash < (*R)->Hash) {
    if ((*R)->A) addRec(&(*R)->A, N);
    else (*R)->A = N;
  } else {
    if ((*R)->B) addRec(&(*R)->B, N);
    else (*R)->B = N;
  }
}

static void printTree(dic *N, int I) {
  unless (N) return;
  printTree(N->A, I+1);
  printf("%03d:%016llX\n", I, N->Hash);
  printTree(N->B, I+1);
}

static void addFilename(u8 Hash, char *Name) {
  dic *Node = n(dic);
  Node->Name = strdup(Name);
  Node->Hash = Hash;
  if (Dic) addRec(&Dic, Node);
  else Dic = Node;
}

static char *searchRec(dic *N, u8 Hash) {
  if (!N) return 0;
  if (N->Hash == Hash) return N->Name;
  if (Hash < N->Hash) return searchRec(N->A, Hash);
  return searchRec(N->B, Hash);
}

static char *searchName(u8 Hash) {
  return searchRec(Dic, Hash);
}


static u8 hash(char *s) {
  u4 L = strlen(s);
  u4 eax, ecx, edx, ebx, esi, edi;

  eax = ecx = edx = ebx = esi = edi = 0;
  ebx = edi = esi = L + 0xDEADBEEF;

  int i = 0;

  for ( i = 0; i + 12 < L; i += 12 ) {
    edi = (u4) ( ( s[ i + 7 ] << 24 ) | ( s[ i + 6 ] << 16 ) | ( s[ i + 5 ] << 8 ) | s[ i + 4 ] ) + edi;
    esi = (u4) ( ( s[ i + 11 ] << 24 ) | ( s[ i + 10 ] << 16 ) | ( s[ i + 9 ] << 8 ) | s[ i + 8 ] ) + esi;
    edx = (u4) ( ( s[ i + 3 ] << 24 ) | ( s[ i + 2 ] << 16 ) | ( s[ i + 1 ] << 8 ) | s[ i ] ) - esi;

    edx = ( edx + ebx ) ^ ( esi >> 28 ) ^ ( esi << 4 );
    esi += edi;
    edi = ( edi - edx ) ^ ( edx >> 26 ) ^ ( edx << 6 );
    edx += esi;
    esi = ( esi - edi ) ^ ( edi >> 24 ) ^ ( edi << 8 );
    edi += edx;
    ebx = ( edx - esi ) ^ ( esi >> 16 ) ^ ( esi << 16 );
    esi += edi;
    edi = ( edi - ebx ) ^ ( ebx >> 13 ) ^ ( ebx << 19 );
    ebx += esi;
    esi = ( esi - edi ) ^ ( edi >> 28 ) ^ ( edi << 4 );
    edi += ebx;
  }

  if ( L - i > 0 ) {
    switch ( L - i ) {
    case 12: esi += (u4) s[ i + 11 ] << 24;
    case 11: esi += (u4) s[ i + 10 ] << 16;
    case 10: esi += (u4) s[ i + 9 ] << 8;
    case  9: esi += (u4) s[ i + 8 ];
    case  8: edi += (u4) s[ i + 7 ] << 24;
    case  7: edi += (u4) s[ i + 6 ] << 16;
    case  6: edi += (u4) s[ i + 5 ] << 8;
    case  5: edi += (u4) s[ i + 4 ];
    case  4: ebx += (u4) s[ i + 3 ] << 24;
    case  3: ebx += (u4) s[ i + 2 ] << 16;
    case  2: ebx += (u4) s[ i + 1 ] << 8;
    case  1: ebx += (u4) s[ i ];
    break;
    }

    esi = ( esi ^ edi ) - ( ( edi >> 18 ) ^ ( edi << 14 ) );
    ecx = ( esi ^ ebx ) - ( ( esi >> 21 ) ^ ( esi << 11 ) );
    edi = ( edi ^ ecx ) - ( ( ecx >> 7 ) ^ ( ecx << 25 ) );
    esi = ( esi ^ edi ) - ( ( edi >> 16 ) ^ ( edi << 16 ) );
    edx = ( esi ^ ecx ) - ( ( esi >> 28 ) ^ ( esi << 4 ) );
    edi = ( edi ^ edx ) - ( ( edx >> 18 ) ^ ( edx << 14 ) );
    eax = ( esi ^ edi ) - ( ( edi >> 8 ) ^ ( edi << 24 ) );

    return ( (u8) edi << 32 ) | eax;
  }

  return ( (u8) esi << 32 ) | eax;
}


static void loadTxt() {
  char ListPath[1024], Name[512];
  int L;
  char *M, *E, *Q;
  sprintf(ListPath, "%s/names/uo.txt", ProgDir);
  unless (fileExist(ListPath)) {
    printf("  Cant open %s\n", ListPath);
    abort();
  }
  L = fileSize(ListPath);
  M = readFile(0, L, ListPath);
  E = M+L;
  while(M < E) {
    Q = strchr(M, '\n');
    strncpy(Name, M, Q-M);
    Name[Q-M] = 0;
    //printf("%016llX=%s\n", hash(Name), Name);
    addFilename(hash(Name), Name);
    M = Q+1;
  }

  //3dbf716d9482919e:build/animationdefinition/00000963.bin
  //char *P = searchName(hash("build/animationdefinition/00000963.bin"));
  //if (P) printf("%s\n", P);
  //saveText(Dic, 0);
  //abort();
}

static void dicToTxt(char *Out, char *In) {
  char Name[512];
  int L, C;
  u1 *M, *E;
  char *Q, *Z;
  L = fileSize(In);
  M = readFile(0, L, In);
  E = M+L;
  Z = Q = ns(char,L);

  M+=5;
  while(M < E) {
    u8 Hash = *(u8*)M; M+=9;
    if (M[-1] == 1) {
      C = *M++;
      memcpy(Name, M, C);
      Name[C] = 0;
      if (Hash == hash(Name)) {
        strncpy(Q, M, C);
        Q[C] = '\n';
        Q += C+1;
      } else {
        //printf("%016llX=%016llX:%s\n", Hash, hash(Name), Name);
      }
      M += C;
    }
  }
  writeFile(0, Q-Z, Out, Z);
}



typedef struct {
  u1 Id[4]; // AMOU (UO Mob Anim)
  u4 Version;
  u4 Size; // size of this whole file, excluding header
  u4 UnitID;
  s2 SX;
  s2 SY;
  s2 EX;
  s2 EY;
  u4 NColors; // some images have just 1 color
  u4 PaletteOff; // for KR sprites
  u4 NFrames;
  u4 Frames;
} __attribute__ ((packed)) amou;

typedef struct {
  u2 AnimID;
  u2 Num; // frame number
  s2 SX;
  s2 SY;
  s2 EX;
  s2 EY;
  u4 Off; //relative to this entry
} __attribute__ ((packed)) amouFrame;

typedef struct { // classic client sprites use it's own palette
  u2 Pal[256]; // wrong, cause NColors=1 can kill us
  s2 X;
  s2 Y;
  s2 W;
  s2 H;
} __attribute__ ((packed)) frameHeader;

//stygian and gray dragons have clipped wings
static char **Names;
static void initNames() {
  if (Names) return;
  Names = ns(char*, 1<<16);
  Names[  0]="Mobs/Imp1";
  Names[  1]="Mobs/Ogre";
  Names[  2]="Mobs/Ettin1";
  Names[  3]="Mobs/Undead_Zombie";
  Names[  4]="Mobs/Gargoyle";
  Names[  5]="Mobs/Bird_Eagle";
  Names[  6]="Mobs/Bird_Generic";
  Names[  7]="Mobs/Orc_Captain";
  Names[  8]="Mobs/Flail_Corpser";
  Names[  9]="Mobs/Daemon_Red1";
  Names[ 10]="Mobs/Daemon_Red2";
  Names[ 11]="Mobs/Spider_Dread";
  Names[ 12]="Mobs/Dragon_Retro_Grey_Drake";
  Names[ 13]="Mobs/Elemental_Air";
  Names[ 14]="Mobs/Elemental_Earth";
  Names[ 15]="Mobs/Elemental_Fire";
  Names[ 16]="Mobs/Elemental_Water";
  Names[ 17]="Mobs/Orc";
  Names[ 18]="Mobs/Ettin2";
  Names[ 19]="Mobs/Spider_Dread";
  Names[ 20]="Mobs/Spider_Frost";
  Names[ 21]="Mobs/Snake_Giant";
  Names[ 22]="Mobs/Gazer";
  Names[ 23]="Mobs/Canine_Wolf_Dire";
  Names[ 24]="Mobs/Lich";
  Names[ 25]="Mobs/Canine_Wolf_Grey1";
  Names[ 26]="Mobs/Spirit_Ghost";
  Names[ 27]="Mobs/Canine_Wolf_Grey2";
  Names[ 28]="Mobs/Spider_BlackWidow";
  Names[ 29]="Mobs/Gorilla_Silverback";
  Names[ 30]="Mobs/Harpy";
  Names[ 31]="Mobs/Headless_One";
  Names[ 33]="Mobs/Lizardmen1";
  Names[ 34]="Mobs/Canine_Wolf_White1";
  Names[ 35]="Mobs/Lizardmen2";
  Names[ 36]="Mobs/Lizardmen3";
  Names[ 37]="Mobs/Canine_Wolf_White2";
  Names[ 38]="Mobs/Daemon_WorldDevourer";
  Names[ 39]="Mobs/Mongbat";
  Names[ 40]="Mobs/Daemon_Elder";
  Names[ 41]="Mobs/Orc_Warrior";
  Names[ 42]="Mobs/Ratman1";
  Names[ 43]="Mobs/Daemon_Ice_Fiend";
  Names[ 44]="Mobs/Ratman_Warrior1";
  Names[ 45]="Mobs/Ratman_Archer1";
  Names[ 46]="Mobs/Dragon_Rust";
  Names[ 47]="Mobs/Flail_Reaper";
  Names[ 48]="Mobs/Scorpion_Giant";
  Names[ 49]="Mobs/Dragon_White_Wyrm";
  Names[ 50]="Mobs/Undead_Skeleton1";
  Names[ 51]="Mobs/Slime";
  Names[ 52]="Mobs/Snake_Giant";
  Names[ 53]="Mobs/Troll1";
  Names[ 54]="Mobs/Troll2";
  Names[ 55]="Mobs/Troll_Frost";
  Names[ 56]="Mobs/Undead_Skeleton2";
  Names[ 57]="Mobs/Undead_Skeleton3";
  Names[ 58]="Mobs/Wisp";
  Names[ 59]="Mobs/Dragon_Retro_Red";
  Names[ 60]="Mobs/Dragon_Retro_Red_Drake";
  Names[ 61]="Mobs/Dragon_Retro_Grey_Drake";
  Names[ 62]="Mobs/Dragon_Wyvern";
  Names[ 63]="Mobs/Cat_Cougar";
  Names[ 64]="Mobs/Cat_Snow_Leopard1";
  Names[ 65]="Mobs/Cat_Snow_Leopard2";
  Names[ 66]="Mobs/Flails_Swamp_Tentacles";
  Names[ 67]="Mobs/Gargoyle_Stone";
  Names[ 68]="Mobs/Gazer_Elder1";
  Names[ 69]="Mobs/Gazer_Elder2";
  Names[ 70]="Mobs/Terathan_Warrior";
  Names[ 71]="Mobs/Terathan_Worker";
  Names[ 72]="Mobs/Terrathan_Queen";
  Names[ 73]="Mobs/Harpy_Stone";
  Names[ 74]="Mobs/Imp2";
  Names[ 75]="Mobs/Cyclopsian_Warrior";
  Names[ 76]="Mobs/Titan";
  Names[ 77]="Mobs/Kraken";
  Names[ 78]="Mobs/Undead_Ancient_Lich";
  Names[ 79]="Mobs/Undead_Lich_Lord1";
  Names[ 80]="Mobs/Frog_Giant_Toad";
  Names[ 81]="Mobs/Frog_Bullfrog";
  Names[ 82]="Mobs/Unead_Lich_Lord2";
  Names[ 83]="Mobs/Ogre_Lord1";
  Names[ 84]="Mobs/Ogre_Lord2";
  Names[ 85]="Mobs/Ophidian_Mage";
  Names[ 86]="Mobs/Ophidian_Warrior";
  Names[ 87]="Mobs/Ophidian_Queen";
  Names[ 88]="Mobs/Goat_Mountain";
  Names[ 89]="Mobs/Snake_Ice";
  Names[ 90]="Mobs/Snake_Lava";
  Names[ 91]="Mobs/Snake_Chrome1";
  Names[ 92]="Mobs/Snake_Chrome2";
  Names[ 93]="Mobs/Snake_Chrome3";
  Names[ 94]="Mobs/Slime_Frost1";
  Names[ 95]="Mobs/Turkey";
  Names[ 96]="Mobs/Slime_Frost2";
  Names[ 97]="Mobs/Canine_Hell_Hound1";
  Names[ 98]="Mobs/Canine_Hell_Hound2";
  Names[ 99]="Mobs/Canine_Wolf_Dark";
  Names[100]="Mobs/Canine_Wolf_Silver";
  Names[101]="Mobs/Centaur";
  Names[102]="Mobs/Daemon_Exodus";
  Names[103]="Mobs/Dragon_Asian";
  Names[104]="Mobs/Dragon_Skeletal";
  Names[105]="Mobs/Dragon_Ancient_Wyrm";
  Names[106]="Mobs/Dragon_Shadow_Wyrm";
  Names[107]="Mobs/Elemental_Ore_Agapite";
  Names[108]="Mobs/Elemental_Ore_Bronze";
  Names[109]="Mobs/Elemental_Ore_Copper";
  Names[110]="Mobs/Elemental_Ore_Dull_Copper";
  Names[111]="Mobs/Elemental_Ore_Shadow_Iron";
  Names[112]="Mobs/Elemental_Ore_Valorite";
  Names[113]="Mobs/Elemental_Ore_Verite";
  Names[114]="Mobs/Horse_Dark_Steed";
  Names[115]="Mobs/Horse_Ethereal";
  Names[116]="Mobs/Horse_Nightmare";
  Names[117]="Mobs/Horse_Silver_Steed";
  Names[118]="Mobs/Horse_War_Brittanian";
  Names[119]="Mobs/Horse_War_Mage_Council";
  Names[120]="Mobs/Horse_War_Minax";
  Names[121]="Mobs/Horse_War_Shadowlord";
  Names[122]="Mobs/Horse_Unicorn";
  Names[123]="Mobs/Ethereal_Warrior1";
  Names[124]="Mobs/Evil_Mage";
  Names[125]="Mobs/Evil_Mage_Lord1";
  Names[126]="Mobs/Evil_Mage_Lord2";
  Names[127]="Mobs/Cat_HellCat";
  Names[128]="Mobs/Pixie";
  Names[129]="Mobs/Flails_Tentacles_of_the_Harrower";
  Names[130]="Mobs/Gargoyle_Blistering";
  Names[131]="Mobs/Efreet";
  Names[132]="Mobs/Horse_Kirin";
  Names[133]="Mobs/Alligator_Small";
  Names[134]="Mobs/Komodo_Dragon";
  Names[135]="Mobs/Ogre_Lord_Arctic";
  Names[136]="Mobs/Ophidian_Archmage";
  Names[137]="Mobs/Ophidian_Knight";
  Names[138]="Mobs/Orc_Lord1";
  Names[139]="Mobs/Orc_Lord2";
  Names[140]="Mobs/Orc_Shaman";
  Names[141]="Mobs/Paladin";
  Names[142]="Mobs/Ratman_Archer2";
  Names[143]="Mobs/Ratman_Shaman";
  Names[144]="Mobs/Sea_Horse";
  Names[145]="Mobs/Sea_Serpent";
  Names[146]="Mobs/Shadowlord";
  Names[147]="Mobs/Skeleton_Knight";
  Names[148]="Mobs/Skeleton_Mage";
  Names[149]="Mobs/Succubus1";
  Names[150]="Mobs/Sea_Serpent2";
  Names[151]="Mobs/Dolphin";
  Names[152]="Mobs/Terathan_Warrior";
  Names[153]="Mobs/Undead_Ghoul";
  Names[154]="Mobs/Undead_Mummy";
  Names[155]="Mobs/Undead_RottingCorpse";
  Names[157]="Mobs/Spider_BlackWidow";
  Names[158]="Mobs/Elemental_Acid";
  Names[159]="Mobs/Elemental_Blood1";
  Names[160]="Mobs/Elemental_Blood2";
  Names[161]="Mobs/Elemental_Ice";
  Names[162]="Mobs/Elemental_Poison";
  Names[163]="Mobs/Elemental_Snow";
  Names[164]="Mobs/Energy_Wortex";
  Names[165]="Mobs/Wisp_Evil";
  Names[166]="Mobs/Elemental_Ore_Gold";
  Names[167]="Mobs/Bear_Brown";
  Names[168]="Mobs/Imp_Shadow_Fiend";
  Names[169]="Mobs/Beetle_Fire";
  Names[170]="Mobs/Llama_Ethereal";
  Names[171]="Mobs/Ostard_Ethereal";
  Names[172]="Mobs/Dragon_Retro_Red2";
  Names[173]="Mobs/Spider_Blackwidow2";
  Names[174]="Mobs/Succubus2";
  Names[175]="Mobs/Ethereal_Warrior2";
  Names[176]="Mobs/Pixie2";
  Names[177]="Mobs/Horse_Nightmare1";
  Names[178]="Mobs/Horse_Nightmare1";
  Names[179]="Mobs/Horse_Nightmare1";
  Names[180]="Mobs/Dragon_White_Wyrm2";
  Names[181]="Mobs/Orc_Scout";
  Names[182]="Mobs/Orc_Bomber";
  Names[183]="Mobs/Savage_Male_Warrior";
  Names[184]="Mobs/Savage_Female_Warrior";
  Names[185]="Mobs/Savage_Rider";
  Names[186]="Mobs/Savage_Shaman";
  Names[187]="Mobs/Ridgeback";
  Names[188]="Mobs/Ridgeback_Savage";
  Names[189]="Mobs/Orc_Brute";
  Names[190]="Mobs/Horse_Fire_Steed";
  Names[191]="Mobs/Horse_Kirin_Ethereal";
  Names[192]="Mobs/Horse_Unicorn_Ethereal";
  Names[193]="Mobs/Ridgeback_Ethereal";
  Names[194]="Mobs/Dragon_Swamp_Ethereal";
  Names[195]="Mobs/Beetle_Ethereal";
  Names[196]="Mobs/Gouzen_Ha";
  Names[197]="Mobs/Dragon_Red_New";
  Names[198]="Mobs/Dragon_Gray_New";
  Names[199]="Mobs/Denkou_Yajuu";
  Names[200]="Mobs/Horse_Dappled_Brown";
  Names[201]="Mobs/Cat";
  Names[202]="Mobs/Alligator";
  Names[203]="Mobs/Pig";
  Names[204]="Mobs/Horse_Dark_Brown";
  Names[205]="Mobs/Rabbit";
  Names[206]="Mobs/Lava_Lizard";
  Names[207]="Mobs/Sheep";
  Names[208]="Mobs/Bird_Hen";
  Names[209]="Mobs/Goat_Billy";
  Names[210]="Mobs/Ostard_Desert";
  Names[211]="Mobs/Bear_Black";
  Names[212]="Mobs/Bear_Grizzly";
  Names[213]="Mobs/Bear_Polar";
  Names[214]="Mobs/Cat_Panther";
  Names[215]="Mobs/Rat_Giant";
  Names[216]="Mobs/Cow_Brown";
  Names[217]="Mobs/Canine_Dog_Hound";
  Names[218]="Mobs/Ostard_Frenzied";
  Names[219]="Mobs/Ostard_Forest";
  Names[220]="Mobs/Llama";
  Names[221]="Mobs/Walrus";
  Names[223]="Mobs/Sheep_Shorn";
  Names[225]="Mobs/Canine_Wolf_Timber";
  Names[226]="Mobs/Horse_Dappled_Grey";
  Names[228]="Mobs/Horse_Tan";
  Names[230]="Mobs/Sheep_Shorn2";
  Names[231]="Mobs/Cow_Spotted";
  Names[232]="Mobs/Bull_Brown";
  Names[233]="Mobs/Bull_Spotted";
  Names[234]="Mobs/Deer_Stag";
  Names[237]="Mobs/Deer_Doe";
  Names[238]="Mobs/Rat_Giant";
  Names[240]="Mobs/Kappa";
  Names[241]="Mobs/Oni";
  Names[242]="Mobs/Beetle_Deathwatch";
  Names[243]="Mobs/Hiryu";
  Names[244]="Mobs/Beetle_Rune";
  Names[245]="Mobs/Yomotsu_Warrior";
  Names[246]="Mobs/Bake_Kitsune";
  Names[247]="Mobs/Fan_Dancer";
  Names[248]="Mobs/Gaman";
  Names[249]="Mobs/Yamandon";
  Names[250]="Mobs/Tsuki_Wolf";
  Names[251]="Mobs/Revenant_Lion";
  Names[252]="Mobs/Lady_of_the_Snow";
  Names[253]="Mobs/Yomotsu_Priest";
  Names[254]="Mobs/Bird_Crane";
  Names[255]="Mobs/Yomotsu_Elder";
  Names[256]="Mobs/Chief_Paroxysmus";
  Names[257]="Mobs/Dreadhorn";
  Names[258]="Mobs/Lady_Melisande";
  Names[259]="Mobs/Interred_Grizzle_Monstrous";
  Names[261]="Mobs/Shimmering_Effusion";
  Names[262]="Mobs/Minotaur_Tormented";
  Names[263]="Mobs/Minotaur";
  Names[264]="Mobs/Changeling";
  Names[265]="Mobs/Hydra";
  Names[266]="Mobs/Dryad";
  Names[267]="Mobs/Troglodyte";
  Names[269]="Mobs/VampireBat";
  Names[270]="Mobs/Bulbous_Putrification";
  Names[271]="Mobs/Satyr";
  Names[272]="Mobs/Interred_Grizzle";
  Names[273]="Mobs/Fetid_Essence";
  Names[274]="Mobs/Mantra_Effervescence";
  Names[275]="Mobs/Corporeal_Brume";
  Names[276]="Mobs/Reptalon";
  Names[277]="Mobs/Cu_Sidhe";
  Names[278]="Mobs/Squirrel";
  Names[279]="Mobs/Ferret";
  Names[280]="Mobs/Minotaur_Armored";
  Names[281]="Mobs/Minotaur_Leather";
  Names[282]="Mobs/Bird_Parrot";
  Names[283]="Mobs/Bird_Crow";
  Names[284]="Mobs/Horse_Mondain_Steed";
  Names[285]="Mobs/Reaper_Form";
  Names[286]="Mobs/Worm";
  Names[287]="Mobs/Worm_Bloodworm";
  Names[290]="Mobs/Pig_Boar";
  Names[291]="Mobs/Horse_Pack";
  Names[292]="Mobs/Llama_Pack";
  Names[293]="Mobs/Vollem";
  Names[300]="Mobs/Elemental_Crystal";
  Names[301]="Mobs/Treefellow";
  Names[302]="Mobs/Skittering_Hopper";
  Names[303]="Mobs/Devourer_of_Souls";
  Names[304]="Mobs/Golem_Flesh";
  Names[305]="Mobs/Gore_Fiend";
  Names[306]="Mobs/Impaler";
  Names[307]="Mobs/Gibberling";
  Names[308]="Mobs/Daemon_Bone";
  Names[309]="Mobs/Undead_Patch_Skeleton";
  Names[310]="Mobs/Banshee1";
  Names[311]="Mobs/Shadow_Knight";
  Names[312]="Mobs/Abysmal_Horror";
  Names[313]="Mobs/Darknight_Creeper";
  Names[314]="Mobs/Ravager";
  Names[315]="Mobs/Flesh_Renderer";
  Names[316]="Mobs/Wanderer_of_The_Void";
  Names[317]="Mobs/Vampire_Bat";
  Names[318]="Mobs/Daemon_Knight";
  Names[319]="Mobs/Mound_of_Maggots";
  Names[334]="Mobs/Goblin_Gray";
  Names[432]="Mobs/Boura2";
  Names[574]="Mobs/Blade_Spirit";
  Names[689]="Mobs/Floater_White";
  Names[704]="Mobs/Floater_Black";
  Names[705]="Mobs/Golem_Stone";
  Names[713]="Mobs/713";
  Names[714]="Mobs/Beetle_Iron";
  Names[715]="Mobs/Boura";
  Names[716]="Mobs/Chicken_Lizard";
  Names[717]="Mobs/Scorpion_Clockwork";
  Names[718]="Mobs/Dragon_Fairy";
  Names[719]="Mobs/719";
  Names[720]="Mobs/Elemental_Lava";
  Names[721]="Mobs/Maddening_Horror";
  Names[722]="Mobs/Undead_Gargoyle";
  Names[723]="Mobs/Goblin_Green";
  Names[724]="Mobs/Gremlin";
  Names[725]="Mobs/Homunculus";
  Names[726]="Mobs/Kepetch_Ambusher";
  Names[727]="Mobs/Kepetch";
  Names[728]="Mobs/Medusa";
  Names[729]="Mobs/729";
  Names[730]="Mobs/Raptor";
  Names[731]="Mobs/731";
  Names[732]="Mobs/Worm_Rotworm";
  Names[733]="Mobs/Dragon_Skree";
  Names[734]="Mobs/Slith";
  Names[735]="Mobs/735";
  Names[736]="Mobs/Spider_Wolf";
  Names[737]="Mobs/Spider_Trapdor";
  Names[738]="Mobs/Fire_Ant";
  Names[739]="Mobs/Wolf_Leather";
  Names[740]="Mobs/Undead_Dream_Wraith";
  Names[741]="Mobs/Slasher_of_Veils";
  Names[742]="Mobs/742";
  Names[743]="Mobs/Tentacle";
  Names[744]="Mobs/Vampire_Male";
  Names[745]="Mobs/Vampire_Female";
  Names[746]="Mobs/Daemon_Four_Armed";
  Names[747]="Mobs/Banshee2";
  Names[748]="Mobs/Undead_Wraith";
  Names[749]="Mobs/Undead_Lich_Transformed";
  Names[750]="Mobs/Savage_Male";
  Names[751]="Mobs/Savage_Female";
  Names[752]="Mobs/Golem_Iron";
  Names[753]="Mobs/Gargoyle_Enslaved";
  Names[754]="Mobs/Gargoyle_Enforcer";
  Names[755]="Mobs/Gargoyle_Destroyer";
  Names[756]="Mobs/Clockwork_Overseer";
  Names[757]="Mobs/Daemon_Exodus_Minion";
  Names[758]="Mobs/Gargoyle_Shopkeeper";
  Names[763]="Mobs/Daemon_Exodus_Minion_Lord";
  Names[764]="Mobs/Juka_Warrior";
  Names[765]="Mobs/Juka_Mage";
  Names[766]="Mobs/Warlord_Kabur";
  Names[767]="Mobs/Betrayer";
  Names[768]="Mobs/Juggernaut";
  Names[770]="Mobs/Meer_Mage";
  Names[771]="Mobs/Meer_Warrior";
  Names[772]="Mobs/Adranath";
  Names[773]="Mobs/Captain_Dasha";
  Names[774]="Mobs/Pixie_Dawn";
  Names[775]="Mobs/Plague_Beast";
  Names[776]="Mobs/Horde_Demon";
  Names[777]="Mobs/Doppleganger";
  Names[778]="Mobs/Gazer_Larva";
  Names[779]="Mobs/Bogling";
  Names[780]="Mobs/Bog_Thing";
  Names[781]="Mobs/Solen_Drone";
  Names[782]="Mobs/Solen_Warrior";
  Names[783]="Mobs/Solen_Queen";
  Names[784]="Mobs/Daemon_Arcane";
  Names[785]="Mobs/Daemon_Moloch";
  Names[786]="Mobs/Spider_Dread";
  Names[787]="Mobs/Ant_Lion";
  Names[788]="Mobs/Sphinx";
  Names[789]="Mobs/Flails_Quagmire";
  Names[790]="Mobs/Sand_Vortex";
  Names[791]="Mobs/Beetle_Giant";
  Names[792]="Mobs/Daemon_Chaos";
  Names[793]="Mobs/Horse_Skeletal_Mount";
  Names[794]="Mobs/Dragon_Swamp";
  Names[795]="Mobs/Horde_Demon_Medium";
  Names[796]="Mobs/Horde_Demon_Large";
  Names[797]="Mobs/Dragon_MF_Fire";
  Names[798]="Mobs/Dragon_MF_Rust";
  Names[799]="Mobs/Dragon_Swamp_Armored";
  Names[804]="Mobs/Solen_Avenger";
  Names[805]="Mobs/Solen_Drone";
  Names[806]="Mobs/Solen_Warrior";
  Names[807]="Mobs/Solen_Queen";
  Names[808]="Mobs/Solen_Avenger";
  Names[826]="Mobs/Dragon_Stygian";
  Names[829]="Mobs/Golem_Stygian";
  Names[830]="Mobs/Undead_Skeletal_lich";
  Names[831]="Mobs/Bird_Parrot2";
  Names[832]="Mobs/Bird_Phoenix";
  Names[949]="Mobs/Empty";
  Names[970]="Mobs/Player_Ghost";
  Names[990]="Mobs/Lord_British";
  Names[991]="Mobs/Blackthorn2";
  Names[994]="Mobs/Dupre";
  Names[999]="Mobs/Horde_Demon_Colored";
  Names[1026]="Mobs/Turkey_Giant";

  Names[400]="Player/Human_Male";
  Names[401]="Player/Human_Female";
  Names[605]="Player/Elf_Male";
  Names[606]="Player/Elf_Female";
  Names[666]="Player/Gargoyle_Male";
  Names[667]="Player/Gargoyle_Female";

  Names[260]="Equip/armor1_torso";
  Names[268]="Equip/armor1_head";
  Names[336]="Equip/Shield";
  Names[522]="Equip/pants";
  Names[404]="Equip/floppy_hat";
  Names[405]="Equip/wide-brim_hat";
  Names[406]="Equip/cap";
  Names[407]="Equip/hat_straw_tall";
  Names[408]="Equip/hat_straw";
  Names[409]="Equip/hat_wizard";
  Names[410]="Equip/bonnet";
  Names[411]="Equip/hat_feathered";
  Names[412]="Equip/hat_tricorne";
  Names[413]="Equip/jester_hat";
  Names[414]="Equip/bear_mask";
  Names[415]="Equip/deer_mask";
  Names[416]="Equip/orc_mask";
  Names[417]="Equip/tribal_mask1";
  Names[418]="Equip/tribal_mask2";
  Names[419]="Equip/skullcap";
  Names[423]="Equip/Japan_Ninja_Leather_Pants";
  Names[424]="Equip/Japan_Ninja_Leather_Mitts";
  Names[425]="Equip/Japan_Ninja_Leather_Hood";
  Names[426]="Equip/Japan_Ninja_Leather_Jacke";
  Names[430]="Equip/pants_short";
  Names[431]="Equip/pants_long";
  Names[434]="Equip/shirt";
  Names[435]="Equip/shirt_fancy";
  Names[442]="Equip/Japan_Mail_Hatsuburi";
  Names[443]="Equip/Japan_Plate_Hatsuburi";
  Names[444]="Equip/Japan_Leather_Jingasa";
  Names[445]="Equip/Japan_Plate_Jingasa1";
  Names[446]="Equip/Japan_Plate_Jingasa2";
  Names[447]="Equip/plain_dress";
  Names[448]="Equip/fancy_dress";
  Names[449]="Equip/skirt";
  Names[450]="Equip/Japan_Plate_Jingasa";
  Names[451]="Equip/Japan_Plate_Kabuto1";
  Names[455]="Equip/kilt";
  Names[456]="Equip/Plate_Mempo";
  Names[457]="Equip/Leather_Do";
  Names[458]="Equip/Studded_Do";
  Names[459]="Equip/Plate_Do";
  Names[460]="Equip/460";
  Names[461]="Equip/460";
  Names[462]="Equip/Japan_Plate_Kabuto2";
  Names[463]="Equip/Japan_Plate_Kabuto3";
  Names[464]="Equip/Japan_Wakizashi";
  Names[465]="Equip/full_apron";
  Names[466]="Equip/half_apron";
  Names[467]="Equip/Japan_Bokuto";
  Names[468]="Equip/cloak";
  Names[469]="Equip/robe";
  Names[470]="Equip/Japan_Daisho";
  Names[471]="Equip/Japan_Leather_Mempo";
  Names[472]="Equip/Japan_Studded_Mempo";
  Names[473]="Equip/Japan_Leather_Hiro_Sode";
  Names[474]="Equip/Japan_Studded_Hiro_Sode";
  Names[476]="Equip/thigh_boots";
  Names[477]="Equip/boots";
  Names[478]="Equip/Japan_Plate_Hiro_Sode";
  Names[479]="Equip/sandals";
  Names[480]="Equip/shoes";
  Names[481]="Equip/flower_garland";
  Names[482]="Equip/fur_boots";
  Names[483]="Equip/fur_cape";
  Names[484]="Equip/fur_surong";
  Names[485]="Equip/gilded_dress";
  Names[486]="Equip/formal_shirt";
  Names[487]="Equip/Japan_Leather_Suneate";
  Names[488]="Equip/Japan_Studded_Suneate";
  Names[489]="Equip/Plate_Suneate";
  Names[490]="Equip/body_sash";
  Names[491]="Equip/Japan_Leather_Haidate";
  Names[492]="Equip/bandana";
  Names[500]="Equip/lantern";
  Names[501]="Equip/torch";
  Names[502]="Equip/candle";
  Names[503]="Equip/lantern";
  Names[504]="Equip/torch";
  Names[505]="Equip/candle";
  Names[506]="Equip/Japan_Plate_Haidate";
  Names[512]="Equip/Japan_Studded_Haidate";
  Names[513]="Equip/Japan_Ninja_Hood";
  Names[514]="Equip/Japan_Ninja_Leather_Belt";
  Names[516]="Equip/Japan_Kasa";
  Names[517]="Equip/Japan_Kamishimo";
  Names[518]="Equip/Japan_Hakama";
  Names[519]="Equip/Japan_No-Dachi";
  Names[520]="Equip/Japan_Tessen";
  Names[521]="Equip/studded_tunic";
  Names[523]="Equip/studded_sleeves";
  Names[525]="Equip/studded_gorget";
  Names[526]="Equip/Lajatang";
  Names[527]="Equip/platemail";
  Names[528]="Equip/platemail_arms";
  Names[529]="Equip/platemail_legs";
  Names[530]="Equip/platemail_gloves";
  Names[532]="Equip/Japan_Fukiya";
  Names[533]="Equip/Japan_Tekagi";
  Names[534]="Equip/Japan_Shuriken";
  Names[535]="Equip/Japan_Kama";
  Names[536]="Equip/Japan_Nunchaku";
  Names[537]="Equip/Japan_Sai";
  Names[538]="Equip/chainmail_tunic";
  Names[539]="Equip/539";
  Names[540]="Equip/chainmail_leggings";
  Names[542]="Equip/leather_tunic";
  Names[543]="Equip/leather_leggings";
  Names[544]="Equip/leather_sleeves";
  Names[545]="Equip/leather_gloves";
  Names[546]="Equip/leather_gorget";
  Names[547]="Equip/547";
  Names[548]="Equip/ringmail_armor";
  Names[549]="Equip/ringmail_leggings";
  Names[550]="Equip/ringmail_sleeves";
  Names[551]="Equip/Women_Torso1";
  Names[552]="Equip/Japan_Tattsuke-Hakama";
  Names[553]="Equip/Japan_Hakama-Shita";
  Names[554]="Equip/bone_armor";
  Names[555]="Equip/bone_leggings";
  Names[556]="Equip/bone_arms";
  Names[557]="Equip/bone_gloves";
  Names[558]="Equip/Japan_Kimono_Male";
  Names[559]="Equip/Japan_Kimono_Female";
  Names[560]="Equip/leather_cap";
  Names[561]="Equip/chainmail__coif";
  Names[562]="Equip/bone_helmet";
  Names[563]="Equip/plate_helm";
  Names[564]="Equip/helm_nose";
  Names[565]="Equip/helm";
  Names[566]="Equip/Japan_kabuto";
  Names[567]="Equip/Japan_Obi";
  Names[568]="Equip/Japan_Jin-Baori";
  Names[569]="Equip/Japan_Samurai_Waraji_and_Ta";
  Names[570]="Equip/570";
  Names[571]="Equip/Japan_Ninja_Jacket";
  Names[572]="Equip/Japan_Ninja_Tall_Tabi";
  Names[575]="Equip/Japan_Yumi";
  Names[576]="Equip/metal_shield";
  Names[577]="Equip/bronze_shield";
  Names[578]="Equip/wooden_shield";
  Names[579]="Equip/buckler";
  Names[580]="Equip/kite_shield1";
  Names[581]="Equip/kite_shield2";
  Names[582]="Equip/heater_shield";
  Names[583]="Equip/Japan_Tetsubo";
  Names[586]="Equip/";
  Names[611]="Equip/axe_large_battle";
  Names[612]="Equip/axe_two_handed";
  Names[613]="Equip/axe_executioner";
  Names[614]="Equip/axe_bardiche";
  Names[615]="Equip/axe_hatchet";
  Names[616]="Equip/heavy_crossbow";
  Names[617]="Equip/Staff_Black";
  Names[618]="Equip/broadsword";
  Names[619]="Equip/cleaver";
  Names[620]="Equip/club";
  Names[621]="Equip/Staff_Shepherds_Crook";
  Names[622]="Equip/dagger";
  Names[623]="Equip/cutlass";
  Names[624]="Equip/halberd";
  Names[625]="Equip/hammer_pick";
  Names[626]="Equip/javelin";
  Names[627]="Equip/katana";
  Names[628]="Equip/Staff_Gnarled";
  Names[629]="Equip/butcher_knife";
  Names[630]="Equip/kryss";
  Names[631]="Equip/mace";
  Names[633]="Equip/maul";
  Names[635]="Equip/pickaxe";
  Names[636]="Equip/pitchfork";
  Names[637]="Equip/scimitar";
  Names[638]="Equip/skinning_knife";
  Names[639]="Equip/short_spear";
  Names[643]="Equip/viking_sword";
  Names[644]="Equip/war_axe";
  Names[645]="Equip/war_fork";
  Names[646]="Equip/war_hammer";
  Names[647]="Equip/Hair";
  Names[648]="Equip/quarter_staff";
  Names[649]="Equip/bow";
  Names[651]="Equip/crossbow";
  Names[653]="Equip/axe";
  Names[675]="Equip/spear_fork";
  Names[679]="Equip/dragon_sleeves";
  Names[680]="Equip/dragon_breastplate";
  Names[681]="Equip/dragon_gloves";
  Names[682]="Equip/dragon_helm";
  Names[683]="Equip/dragon_leggings";
  Names[684]="Equip/amazon_armor1";
  Names[685]="Equip/amazon_armor2";
  Names[686]="Equip/amazon_armor3";
  Names[687]="Equip/fancy_dress1";
  Names[688]="Equip/fancy_dress2";
  Names[696]="Equip/Hair";
  Names[700]="Equip/700";
  Names[701]="Equip/Hair_Long_Hair";
  Names[702]="Equip/Hair_Ponytail";
  Names[703]="Equip/703";
  Names[710]="Equip/710";
  Names[712]="Equip/712";
  Names[759]="Equip/Arcane_Cloak";
  Names[760]="Equip/Arcane_Gloves";
  Names[761]="Equip/Arcane_Robe";
  Names[762]="Equip/Arcane_Thigh_Boots";
  Names[800]="Equip/face_hair_goatee";
  Names[801]="Equip/face_hair_long_beard";
  Names[802]="Equip/face_hair_short_beard";
  Names[803]="Equip/face_hari_mustache";
  Names[809]="Equip/809";
  Names[810]="Equip/810";
  Names[811]="Equip/811";
  Names[812]="Equip/Women_Torso2";
  Names[813]="Equip/Women_Torso3";
  Names[814]="Equip/Women_Torso4";
  Names[815]="Equip/815";
  Names[816]="Equip/816";
  Names[817]="Equip/817";
  Names[818]="Equip/818";
  Names[819]="Equip/819";
  Names[821]="Equip/821";
  Names[822]="Equip/822";
  Names[823]="Equip/823";
  Names[833]="Equip/833";
  Names[834]="Equip/834";
  Names[835]="Equip/835";
  Names[836]="Equip/836";
  Names[837]="Equip/837";
  Names[838]="Equip/838";
  Names[839]="Equip/839";
  Names[840]="Equip/840";
  Names[841]="Equip/841";
  Names[842]="Equip/842";
  Names[866]="Equip/helm_winged";
  Names[867]="Equip/867";
  Names[868]="Equip/868";
  Names[869]="Equip/Helm_of_Spirituali";
  Names[870]="Equip/Gorget_of_Truth";
  Names[871]="Equip/Breastplate_of_Jus";
  Names[872]="Equip/Arms_of_Compassion";
  Names[873]="Equip/Gauntlets_of_Valor";
  Names[874]="Equip/Legs_of_Honor";
  Names[875]="Equip/Solaretes_of_Sacri";
  Names[876]="Equip/Cloak_of_Humility";
  Names[877]="Equip/Book_Spell_Weaving";
  Names[878]="Equip/Japan_book_of_ninjitsu";
  Names[879]="Equip/Japan_book_of_bushido";
  Names[880]="Equip/880";
  Names[881]="Equip/881";
  Names[882]="Equip/Book_Necromancer";
  Names[883]="Equip/Book_of_Arms";
  Names[884]="Equip/Book_Paladin_Spellbook";
  Names[886]="Equip/Lt_Armor_Gorget";
  Names[887]="Equip/Quiver_of_Infinity";
  Names[888]="Equip/Shield_Dupres";
  Names[889]="Equip/Elven_Hair1";
  Names[890]="Equip/Elven_Hair2";
  Names[891]="Equip/Elven_Hair3";
  Names[892]="Equip/Elven_Hair4";
  Names[893]="Equip/Elven_Hair5";
  Names[894]="Equip/Elven_Hair6";
  Names[895]="Equip/Elven_Hair7";
  Names[896]="Equip/elven_quiver_r";
  Names[897]="Equip/elven_glasses_s";
  Names[898]="Equip/elven_robe_e01";
  Names[899]="Equip/elven_robe_e02";
  Names[900]="Equip/900";
  Names[901]="Equip/901";
  Names[902]="Equip/902";
  Names[903]="Equip/903";
  Names[904]="Equip/904";
  Names[905]="Equip/905";
  Names[906]="Equip/906";
  Names[907]="Equip/elven_shirt_Male";
  Names[908]="Equip/elven_shirt_Male";
  Names[909]="Equip/tunic";
  Names[910]="Equip/surcoat";
  Names[911]="Equip/checkered_shirt";
  Names[912]="Equip/jester_suit";
  Names[913]="Equip/doublet";
  Names[914]="Equip/elven_shirt_t__Ches";
  Names[915]="Equip/elven_shirt_t__Ches";
  Names[916]="Equip/Elven_Hair01_BL_Mu";
  Names[917]="Equip/hair1";
  Names[918]="Equip/hair2";
  Names[919]="Equip/Elven_Hair04_BR_Mu";
  Names[920]="Equip/Elven_Pants_s";
  Names[921]="Equip/studded_armor";
  Names[922]="Equip/plate_armor";
  Names[923]="Equip/leather_armor";
  Names[924]="Equip/leather_bustier";
  Names[925]="Equip/studded_bustier";
  Names[926]="Equip/leather_skirt";
  Names[927]="Equip/leather_shorts";
  Names[928]="Equip/Helm_Close";
  Names[929]="Equip/bascinet";
  Names[930]="Equip/Scythe";
  Names[931]="Equip/bone_harvester";
  Names[932]="Equip/scepter";
  Names[933]="Equip/staff_bladed";
  Names[934]="Equip/pike";
  Names[935]="Equip/staff_double_bladed";
  Names[936]="Equip/lance";
  Names[937]="Equip/crescent_blade";
  Names[938]="Equip/composite_bow";
  Names[939]="Equip/repeating_crossbow";
  Names[941]="Equip/Elven_Boot_t";
  Names[942]="Equip/Elven_Plate";
  Names[943]="Equip/Elven_Plate_Belt";
  Names[944]="Equip/Elven_Plate_Gorget";
  Names[945]="Equip/Elven_Plate_Gloves";
  Names[946]="Equip/Elven_Plate_Legs";
  Names[947]="Equip/Elven_Arm_Plate";
  Names[948]="Equip/Elven_Plate";
  Names[950]="Equip/Elven_Composite_Lo";
  Names[951]="Equip/Magical_Shortbow";
  Names[952]="Equip/Elven_Spellblade";
  Names[953]="Equip/Assassin_Spike";
  Names[954]="Equip/Leafblade";
  Names[955]="Equip/War_Cleaver";
  Names[956]="Equip/Diamond_Mace";
  Names[957]="Equip/Wild_Staff";
  Names[958]="Equip/Rune_Blade";
  Names[959]="Equip/Radiant_Scimitar";
  Names[960]="Equip/Axe_Ornate";
  Names[961]="Equip/bone_machete";
  Names[965]="Equip/Helm_Raven";
  Names[966]="Equip/Helm_Vulture";
  Names[967]="Equip/Helm_Winged";
  Names[968]="Equip/Hide_Chest";
  Names[969]="Equip/Hide_Gloves";
  Names[971]="Equip/knee_skirt";
  Names[972]="Equip/fishing_pole";
  Names[973]="Equip/Helm_Orc";
  Names[974]="Equip/Hide_Gorget";
  Names[975]="Equip/Hide_Paldrons";
  Names[976]="Equip/Hide_Pants";
  Names[977]="Equip/Hide_Female_Chest";
  Names[978]="Equip/Lt_Armor_Chest";
  Names[979]="Equip/Lt_Armor_Gloves";
  Names[980]="Equip/wand1";
  Names[981]="Equip/wand2";
  Names[982]="Equip/wand3";
  Names[983]="Equip/wand4";
  Names[984]="Equip/Book_Spellbook";
  Names[985]="Equip/985";
  Names[986]="Equip/Women_Torso5";
  Names[987]="Equip/Robe_Red";
  Names[989]="Equip/Lt_Armor_Paldrons";
  Names[992]="Equip/Shield_Order";
  Names[993]="Equip/Shield_Chaos";
  Names[995]="Equip/Lt_Armor_Pants";
  Names[996]="Equip/leaf_tonlet_t_r_Kil";
  Names[997]="Equip/LtF_Chest_Chest";

  Names[1027]="Equip/";
  Names[1028]="Equip/";
  Names[1029]="Equip/";
  Names[1030]="Equip/";
  Names[1032]="Equip/";
  Names[1033]="Equip/";
  Names[1034]="Equip/";
  Names[1035]="Equip/";
  Names[1036]="Equip/";
  Names[1037]="Equip/";
  Names[1038]="Equip/";
  Names[1039]="Equip/";
  Names[1040]="Equip/";
  Names[1041]="Equip/";
  Names[1042]="Equip/";
  Names[1043]="Equip/";
  Names[1044]="Equip/";
  Names[1045]="Equip/";
  Names[1046]="Equip/";
  Names[1047]="Equip/";

  Names[320]="Equip/";
  Names[335]="Equip/";
  Names[337]="Equip/";
  Names[338]="Equip/";
  Names[339]="Equip/";
  Names[340]="Equip/";
  Names[341]="Equip/";
  Names[342]="Equip/";
  Names[343]="Equip/";
  Names[344]="Equip/";
  Names[427]="Equip/";
  Names[499]="Equip/";
  Names[588]="Equip/";
  Names[592]="Equip/";
  Names[594]="Equip/";
  Names[596]="Equip/";
  Names[665]="Equip/";
  Names[673]="Equip/";
  Names[676]="Equip/";
  Names[690]="Equip/";
  Names[697]="Equip/";
  Names[699]="Equip/";
  Names[843]="Equip/";
  Names[940]="Equip/";
  Names[962]="Equip/";
  Names[333]="Equip/";
  Names[509]="Equip/";
  Names[598]="Equip/";
  Names[604]="Equip/";
  Names[652]="Equip/";
  Names[691]="Equip/";
  Names[692]="Equip/";
  Names[698]="Equip/";

  Names[321]="Equip/";
  Names[322]="Equip/";
  Names[323]="Equip/";
  Names[324]="Equip/";
  Names[325]="Equip/";
  Names[326]="Equip/";
  Names[327]="Equip/";
  Names[328]="Equip/";
  Names[329]="Equip/";
  Names[330]="Equip/";
  Names[331]="Equip/";
  Names[332]="Equip/";
  Names[428]="Equip/";
  Names[475]="Equip/";
  Names[541]="Equip/";
  Names[585]="Equip/";
  Names[587]="Equip/";
  Names[589]="Equip/";
  Names[590]="Equip/";
  Names[591]="Equip/";
  Names[593]="Equip/";
  Names[595]="Equip/";
  Names[597]="Equip/";
  Names[599]="Equip/";
  Names[600]="Equip/";
  Names[601]="Equip/";
  Names[602]="Equip/";
  Names[603]="Equip/";
  Names[609]="Equip/";
  Names[610]="Equip/";
  Names[632]="Equip/";
  Names[650]="Equip/";
  Names[654]="Equip/";
  Names[655]="Equip/";
  Names[656]="Equip/";
  Names[657]="Equip/";
  Names[658]="Equip/";
  Names[659]="Equip/";
  Names[660]="Equip/";
  Names[661]="Equip/";
  Names[662]="Equip/";
  Names[663]="Equip/";
  Names[664]="Equip/";
  Names[668]="Equip/";
  Names[669]="Equip/";
  Names[670]="Equip/";
  Names[671]="Equip/";
  Names[672]="Equip/";
  Names[674]="Equip/";
  Names[677]="Equip/";
  Names[678]="Equip/";
  Names[707]="Equip/";
  Names[708]="Equip/";
  Names[769]="Equip/";
  Names[827]="Equip/";
  Names[1048]="Equip/";
  Names[1049]="Equip/";
  Names[1050]="Equip/";
  Names[1051]="Equip/";
  Names[1053]="Equip/";
  Names[1054]="Equip/";
  Names[1055]="Equip/";
  Names[1056]="Equip/";
  Names[1057]="Equip/";
  Names[1058]="Equip/";
  Names[1063]="Equip/";
  Names[1064]="Equip/";
  Names[1065]="Equip/";
  Names[1066]="Equip/";

  Names[634]="Equip/";
  Names[640]="Equip/";
  Names[641]="Equip/";
  Names[642]="Equip/";
  Names[1062]="Equip/";
  Names[1067]="Equip/";
}


static void saveSprite(char *Output, u1 *M, int L) {
  char Tmp[1024], Dir[256];
  int I, J, K, C, CC, CX, CY, X, Y, Off, Wide, High, SX, SY, EX, EY, Terms=0;
  int A, R, G, B;
  u4 U, *Pal;
  u1 *Q;
  pic *P;
  amou *H = (amou*)M;
  amouFrame *F;
  frameHeader *FH;

  printf("    Unit=%04d NColors=%d Pal=#%X Tbl=#%X NFrames=%d XY=%d,%d:%d,%d\n"
        ,H->UnitID, H->NColors, H->PaletteOff, H->Frames, H->NFrames, H->SX, H->SY, H->EX, H->EY);

  Pal = (u4*)(M+H->PaletteOff);

  if (H->NColors == 1) return; // RETREAT!

  initNames();

#if 0
  I = 0;
  do {
    if (I++) sprintf(Dir, "%03d_%d", H->UnitID, I);
    else sprintf(Dir, "%03d", H->UnitID);
    sprintf(Tmp, "%s/%s", Output, Dir);
  } while (fileExist(Tmp));
#endif

  // determine sprite encoding
  times (I,L-3) if (*(u4*)(M+I) == 0x7FFF7FFF) Terms++;

  F = (amouFrame*)(M+H->Frames);
  times (I, H->NFrames) {
    Off = (u1*)F - M + F->Off;

    //if (I == 1) abort();
    //printf("    Anim=%03d N=%03d Off=#%08X XY=%d,%d:%d,%d\n"
    //      ,F->AnimID, F->Num, Off, F->SX, F->SY, F->EX, F->EY);
    //printf("      W=%d H=%d\n", F->EX-F->SX, F->EY-F->SY);

#if 0
    sprintf(Tmp, "%s/anims/%06d/%03d", Output, H->UnitID, F->AnimID);
    writeFile(0, L, Tmp, M);
    return;
#endif

    if (Terms < H->NFrames) { // KR sprite (decompiled from sub_420E50)
      Wide=F->EX-F->SX;
      High=F->EY-F->SY;

      CX = max(abs(F->SX),abs(F->EX));
      CY = max(abs(F->SY),abs(F->EY));
      P = picNew(CX*2, CY*2, 32);
      CX += F->SX;
      CY += F->SY;

      Q = M+Off;
      X=Y=0;
      do {
        C = *Q++;
        if (C < 0x80) {
          while (C--) { // alpha
            fromR8G8B8(B,G,R, Pal[0]);
            picPut32(P,CX+X++,CY+Y,R8G8B8A8(R,G,B,0));
            if (X == Wide) {Y++; X=0;}
          }
          continue;
        }
        U = *Q++;
        A = U&0xF0;
        if (A) {
          fromR8G8B8(B,G,R, Pal[*Q++]);
          picPut32(P,CX+X++,CY+Y,R8G8B8A8(R,G,B, (A|(A>>4))));
          if (X == Wide) {Y++; X=0;}
        }
        C ^= 0x80;
        while (C--) {
          fromR8G8B8(B,G,R, Pal[*Q++]);
          picPut32(P,CX+X++,CY+Y,R8G8B8A8(R,G,B, 0xFF));
          if (X == Wide) {Y++; X=0;}
        }
        A = (U&0xF)<<4;
        if (A) {
          fromR8G8B8(B,G,R, Pal[*Q++]);
          picPut32(P,CX+X++,CY+Y,R8G8B8A8(R,G,B, (A|(A>>4))));
          if (X == Wide) {Y++; X=0;}
        }
      } while (Y<High);
      goto save;
    }

    FH = (frameHeader*)(M+Off);

    Wide=8;
    High=8;
    Q = (u1*)(FH+1);
    while(*(u4*)Q != 0x7FFF7FFF && Q <= M+L-4) {
      U = ru4(Q);
      C = U&((1<<12)-1);
      Y = (U>>12)&((1<<10)-1);
      X = (U>>22)&((1<<10)-1);

      X=(X^0x200)-0x200;
      Y=(Y^0x200)-0x200;

      if (X < 0) Wide = max(abs(X)+1,Wide);
      Wide = max(X+C+1,Wide);
      High = max(abs(Y)+1,High);
      Q += C;
    }

    P = picNew(Wide*2, High*2, 8);
    P->K = 0;

    times(J, 256) {
      int A, R, G, B;
      fromA1R5G5B5(A, R, G, B, FH->Pal[J]); //(X>>1)&0x3DEF;
      P->P[J*4+0] = R;
      P->P[J*4+1] = G;
      P->P[J*4+2] = B;
      P->P[J*4+3] = 0;
    }


    CX = P->W/2;
    CY = P->H/2;
    //if (!F->W || !F->H) continue;
    Q = (u1*)(FH+1);
    while(*(u4*)Q != 0x7FFF7FFF && Q <= M+L-4) {
      U =*(u4*)Q; Q+=4;
      C = U&((1<<12)-1);
      Y = (U>>12)&((1<<10)-1);
      X = (U>>22)&((1<<10)-1);

      X=(X^0x200)-0x200;
      Y=(Y^0x200)-0x200;

      //printf("      %05x: %d,%d:%d\n", Q-M-4, X, Y, C);
      while (C-- && Q < M+L) picPut(P, CX + X++, CY + Y, *Q++);
    }

save:
    if (H->UnitID < 0x10000 && Names[H->UnitID]) {
      sprintf(Tmp, "%s/%s (%06d)/%03d/%04d.png"
             ,Output, Names[H->UnitID], H->UnitID, F->AnimID, F->Num);
    } else {
      sprintf(Tmp, "%s/Unknown/%06d/%03d/%04d.png", Output, H->UnitID, F->AnimID, F->Num);
    }
    //sprintf(Tmp, "%s/%s/%03d_%03d.png", Output, Dir, F->Num, F->MoveID);
    pngSave(Tmp, P);
    picDel(P);
next:
    F++;
  }
}

static void uopDecompile(char *Output, char *Input) {
  fileList *FL;
  char Tmp[1024], Name[256], *P;
  int I, J, K, C, X, Y, Next, Off, Len, EOff;
  u1 *Q, *Z;
  grp *G;
  entry *E;

#if 0
  //strcpy(Tmp, "input/anims1s/000197/030"); //broken palette
  //strcpy(Tmp, "input/anims1s/000198/028"); //clipped wings
  //strcpy(Tmp, "input/anims1s/000286/000"); //broken
  //strcpy(Tmp, "input/anims1s/000130/001");
  //strcpy(Tmp, "input/paperdoll/001");
  strcpy(Tmp, "input/anims1/000014/000");
  Len = fileSize(Tmp);
  Z = readFile(0, Len, Tmp);
  saveSprite("out", Z, Len);
  abort();
#endif

  int L = fileSize(Input);
  header *H = readFile(0, sizeof(header), Input);

  //sprintf(Tmp, "%s/input/uo.dic", ProgDir);
  //sprintf(Name, "%s/input/uo.txt", ProgDir);
  //dicToTxt(Name, Tmp);

  //loadDic();
  loadTxt();

  if(!memcmp(H->Id, "UOP", 3)) {
    printf("  this is an old UOP, not MYP\n");
    abort();
  }

  if(memcmp(H->Id, "MYP", 3)) {
    printf("  not a UOP file\n");
    abort();
  }

  printf("  NBlocks=%d\n", H->NBlocks);

  for (I = 0, Next = H->First; Next; Next = G->Next, I++) {
    G = readFile(Next, sizeof(grp), Input);
    printf("  NFiles=%d Next=#%08x\n", G->NFiles, Next);

    EOff = Next+sizeof(grp);
    times (J, G->NFiles) {
      E = readFile(EOff, sizeof(entry), Input);
      sprintf(Name, "%04d_%03d_%08X%08X", I, J, *(u4*)(E->Hash+0), *(u4*)(E->Hash+4));
      if ((P = searchName(*(u8*)E->Hash))) strcpy(Name, P);

      printf("  Extracting: %s\n", Name);
      printf("    Off=#%08X Len=#%08X/#%08X E=%d H=%d CRC=#%08X\n"
            , (u4)E->Off, E->Len, E->ULen, E->Enc, E->HdrLen, E->CRC);

      unless (E->ULen) goto next; // some entries are missing

      if (E->Enc == 0) Z = readFile(E->Off+E->HdrLen, E->ULen, Input);
      else if (E->Enc == 1) {
        Q = readFile(E->Off+E->HdrLen, E->Len, Input);
        Z = inflateBuf(E->ULen, E->Len, Q);
        free(Q);
      } else {
        printf("  invalid encoding\n");
        abort();
      }

      if (!memcmp("AMOU", Z, 4)) saveSprite(Output, Z, E->ULen);
      else {
        sprintf(Tmp, "%s/%s", Output, Name);
        writeFile(0, E->ULen, Tmp, Z);
      }

      free(Z);
next:
      EOff += sizeof(entry);
    }
  }
}

int uopInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "uop";
  F->Description = "Ultima Online Mythic Package";
  F->Decompile = uopDecompile;
  return 1;
}

