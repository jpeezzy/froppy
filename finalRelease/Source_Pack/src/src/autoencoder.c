#include <assert.h>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fenv.h>

#include "boardToVector.h"
#include "dataEntry.h"
#include "fenToBoardState.h"
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
  randReluArray((double *)aw->weight0, 773, 600, 773);
  randReluArray((double *)dw->weight3, 600, 773, 600);
  int yy, xx;
  for (yy = 0; yy < 773; ++yy)
    {
      for (xx = 0; xx < 600; ++xx)
        {
          am->weight0[yy][xx] = 0.0;
          av->weight0[yy][xx] = 0.0;
          dm->weight3[xx][yy] = 0.0;
          dv->weight3[xx][yy] = 0.0;
        }
    }

  // load the data in
  double     vect[1][773];
  DATABASE *dataB;
  dataB = createDataB();
  assert(dataB);
  FILE *che = fopen("res1.txt", "r");
  assert(che);
  printf("Finished asserting che! \n");

  readFenfile((FILE *)che, dataB);
  printf("Finished reading fen file che! \n");
  move = pickRandMove(dataB);
  boardToVector(&move, (double *)vect);
  int t = 1;
  int i;
  for (int i = 0; i < 773; ++i)
    {
      al->input[0][i] = vect[0][i];
    }

  for (i = 0; i < 100; ++i)
    { if(1)
        {
          printf("\n %d  ",i);
        }
      fowardpropAuto(aw, al, dw, dl, 1);
#ifdef DEBUG
      //printf("finished fprop\n");
#endif
      backpropAutoN(aw, al, dw, dl, ag, dg, 1);
#ifdef DEBUG
      //printf("finished bprop\n");
#endif
      nadamAuto(aw, dw, ag, dg, am, av, dm, dv, t, 1);
      t    = t + 1;
      //move = pickRandMove(dataB);
      //boardToVector(&move, (double *)vect);
#ifdef DEBUG
      //printf("finished one move\n");
#endif
    
        //for (int i = 0; i < 773; ++i)
        //{
        //    al->input[0][i] = vect[0][i];
        //}
    }
  return 0;
}
