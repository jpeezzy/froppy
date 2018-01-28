#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>
#include "matrix.h"

int main()
{
    long double a[600][773];
    long double b[773][1];
    long double c[600][1];
    srand(time(NULL));

    int i,j;
    for(i=0; i<600; ++i)
    {
        for(j=0; j<773; ++j)
        {
            a[i][j] = rand();
            b[j][0] = rand();
        }
        c[i][0] = rand();
    }
    printf("\narrays intialized");

    matrixMultiplication((long double *) a, (long double *) b, (long double *) c, 600, 773,1);



}
