
#ifndef MATRIX_H_
#define MATRIX_H_

void printMatrix(double *C, int M, int N);
__global__ void matrixMultiplication(double *h_a, double *h_b, double *h_result, int m, int n, int k);
__global__ void transposeMatrix(double *inputmatrix, double *outputMatrix, const unsigned int row, const unsigned int column);
void combine3Matrix(double *a, double*b,double*c, double*combine, int rowa, int rowb, int rowc);

#endif
