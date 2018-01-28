#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "matrix.h"
#include "neuralnet.h"

void matrixMultiplication(float *h_a, float *h_b, float *h_result, int m, int n, int k) 
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

void printMatrix(float *C, int M, int N)
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

float *transposeMatrix(float *matrix, int row, int column)
{
	float* temp = (float*)malloc(column*row*sizeof(float*));
	
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

void combine3Matrix(float*a, float*b,float*c, float*combine, int rowa, int rowb, int rowc)
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

void mm(float *h_a, float *h_b, float *h_result, int m, int n, int k) 
{
    for (int i = 0; i < m; ++i) 
    {
        for (int j = 0; j < k; ++j) 
        {
            float tmp = 0.0;
            for (int h = 0; h < n; ++h) 
            {
                tmp += h_a[i * n + h] * h_b[h * k + j];
            }
            h_result[i * k + j] = tmp;
        }
    }
}


void TMatrix(float *A, float *B, int m, int n)
{
	
	int	tid, nthreads, chunk;
	tid = nthreads = chunk = 0;
	chunk = 10;                    /* set loop iteration chunk size */
#pragma omp parallel shared(nthreads,chunk) private(tid)
	{
		tid = omp_get_thread_num();
		if (tid == 0)
		{
			nthreads = omp_get_num_threads();
		}
	#pragma omp for schedule (static, chunk) 
		for(int i =0; i < m; i++)
		{
			for(int j = 0; j < n; j++)
			{
				B[j*m + i] = A[i*n + j];
			}
		}
	}
}


void matrixSubtraction(float *h_a, float *h_b, float *h_result, int m, int n) 
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
				for (int j = 0; j < n; ++j) 
				{
				    h_result[i*n+j] = h_a[i*n+j] - h_b[i*n+j];
				}
			}
	}
} 

void matrixScalarMul(float *h_a, float *h_b, float *h_result, int m, int n) 
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
				for (int j = 0; j < n; ++j) 
				{
				    h_result[i*n+j] = h_a[i*n+j] * h_b[i*n+j];
				}
			}
	}
}

void matrixDelta(float *e, float *layer, float *delta, int m, int n)
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
				for (int j = 0; j < n; ++j) 
				{
				    delta[i*n+j] = e[i*n+j] * reluActivation(layer[i*n+j],1);
				}
			}
	}
}


void autoencoderE(float *a, float *b) 
{
    float te;
	for (int i = 0; i < 1; ++i) 
	{
	    for (int j = 0; j < 773; ++j) 
		{
		    te += (1.0/2.0) * pow((a[i*773+j] - b[i*773+j]),2);
		}
	}
    printf("\n %.5f",te);
} 



