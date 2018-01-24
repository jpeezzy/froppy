/*
 *file name: matrix.cu
 *this program contains code to do realtively simple matrix operations
 *This was designed to work with matrices hundreds of indexes long
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "cuPrintf.cu"
#include "matrix.h"

__global__ void matrixMultiplicationCUDA(
    float *h_a, float *h_b, float *h_result, int m, int n, int k)
{
  // calculate matrix index
  int              row = blockIdx.y * blockDim.y + threadIdx.y;
  int              col = blockIdx.x * blockDim.x + threadIdx.x;
  float            sum = 0f;
  __shared__ float temp_left_matrix[m][n];
  __shared__ float temp_right_matrix[n][k];
#pragma unroll
  for (int i = 0; i < n; ++i)
    {
      sum += h_a[row * n + i] * h_b[i * k + col];
    }
  h_result[row * k + col] = sum;
}

void matrixMultiplication(float *left_matrix,
                          float *right_matrix,
                          float *result,
                          int    row_num_left,
                          int    col_num_left,
                          int    co_num_right)
{
  assert(left_matrix);
  assert(right_matrix);
  assert(result);

  int sizeLeft  = sizeof(left_matrix);
  int sizeRight = sizeof(right_matrix);
  int sizeRes   = sizeof(result);

  // calculate correct block and thread number for indexing
  // TODO: optimize block size
  unsigned int grid_rows = (row_num_left + BLOCK_SIZE - 1) / BLOCK_SIZE;
  unsigned int grid_cols = (co_num_right + BLOCK_SIZE - 1) / BLOCK_SIZE;
  dim3         dimGrid(grid_cols, grid_rows);
  dim3         dimBlock(BLOCK_SIZE, BLOCK_SIZE);

  // assign and prepare memory for CUDA operation
  float *cuda_inputA = NULL;
  float *cuda_inputB = NULL;
  float *cuda_output = NULL;

  assert(cudaMalloc((void **)&cuda_inputA, sizeLeft) !=
         cudaErrorMemoryAllocation);
  assert(cudaMalloc((void **)&cuda_inputB, sizeRight) !=
         cudaErrorMemoryAllocation);
  assert(cudaMalloc((void **)&cuda_output, sizeRes) !=
         cudaErrorMemoryAllocation);

  cudaMemcpy(cuda_inputA, left_matrix, sizeA, cudaMemcpyHostToDevice);
  cudaMemcpy(cuda_inputB, right_matrix, sizeB, cudaMemcpyHostToDevice);

  matrixMultiplicationCUDA<<<dimGrid, dimBlock>>>(cuda_inputA,
                                                  cuda_inputB,
                                                  cuda_output,
                                                  row_num_left,
                                                  col_num_left,
                                                  co_num_right);

  cudaMemcpy(result, cuda_output, sizeO, cudaMemcpyDeviceToHost);

  // cleanup
  cudaFree(cuda_inputA);
  cudaFree(cuda_inputB);
  cudaFree(cuda_output);
}

/*
function: transposeMatrix

this function takes an rows x cols matrix inputMatrix
and transposes that matrix to outputMatrix that has dimension cols x row
i  */
__global__ void transposeMatrix(float *            inputMatrix,
                                float *            outputMatrix,
                                const unsigned int rows,
                                const unsigned int cols)
{
  unsigned int idx = blockIdx.x * blockDim.x + threadIdx.x;
  unsigned int idy = blockIdx.y * blockDim.y + threadIdx.y;
  if (idx < cols && idy < rows)
    {
      unsigned int pos        = idy * cols + idx;
      unsigned int trans_pos  = idx * rows + idy;
      outputMatrix[trans_pos] = inputMatrix[pos];
    }
}

/*
function: printMatrixCuda
this function prints out the matrix using CUDA
 */

__global__ void printMatrixCuda(float *C, int M, int N)
{
  cuPrintf("nothing is happening \n");
  for (int i = 0; i < M; i++)
    {
      for (int j = 0; j < N; j++)
        {
          cuPrintf("%f ", *((C + i * N) + j));
        }
      cuPrintf("\n");
    }
}

/*
function: main
 */

int main(void)
{
  printf("Hello world Matrix cuda function\n");
  float inputA[3][3];
  float inputB[3][3];
  float output[3][3];
  int   m = 3;
  int   n = 3;
  int   k = 3;
  for (int i = 0; i < 3; ++i)
    {
      for (int j = 0; j < 3; ++j)
        {
          inputA[i][j] = j + 1;
          inputB[i][j] = j + 1;
        }
    }
  int BLOCK_SIZE = 16;

  /*setting up how many blocks and threads we will need based on block size */
  /*mallocing temps for cuda gpu */
  float *cuda_inputA = NULL;
  float *cuda_inputB = NULL;
  float *cuda_output = NULL;

  int sizeA = sizeof(inputA);
  int sizeB = sizeof(inputB);
  int sizeO = sizeof(output);
  cudaMalloc((void **)&cuda_inputA, sizeA);
  cudaMalloc((void **)&cuda_inputB, sizeB);
  cudaMalloc((void **)&cuda_output, sizeO);
  printf("Finished Allocating!!\n");
  /* For cudaMemcpy to work, you must pass items by reference,
   */
  cudaMemcpy(cuda_inputA, inputA, sizeA, cudaMemcpyHostToDevice);
  cudaMemcpy(cuda_inputB, inputB, sizeB, cudaMemcpyHostToDevice);
  // cudaMemcpy(cuda_output, output, sizeO, cudaMemcpyHostToDevice);
  printf("Finished Copying Cuda Values!!\n");

  printf("starting matrix Multiplication \n");

  /* the parameters <<<N, K>>> indicate that you are calling to gpu.
     N = number of blocks
     k = number of threads inside blocks */
  matrixMultiplicationCUDA<<<dimGrid, dimBlock>>>(
      cuda_inputA, cuda_inputB, cuda_output, m, n, k);

  printf("finished matrix multiplcaiton! \n");

  printf("bringing result from gpu to device....\n");
  cudaMemcpy(output, cuda_output, sizeO, cudaMemcpyDeviceToHost);
  printf("done!\n");
  printf("Matrix A: \n");
  /*initialize cuPrintf */

  cudaPrintfInit();
  printMatrixCuda<<<1, 1>>>(cuda_inputA, m, n);
  cudaPrintfDisplay(NULL, true);
  cudaPrintfEnd();
  //	printMatrix((float*)inputA, m, n);
  printf("Matrix B\n");
  cudaPrintfInit();
  printMatrixCuda<<<1, 1>>>(cuda_inputB, n, k);
  cudaPrintfDisplay(NULL, true);
  cudaPrintfEnd();
  //	printMatrix((float*)inputB, n, k);
  printf("Matrix Out: \n");
  cudaPrintfInit();
  printMatrixCuda<<<1, 1>>>(cuda_output, m, k);
  cudaPrintfDisplay(NULL, true);
  cudaPrintfEnd();
  //	printMatrix((float*)output, m, k);
  // cudaPrintfEnd();
  return 0;
}

void printMatrix(float *C, int M, int N)
{
  assert(C);
  for (int i = 0; i < M; i++)
    {
      for (int j = 0; j < N; j++)
        {
          printf("%f ", *((C + i * N) + j));
        }
      printf("\n");
    }
}
