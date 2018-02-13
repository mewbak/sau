// Blizzard MPQ archives

extern "C" {
#include "common.h"
}

#include "storm/StormLib.h"

extern "C" {


static char *normalizePath(char *s) {
	int i;
	for(i = 0; s[i]; i++) {
		s[i] = tolower(s[i]);
		if(s[i]=='\\') s[i]='/';
	}
	return s;
}

static void extractFile(HANDLE mpq, char *OutDir, char *Name) {
  //HANDLE H = 0;
  char Tmp[1024];

  printf("  Extracting: %s\n", Name);
  sprintf(Tmp, "%s/%s", OutDir, Name);
  normalizePath(Tmp);

  writeFile(0, 0, Tmp, 0); // this creates filepath
  SFileExtractFile(mpq, Name, Tmp);

#if 0
	FILE out = fopen(outName, "rb")
	if(!out) {
		printf("    failed to create file.\n");
		goto end;
	}
	SFileOpenFileEx(mpq, inName, SFILE_OPEN_FROM_MPQ, &in);
	SFileCloseFile(in);
#endif
}

char MPQName[256];

void mpqDecompile(char *OutDir, char *FileName) {
  HANDLE mpq = 0;
  HANDLE search;
  SFILE_FIND_DATA ffd;
  char ListPath[1024];
  char Ext[64];
  char *ListFile = (char*)"mpq_default.txt";

  pathParts(0, MPQName, Ext, FileName);
  downcase(MPQName);
  downcase(Ext);

  if (!strcmp(MPQName, "war2dat")) {
    ListFile = (char*)"mpq_war2.txt";
    sprintf(ListPath, "%s/names/%s", ProgDir, ListFile);
    mpqDecompileOld(OutDir, FileName, ListPath);
    return;
  } else if (!strcmp(MPQName, "diabdat")
          || !strcmp(MPQName, "hellfire")
          || !strcmp(MPQName, "hfmonk")
          || !strcmp(MPQName, "hfmusic")
          || !strcmp(MPQName, "hfvoice")) {
    ListFile = (char*)"mpq_diablo.txt";
    sprintf(ListPath, "%s/names/%s", ProgDir, ListFile);
    mpqDecompileOld(OutDir, FileName, ListPath);
    return;
  } else if (!strcmp(MPQName, "broodat")
          || !strcmp(MPQName, "patch_rt")
          || !strcmp(MPQName, "starcraft")
          || !strcmp(MPQName, "stardat")
          || !strcmp(MPQName, "install")
          || !strcmp(Ext, "scm")) {
    ListFile = (char*)"mpq_starcraft.txt";
    sprintf(ListPath, "%s/names/%s", ProgDir, ListFile);
    //mpqDecompileOld(OutDir, FileName, ListPath);
    //return;
  } else if (!strcmp(MPQName, "d2char")
          || !strcmp(MPQName, "d2data")
          || !strcmp(MPQName, "d2exp")
          || !strcmp(MPQName, "d2music")
          || !strcmp(MPQName, "d2sfx")
          || !strcmp(MPQName, "d2speech")
          || !strcmp(MPQName, "d2xmusic")
          || !strcmp(MPQName, "d2xtalk")
          || !strcmp(MPQName, "d2xvideo")
          || !strcmp(MPQName, "patch_d2")
          || !strcmp(MPQName, "setup")) {
    ListFile = (char*)"mpq_diablo2.txt";
  } else if (!strcmp(MPQName, "pic")
          || !strcmp(MPQName, "gs")
          || !strcmp(MPQName, "imp")
          || !strcmp(MPQName, "sndfx")
          || !strcmp(MPQName, "special")) {
    ListFile = (char*)"mpq_lom.txt";
  }

  sprintf(ListPath, "%s/names/%s", ProgDir, ListFile);

  SFileOpenArchive(FileName, 0, MPQ_OPEN_READ_ONLY, &mpq);
  if (!mpq) {
    printf("  cant open MPQ (Error=%d)\n", GetLastError());
    abort();
  }

  if (SFileAddListFile(mpq, ListPath) != ERROR_SUCCESS) {
    printf("  cant load %s\n", ListPath);
    abort();
  }

  search = SFileFindFirstFile(mpq, "*", &ffd, 0);
  while (search) {
    extractFile(mpq, OutDir, ffd.cFileName);
    if(!SFileFindNextFile(search, &ffd)) search = 0;
  }

  SFileCloseArchive(mpq);
}

void mpqCompile(char *FileName, fileList *FL) {
  char Tmp[2048], Name[2048];
  int I, Sz;
  char *P;
  u1 *D;
  HANDLE mpq = 0;
  HANDLE file = 0;
  writeFile(0, 0, FileName, FileName);
  if(!SFileCreateArchive(FileName, 0, FL->Size, &mpq)) {
    printf("  Failed to create MPQ (Error=%d)\n", GetLastError());
    abort();
  }
  times (I, FL->Size) {
    sprintf(Tmp, "%s/%s", FL->Root, FL->Names[I]);
    strcpy(Name, FL->Names[I]);
    for(P = Name; *P; P++) if(*P == '/') *P = '\\';
    printf("  Adding: %s\n", Name);
    SFileAddFileEx(mpq, Tmp, Name, MPQ_FILE_IMPLODE, 0, 0);

    //Sz = fileSize(FL->Names[I]);
    //SFileCreateFile(mpq, Tmp, 0, FL->Names[I], 0, 0, &file);
    //D = readFile(0, 0, FL->Names[I]);
    //SFileWriteFile(file, D, Sz, MPQ_COMPRESSION_ZLIB);
  }
  SFileCloseArchive(mpq);
}


int mpqInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = (char*)"mpq";
  F->Description = (char*)"Blizzard MPQ archives (Warcraft 2, Diablo, Starcraft, Lords of Magic)";
  F->Compile = (void*)mpqCompile;
  F->Decompile = (void*)mpqDecompile;
  return 1;
}

}

