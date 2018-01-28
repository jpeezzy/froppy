#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#include "neuralnet.h"
#include "matrix.h"

int main()
{
    long double input[1][360];
    long double w0[360][540];
    long double w1[540][540];
    long double w2[540][540];
    long double w3[540][1];
    long double l1[1][540];
    long double l2[1][540];
    long double l3[1][540];
    long double output[1][1];

    int i,j;
    srand(1);
    for(i = 0; i < 360; ++i)
    {
        input[0][i] = (((long double)rand()/(long double)(RAND_MAX)));
        /*printf("\n%f\n", input[0][i]);*/ 
    }
    printf("\ninput\n");
    for(i = 0; i < 360; ++i)
    {
        for(j = 0; j < 540; ++j)
        {
            w0[i][j] = (((long double)rand()/(long double)(RAND_MAX)))*.07;       
        }
    }
    printf("\nw0\n");
    for(i = 0; i < 540; ++i)
    {
        for(j = 0; j < 540; ++j)
        {
            w1[i][j] = (((long double)rand()/(long double)(RAND_MAX)))*.06;  

        }
    }
    for(i = 0; i < 540; ++i)
    {
        for(j = 0; j < 540; ++j)
        {
            w2[i][j] = (((long double)rand()/(long double)(RAND_MAX)))*.06;  

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

    matrixMultiplication((long double*)input, (long double *)w0,(long double *)l1, 1, 360, 540);
    reluArray((long double *)l1,1,540,0);
    matrixMultiplication((long double*)l1, (long double *)w1,(long double *)l2, 1, 540, 540);
    reluArray((long double *)l2,1,540,0);
    matrixMultiplication((long double*)l2, (long double *)w2,(long double *)l3, 1, 540, 540);
    reluArray((long double *)l3,1,540,0);
    matrixMultiplication((long double*)l3, (long double *)w3,(long double *)output, 1, 540, 1);
    printf("\n %f \n", output[0][0]);
    tanhArray((long double *)output,1,1,0);
    
    clock_gettime(CLOCK_MONOTONIC, &tend);
    printf("Took about %.5f seconds\n", ((long double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - ((long double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
    printf("\n %f \n",output[0][0]);

}

