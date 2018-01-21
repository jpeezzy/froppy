
#ifndef MATRIX_H_
#define MATRIX_H_

void printMatrix(float *C, int M, int N);
void matrixMultiplication(float *h_a, float *h_b, float *h_result, int m, int n, int k);
float *transposeMatrix(float *matrix, int row, int column);
void combine3Matrix(float *a, float*b,float*c, float*combine, int rowa, int rowb, int rowc);

#endif
