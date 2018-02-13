//Credits go to SNV

#include "common.h"

//AFAIK, it uses LZ77 (at bit level).
//Unpacker could be found by searching references to "CMDS"
typedef struct {
  u1 Id[4]; //CMDS
  u4 ULen; //Width*Height + 16 (size image of header) + 256*2 (16-bit color map)
  u4 Len; // compressed length
  u1 Type[4];
  //Data[Len];
  //Crud[FileSize-16-Len]; // no idea what is that
} __attribute__ ((packed)) header;



//file: 0009_003D60.NTC8
//ROM: 0x178D360
//what: unknown

//file: 0010_005E00.NTC8
//ROM: 0x178F400
//what: title (after Game Arts and JVM logos)

//file: 0011_00E7E0.NTC8
//ROM: 0x1797DE0
//what: title (after Game Arts and JVM logos)

//file: 0012_018040.NTC8
//ROM: 0x17A1640
//what: title (after Game Arts and JVM logos)

//file: 0013_01A8E0.NTC8
//ROM: 0x17A3EE0
//what: unknown

//file: 0014_01D100.NTC8
//ROM: 0x17A6700
//what: ubisoft logo (after licensed by nintendo)
//info:
//unpacked_size: 0xC210 = 0x100*0xC0 (pixels) + 0x100*2 (pal) + 0x10 (header)
//43 4F 44 53  10 C2 00 00  00 38 00 00  4E 54 43 38
//00 03 01 00  00 50 1F 00  A5 14 A5 18  A5 18 A6 05

//file: 0015_020E20.NTC8
//ROM: 0x17AA420
//what: lead character design (after ubisoft)

//file: 0016_023EE0.NTC8
//ROM: 0x17AD4E0
//what: game arts logo (after ubisoft lead character design)

//file: 0017_0263E0.NTC8
//ROM: 0x17AF9E0
//what: JVM / japan art media (after game arts logo)

//file: 0018_028020.NTC8
//ROM: 0x17B1620
//what: first screen (licensed by nintendo)

//file: 0019_02A640.CLT8
//ROM: 0x17B3CE0
//what: title (after Game Arts and JVM logos)

//file: 0020_02A6E0.NTC8
//ROM: 0x17B3CE0
//what: title (after Game Arts and JVM logos)

//desmume: MMU.cpp:1868: void DmaController::doCopy(): Assertion `false' failed.
//for i in desmume-screenshot-*.png; do crop 0 0 256 192 $i $i; done



//NTC4 - 4-bit tileset
//NTC8 - 8-bit tileset
//CLT4 - 4-bit color table
//CLT8 - 8-bit color table
static void lgDecompile(char *Output, char *Input) {
  char Tmp[1024], Type[32], Name[256];
  int I, J, Off, Len;
  int L = fileSize(Input);
  u1 *M = readFile(0, L, Input);
  int NFiles = *(u4*)M;
  u4 *O = (u4*)(M+4);
  u1 *Q, *Z;
  header *H;

  times (I, NFiles) {
    Off = O[I]*32;
    Len = O[I+1]*32 - Off; // last entry holds filesize
    H = (header*)(M+Off);

    memcpy(Type, H->Type, 4);
    for (J=0; J<4 && ('A'<=Type[J] && Type[J]<='Z' || '0'<=Type[J] && Type[J]<='9'); J++);
    Type[J] = 0;
    if (*Type) sprintf(Name, "%04d_%06X.%s", I, Off, Type);
    else sprintf(Name, "%04d_%06X.%X", I, Off, *(u4*)H->Type);

    unless (I == 14) continue;
    unless (!memcmp(H->Type, "NTC8", 4)) continue;

    printf("  Extracting: %s\n", Name);
    printf("    %08X/%08X\n", H->Len, H->ULen);
    sprintf(Tmp, "%s/%s", Output, Name);

    Q = M+Off;
    writeFile(0, Len, Tmp, Q);
  }
}

int lgInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "lg";
  F->Description = "Lunar: Genesis / Dragon Song (use on *.dat files)";
  F->Decompile = lgDecompile;
  return 1;
}


