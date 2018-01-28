#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "matrix.h"

void matrixMultiplication(long double *h_a, long double *h_b, long double *h_result, int m, int n, int k) 
{
	int	tid, nthreads, chunk;
	tid = nthreads = chunk = 0;
	chunk = 10;                    /* set loop iteration chunk size */

#pragma omp parallel num_threads(56)
	{
		tid = omp_get_thread_num();
		if (tid == 0)
		{
			nthreads = omp_get_num_threads();
		}
        /*printf("Thread %d starting matrix multiply...\n"i,tid);*/
	#pragma omp for schedule(static, chunk)
			for (int i = 0; i < m; ++i) 
			{
				for (int j = 0; j < k; ++j) 
				{
					for (int h = 0; h < n; ++h) 
					{
						
						h_result[i * k + j] += h_a[i * n + h] * h_b[h * k + j];
					}
				}
			}
	}
} 

void printMatrix(long double *C, int M, int N)
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

long double *transposeMatrix(long double *matrix, int row, int column)
{
	long double* temp = (long double*)malloc(column*row*sizeof(long double*));
	
	int	tid, nthreads, chunk;
	tid = nthreads = chunk = 0;
	chunk = 10;                    /* set loop iteration chunk size */
#pragma omp parallel shared(nthreads,chunk) private(tid)
	{
		tid = omp_get_thread_num();
		if (tid == 0)
		{
			nthreads = omp_get_num_threads();
			printf("Starting matrix multiple example with %d threads\n",nthreads);
			printf("Initializing matrices...\n");
		}
	#pragma omp for schedule (static, chunk) 
		for(int i =0; i < row; i++)
		{
			for(int j = 0; j < column; j++)
			{
				temp[j*row + i] = matrix[i*column + j];
			}
		}
	}
	return temp;
}

void combine3Matrix(long double*a, long double*b,long double*c, long double*combine, int rowa, int rowb, int rowc)
{
	assert(a); assert(b); assert(c); assert(combine);
	if(sizeof(combine) < (sizeof(a)+ sizeof(b) + sizeof(c)))
	{
		printf("combine array size is less than the total space needed... Exiting");
	}
	int total = rowa+rowb+rowc;
	/*here we assume its a matrix mx1*/
	for(int i = 0; i < total; i++)
	{
		combine[i] = (i >= 0 && i < rowa) ? a[i] : 
				(i >= rowa && i < (rowa+rowb)) ? b[i-rowa]:
			(i >= (total - rowc) && i < (total)) ? c[i-rowa-rowb] : 0;
	}
}





