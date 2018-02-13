#include "zlib/zlib.h"
#include "common.h"


void hd(u1 *P, int S) {
  int X, Y, I;
  times(Y, (S+15)/16) {
    printf("%04X |", Y*16);
    times(X, 16) {
      I = Y*16 + X;
      if ((I&0xf) == 8) printf(" ");
      if (I<S) printf(" %02X", P[I]);
      else printf("   ");
    }
    printf(" |");
    times(X, 16) {
      I = Y*16 + X;
      if (I<S && isprint(P[I]) && P[I]!='\n' && P[I]!='\t') printf("%c", P[I]);
      else printf(" ");
    }
    printf("\n");
  }
}

char *downcase(char *t) {
  char *s = t;
  while(*s) {if(isupper(*s)) *s = tolower(*s); s++;}
  return t;
}


char *upcase(char *t) {
  char *s = t;
  while(*s) {if(islower(*s)) *s = toupper(*s); s++;}
  return t;
}

void pathParts(char *Dir, char *Name, char *Ext, char *Path) {
  char *P, *Q;
  int L;

  if ((P = strrchr(Path, '/'))) {
    if (Dir) {
      L = P-Path;
      strncpy(Dir, Path, L);
      Dir[L] = 0;
    }
    P++;
  } else {
    if (Dir) Dir[0] = 0;
    P = Path;
  }
  if ((Q = strrchr(P, '.'))) {
    if (Ext) strcpy(Ext, Q+1);
    if (Name) {
       L = Q-P;
       strncpy(Name, P, L);
       Name[L] = 0;
    }
  } else {
    if (Name) strcpy(Name, P);
    if (Ext) Ext[0] = 0;
  }
}


int fileExist(char *File) {
  struct stat S;
  if(stat(File, &S) == 0) return 1;
  return 0;
}

int folderP(char *Name) {
  DIR *Dir;
  struct dirent *Ent;
  if(!(Dir = opendir(Name))) return 0;
  closedir(Dir);
  return 1;
}

int fileP(char *Name) {
  FILE *F;
  if (folderP(Name)) return 0;
  F = fopen(Name, "r");
  unless (F) return 0;
  fclose (F);
  return 1;
}

int fileSize(char *File) {
  int S;
  FILE *F = fopen(File, "r");
  if(!F) return 0;
  fseek(F, 0, SEEK_END);
  S = ftell(F);
  fclose(F);
  return S;
}

char *loadFile(char *name) {
	int sz;
	char *r;
	FILE *f = fopen(name, "r");
	if(!f) return malloc(0);
	fseek(f, 0, SEEK_END);
	sz = ftell(f);
	r = malloc(sz);
	fseek(f, 0, SEEK_SET);
	fread(r, 1, sz, f);
	fclose(f);
	return r;
}

void removeFile(char *fn) {
	char b[1024];
	sprintf(b, "rm -f '%s'", fn);
	system(b);
}

void tmpName(char *rn, char *pat) {
	FILE *f;
	srand(time(0));
	for(;;) {
		sprintf(rn, pat, rand());
		f = fopen(rn, "r");
		if(!f) return;
		fclose(f);
	}
}

char *shell(char *fmt, ...) {
	char b[1024], c[1024], rn[128], *r;
	va_list args;
	va_start(args, fmt);
	vsprintf(b, fmt, args);
	va_end(args);
	tmpName(rn, "/tmp/fc_%x.tmp");
	sprintf(c, "%s > %s", b, rn);
	system(c);
	r = loadFile(rn);
	removeFile(rn);
	return r;
}

#ifdef WIN32
#define m_mkdir(X) mkdir(X)
#else
#define m_mkdir(X) mkdir(X, S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH)
#endif

void makePath(char *Path) {
  char T[1024], *P;
  strcpy(T, Path);
  P = T;
  while((P = strchr(P+1, '/'))) {
    *P = 0;
    // create a directory named with read/write/search permissions for
    // owner and group, and with read/search permissions for others.
    unless (fileExist(T)) m_mkdir(T);
    *P = '/';
  }
  unless (fileExist(T)) m_mkdir(T);
  //free(shell("mkdir -p '%s'", Path));
}

void *readFile(u4 Offset, u4 Length, char *FileName) {
   u1 *Buffer;
   FILE *F = fopen(FileName, "rb");
   unless (F) {
     printf("  cant open (%s)\n", FileName);
     abort();
   }
   unless (fseek(F, Offset, SEEK_SET) == 0) {
     printf("  cant seek to %08x (%s)\n", Offset, FileName);
     abort();
   }
   Buffer = ns(u1, Length);
   fread(Buffer, 1, Length, F);
   fclose(F);
   return Buffer;
}


void createPathForFile(char *FileName) {
   char Buffer[4*1024];
   char *Q = 0, *P = FileName;
   while(*P) {
     if(*P == '/') Q = P;
     ++P;
   }
   if(Q) {
      int N = Q - FileName;
      memcpy(Buffer, FileName, N);
      Buffer[N] = 0;
      unless(fileExist(Buffer)) makePath(Buffer);
   }
}

int writeFile(u4 Offset, u4 Length, char *FileName, void *Data) {
   FILE *F;
   createPathForFile(FileName);
   F = fopen(FileName, "wb");
   unless(F) return 0;
   unless(fseek(F, Offset, SEEK_SET) == 0) {
     fclose(F);
     return 0;
   }
   fwrite(Data, 1, Length, F);
   fclose(F);
   return 1;
}




typedef struct Link {char *Name; struct Link *Prev;} Link;

static int cmpstringp(void *p1, void *p2) {
  return strcmp(*(char**)p1, *(char**)p2);
}

fileList *getFileList(char *Root) {
  char Path[2048], Tmp[2048];
  int I;
  fileList *FL = n(fileList);
  DIR *Ds[1024], *D;
  struct dirent *Ent;
  Link *L=0, *M;

  I = 0;
  if (!(Ds[I] = opendir(Root))) {
    printf("Cant read directory `%s`", Root);
    abort();
  }

  FL->Root = strdup(Root);
  FL->Size = 0;
  Path[0] = 0;

again:
  while (Ent = readdir(Ds[I])) {
    if (!strcmp(Ent->d_name, ".") || !strcmp(Ent->d_name, "..")) continue;
    sprintf(Tmp, "%s/%s/%s", Root, Path, Ent->d_name);
    if (!(D = opendir(Tmp))) {
      M = n(Link);
      M->Prev = L;
      sprintf(Tmp, "%s/%s", Path, Ent->d_name);
      M->Name = strdup(Tmp+1);
      L = M;
      FL->Size++;
    } else {
      Ds[++I] = D;
      sprintf(Path, "%s/%s", Path, Ent->d_name);
    }
  }
  closedir(Ds[I]);
  if (I--) {
    *strrchr(Path, '/') = 0;
    goto again;
  }

  FL->Names = ns(char*, FL->Size);
  times (I, FL->Size) {
    FL->Names[I] = L->Name;
    M = L->Prev;
    free(L);
    L = M;
  }

  qsort(FL->Names, FL->Size, sizeof(FL->Names[0]), (void*)cmpstringp);

  return FL;
}

void freeFileList(fileList *FL) {
  int I;
  times(I, FL->Size) if(FL->Names[I]) free(FL->Names[I]);
  if (FL->Names) free(FL->Names);
  if (FL->Root) free(FL->Root);
  free(FL);
}

pic *picNew(int W, int H, int BPP) {
  int I, X,Y;
  pic *P = n(pic);
  P->W = W;
  P->H = H;
  P->B = BPP;
  P->I = (P->W*BPP+7)/8;
  P->S = P->H*P->I;
  P->D = ns(u1, P->S);
  P->K = P->BK = P->SK = -1;
  if (P->B <= 8) {
    P->P = ns(u1,256*4);
    memset(P->P, 0, 256*4);
    times(Y,16) times(X,16) {
      I = (Y*16+X)*4;
      P->P[I+0] = (X<<4)|Y;
      P->P[I+1] = (Y<<4)|X;
      P->P[I+2] = X*Y;
    }
  }
  return P;
}

void picDel(pic *P) {
  if (!P->Proxy) {
    if (P->D) free(P->D);
    if (P->N) free(P->N);
    if (P->B <= 8 && !P->SharedPalette && P->P) free(P->P);
  }
  free(P);
}

pic *picDup(pic *Src) {
  int X, Y;
  pic *P = picNew(Src->W,Src->H,Src->B);
  if (Src->P) memcpy(P->P, Src->P, 256*4);
  picBlt(P,Src,0,0,0,0,0,P->W,P->H);
  P->X = Src->X;
  P->Y = Src->Y;
  return P;
}

pic *picProxy(pic *Src, int X, int Y, int W, int H) {
  pic *P = n(pic);
  memcpy(P, Src, sizeof(pic));
  P->D += Y*Src->I + X*Src->B/8;
  P->Proxy = Src;
  P->X = X;
  P->Y = Y;
  P->W = W;
  P->H = H;
  return P;
}

void picPut(pic *P, int X, int Y, u4 C) {
  if (0 <= X && X<P->W && 0 <= Y && Y<P->H) {
    if (P->Proxy) picPut(P->Proxy, P->X+X, P->Y+Y, C);
    else P->D[Y*P->I + X] = C;
  }
}

void picPut24(pic *P, int X, int Y, u4 C) {
  if (0 <= X && X<P->W && 0 <= Y && Y<P->H) {
    if (P->Proxy) picPut24(P->Proxy, P->X+X, P->Y+Y, C);
    else {
      fromR8G8B8(P->D[Y*P->I+X*3+0],P->D[Y*P->I+X*3+1],P->D[Y*P->I+X*3+2],C);
    }
  }
}

void picPut32(pic *P, int X, int Y, u4 C) {
  if (0 <= X && X<P->W && 0 <= Y && Y<P->H) {
    if (P->Proxy) picPut32(P->Proxy, P->X+X, P->Y+Y, C);
    else ((u4*)P->D)[Y*P->W + X] = C;
  }
}

u4 picGet(pic *P, int X, int Y) {
  if (P->Proxy) return picGet(P->Proxy, P->X+X, P->Y+Y);
  else if (0 <= X && X<P->W && 0 <= Y && Y<P->H) {
    return P->D[Y*P->I + X];
  }
  return P->K;
}

u4 picGet24(pic *P, int X, int Y) {
  if (P->Proxy) return picGet24(P->Proxy, P->X+X, P->Y+Y);
  else if (0 <= X && X<P->W && 0 <= Y && Y<P->H) {
    return R8G8B8(P->D[Y*P->I+X*3+0], P->D[Y*P->I+X*3+1], P->D[Y*P->I+X*3+2]);
  }
  return P->K;
}

u4 picGet32(pic *P, int X, int Y) {
  if (P->Proxy) return picGet32(P->Proxy, P->X+X, P->Y+Y);
  else if (0 <= X && X<P->W && 0 <= Y && Y<P->H) {
    return ((u4*)P->D)[Y*P->W + X];
  }
  return P->K;
}

pic *picClear(pic *P, u4 V) {
  int X, Y;
  if (P->B == 8) {
    times (Y, P->H)
      times (X, P->W)
        picPut(P, X, Y, V);
  } else if (P->B == 24) {
    times (Y, P->H)
      times (X, P->W)
        picPut24(P, X, Y, V);
  } else if (P->B == 32) {
    times (Y, P->H)
      times (X, P->W)
        picPut32(P, X, Y, V);
  } else {
    printf("cant clear %d-bit image\n", P->B);
    abort();
  }
  return P;
}

void picBlt(pic *B, pic *A, int Flags, int DX, int DY
           ,int SX, int SY, int W, int H)
{
  int BX=0, BY=0, XI=1, YI=1, X, Y;
  u4 *T = ns(u4, W*H);

  if (A->B==8) {
    // blocks can overlap, so we use temporary buffer
    times (Y, H) times (X, W) T[Y*W + X] = picGet(A, SX+X, SY+Y);

    if (Flags & PIC_FLIP_X) {
      BX = W-1;
      XI = -1;
    }

    if (Flags & PIC_FLIP_Y) {
      BY = H-1;
      YI = -1;
    }

    for (SY = BY, Y=0; Y < H; SY+=YI, Y++) {
      for (SX = BX, X=0; X < W; SX+=XI, X++) {
        unless (T[SY*W + SX]!=B->K) continue;
        picPut(B, DX+X, DY+Y, T[SY*W + SX]);
      }
    }
    //times (Y, H) times (X, W) picPut(B, DX+X, DY+Y, T[Y*W + X]);

    free(T);
  } else if (A->B==24) {
    // blocks can overlap, so we use temporary buffer

    times (Y, H) times (X, W) T[Y*W + X] = picGet24(A, SX+X, SY+Y);

    if (Flags & PIC_FLIP_X) {
      BX = W-1;
      XI = -1;
    }

    if (Flags & PIC_FLIP_Y) {
      BY = H-1;
      YI = -1;
    }

    for (SY = BY, Y=0; Y < H; SY+=YI, Y++) {
      for (SX = BX, X=0; X < W; SX+=XI, X++) {
        unless (T[SY*W + SX]&0xFF000000) continue;
        picPut24(B, DX+X, DY+Y, T[SY*W + SX]);
      }
    }
    //times (Y, H) times (X, W) picPut(B, DX+X, DY+Y, T[Y*W + X]);

    free(T);
  } else if (A->B==32) {
    // blocks can overlap, so we use temporary buffer

    times (Y, H) times (X, W) T[Y*W + X] = picGet32(A, SX+X, SY+Y);

    if (Flags & PIC_FLIP_X) {
      BX = W-1;
      XI = -1;
    }

    if (Flags & PIC_FLIP_Y) {
      BY = H-1;
      YI = -1;
    }

    for (SY = BY, Y=0; Y < H; SY+=YI, Y++) {
      for (SX = BX, X=0; X < W; SX+=XI, X++) {
        unless (T[SY*W + SX]&0xFF000000) continue;
        picPut32(B, DX+X, DY+Y, T[SY*W + SX]);
      }
    }
    //times (Y, H) times (X, W) picPut(B, DX+X, DY+Y, T[Y*W + X]);

    free(T);
  } else {
    printf("cant blit %d-bit image\n", A->B);
    abort();
  }
}

spr *sprNew() {
  spr *S = n(spr);
  S->ColorKey = -1;
  return S;
}

void sprDel(spr *S) {
  int I, J, K;
  if (S->Palette) free(S->Palette);
  times(I, S->NAnis) {
    times(J, S->Anis[I].NFacs) {
      times(K, S->Anis[I].Facs[J].NPics) {
        unless (S->Anis[I].Facs[J].Pics[K]) continue;
        picDel(S->Anis[I].Facs[J].Pics[K]);
      }
      if (S->Anis[I].Facs[J].Pics) free(S->Anis[I].Facs[J].Pics);
    }
    if (S->Anis[I].Facs) free(S->Anis[I].Facs);
    if (S->Anis[I].Name) free(S->Anis[I].Name);
  }
  if (S->Anis) free(S->Anis);
  free(S);
}



void bytesDel(bytes *B) {
  if (B->D) free(B->D);
  free(B);
}



static char *zerr(int ret) {
  switch (ret) {
  case Z_ERRNO:
      if (ferror(stdin)) return "error reading stdin";
      if (ferror(stdout)) return "error writing stdout";
      break;
  case Z_STREAM_ERROR: return "invalid compression level"; break;
  case Z_DATA_ERROR: return "invalid or incomplete deflate data"; break;
  case Z_MEM_ERROR: return "out of memory"; break;
  case Z_VERSION_ERROR: return "zlib version mismatch!"; break;
  }
  return "unknown error";
}

u1 *inflateBufIE(int OLen, int Len, u1 *In, int IgnoreError) {
  int ret;
  z_stream Z;
  u1 *Out = ns(u1, OLen);
  Z.zalloc = Z_NULL;
  Z.zfree = Z_NULL;
  Z.opaque = Z_NULL;
  Z.avail_in = 0;
  Z.next_in = Z_NULL;
  inflateInit(&Z);
  Z.next_in = In;
  Z.avail_in = Len;
  Z.next_out = Out;
  Z.avail_out = OLen;
  ret = inflate(&Z, Z_NO_FLUSH);
  switch (ret) {
   case Z_NEED_DICT:
    ret = Z_DATA_ERROR;     /* and fall through */
  case Z_DATA_ERROR:
  case Z_MEM_ERROR:
    inflateEnd(&Z);
    free(Out);
    if (IgnoreError) return 0;
    printf("ZLIB error: %s\n", zerr(ret));
    abort();
  }
  inflateEnd(&Z);
  return Out;
}

u1 *inflateBuf(int OLen, int Len, u1 *In) {
  return inflateBufIE(OLen, Len, In, 0);
}




#define WAVE_FORMAT_G723_ADPCM  	0x0014  	/* Antex Electronics Corporation */
#define WAVE_FORMAT_ANTEX_ADPCME 	0x0033 	/* Antex Electronics Corporation */
#define WAVE_FORMAT_G721_ADPCM 	0x0040 	/* Antex Electronics Corporation */
#define WAVE_FORMAT_APTX 	0x0025 	/* Audio Processing Technology */
#define WAVE_FORMAT_AUDIOFILE_AF36 	0x0024 	/* Audiofile, Inc. */
#define WAVE_FORMAT_AUDIOFILE_AF10 	0x0026 	/* Audiofile, Inc. */
#define WAVE_FORMAT_CONTROL_RES_VQLPC 	0x0034 	/* Control Resources Limited */
#define WAVE_FORMAT_CONTROL_RES_CR10 	0x0037 	/* Control Resources Limited */
#define WAVE_FORMAT_CREATIVE_ADPCM 	0x0200 	/* Creative Labs, Inc */
#define WAVE_FORMAT_DOLBY_AC2 	0x0030 	/* Dolby Laboratories */
#define WAVE_FORMAT_DSPGROUP_TRUESPEECH 	0x0022 	/* DSP Group, Inc */
#define WAVE_FORMAT_DIGISTD 	0x0015 	/* DSP Solutions, Inc. */
#define WAVE_FORMAT_DIGIFIX 	0x0016 	/* DSP Solutions, Inc. */
#define WAVE_FORMAT_DIGIREAL 	0x0035 	/* DSP Solutions, Inc. */
#define WAVE_FORMAT_DIGIADPCM 	0x0036 	/* DSP Solutions, Inc. */
#define WAVE_FORMAT_ECHOSC1 	0x0023 	/* Echo Speech Corporation */
#define WAVE_FORMAT_FM_TOWNS_SND 	0x0300 	/* Fujitsu Corp. */
#define WAVE_FORMAT_IBM_CVSD 	0x0005 	/* IBM Corporation */
#define WAVE_FORMAT_OLIGSM 	0x1000 	/* Ing C. Olivetti & C., S.p.A. */
#define WAVE_FORMAT_OLIADPCM 	0x1001 	/* Ing C. Olivetti & C., S.p.A. */
#define WAVE_FORMAT_OLICELP 	0x1002 	/* Ing C. Olivetti & C., S.p.A. */
#define WAVE_FORMAT_OLISBC 	0x1003 	/* Ing C. Olivetti & C., S.p.A. */
#define WAVE_FORMAT_OLIOPR 	0x1004 	/* Ing C. Olivetti & C., S.p.A. */
#define WAVE_FORMAT_IMA_ADPCM 	(WAVE_FORM_DVI_ADPCM) 	/* Intel Corporation */
#define WAVE_FORMAT_DVI_ADPCM 	0x0011 	/* Intel Corporation */
#define WAVE_FORMAT_UNKNOWN 	0x0000 	/* Microsoft Corporation */
#define WAVE_FORMAT_PCM 	0x0001 	/* Microsoft Corporation */
#define WAVE_FORMAT_ADPCM 	0x0002 	/* Microsoft Corporation */
#define WAVE_FORMAT_ALAW 	0x0006 	/* Microsoft Corporation */
#define WAVE_FORMAT_MULAW 	0x0007 	/* Microsoft Corporation */
#define WAVE_FORMAT_GSM610 	0x0031 	/* Microsoft Corporation */
#define WAVE_FORMAT_MPEG 	0x0050 	/* Microsoft Corporation */
#define WAVE_FORMAT_NMS_VBXADPCM 	0x0038 	/* Natural MicroSystems */
#define WAVE_FORMAT_OKI_ADPCM 	0x0010 	/* OKI */
#define WAVE_FORMAT_SIERRA_ADPCM 	0x0013 	/* Sierra Semiconductor Corp */
#define WAVE_FORMAT_SONARC 	0x0021 	/* Speech Compression */
#define WAVE_FORMAT_MEDIASPACE_ADPCM 	0x0012 	/* Videologic */
#define WAVE_FORMAT_YAMAHA_ADPCM 	0x0020 	/* Yamaha Corporation of America */

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
} __attribute__ ((packed)) wavHeader;


void wavSave(char *Output, u1 *Q, int L, int Bits, int Chns, int Freq) {
  int I;
  wavHeader W;
  FILE *OF;
  memcpy(W.Id, "RIFF", 4);
  W.Len = 36 + L;
  memcpy(W.Type, "WAVE", 4);
  memcpy(W.FmtId, "fmt ", 4);
  W.FmtLen = 16;
  W.Format = 1;
  W.Chns = Chns;
  W.Freq = Freq;
  W.Depth = Bits;
  W.ByteRate = W.Freq*W.Chns*W.Depth/8;
  W.Align = W.Chns*W.Depth/8;
  memcpy(W.DataId, "data", 4);
  W.DataLen = L;

  writeFile(0, 0, Output, Q); // create path
  OF = fopen(Output, "wb");
  fwrite(&W, 1, sizeof(wavHeader), OF);
  fwrite(Q, 1, L, OF);
  fclose(OF);
}



// HSV to RGB and vice-versa
void rgb_to_hsv(u1 r, u1 g, u1 b, double *h, double *s, double *v) {
  u1 maxval;
  double hue, delta;

  maxval = max(r, max(g, b));
  if (maxval == 0) {
    *h = *s = *v = 0;
    return;
  } else {
    *v = maxval / 255.0;
  }
  delta = maxval - min(r, min(g, b));
  if (delta == 0) {
    *h = *s = 0;
    return;
  }

  *s = delta / maxval;
  if (r == maxval) {
    if (g > b)
      hue = 0 + (g - b) / delta;
    else
      hue = 6 + (g - b) / delta;
  } else if (g == maxval) {
    hue = 2 + (b - r) / delta;
  } else {
    hue = 4 + (r - g) / delta;
  }

  *h = hue / 6;
}

#define SET_RGBVAL(i, a)  returncol[i] = (unsigned char) (255 * a + 0.5)
#define SET_RGB(a, b, c) {  \
  SET_RGBVAL(0, a);         \
  SET_RGBVAL(1, b);         \
  SET_RGBVAL(2, c);         \
}

void hsv_to_rgb(double h, double s, double v, u1 *returncol) {
  int sextant;
  double interp, p, q, t;

  if (s < 1e-03) {
    returncol[0] = returncol[1] = returncol[2] = 0;
    return;
  }

  h *= 6;
  sextant = (int) h;
  if (sextant >= 6) sextant -= 6;
  interp = h - sextant;

  p = v * (1 - s);
  q = v * (1 - (s *      interp));
  t = v * (1 - (s * (1 - interp)));

  switch(sextant) {
  case 0: SET_RGB(v, t, p); break;
  case 1: SET_RGB(q, v, p); break;
  case 2: SET_RGB(p, v, t); break;
  case 3: SET_RGB(p, q, v); break;
  case 4: SET_RGB(t, p, v); break;
  case 5: SET_RGB(v, p, q); break;
  }
}
