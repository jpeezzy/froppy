
#ifndef MATRIX_H_
#define MATRIX_H_

void printMatrix(double *C, int M, int N);
void matrixMultiplication(double *h_a, double *h_b, double *h_result, int m, int n, int k);
double *transposeMatrix(double *matrix, int row, int column);
void combine3Matrix(double *a, double*b,double*c, double*combine, int rowa, int rowb, int rowc);
void mm(double *h_a, double *h_b, double *h_result, int m, int n, int k);
void TMatrix(double *A, double *B, int m, int n);
void matrixSubtraction(double *h_a, double *h_b, double *h_result, int m, int n);
void matrixScalarMul(double *h_a, double *h_b, double *h_result, int m, int n);
void matrixDelta(double *e, double *layer, double *delta, int m, int n);
void autoencoderE(double *a, double *b);
#endif
