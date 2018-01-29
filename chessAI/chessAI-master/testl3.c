#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#include "neuralnet.h"
#include "matrix.h"

int main()
{
    double input[1][360];
    double w0[360][540];
    double w1[540][540];
    double w2[540][540];
    double w3[540][1];
    double l1[1][540];
    double l2[1][540];
    double l3[1][540];
    double output[1][1];

    int i,j;
    srand(1);
    for(i = 0; i < 360; ++i)
    {
        input[0][i] = (((double)rand()/(double)(RAND_MAX)));
        /*printf("\n%f\n", input[0][i]);*/ 
    }
    printf("\ninput\n");
    for(i = 0; i < 360; ++i)
    {
        for(j = 0; j < 540; ++j)
        {
            w0[i][j] = (((double)rand()/(double)(RAND_MAX)))*.07;       
        }
    }
    printf("\nw0\n");
    for(i = 0; i < 540; ++i)
    {
        for(j = 0; j < 540; ++j)
        {
            w1[i][j] = (((double)rand()/(double)(RAND_MAX)))*.06;  

        }
    }
    for(i = 0; i < 540; ++i)
    {
        for(j = 0; j < 540; ++j)
        {
            w2[i][j] = (((double)rand()/(double)(RAND_MAX)))*.06;  

        }
    }
    for(i = 0; i < 200; ++i)
    {
        w3[i][0] = .0000003;
    }
    for(i = 200; i < 540; ++i)
    {
        w3[i][0] = .0000009;
    }
    struct timespec tstart = {0,0}, tend = {0,0};
    clock_gettime(CLOCK_MONOTONIC, &tstart);

    matrixMultiplication((double*)input, (double *)w0,(double *)l1, 1, 360, 540);
    reluArray((double *)l1,1,540,0);
    matrixMultiplication((double*)l1, (double *)w1,(double *)l2, 1, 540, 540);
    reluArray((double *)l2,1,540,0);
    matrixMultiplication((double*)l2, (double *)w2,(double *)l3, 1, 540, 540);
    reluArray((double *)l3,1,540,0);
    matrixMultiplication((double*)l3, (double *)w3,(double *)output, 1, 540, 1);
    printf("\n %f \n", output[0][0]);
    tanhArray((double *)output,1,1,0);
    
    clock_gettime(CLOCK_MONOTONIC, &tend);
    printf("Took about %.5f seconds\n", ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
    printf("\n %f \n",output[0][0]);

}

