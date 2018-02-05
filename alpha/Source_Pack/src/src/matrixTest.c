#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>
#include "matrix.h"

int main()
{
    double a[2][4] = {{1,2,3,4},{5,6,7,8}};
    double b[4][2]= {{1,2},{3,4},{5,6},{7,8}};
    double c[2][2];
    srand(time(NULL));

    printf("\narrays intialized");

    matrixMultiplication((double *) a, (double *) b, (double *) c, 2, 4,2);
    printMatrix((double *)c,2,2);


}
