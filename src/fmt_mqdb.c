//Credits go to Eugene Popov, author of Open D2 project.

#include "common.h"

typedef struct {
  u1 Id[4];
  u4 U1;
  u4 U2;
  u4 U3;
  u4 U4;
  u4 U5;
  u4 Size; // size of complete archive file
} __attribute__ ((packed)) header;

typedef struct {
  u4 Num;
  u1 Name[12];
} __attribute__ ((packed)) entry;

typedef struct {
  u1 Name[256];
  u4 Num;
} __attribute__ ((packed)) entry256;


typedef struct {
  u1 Id[4]; //MQRC
  u4 U1;
  u4 Num;
  u4 Len2;
  u4 Len;
  u4 U2;
  u4 U3;
} __attribute__ ((packed)) rec;


static void mqdbDecompile(char *Output, char *Input) {
  char Tmp[1024], Dir[256], Name[256], Ext[32];
  int I, J, NFiles;
  u4 Id;
  int L = fileSize(Input);
  u1 *M = readFile(0, L, Input);
  u1 *Q, *Z;
  header *H = (header*)M;
  rec *R = (rec*)(H+1);
  rec **Rs = ns(rec*, 0x10000);
  rec *WDB=0, *FF=0, *DBI=0;
  int NRecs = 0;

  unless (!strncmp(H->Id, "MQDB", 4)) {
    printf("  Not an MQDB file.\n");
    abort();
  }

  // proces MQ records
  while ((u1*)R < M+L && !strncmp(R->Id, "MQRC", 4)) {
    Q = (u1*)(R+1);
    //printf("  N=%d L=%d\n", R->Num, R->Len);
    if (R->Num == 1 && !strncmp(Q, "WDB", 3)) WDB = R; //wave db
    if (R->Num == 2) FF = R;
    if (R->Num == 3) DBI = R;
    Rs[NRecs++] = R;
    R = (rec*)(Q + R->Len);
  }

  //printf("  NRecs=%d\n", NRecs);
  times (J, NRecs) {
    R = Rs[J];

    if (R == WDB || R == FF || R->Num == 0) goto skip;

    Q = (u1*)(R+1);


    Dir[0] = 0;
    Name[0] = 0;

    if (FF) {
      entry256 *E = (entry256*)((u1*)(FF+1)+4);
      NFiles = *(u4*)(FF+1);

      times (I, NFiles) {
        if (E[I].Num != R->Num) continue;
        strcpy(Name, E[I].Name);
        goto found_name;
      }
    }

    if (DBI && !FF) {
      entry *E = (entry*)((u1*)(DBI+1)+12);
      NFiles = (DBI->Len-12)/sizeof(entry);

      sprintf(Dir, "%s/", (u1*)(DBI+1));

      times (I, NFiles) {
        if (E[I].Num != R->Num) continue;
        strcpy(Name, E[I].Name);
        goto found_name;
      }
    }

    if (WDB) {
      entry *E = (entry*)((u1*)(WDB+1)+8);
      NFiles = (WDB->Len-8)/sizeof(entry);

      times (I, NFiles) {
        if (E[I].Num != R->Num) continue;
        strcpy(Name, E[I].Name);
        goto found_name;
      }
    }

    if (!Name[0]) sprintf(Name, "%08d", R->Num);

found_name:
    printf("  Extracting: %s%s\n", Dir, Name);
    Ext[0] = 0;
    if (!strncmp(Q, "RIFF", 4)) strcpy(Ext, ".wav");

    sprintf(Tmp, "%s/%s%s%s", Output, Dir, Name, Ext);
    writeFile(0, R->Len, Tmp, Q);
skip:;
  }
}


int mqdbInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "mqdb";
  F->Description = "Disciples: Sacred Lands (use on DBI, FF and WDB)";
  F->Decompile = mqdbDecompile;
  return 1;
}

