
#ifndef MATRIX_H_
#define MATRIX_H_

void printMatrix(float *C, int M, int N);

// implement two dimension matrix multiplications
__global__ void matrixMultiplicationCUDA(
    float *h_a, float *h_b, float *h_result, int m, int n, int k);

// function called by host to ask CUDA to multiply matrix
// wrapper for matrixMultiplicationCUDA
void matrixMultiplication(float *left_matrix,
                          float *right_matrix,
                          float *result,
                          int    row_num_left,
                          int    col_num_left,
                          int    co_num_right);

__global__ void transposeMatrix(float *            inputmatrix,
                                float *            outputMatrix,
                                const unsigned int row,
                                const unsigned int column);
void            combine3Matrix(
               float *a, float *b, float *c, float *combine, int rowa, int rowb, int rowc);

#endif
