
#ifndef MATRIX_H_
#define MATRIX_H_

void printMatrix(long double *C, int M, int N);
void matrixMultiplication(long double *h_a, long double *h_b, long double *h_result, int m, int n, int k);
long double *transposeMatrix(long double *matrix, int row, int column);
void combine3Matrix(long double *a, long double*b,long double*c, long double*combine, int rowa, int rowb, int rowc);
void mm(long double *h_a, long double *h_b, long double *h_result, int m, int n, int k);
void TMatrix(long double *A, long double *B, int m, int n);
void matrixSubtraction(long double *h_a, long double *h_b, long double *h_result, int m, int n);
void matrixScalarMul(long double *h_a, long double *h_b, long double *h_result, int m, int n);
void matrixDelta(long double *e, long double *layer, long double *delta, int m, int n);
void autoencoderE(long double *a, long double *b);
#endif
