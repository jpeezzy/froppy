#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#include "neuralnet.h"
#include "matrix.h"

int main()
{
    float input[1][360];
    float w0[360][540];
    float w1[540][540];
    float w2[540][540];
    float w3[540][1];
    float l1[1][540];
    float l2[1][540];
    float l3[1][540];
    float output[1][1];

    int i,j;
    srand(1);
    for(i = 0; i < 360; ++i)
    {
        input[0][i] = (((float)rand()/(float)(RAND_MAX)));
        /*printf("\n%f\n", input[0][i]);*/ 
    }
    printf("\ninput\n");
    for(i = 0; i < 360; ++i)
    {
        for(j = 0; j < 540; ++j)
        {
            w0[i][j] = (((float)rand()/(float)(RAND_MAX)))*.07;       
        }
    }
    printf("\nw0\n");
    for(i = 0; i < 540; ++i)
    {
        for(j = 0; j < 540; ++j)
        {
            w1[i][j] = (((float)rand()/(float)(RAND_MAX)))*.06;  

        }
    }
    for(i = 0; i < 540; ++i)
    {
        for(j = 0; j < 540; ++j)
        {
            w2[i][j] = (((float)rand()/(float)(RAND_MAX)))*.06;  

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

    matrixMultiplication((float*)input, (float *)w0,(float *)l1, 1, 360, 540);
    reluArray((float *)l1,1,540,0);
    matrixMultiplication((float*)l1, (float *)w1,(float *)l2, 1, 540, 540);
    reluArray((float *)l2,1,540,0);
    matrixMultiplication((float*)l2, (float *)w2,(float *)l3, 1, 540, 540);
    reluArray((float *)l3,1,540,0);
    matrixMultiplication((float*)l3, (float *)w3,(float *)output, 1, 540, 1);
    printf("\n %f \n", output[0][0]);
    tanhArray((float *)output,1,1,0);
    
    clock_gettime(CLOCK_MONOTONIC, &tend);
    printf("Took about %.5f seconds\n", ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
    printf("\n %f \n",output[0][0]);

}

