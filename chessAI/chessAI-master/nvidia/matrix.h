
#ifndef MATRIX_H_
#define MATRIX_H_

void printMatrix(long double *C, int M, int N);
__global__ void matrixMultiplication(long double *h_a, long double *h_b, long double *h_result, int m, int n, int k);
__global__ void transposeMatrix(long double *inputmatrix, long double *outputMatrix, const unsigned int row, const unsigned int column);
void combine3Matrix(long double *a, long double*b,long double*c, long double*combine, int rowa, int rowb, int rowc);

#endif
