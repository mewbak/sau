//Credits go to Alazane, who published specs

#include "common.h"
#include "uthash.h"


static char UODir[1024];


typedef struct {
  u4 Off;
  u4 Len;
  u4 Info;
} __attribute__ ((packed)) idx;




typedef struct {
  u4 Flags;
  u2 Texture;
  u1 Name[20];
} __attribute__ ((packed)) tdLandBlk;

typedef struct {
  u4 Header;
  tdLandBlk E[32];
} __attribute__ ((packed)) tdLand;


typedef struct {
  u4 Flags;
  u1 Weight; // 255 if not moveable
  u1 Quality;
  u2 U1;
  u1 U2;
  u1 Quantity;
  u2 Anim; // BodyID
  u1 U3;
  u1 Hue;
  u1 U4;
  u1 U5;
  u1 Height;
  u1 Name[20];
} __attribute__ ((packed)) tdStaticBlk;


typedef struct {
  u4 Header;
  tdStaticBlk E[32];

  // If Weapon, Quantity is Weapon Class.
  // If Armor, Quantity is Armor Class.
  // If Wearable, Quality is Layer.
  // If Light Source, Quality is LightID
  // If Container, Height is "Contains" (Something determining how much the container can hold?)
} __attribute__ ((packed)) tdStatic;

// these used to create directory path
static struct {u4 Flag; char*Name;} Flags[] = {
  {0x00000001, "Background"},
  {0x10000000, "Roof"},
  {0x20000000, "Door"},
  {0x00000010, "Wall"},
  {0x40000000, "StairBack"},
  {0x80000000, "StairRight"},
  {0x00000400, "Bridge"},
  {0x00020000, "Foliage"},
  {0x00000200, "Surface"},
  {0x00001000, "Window"},
  {0x00000002, "Weapon"},
  {0x00200000, "Container"},
  {0x00400000, "Wearable"},
  {0x08000000, "Armor"},
  {0x00000080, "Wet"},
  {0x00000040, "Impassable"},
  {0x00000020, "Damaging"},
  {0x01000000, "Animation"},
  {0x00800000, "LightSource"},
  {0x00000004, "Transparent"},
  {0x00000008, "Translucent"},
  //{0x00000100, "Unknown1"},
  //{0x00000800, "Generic"},
  //{0x00002000, "NoShoot"},
  //{0x00004000, "ArticleA"},
  //{0x00008000, "ArticleAn"},
  //{0x00010000, "Internal"},
  //{0x00040000, "PartialHue"},
  //{0x00080000, "Unknown2"},
  //{0x00100000, "Map"},
  //{0x02000000, "NoDiagonal"},
  //{0x04000000, "Unknown3"},
  {0, 0}
  };

static tdLand*TDLand;
static tdStatic*TDStatic;
static int TDStaticLen;
static void **TileData;

static void loadTileData() {
  char Name[32];
  int I, J, K, L, Len;
  tdStatic*S;
  u4 Key;
  u1*M,*E;
  char Tmp[1024],*P;

  TileData = ns(void*, 6*(1<<16));

  sprintf(Tmp, "%s/Tiledata.mul", UODir);
  if (fileExist(Tmp)) goto found;

  sprintf(Tmp, "%s/tiledata.mul", UODir);
  if (fileExist(Tmp)) goto found;

  printf("  cant open %s\n", Tmp);
  abort();

found:
  L = fileSize(Tmp);
  M = readFile(0, L, Tmp);
  E = M + L;
  TDLand = (tdLand*)M;
  TDStatic = (tdStatic*)(TDLand+512);
  TDStaticLen = (tdStatic*)E - TDStatic;

  Name[20] = 0;

  K=0;

  times (I, 512) {
    times (J, 32) {
      //printf("  %03d:%02d:%s\n", I, J, TDLand[I].E[J].Name);
      TileData[K++] = &TDLand[I].E[J];
    }
  }

  S = TDStatic;
  times (I, TDStaticLen ) {
    times (J, 32) {
      memcpy(Name, S[I].E[J].Name, 20);
      //printf("  %05d:%03d:%02d:%04d:%s\n", K, I, J, S[I].E[J].Anim, Name);
      TileData[K++] = &S[I].E[J];
    }
  }
}



typedef struct {
    u4 Key;
    char*Value;
    UT_hash_handle hh;
} cliloc;

static cliloc*Cliloc;

static void clilocAdd(u4 Key, char*Value) {
  cliloc*C = n(cliloc);
  C->Value = strdup(Value);
  HASH_ADD_INT(Cliloc, Key, C);
}

static char*clilocGet(u4 Key) {
  cliloc*C;
  HASH_FIND_INT(Cliloc, &Key, C);
  return C->Value;
}

static void loadCliloc() {
  int L, Len;
  u4 Key;
  u1*M,*E;
  char*Tmp,*P;
  Tmp = ns(char, 1<<16);
  sprintf(Tmp, "%s/Cliloc.enu", UODir);
  if (fileExist(Tmp)) goto found;

  sprintf(Tmp, "%s/cliloc.enu", UODir);
  if (fileExist(Tmp)) goto found;

  printf("  cant open %s\n", Tmp);
  abort();

found:

  //printf("  cliloc: %s\n", Tmp);
  L = fileSize(Tmp);
  M = readFile(0, L, Tmp);
  E = M + L;
  M += 6; // skip header
  while (M < E) {
    Key =*(u4*)M; M+=4;
    M+=1; // unknown
    Len =*(u2*)M; M+=2;
    memcpy(Tmp, M, Len); M += Len;
    Tmp[Len] = 0;
    //printf("  %08X:%s\n", Key, Tmp);
    clilocAdd(Key, Tmp);
  }
}

typedef struct {
  u2 Pal[256]; // palette
  u4 NFrames;
  //u4 Offs[NFrames];
} __attribute__ ((packed)) animHeader;

typedef struct {
  s2 X;
  s2 Y;
  s2 W;
  s2 H;
  //u1 Data[]
} __attribute__ ((packed)) animFrame;


static char*Out;

static struct {
  char *Mul;
  int A;
  int B;
  char *Name;
} Names[] = {
  // anim.mul: Original Models (passable 3d render)
  {"anim",  110,  204, "monsters/Ogre"},
  {"anim",  220,  314, "monsters/Ettin"},
  {"anim",  330,  424, "monsters/Zombie"},
  {"anim",  440,  549, "monsters/Gargoyle"},
  {"anim",  450,  659, "animals/Eagle"},
  {"anim",  660,  769, "animals/Bird"},
  {"anim",  770,  864, "monsters/Orc Captain"},
  {"anim",  880,  974, "monsters/Swamp Tentacle"},
  {"anim",  990, 1209, "monsters/Daemon"},
  {"anim", 1320, 1429, "monsters/Dragon Gray"},
  {"anim", 1430, 1524, "monsters/Air Elemental"},
  {"anim", 1540, 1634, "monsters/Earth Elemental"},
  {"anim", 1650, 1744, "monsters/Fire Elemental"},
  {"anim", 1760, 1854, "monsters/Water Elemental"},
  {"anim", 1870, 1964, "monsters/Orc"},
  {"anim", 1980, 2074, "monsters/Ettin2"},
  {"anim", 2310, 2404, "animals/Giant Serpent"},
  {"anim", 2420, 2514, "monsters/Gazer"},
  {"anim", 2640, 2734, "monsters/Lich"},
  {"anim", 2860, 2954, "monsters/Ghoul"},
  {"anim", 3080, 3174, "monsters/Giant Spider"},
  {"anim", 3190, 3284, "animals/Gorilla"},
  {"anim", 3300, 3409, "monsters/Harpy"},
  {"anim", 3410, 3504, "monsters/Headless One"},
  {"anim", 3630, 4054, "monsters/Lizardman"},
  {"anim", 4290, 4399, "monsters/Mongbat"},
  {"anim", 4510, 4604, "monsters/Orc"},
  {"anim", 4620, 5044, "monsters/Ratman"},
  {"anim", 5170, 5264, "monsters/Reaper"},
  {"anim", 5280, 5374, "monsters/Scorpion"},
  {"anim", 5500, 5594, "monsters/Skeleton"},
  {"anim", 5610, 5704, "monsters/Slime"},
  {"anim", 5720, 5814, "animals/Snake"},
  {"anim", 5830, 6144, "monsters/Troll"},
  {"anim", 6160, 6364, "monsters/Skeleton"},
  {"anim", 6380, 6474, "monsters/Wisp"},
  {"anim", 6490, 6599, "monsters/Dragon Red"},
  {"anim", 6600, 6709, "monsters/Dragon Gray Small"},
  {"anim", 6710, 6819, "monsters/Dragon Red Small"},
  {"anim", 7700, 7794, "monsters/Terathan Warrior"},
  {"anim", 7810, 7904, "monsters/Terathan Drone"},
  {"anim", 7920, 8014, "monsters/Terathan Matriarch"},
  {"anim", 8250, 8454, "monsters/Cyclopean Warrior"},
  {"anim", 8800, 8894, "animals/Giant Toad"},
  {"anim", 8910, 9004, "animals/Bullfrog"},
  {"anim", 9350, 9444, "monsters/Ophidian Shaman"},
  {"anim", 9460, 9554, "monsters/Ophidian Warrior"},
  {"anim", 9570, 9664, "monsters/Ophidian Matriarch"},
  {"anim", 10450, 10544, "monsters/Ice Serpent"},
  {"anim", 16500, 16544, "monsters/Sea Serpent"},
  {"anim", 16610, 16654, "animals/Dolphin"},
  {"anim", 22000, 22064, "animals/Horse1"},
  {"anim", 22065, 22129, "animals/Cat"},
  {"anim", 22130, 22194, "animals/Alligator"},
  {"anim", 22195, 22259, "animals/Pig1"},
  {"anim", 22260, 22324, "animals/Horse2"},
  {"anim", 22325, 22389, "animals/Rabbit"},
  {"anim", 22390, 22454, "animals/Alligator Grayscale"},
  {"anim", 22455, 22519, "animals/Sheep"},
  {"anim", 22520, 22584, "animals/Chicken"},
  {"anim", 22585, 22649, "animals/Goat"},
  {"anim", 22650, 22714, "animals/Ostard Desert"},
  {"anim", 22715, 22779, "animals/Bear Black"},
  {"anim", 22780, 22844, "animals/Bear Brown"},
  {"anim", 22845, 22909, "animals/Bear White"},
  {"anim", 22910, 22974, "animals/Panther"},
  {"anim", 22975, 23039, "animals/Giant Rat"},
  {"anim", 23040, 23104, "animals/Cow1"},
  {"anim", 23105, 23169, "animals/Dog"},
  {"anim", 23170, 23299, "animals/Ostard Frenzied"},
  {"anim", 23300, 23364, "animals/Llama"},
  {"anim", 23365, 23429, "animals/Walrus"},
  {"anim", 23495, 23559, "animals/Sheep without Wool"},
  {"anim", 23625, 23689, "animals/Wolf Timber"},
  {"anim", 23690, 23754, "animals/Horse3"},
  {"anim", 23820, 23884, "animals/Horse4"},
  {"anim", 24015, 24079, "animals/Cow2"},
  {"anim", 24080, 24144, "animals/Bull1"},
  {"anim", 24145, 24209, "animals/Bull2"},
  {"anim", 24210, 24274, "animals/Deer Male"},
  {"anim", 24405, 24469, "animals/Deer Female"},
  {"anim", 24470, 24534, "animals/Rat"},
  {"anim", 27850, 27914, "animals/Pig2"},
  {"anim", 27915, 27979, "animals/Horse Pack 1"},
  {"anim", 27980, 28044, "animals/Llama Pack"},
  {"anim", 35000, 35174, "player/body male"},
  {"anim", 35175, 35349, "player/body female"},
  {"anim", 35700, 38499, "player/hats1"},
  {"anim", 40250, 40599, "player/pants"},
  {"anim", 40950, 41299, "player/shirts1"},
  {"anim", 43225, 43749, "player/robes1"},
  {"anim", 44625, 44799, "player/skirts1"},
  {"anim", 46375, 47249, "player/cloaks1"},
  {"anim", 48300, 49174, "player/boots1"},
  {"anim", 50750, 51274, "player/hats2"},
  {"anim", 52500, 53549, "player/lanterns"},
  {"anim", 56175, 56699, "player/shirts2"},
  {"anim", 56875, 57049, "player/hats3"},
  {"anim", 57225, 57749, "player/plate-mail1"},
  {"anim", 57750, 59674, "player/chain-mail1"},
  {"anim", 59850, 60724, "player/leather"},
  {"anim", 60900, 61424, "player/chain-mail2"},
  {"anim", 61950, 62649, "player/plate-mail2"},
  {"anim", 63000, 64224, "player/hats4"},
  {"anim", 65275, 65449, "effects/storm"},
  {"anim", 65450, 65624, "effects/blades"},
  {"anim", 65800, 67024, "player/shields1"},
  {"anim", 71925, 79449, "player/weapons1"},
  {"anim", 87500, 89774, "player/wigs1"},
  {"anim", 105000, 105698, "player/beards1"},
  {"anim", 108615, 108649, "animals/horse mounted 1"},
  {"anim", 109315, 109349, "animals/horse mounted 2"},
  {"anim", 109490, 109524, "animals/Ostard Desert mounted"},
  {"anim", 109664, 109874, "animals/Ostard Frenzied mounted"},
  {"anim", 110015, 110049, "animals/Llama mounted"},
  {"anim", 113165, 113199, "animals/horse mounted 3"},
  {"anim", 113515, 113549, "animals/horse mounted 4"},
  {"anim", 122500, 123024, "player/wigs2"},
  {"anim", 123025, 123723, "player/beards2"},
  {"anim", 124075, 127049, "player/shirts3"},
  {"anim", 127050, 127399, "player/skirts2"},
  {"anim", 127400, 127749, "player/hats5"},
  {"anim", 134750, 135099, "player/robes2"},
  {"anim", 135100, 135274, "player/tools1"},
  {"anim", 135275, 135449, "player/hats6"},
  {"anim", 136500, 137549, "player/weapons2"},
  {"anim", 137550, 137724, "player/shirts4"},
  {"anim", 137725, 137899, "player/robes1"},
  {"anim", 137900, 138074, "player/shirts5"},
  {"anim", 138250, 138424, "npc/02"},
  {"anim", 138425, 138599, "npc/03"},
  {"anim", 138600, 138949, "player/shields2"},
  {"anim", 138950, 139124, "npc/04"},

  // anim2.mul: Lord Blackthorn's Revenge (ugly polygonal models)
  {"anim2", 110, 204, "monsters/LBR Spiders/Black Widow"},
  {"anim2", 220, 314, "monsters/LBR Spiders/Black Widow Giant"},
  {"anim2", 330, 424, "monsters/LBR Spiders/Dread"},
  {"anim2", 440, 534, "monsters/LBR Spiders/Frost"},
  {"anim2", 550, 644, "monsters/LBR Spiders/Normal"},
  {"anim2", 660, 754, "monsters/Centaur"},
  {"anim2", 770, 879, "monsters/LBR Daemons/Normal"},
  {"anim2", 880, 989, "monsters/LBR Daemons/Black Gate Daemon"},
  {"anim2", 990, 1099, "monsters/LBR Daemons/Elder Daemon"},
  {"anim2", 1100, 1209, "monsters/LBR Daemons/Exodus"},
  {"anim2", 1210, 1319, "monsters/LBR Daemons/Ice Fiend"},
  {"anim2", 1320, 1414, "monsters/Serpentine Dragon"},
  {"anim2", 1430, 1539, "monsters/Skeletal Dragon"},
  {"anim2", 1540, 1649, "monsters/Wyvern"},
  {"anim2", 1650, 1744, "monsters/Pixie"},
  {"anim2", 1760, 1854, "monsters/Pixie Silvani"},
  {"anim2", 1870, 1979, "monsters/LBR Gargoyle"},
  {"anim2", 1980, 2074, "monsters/Imp"},
  {"anim2", 2090, 2184, "monsters/Kraken"},
  {"anim2", 2200, 2404, "monsters/Evil Mage"},
  {"anim2", 2420, 2514, "monsters/Harrower"},
  {"anim2", 2530, 2624, "monsters/Skeleton Mage"},
  {"anim2", 2640, 2734, "monsters/Wraith"},
  {"anim2", 2750, 2844, "monsters/Succubus"},
  {"anim2", 2860, 2954, "monsters/Terathan Avenger"},
  {"anim2", 2970, 3064, "monsters/Titan"},
  {"anim2", 3080, 3174, "monsters/Mummy"},
  {"anim2", 3190, 3284, "monsters/Golem"},
  {"anim2", 3300, 3394, "monsters/LBR Orc"},
  {"anim2", 3410, 3504, "monsters/LBR Orc Lord"},
  {"anim2", 3520, 3614, "monsters/Juka Warrior"},
  {"anim2", 3630, 3724, "monsters/Juka Mage"},
  {"anim2", 3740, 3834, "monsters/Juka Lord"},
  {"anim2", 3850, 3944, "monsters/Betrayer"},
  {"anim2", 3960, 4054, "monsters/Juggernaut"},
  {"anim2", 4070, 4164, "monsters/Blackthorn"},
  {"anim2", 4180, 4274, "monsters/Meer Mage"},
  {"anim2", 4290, 4384, "monsters/Meer Warrior"},
  {"anim2", 4400, 4494, "monsters/Meer Eternal"},
  {"anim2", 4510, 4604, "monsters/Meer Captain"},
  {"anim2", 4620, 4714, "monsters/Dawn"},
  {"anim2", 4730, 4824, "monsters/Plague Beast"},
  {"anim2", 4840, 4934, "monsters/Horde Minion"},
  {"anim2", 4950, 5044, "monsters/Doppleganger"},
  {"anim2", 5060, 5154, "monsters/Gazer Larva"},
  {"anim2", 5170, 5264, "monsters/Bogling"},
  {"anim2", 5280, 5374, "monsters/Bog Thing"},
  {"anim2", 5390, 5484, "monsters/Solen Worker"},
  {"anim2", 5500, 5594, "monsters/Solen Warrior"},
  {"anim2", 5610, 5704, "monsters/Solen Queen"},
  {"anim2", 5720, 5814, "monsters/Arcane Daemon"},
  {"anim2", 5830, 5924, "monsters/Moloch"},
  {"anim2", 6050, 6144, "monsters/Ant Lion"},
  {"anim2", 6160, 6254, "monsters/Sphynx"},
  {"anim2", 6270, 6364, "monsters/Quagmire"},
  {"anim2", 6490, 6584, "monsters/Chaos Daemon"},
  {"anim2", 6600, 6694, "monsters/Horde Daemon"},
  {"anim2", 6710, 6804, "monsters/Horde Daemon Large"},
  {"anim2", 6820, 6914, "monsters/Dragon Rikktor"},
  {"anim2", 6930, 7024, "monsters/Ancient Wyrm"},
  {"anim2", 7040, 7244, "monsters/Crystal Lattice Seeker"},
  {"anim2", 7260, 7354, "monsters/Succubus Semidar"},
  {"anim2", 7370, 7464, "monsters/Orc Scout"},
  {"anim2", 7590, 7684, "monsters/Horde Daemon Colored"},
  {"anim2", 7700, 7809, "monsters/LBR Gargoyle Stone"},
  {"anim2", 7810, 7919, "monsters/LBR Gargoyle Enslaved"},
  {"anim2", 7920, 8029, "monsters/LBR Gargoyle Fire"},
  {"anim2", 8030, 8139, "monsters/LBR Gargoyle Destroyer"},
  {"anim2", 8140, 8249, "monsters/LBR Gargoyle Enforcer"},
  {"anim2", 8250, 8344, "monsters/Exodus Overseer"},
  {"anim2", 8360, 8564, "monsters/Exodus Minion"},
  {"anim2", 13420, 13484, "monsters/Kirin"},
  {"anim2", 22000, 22064, "monsters/Unicorn"},
  {"anim2", 22065, 22194, "animals/Ridgeback"},
  {"anim2", 22195, 22259, "monsters/Giant Beetle"},
  {"anim2", 22260, 22324, "monsters/Hellsteed"},
  {"anim2", 22325, 22389, "animals/Swamp Dragon 1"},
  {"anim2", 22390, 22454, "animals/Swamp Dragon 2"},


  // anim3.mul: Age of Shadows (still polygonal)
  {"anim3", 15175, 15224, "animals/Turkey"},
  {"anim3", 33000, 33094, "monsters/Crystal Elemental"},
  {"anim3", 33110, 33204, "monsters/Treefellow"},
  {"anim3", 33220, 33314, "monsters/Skittering Hopper"},
  {"anim3", 33330, 33424, "monsters/Devourer of Souls"},
  {"anim3", 33440, 33534, "monsters/Flesh Golem"},
  {"anim3", 33550, 33644, "monsters/Gore Fiend"},
  {"anim3", 33660, 33754, "monsters/Impaler"},
  {"anim3", 33770, 33864, "monsters/Gibberling"},
  {"anim3", 33880, 33974, "monsters/Bone Demon"},
  {"anim3", 33990, 34084, "monsters/Patchwork Skeleton"},
  {"anim3", 34100, 34194, "monsters/Wailing Banshee"},
  {"anim3", 34210, 34304, "monsters/Shadow Knight"},
  {"anim3", 34320, 34414, "monsters/Abyssmal Horror"},
  {"anim3", 34430, 34524, "monsters/Darknight Creeper"},
  {"anim3", 34540, 34634, "monsters/Ravager"},
  {"anim3", 34650, 34744, "monsters/Flesh Renderer"},
  {"anim3", 34760, 34854, "monsters/Wanderer of the Void"},
  {"anim3", 34870, 34979, "monsters/Vampire Bat"},
  {"anim3", 34980, 35074, "monsters/Demon Knight"},
  {"anim3", 35090, 35184, "monsters/Mound of Maggots"},
  {"anim3", 127750, 129849, "player/weapons3"},

  // anim4.mul: Samurai Empire (more lowpoly models)
  {"anim4", 110, 204, "monsters/Kappa"},
  {"anim4", 220, 314, "monsters/Oni"},
  {"anim4", 330, 424, "monsters/Deathwatch Beetle"},
  {"anim4", 440, 534, "monsters/Rune Beetle"},
  {"anim4", 550, 644, "monsters/Yomotsu Warrior"},
  {"anim4", 660, 754, "monsters/Fan Dancer"},
  {"anim4", 770, 864, "monsters/Yamandon"},
  {"anim4", 880, 974, "monsters/Tsuki Wolf"},
  {"anim4", 990, 1084, "monsters/Revenant Lion"},
  {"anim4", 1100, 1194, "monsters/Lady of the Snow"},
  {"anim4", 1210, 1304, "monsters/Yomotsu Priest"},
  {"anim4", 1320, 1414, "monsters/Yomotsu Elder"},
  {"anim4", 1430, 1524, "monsters/Kaze Kemono"},
  {"anim4", 1540, 1634, "monsters/Rai-ju"},
  {"anim4", 22065, 22129, "monsters/Hiryu"},
  {"anim4", 22130, 22194, "monsters/Bake Kitsune"},
  {"anim4", 22195, 22259, "animals/Gaman"},
  {"anim4", 22260, 22324, "animals/Crane"},
  {"anim4", 26750, 84699, "player/japan-themed"},

  // anim5,mul: Mondain's Legacy (even worse)
  {"anim5", 1650, 1744, "monsters/Chief Paroxysmus"},
  {"anim5", 1760, 1854, "monsters/Dread Horn"},
  {"anim5", 1870, 1964, "monsters/Lady Melisande"},
  {"anim5", 1980, 2074, "monsters/Interred Grizzle"},
  {"anim5", 2200, 2294, "monsters/Shimmering Effusion"},
  {"anim5", 2310, 2404, "monsters/Minotaur Tormented"},
  {"anim5", 2420, 2514, "monsters/Minotaur"},
  {"anim5", 2530, 2624, "monsters/Changeling"},
  {"anim5", 2640, 2734, "monsters/Hydra"},
  {"anim5", 2750, 2844, "monsters/Dryad"},
  {"anim5", 2860, 2954, "monsters/Troglodyte"},
  {"anim5", 3190, 3284, "monsters/Satyr"},
  {"anim5", 3410, 3504, "monsters/Fetid Essence"},
  {"anim5", 3850, 3944, "monsters/Minotaur Captain"},
  {"anim5", 3960, 4054, "monsters/Minotaur Scout"},
  {"anim5", 4070, 4164, "monsters/treeman"},
  {"anim5", 4180, 4189, "npc/ml01"},
  {"anim5", 4290, 4384, "npc/ml02"},
  {"anim5", 11210, 11274, "monsters/Reptalon"},
  {"anim5", 21670, 21759, "monsters/ML Dragon Red"},
  {"anim5", 21780, 21869, "monsters/ML Dragon Blue"},
  {"anim5", 22325, 22389, "monsters/Cu Sidhe"},
  {"anim5", 22390, 22454, "animals/Squirrel"},
  {"anim5", 22455, 22519, "animals/Ferret"},
  {"anim5", 22520, 22574, "animals/MD Bird 1"},
  {"anim5", 22585, 22639, "animals/MD Bird 2"},
  {"anim5", 22650, 22714, "animals/MD Horse"},
  {"anim5", 28700, 79274, "player/MD wearables"},


  {0, 0, 0, 0}
};

// Note: we can use Anim256.bin and Anim1024.bin to get precise indices
static void unpackAnim(char*Mul, int L, u1*Idx, int IL) {
  char Tmp[1024], Name[256], MULName[128];
  int I, J, C, X, Y, CX, CY, Off, Wide, High;
  u4 U;
  u1*M,*Q,*Z;
  u4*O;
  int NIdx = IL/sizeof(idx);
  pic*P;
  spr*S;
  animHeader*H;
  animFrame*F;
  idx*D = (idx*)Idx;

  pathParts(0, MULName, 0, Mul);
  downcase(MULName);

  times(I, NIdx) { // for every animation
    //if (I<34870 || I>34870+20) continue;
    //if (I!=33448) continue;
    if (!D[I].Len || D[I].Len==0xFFFFFFFF) continue;
    if ((u8)D[I].Off+D[I].Len > (u8)L) continue;

    for(J = 0; Names[J].Mul; J++) {
      if (strcmp(Names[J].Mul, MULName)) continue;
      if (I < Names[J].A || I > Names[J].B) continue;

      //unless(!strcmp(Names[J].Name, "monsters/Dragon Black")) goto skip;

      sprintf(Name, "%s/%04d_%08X.gif", Names[J].Name, I, D[I].Off);
      goto found_name;
      //goto skip;
    }

    sprintf(Name, "%04d_%08X.gif", I, D[I].Off);
found_name:

    M = readFile(D[I].Off, D[I].Len, Mul);
    H = (animHeader*)M;
    O = (u4*)(H+1);

    printf("  Extracting: %s\n", Name);
    printf("    Off=%08x Len=%08x NFrames=%d\n", D[I].Off, D[I].Len, H->NFrames);

    S = sprNew();
    S->NAnis = 1;
    S->Anis = ns(ani, S->NAnis);
    S->Anis[0].NFacs = 1;
    S->Anis[0].Facs = ns(fac,S->Anis[0].NFacs);
    S->Anis[0].Facs[0].NPics = H->NFrames;
    S->Anis[0].Facs[0].Pics = ns(pic*, S->Anis[0].Facs[0].NPics);
    S->ColorKey = 0;

    S->Palette = ns(u1,4*256);
    times(J, 256) {
      int A, R, G, B;
      fromA1R5G5B5(A, R, G, B, H->Pal[J]);
      S->Palette[J*4+0] = R;
      S->Palette[J*4+1] = G;
      S->Palette[J*4+2] = B;
      S->Palette[J*4+3] = 0;
    }

    Wide = 8;
    High = 8;
    times(J, H->NFrames) { // determine width and height
      Off = O[J] + 256*2;
      F = (animFrame*)(M+Off);
      Q = (u1*)(F+1);

      if (!F->W || !F->H) continue;
      //Wide = max(F->W,Wide);
      //High = max(F->H,High);
      while(*(u4*)Q != 0x7FFF7FFF) {
        U =*(u4*)Q; Q+=4;
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
    }

    times(J, H->NFrames) {
      Off = O[J] + 256*2;
      F = (animFrame*)(M+Off);
      Q = (u1*)(F+1);

      P = S->Anis[0].Facs[0].Pics[J] = picNew(Wide*2, High*2, 8);
      P->SharedPalette = 1;
      free(P->P);
      P->P = S->Palette;
      P->K = 0;

      //printf("    %dx%d:%d,%d\n", F->W, F->H, F->X, F->Y);

      CX = P->W/2;
      CY = P->H/2;
      if (!F->W || !F->H) continue;
      while(*(u4*)Q != 0x7FFF7FFF) {
        U =*(u4*)Q; Q+=4;
        C = U&((1<<12)-1);
        Y = (U>>12)&((1<<10)-1);
        X = (U>>22)&((1<<10)-1);

        X=(X^0x200)-0x200;
        Y=(Y^0x200)-0x200;

        //printf("      %d,%d:%d\n", X, Y, C);
        while (C--) picPut(P, CX + X++, CY + Y, *Q++);
      }
      //pngSave("out/1.png", P);
    }

    sprintf(Tmp, "%s/%s", Out, Name);
    gifSave(Tmp, S);
    sprDel(S);
    free(M);

  skip:;
    //abort();
  }
}

void unpackStatic(char*Mul, int L, u1*Idx, int IL) {
  char Tmp[1024], Name[256], MULName[128];
  int I, J, C, X, Y, CX, CY, Off, Wide, High, BW, BH;
  u4 U;
  u1*M,*Q,*Z;
  u4*O;
  int NIdx = IL/sizeof(idx);
  pic*P;
  spr*S;
  animHeader*H;
  animFrame*F;
  idx*D = (idx*)Idx;

  pathParts(0, MULName, 0, Mul);
  downcase(MULName);

  times(I, NIdx) {
    //if (I<34870 || I>34870+20) continue;
    //if (I!=33448) continue;
    if (!D[I].Len || D[I].Len==0xFFFFFFFF) continue;
    if ((u8)D[I].Off+D[I].Len > (u8)L) continue;
    //printf( "%08X %08X\n", D[I].Off, D[I].Len);
  }
}

void unpackArt(char*Mul, int L, u1*Idx, int IL) {
  char Tmp[1024], Path[1024], Name[32], OutName[256];
  tdLandBlk*TL;
  tdStaticBlk*TS;
  int R, G, B, A;
  int I, J, C, X, Y, CX, CY, Off, Wide, High, BW, BH;
  u4 U;
  u1*M,*Q;
  u2*O,*Z;
  int NIdx = IL/sizeof(idx);
  pic*P;
  spr*S;
  idx*D = (idx*)Idx;

  loadTileData();

  memset(Name, 0, 32);

  times(I, NIdx) {
    //if (I<34870 || I>34870+20) continue;
    //if (I!=10147) continue;
    if (!D[I].Len || D[I].Len==0xFFFFFFFF) continue;
    if ((u8)D[I].Off+D[I].Len > (u8)L) continue;

    //printf( "  %06d: %08X %08X\n", I, D[I].Off, D[I].Len);

    Path[0] = 0;

    Q = M = readFile(D[I].Off, D[I].Len, Mul);

    U = *(u4*)Q; // try guessing type
    if (U>0xFFFF || U==0) { //tile
      //goto skip;
      TL = (tdLandBlk*)TileData[I];
      memcpy(Name, TL->Name, 20);
      if (Name[0]) sprintf(OutName, "%05d_%s.png", I, Name);
      else sprintf(OutName, "%05d_misc.png", I);
      for (J = 0; Flags[J].Name; J++) {
         unless (TL->Flags&Flags[J].Flag) continue;
         if(Path[0]) strcat(Path, "/");
         strcat(Path, Flags[J].Name);
      }
      unless (Path[0]) strcat(Path, "Misc");
      strcat(Path, "/");
      strcat(Path, OutName);

      printf("  Extracting: %s\n", Path);

      P = picNew(44, 44, 32);
      times(Y,44) times(X,44) picPut32(P,X,Y,R8G8B8A8(0,0,0,0));
      X = 22;
      C = 2;
      for(Y=0; Y<22; Y++, C+=2) {
        X--;
        times(J,C) {
          fromA1R5G5B5(A, R, G, B, *(u2*)Q);
          picPut32(P,X+J,Y,R8G8B8A8(R,G,B,0xFF));
          Q+=2;
        }
      }
      for(C-=2; Y<44; X++, Y++, C-=2) {
        times(J,C) {
          fromA1R5G5B5(A, R, G, B, *(u2*)Q);
          picPut32(P,X+J,Y,R8G8B8A8(R,G,B,0xFF));
          Q+=2;
        }
      }
      sprintf(Tmp, "%s/tileset/%s", Out, Path);
      pngSave(Tmp, P);
      picDel(P);
    } else { //sprite
      //goto skip;
      TS = (tdStaticBlk*)TileData[I];
      memcpy(Name, TS->Name, 20);
      if (Name[0]) sprintf(OutName, "%05d_%s", I, Name);
      else sprintf(OutName, "%05d_misc", I);
      for (J = 0; Flags[J].Name; J++) {
         unless (TS->Flags&Flags[J].Flag) continue;
         if(Path[0]) strcat(Path, "/");
         strcat(Path, Flags[J].Name);
      }
      unless (Path[0]) strcat(Path, "Misc");
      strcat(Path, "/");
      strcat(Path, OutName);

      printf("  Extracting: %s\n", Path);

      Q += 4; // skip size
      Wide = *(u2*)Q; Q+=2;
      High = *(u2*)Q; Q+=2;
      if (Wide == 0 || Wide >= 1024) goto skip;
      if (High == 0 || High >= 1024) goto skip;
      P = picNew(Wide, High, 32);
      times(Y,Wide) times(X,High) picPut32(P,X,Y,R8G8B8A8(0,0,0,0));
      O = (u2*)Q;
      times(Y, High) {
        X = 0;
        Z = O + High + O[Y];
        while(Z[0]||Z[1]) {
          X += *Z++;
          C = *Z++;
          while (C--) {
            fromA1R5G5B5(A, R, G, B, *Z);
            picPut32(P,X++,Y,R8G8B8A8(R,G,B,0xFF));
            Z++;
          }
        }
      }
      sprintf(Tmp, "%s/static/%s.png", Out, Path);
      pngSave(Tmp, P);
      picDel(P);
    }
skip:
    free(M);
  }
}

void unpackSound(char*Mul, int L, u1*Idx, int IL) {
  char Tmp[1024], Name[32];
  int I;
  u1*M;
  int NIdx = IL/sizeof(idx);
  idx*D = (idx*)Idx;

  memset(Name, 0, 32);
  times(I, NIdx) {
    if (!D[I].Len || D[I].Len==0xFFFFFFFF) continue;
    if ((u8)D[I].Off+D[I].Len > (u8)L) continue;

    M = readFile(D[I].Off, D[I].Len, Mul);
    memcpy(Name, M, 20);
    printf("  Extracting: %s\n", Name);
    sprintf(Tmp, "%s/%05d_%s", Out, I, Name);
    wavSave(Tmp, M+40, D[I].Len-4, 16, 1, 22050);
    free(M);
  }
}

void unpackTexture(char*Mul, int L, u1*Idx, int IL) {
  char Tmp[1024], Name[512];
  int I, J, X, Y, Wide, High, R,G,B,A;
  pic *P;
  u1*M;
  u2 *Z;
  int NIdx = IL/sizeof(idx);
  idx*D = (idx*)Idx;

  times(I, NIdx) {
    if (!D[I].Len || D[I].Len==0xFFFFFFFF) continue;
    if ((u8)D[I].Off+D[I].Len > (u8)L) continue;

    M = readFile(D[I].Off, D[I].Len, Mul);

    sprintf(Name, "%05d_%08X", I, D[I].Off);
    printf("  Extracting: %s\n", Name);

    for(J=0;;J++) {
      if (J>1024) {
        printf("    invalid texure size\n");
        abort();
      }
      if (D[I].Len == J*J*2)  {
        Wide=J;
        High=J;
        break;
      }
    }

    P = picNew(Wide, High, 24);
    times(Y,High) times (X, Wide) {
      fromA1R5G5B5(A, R, G, B, ((u2*)M)[Y*Wide+X]);
      picPut24(P,X,Y,R8G8B8(R,G,B));
    }

    sprintf(Tmp, "%s/%dx%d/%s.png", Out, Wide, High, Name);
    pngSave(Tmp, P);

    picDel(P);
    free(M);
  }
}

void unpackGUMP(char*Mul, int L, u1*Idx, int IL) {
  char Tmp[1024], Name[512];
  int I, J, C, Run, X, Y, Wide, High, Val, R,G,B,A;
  pic *P;
  u1*M;
  u4 *O, *Q;
  u4 U;
  int NIdx = IL/sizeof(idx);
  idx*D = (idx*)Idx;

  times(I, NIdx) {
    if (!D[I].Len || D[I].Len==0xFFFFFFFF) continue;
    if ((u8)D[I].Off+D[I].Len > (u8)L) continue;

    M = readFile(D[I].Off, D[I].Len, Mul);

    sprintf(Name, "%05d_%08X", I, D[I].Off);
    printf("  Extracting: %s\n", Name);

    Wide = D[I].Info>>16;
    High = D[I].Info&0xFFFF;

    P = picNew(Wide, High, 24);
    O = (u4*)M;
    times(Y,High) {
      if (Y < High-1) C = O[Y+1]-O[Y];
      else C = D[I].Len/4-O[Y];
      Q = (u4*)M + O[Y];
      X = 0;
      while(C--) {
        U = *Q++;
        Val = U&0xFFFF;
        Run = U>>16;
        while(Run--) {
          fromA1R5G5B5(A, R, G, B, Val);
          picPut24(P,X++,Y,R8G8B8(R,G,B));
        }
      }
    }

    sprintf(Tmp, "%s/%s.png", Out, Name);
    pngSave(Tmp, P);

    picDel(P);
    free(M);
  }
}



static struct {char*Mul,*Idx; void*unpacker;} Muls[] = {
  {"anim.mul", "anim.idx", unpackAnim},
  {"anim1.mul", "anim1.idx", unpackAnim},
  {"anim2.mul", "anim2.idx", unpackAnim},
  {"anim3.mul", "anim3.idx", unpackAnim},
  {"anim4.mul", "anim4.idx", unpackAnim},
  {"anim5.mul", "anim5.idx", unpackAnim},
  {"art.mul", "artidx.mul", unpackArt},
  {"gumpart.mul", "gumpidx.mul", unpackGUMP},
  //{"multi.mul", "multi.idx", 0},
  {"sound.mul", "soundidx.mul", unpackSound},
  {"texmaps.mul", "texidx.mul", unpackTexture},

  // facet??.mul are new format for statics?.mul
  // see uokr_facet_reader_source.rar for specs
  /*{"facet00.mul", 0, unpackFacet},
  {"facet01.mul", 0, unpackFacet},
  {"facet02.mul", 0, unpackFacet},
  {"facet03.mul", 0, unpackFacet},
  {"facet04.mul", 0, unpackFacet},
  {"facet05.mul", 0, unpackFacet},*/

  /*{"statics0.mul", "staidx0.mul", unpackStatic},
  {"statics1.mul", "staidx1.mul", unpackStatic},
  {"statics2.mul", "staidx2.mul", unpackStatic},
  {"statics3.mul", "staidx3.mul", unpackStatic},
  {"statics4.mul", "staidx4.mul", unpackStatic},
  {"statics5.mul", "staidx5.mul", unpackStatic},
  {"statics0x.mul", "staidx0x.mul", unpackStatic},*/
  {0,0}
};


static void mulDecompile(char*Output, char*Input) {
  char Tmp[1024], Name[256], Ext[16],*P;
  int I, J, IL, L = fileSize(Input);
  u1*M,*Idx;

  Out = Output;

  pathParts(UODir, Name, 0, Input);
  downcase(Name);
  sprintf(Tmp, "%s.mul", Name);

  //loadCliloc();
  //abort();

  for (I=0; Muls[I].Mul; I++) if (!strcmp(Tmp, Muls[I].Mul)) {
    unless(Muls[I].unpacker) goto fail;
    IL = 0;
    Idx = 0;
    if (Muls[I].Idx) {
      sprintf(Tmp, "%s/%s", UODir, Muls[I].Idx);
      unless (fileExist(Tmp)) {
        printf("  cant open %s\n", Tmp);
        abort();
      }
      IL = fileSize(Tmp);
      Idx = readFile(0, IL, Tmp);
    }
    ((void (*)(char*, int, u1*, int))Muls[I].unpacker)(Input, L, Idx, IL);
    return;
  }
fail:
  printf("  cant handle this mul\n"
         "  try art.mul, anim.mul, gumpsart.mul, sound.mul or texmaps.mul\n");
  abort();
}

int mulInit(format*F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "mul";
  F->Description = "Ultima Online Archive";
  F->Decompile = mulDecompile;
  return 1;
}

