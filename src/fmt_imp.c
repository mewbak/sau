//Credits go to SNV


#include "common.h"

#define IMP_1BIT_RLE  25
#define IMP_4BIT_RLE  57
#define IMP_8BIT      8
#define IMP_8BIT_RLE  9

typedef struct {
  u2 Type; //   8: 8-bit uncompressed
           //   9: 8-bit RLE packed
           //   25: 1-bit RLE packed
           //   57: 4-bit RLE-packed image???
  u1 Unknown1;
  u1 ColorKey;
  u2 Width;
  u2 Height;
  u4 Pal; // offset to palette
  u4 Pal2;
  u4 Unknown3;
  u4 Unknown4;
  u2 Unknown5;
  u2 NSeqs; // number of sequences
  u4 Seqs; // offset to sequences

  //Sequence Sequences[NumSequences];
  // Sequence represents specific animation (like movement or spellcasting),
  // together with facings

  //Facing Facings[]; // directions, sprite can face (some are mirrored)
  //Facing Frames[];  // frame headers

  //u1 Palette[256*4]; // RGBA palette:
                       //    index 0 used for transparency,
                       //    index 1 is shadow

  //Data[] // all frames compressed or uncompressed data

  
  //HotSpot HPs[]; // hotspot (selection) at the end of the file
} __attribute__ ((packed)) header;


typedef struct {
  u4 Unknown1;
  u4 Unknown2; // never seen it other than #FF (-1?)
  u1 Unknown3; // never seen it other than 0
  u2 Unknown4; // never seen it other than 0
  u1 NFaces;
  u4 Off; // offset of facing groups (directions)
} __attribute__ ((packed)) seq;

// Facings go clockwise
typedef struct {
  u2 Unknown1; // never seen it other than 0
  u2 NFrames;
  u4 Off;
} __attribute__ ((packed)) face;

typedef struct {
  u1 Type;   // 0=normal, 8=duplicate-back-reference
  u1 HSType; // HotSpot type:
             // 0: packed 16-bit DispX and DispY (both signed)
             // 2: offset to HotSpot structure
             // 3: offset to HotSpot structure
             // 4: offset to HotSpot structure
  u2 W;      // real width
  u2 H;      // real height
  u2 Size;   // never use. unreliable.
  u4 HSpot;  // Hot Spot: offset or packed Disp XY-pair
  u4 Offset; // offset or number of duplicating frame
  // Note: Duplicate frames provide animation delay.
  //       Don't simply strip them.
} __attribute__ ((packed)) frame;

typedef struct {
  s2 Unknown1;

  s2 DispX; // XY displacement from virtual Width and Height
  s2 DispY; // It's where unit's feet are

  s2 Unknown2;

  s2 HotX; // click-point for cursors
  s2 HotY; // unsure what it does for unit's (missile spawning place?)

  s2 Unknown3;
  s2 Unknown4;
} __attribute__ ((packed)) hotSpot;


#define MAX_FRAMES   (16*1024)
#define MAX_SEQS     1024

static char *SeqNames[MAX_SEQS];

static char *impTypeName(int Type) {
  if(Type == IMP_8BIT) return "8-bit";
  else if (Type == IMP_8BIT_RLE) return "8-bit, RLE-compressed";
  else if (Type == IMP_1BIT_RLE) return "1-bit, RLE-compressed";
  else if (Type == IMP_4BIT_RLE) return "4-bit, RLE-compressed";
  return "unknown format";
}


static void impDecompile(char *Output, char *Input) {
  char Tmp[1024];
  FILE *Ds;
  pic *A, *B;
  u1 *P, *M;
  int I, J, K, L, C, N, NFrames;
  frame *F;
  face *FC;
  seq *Seqs;
  hotSpot *HS;
  int Sz = fileSize(Input);
  header *H = (header *)readFile(0, Sz, Input);


  printf("%dx%d Type%d (%s), %d animations, Key=%d\n"
        ,H->Width, H->Height, H->Type, impTypeName(H->Type)
        ,H->NSeqs, H->ColorKey);


  strcpy(Tmp, Input);
  strcpy(Tmp+strlen(Tmp)-3, "h");
  if (Ds = fopen(Tmp, "r")) {
    char *P, *Q;
    I = -1;
    while(fgets(Tmp, 1024, Ds)) {
      unless(!strncmp("#define ", Tmp, 7)) continue;
      if(I == -1) {I=0; continue;}
      P = Tmp+8;
      while(isalnum(*P++));
      Q = P;
      while(isalnum(*P) || *P == '_') P++;
      *P = 0;
      SeqNames[I++] = downcase(strdup(Q));
    }
  } else {
    printf("No %s: proceeding without animation names\n", Tmp);
    times(I, H->NSeqs) SeqNames[I] = "";
  }

  NFrames = 0;
  Seqs = (seq*)((u1*)H+H->Seqs);
  times(I, H->NSeqs) {
    times(J, Seqs[I].NFaces) {
      FC = (face*)((u1*)H + Seqs[I].Off + J*sizeof(face));
      times(K, FC->NFrames) {
        F = (frame*)((u1*)H + FC->Off + K*sizeof(frame));
        if (F->Type==8) continue;

        A = picNew(F->W?F->W:1, F->H?F->H:1, 8);

        P = (u1*)H + H->Pal;
        times (C, 256) {
          A->P[C*4+0] = P[C*4+2];
          A->P[C*4+1] = P[C*4+1];
          A->P[C*4+2] = P[C*4+0];
          A->P[C*4+3] = 0;
        }
        A->K = H->ColorKey;
        A->P[A->K*4+3] = 255;
        A->K = H->ColorKey;
        picClear(A, A->K);

        P = (u1*)H + F->Offset;
        if (H->Type == IMP_8BIT) memcpy(A->D, P, A->H*A->W);
        else if (H->Type == IMP_8BIT_RLE) {
          for(L = 0; L < F->W*F->H; ) {
            C = (s1)*P++;
            if(C<0) {
              C = -C;
              while(C--) A->D[L++] = *P++;
            } else {
              C =  C + 3;
              while(C--) A->D[L++] = *P;
              P++;
            }
          }
        } else if(H->Type == IMP_1BIT_RLE) {
          // missile/stickaps.imp uses it
          M = malloc(A->H*A->W+0xffff);

          for(L = 0; L < (A->W*A->H+7)/8; ) {
            C = (s1)*P++;
            if(C<0) {
              C = -C;
              while(C--) M[L++] = *P++;
            } else {
              C =  C + 3;
              while(C--) M[L++] = *P;
              P++;
            }
          }
          P = M;

          for(C = L = 0; L < A->W*A->H; L++) {
            A->D[L] = (*P>>C++)&1;
            if(C == 8) {P++; C = 0;}
          }

          free(M);
        } else if(H->Type == IMP_4BIT_RLE) {
          // aicr5aa.imp uses it
          continue;
        } else {
          continue;
        }

        HS = n(hotSpot);
        if (F->HSType == 0) {
          HS->DispX = (s2)((F->HSpot)&0xffff);
          HS->DispY = (s2)((F->HSpot>>16)&0xffff);
        } else if (F->HSType == 2 || F->HSType == 3 || F->HSType == 4) {
          memcpy(HS, (u1*)H + F->HSpot, sizeof(hotSpot));
        }

#if 0
        // all sprites were rendered at 512x512
        B = picNew(512,512,8);
        memcpy(B->P, A->P, 256*4);
        B->K = A->K;
        picClear(B, B->K);

        // align by 2 or it will jump like crazy
        picBlt(B, A, 0
              ,(B->W-A->W+1)/2 + HS->DispX, (B->H-A->H+1)/2 + HS->DispY
              ,0, 0, A->W, A->H);
#endif


        sprintf(Tmp, "%s/%3d_%s_%3d_%3d.png", Output, I, SeqNames[I], K, J);
        pngSave(Tmp, A);
      }
    }
  }
}

int impInit(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "imp";
  F->Description = "Lords of Magic sprites";
  F->Decompile = impDecompile;
  return 1;
}

