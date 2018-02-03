/* 
 *file name: matrix.cu
 *this program contains code to do realtively simple matrix operations
 *This was designed to work with matrices hundreds of indexes long
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "cuPrintf.cu"
#include "matrix.h"

/*
function: matrixMultiplication

this function takes in an m X n matrix h_a,
a n X k matrix h_b, and computes the matrix multiplication and 
stores the result int m x k matrix (C)
 */

__global__ void matrixMultiplication(double *h_a, double *h_b, double *h_result, int m, int n, int k)
{
	int row = blockIdx.y * blockDim.y + threadIdx.y;
	int col = blockIdx.x * blockDim.x + threadIdx.x;
	int sum = 0;
	if(col < k && row < m)
	{
		for(int i = 0; i < n; ++i)
		{
			sum+=h_a[row*n+i] * h_b[i*k+col];
		}
		h_result[row*k + col] = sum;
	}
}

/*
function: transposeMatrix

this function takes an rows x cols matrix inputMatrix
and transposes that matrix to outputMatrix that has dimension cols x row
i  */
__global__ void transposeMatrix(double *inputMatrix, double* outputMatrix, const unsigned int rows, const unsigned int cols)
{
	unsigned int idx = blockIdx.x * blockDim.x + threadIdx.x;
	unsigned int idy = blockIdx.y * blockDim.y + threadIdx.y;
	if(idx < cols && idy < rows)
	{
		unsigned int pos = idy * cols + idx;
		unsigned int trans_pos = idx * rows + idy;
		outputMatrix[trans_pos] = inputMatrix[pos];
	}
}


/*
function: printMatrixCuda
this function prints out the matrix using CUDA
 */

__global__ void printMatrixCuda(double *C, int M, int N)
{
	cuPrintf("nothing is happening \n");
	for(int i = 0; i < M; i++)
	{
		for(int j = 0; j < N; j++)
		{
			cuPrintf("%f ", *((C+i*N) + j));
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
	double inputA[3][3];
	double inputB[3][3];
	double output[3][3];
	int m = 3;
	int n = 3;
	int k = 3;
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			inputA[i][j] = j+1;
			inputB[i][j] = j+1;
		}
	}
	int BLOCK_SIZE = 16;

	unsigned int grid_rows = (m + BLOCK_SIZE - 1) / BLOCK_SIZE;
	unsigned int grid_cols = (k + BLOCK_SIZE - 1) / BLOCK_SIZE;
	dim3 dimGrid(grid_cols, grid_rows);
	dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
	/*setting up how many blocks and threads we will need based on block size */
	/*mallocing temps for cuda gpu */
	double *cuda_inputA = NULL;
	double *cuda_inputB = NULL;
	double *cuda_output = NULL;

	int sizeA = sizeof(inputA);
	int sizeB = sizeof(inputB);
	int sizeO = sizeof(output);
	cudaMalloc((void**) &cuda_inputA, sizeA);
	cudaMalloc((void**) &cuda_inputB, sizeB);
	cudaMalloc((void**) &cuda_output, sizeO);
	printf("Finished Allocating!!\n");
	/* For cudaMemcpy to work, you must pass items by reference,
	 */	
	cudaMemcpy(cuda_inputA, inputA, sizeA, cudaMemcpyHostToDevice);
	cudaMemcpy(cuda_inputB, inputB, sizeB, cudaMemcpyHostToDevice);
	//cudaMemcpy(cuda_output, output, sizeO, cudaMemcpyHostToDevice);
	printf("Finished Copying Cuda Values!!\n");

	printf("starting matrix Multiplication \n");
	/* the parameters <<<N, K>>> indicate that you are calling to gpu.
	   N = number of blocks 
	   k = number of threads inside blocks */
	matrixMultiplication<<<dimGrid,dimBlock>>>(cuda_inputA, cuda_inputB, cuda_output, m, n, k);
	printf("finished matrix multiplcaiton! \n");

	printf("bringing result from gpu to device....\n");
	cudaMemcpy(output, cuda_output, sizeO, cudaMemcpyDeviceToHost);
	printf("done!\n");
	printf("Matrix A: \n");
	/*initialize cuPrintf */

	cudaPrintfInit();
	printMatrixCuda<<<1,1>>>(cuda_inputA, m, n);
	cudaPrintfDisplay(NULL, true);
	cudaPrintfEnd();
//	printMatrix((double*)inputA, m, n);
	printf("Matrix B\n");
	cudaPrintfInit();
	printMatrixCuda<<<1,1>>>(cuda_inputB, n, k);
	cudaPrintfDisplay(NULL, true);
	cudaPrintfEnd();
//	printMatrix((double*)inputB, n, k);
	printf("Matrix Out: \n");
	cudaPrintfInit();
	printMatrixCuda<<<1,1>>>(cuda_output, m, k);
	cudaPrintfDisplay(NULL, true);
	cudaPrintfEnd();
//	printMatrix((double*)output, m, k);
	//cudaPrintfEnd();
	return 0;
}

void printMatrix(double *C, int M, int N)
{
	assert(C);
	for(int i = 0; i < M; i++)
	{
		for(int j = 0; j < N; j++)
		{
			printf("%f ", *((C+i*N) + j));
		}
		printf("\n");
	}
}

