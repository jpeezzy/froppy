
#ifndef MATRIX_H_
#define MATRIX_H_

void printMatrix(double *C, int M, int N);
void matrixMultiplication(double *h_a, double *h_b, double *h_result, int m, int n, int k);
double *transposeMatrix(double *matrix, int row, int column);
void combine3Matrix(double *a, double*b,double*c, double*combine, int rowa, int rowb, int rowc);

#endif
