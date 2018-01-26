#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "boardToVector.h"
#include "fentoBoardState.h"
#include "matrix.h"
#include "neuralnet.h"
#include "openMP_backprop.h"
#include "randGen.h"

int main()
{
  // create the structs
  AUTOW *  aw, *ag, *am, *av;
  AUTOL *  al;
  DECODEW *dw, *dg, *dm, *dv;
  DECODEL *dl;
  BSTATE   move;
  aw = malloc(sizeof(AUTOW));
  al = malloc(sizeof(AUTOL));
  dw = malloc(sizeof(DECODEW));
  dl = malloc(sizeof(DECODEL));
  ag = malloc(sizeof(AUTOW));
  dg = malloc(sizeof(DECODEW));
  am = malloc(sizeof(AUTOW));
  av = malloc(sizeof(AUTOW));
  dm = malloc(sizeof(DECODEW));
  dv = malloc(sizeof(AUTOW));

  // Intialize the weights
  randReluArray((float *)aw->weight0, 773, 600, 773);
  randReluArray((float *)dw->weight3, 600, 773, 600);
  int yy,xx;
  for(yy=0; yy<773; ++yy)
  {
      for(xx=0; xx<600; ++xx)
      {
          am->weight0[yy][xx] = 0;
          av->weight0[yy][xx] = 0;
          dm->weight3[xx][yy] = 0;
          dv->weight3[xx][yy] = 0;
      }

  }


  // load the data in
  float vect[1][773];
  dataB = createDataB(void);
  readFenfile(fopen(res1.txt, 'r'), dataB);
  move = pickRandMove(dataB);
  boardToVector(move, (float *)vect);
  int t = 1;
  int i;
  aw->input = vect;
  for (i = 0; i < 10000; ++i)
    {
      fowardpropAuto(aw, al, dw, dl, 1);
      backpropAuto(aw, al, dw, dl, ag, dg,1);
      nadamAuto(aw, dw, ag, dg, am, av, dm, dv, t, 1);
      t    = t + 1;
      move = pickRandMove(dataB);
      boardToVector(move, (float *)vect);
      aw->input = vect;
    }

  return 0;
}
