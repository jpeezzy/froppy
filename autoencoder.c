#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>
#include "matrix.h"
#include "neuralnet.h"
#include "openMP_backprop.h"
#include "fentoBoardState.h"
#include "boardToVector.h"
#include "randGen.h"

int main()
{
    //create the structs
    AUTOW *aw, *ag, *am, *av;
    AUTOL *al;
    DECODEW *dw, *dg, *dm, *dv;
    DECODEL *dl;

    aw = malloc(sizeof(AUTOW));
    al = malloc(sizeof(AUTOL));
    dw = malloc(sizeof(DECODEW));
    dl = malloc(sizeof(DECODEL));
    ag = malloc(sizeof(AUTOW));
    dg = malloc(sizeof(DECODEW));
    am = malloc(sizeof(AUTOW));
    av = mallox(sizeof(AUTOW));
    dm = malloc(sizeof(DECODEW));
    dv = malloc(sizeof(AUTOW));

    //Intialize the weights 
    randReluArray((float *) aw->weight0, 773, 600, 773);
    randReluArray((float *) dw->weight3, 600, 773, 600);

    //load the data in 
    




    int t = 1;
    int i;
    for(i=0; i<10000; ++i)
    {
        fowardpropAuto(aw,al,dw,dl,1);
        backpropAuto(aw,al,dw,dl,ag,dg);
        nadamAuto(aw,dw,ag,dg,am,av,dm,dv,t,1);
        t=t+1;
    }


    return 0;
}
