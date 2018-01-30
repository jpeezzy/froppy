#include <assert.h>
#include <fenv.h>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
//    BSTATE   move;

    //temp grad
    AUTOW * tempag;
    DECODEW * tempdg;
    tempag = malloc(sizeof(AUTOW));
    tempdg = malloc(sizeof(DECODEW));


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
    randReluArray((double *)aw->weight1, 600, 400, 600);
    randReluArray((double *)aw->weight2, 400, 200, 400);
    randReluArray((double *)aw->weight3, 200, 100, 200);
    randReluArray((double *)dw->weight0, 100, 200, 100);
    randReluArray((double *)dw->weight1, 200, 400, 200);
    randReluArray((double *)dw->weight2, 400, 600, 400);
    randReluArray((double *)dw->weight3, 600, 773, 600);

    
    //intialize the momentum and variance vectors
    matrixZero((double *) am->weight0, 773, 600);
    matrixZero((double *) av->weight0, 773, 600);
    matrixZero((double *) am->weight1, 600, 400);
    matrixZero((double *) av->weight1, 600, 400);
    matrixZero((double *) am->weight2, 400, 200);
    matrixZero((double *) av->weight2, 400, 200);
    matrixZero((double *) am->weight3, 200, 100);
    matrixZero((double *) av->weight3, 200, 100);
   
    matrixZero((double *) dm->weight0, 100, 200);
    matrixZero((double *) dv->weight0, 100, 200);
    matrixZero((double *) dm->weight1, 200, 400);
    matrixZero((double *) dv->weight1, 200, 400);
    matrixZero((double *) dm->weight2, 400, 600);
    matrixZero((double *) dv->weight2, 400, 600);
    matrixZero((double *) dm->weight3, 600, 773);
    matrixZero((double *) dv->weight3, 600, 773);

    // load the data in
    double vect[1][773];
    DATABASE *dataB;
    dataB = createDataB();
    assert(dataB);
    FILE *che = fopen("res1.txt", "r");
    assert(che);
    printf("Finished asserting che! \n");

    readFenfile((FILE *)che, dataB);
    printf("Finished reading fen file che! \n");
    BSTATE move = pickRandMove(dataB);
    boardToVector(&move, (double *) vect);

                    
    matrixCopy((double *)al->input, (double *)vect, 1, 773);

    int t = 1;
    int epochs, stagenum, iter;
    int batch = 1, bint;

    //this is where the training starts
    for(stagenum = 1; stagenum <= 4; ++stagenum)
    {   
        t = 1;
        for (epochs = 0; epochs < 1; ++epochs)
        {
            for(iter=0; iter<100; ++iter)
            {   
                
                for(bint = 0; bint < batch; ++bint)
                {
                    //BSTATE move = pickRandMove(dataB);
                    //boardToVector(&move, (double *) vect);

                    
                    //matrixCopy((double *)al->input, (double *)vect, 1, 773);
                    
                    fowardpropAuto(aw, al, dw, dl, stagenum);
                    backpropAutoN(aw, al, dw, dl, ag, dg, stagenum);
                    
                    if(stagenum == 1)
                    {
                        matrixAddition((double *) tempag->weight0, (double *) ag->weight0, 773, 600);
                        matrixAddition((double *) tempdg->weight3, (double *) dg->weight3, 600, 773);
                    }
                    else if(stagenum == 2)
                    {
                        matrixAddition((double *) tempag->weight1, (double *) ag->weight1, 600, 400);
                        matrixAddition((double *) tempdg->weight2, (double *) dg->weight2, 400, 600);
                    }
                    else if(stagenum == 3)
                    {
                        matrixAddition((double *) tempag->weight2, (double *) ag->weight2, 400, 200);
                        matrixAddition((double *) tempdg->weight1, (double *) dg->weight1, 200, 400); 
                    }
                    else if(stagenum == 4)
                    {
                        matrixAddition((double *) tempag->weight3, (double *) ag->weight3, 200, 100);
                        matrixAddition((double *) tempdg->weight0, (double *) dg->weight0, 100, 200);
                    }
                }
                nadamAuto(aw, dw, tempag, tempdg, am, av, dm, dv, t, stagenum);
                if(stagenum == 1)
                {
                    matrixZero((double *) tempag->weight0, 773, 600);
                    matrixZero((double *) tempdg->weight3, 600, 773);
                }
                else if(stagenum == 2)
                {
                    matrixZero((double *) tempag->weight1, 600, 400);
                    matrixZero((double *) tempdg->weight2, 400, 600);
                }
                else if(stagenum == 3)
                {
                    matrixZero((double *) tempag->weight2, 400, 200);
                    matrixZero((double *) tempdg->weight1, 200, 400); 
                }
                else if(stagenum == 4)
                {
                    matrixZero((double *) tempag->weight3, 200, 100);
                    matrixZero((double *) tempdg->weight0, 100, 200);
                }
                t = t+1;   
            }
        }
        printf("\n on the next one now \n");
    }


    //free the structs
    free(aw);
    free(al);
    free(dw);
    free(dl);
    free(ag);
    free(dg);
    free(am);
    free(av);
    free(dm);
    free(dv);
    free(tempag);
    free(tempdg);

    return 0;
}
