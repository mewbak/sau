//Credits go to SNV

#include "common.h"



typedef struct {
  u1 Syn[4];
  u1 ColorMap[0x300];
  // ...
} __attribute__ ((packed)) header;

typedef struct {
  //u2 NameLen;
  //u1 Name[NameLen];
  u1 T; // Type: 0 - 8-bit
        //       1 - 8-bit
        //       4 - 8-bit and 4-bit alpha
  u4 X; // X/Y offs
  u4 Y;
  u4 W;
  u4 H;
  //line Lines[H]; 
  //u1 Data[Lines[H-1].Off+Lines[H-1].E-Lines[H-1].X]
} __attribute__ ((packed)) frame;


typedef struct {
  u2 S; // start x offset
  u2 E; // end x offset
  u4 O; // offset into data
} __attribute__ ((packed)) line;


void df2Decompile(char *Output, char *Input) {
  u1 Syn[4];
  char Tmp[1024], Name[256], DF2Name[256];
  pic *P, *A, *T;
  int I, J, C, N, X, Y, NFrames, NColors, ColorKey, DX, DY;
  frame *F;
  line *Ys;
  int L = fileSize(Input);
  u1 *M, *Q, *Z, *E, *ColorMap, *B=0;
  M = Q = readFile(0, L, Input);
  E = M+L;

  pathParts(0, DF2Name, 0, Input);
  downcase(DF2Name);

  // probably not a number of frames, but a number of sections
  // for bone dragon it says 2, while there are 7 frames + 7 shadow frames (i.e. 14)
  NFrames = *(u2*)Q; Q+=2;

  //printf("  NFrames: %d\n", NFrames);

  times (I, NFrames) {
    A = 0;
    if (0) {
bt:
      Q = B+1;
    }

    // alpha-masked frames have unidentified chunk of bytes at the end...
    C = 0x2000; // should be enough
    while (C-- && Q+32 < E && ((*(u2*)Q > 0x100)
                             || *(u2*)(Q+2)>2 && *(u2*)(Q+2) != 0x1000)) {
      Q++;
    }
    if (C<0 || Q+26==E) {
      printf("  sync failed\n");
      abort();
      break;
    }
    B = Q;

    ColorKey = 0;
    NColors = *(u2*)Q; Q+=2;
    Q += 2; // color key?
    ColorMap = Q; Q += NColors*3;

    if (Q+2 >= E) goto bt;

    J = *(u2*)Q; Q+=2;
    if (J>64) goto bt;
    memcpy(Name, Q, J);
    Name[J] = 0;
    if (Q+sizeof(frame) >= E) goto bt;
    Z = Q; Q += J;
    for (; J--; Z++) if (*Z < 0x20 || *Z>0x7E) goto bt;

    F = (frame*)Q;
    DX = F->W - F->X;
    DY = F->H - F->Y;

    if (DX<0 || DX>1280 || DY<0 || DY>1024
     || F->W<0 || F->W>1280 || F->H<0 || F->H>1024
     || (F->T != 0 && F->T != 1 && F->T != 4))
      goto bt;

    printf("  Extracting: %s\n", Name);
    //printf("  %04d=#%08x: Colors=%d Type=%d X=%d Y=%d W=%d H=%d\n"
    //      ,I, B-M, NColors, F->T, F->X, F->Y, F->W, F->H);


    //ColorMap[ColorKey*3 + 0] = 255;
    //ColorMap[ColorKey*3 + 1] = 255;
    //ColorMap[ColorKey*3 + 2] = 0;


    P = picNew(F->W, F->H, 8);
    picClear(P, ColorKey);

    Ys = (line*)(F+1);
    Q = (u1*)(Ys + DY);
    Z = Q;
    times (J, 256) {
      P->P[J*4+0] = ColorMap[J*3+2];
      P->P[J*4+1] = ColorMap[J*3+1];
      P->P[J*4+2] = ColorMap[J*3+0];
      P->P[J*4+3] = 0;
    }

    times (Y, DY) {
      //printf("    %04d: %04x %04x %08x (%04x)\n"
      //        ,Y, Ys->S, Ys->E, Ys->O, Ys->E-Ys->S);
      for (X=Ys->S; X<Ys->E; X++) picPut(P, X+F->X, Y+F->Y, *Q++);
      Ys++;
    }
    //pngSave("out/1.png", P);
    //printf("  #%x #%x\n", Q-M, Q-Z);

    if (F->T != 4) goto skip_alpha;

    A = picNew(P->W, P->H, 8);
    times (J, 256) {
      A->P[J*4+0] = J;
      A->P[J*4+1] = J;
      A->P[J*4+2] = J;
      A->P[J*4+3] = 0;
    }

    J=0;
    Ys = (line*)(F+1);
    Q = (u1*)(Ys + DY) + Ys[DY-1].O+Ys[DY-1].E-Ys[DY-1].S;
    Z = Q;
    N = 0;
    times (Y, DY) {
      for (X=Ys->S; X<Ys->E; X++) {
        if (J++&1) C = (*Q++>>4);
        else C = (*Q&0xF);
        picPut(A, X+F->X, Y+F->Y, C*255/15);
      }
      Ys++;
    }
    if (J&1) Q++;

    //pngSave("out/2.png", A);

    //printf("  #%x #%x %d\n", Q-M, Q-Z, J);

skip_alpha: ;
    if (A) {
      T = picNew(P->W, P->H, 32);
      times (Y, T->H) {
        times (X, T->W) {
          C = picGet(P, X,Y);
          picPut32(T,X,Y,R8G8B8A8(P->P[C*4+0],P->P[C*4+1],P->P[C*4+2],picGet(A,X,Y)));
        }
      }
      picDel(P);
      picDel(A);
      P = T;
    }
    if (P->W && P->H) { // crappy PNG doesnt support null-images
      sprintf(Tmp, "%s/%04d-%s.png", Output, I, Name);
      pngSave(Tmp, P);
    }
    picDel(P);
  }
}


int df2Init(format *F) {
  F->Type = FMT_ARCHIVE;
  F->Name = "df2";
  F->Description = "Heroes of Might & Magic 4 sprite archives";
  F->Decompile = df2Decompile;
  return 1;
}

