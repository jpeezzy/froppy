#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "matrix.h"
#include "neuralnet.h"

void matrixMultiplication(double *h_a, double *h_b, double *h_result, int m, int n, int k) 
{
	int	tid, nthreads, chunk;
	tid = nthreads = chunk = 0;
	chunk = 1;                    /* set loop iteration chunk size */
	int i =0;
	int j = 0;
	int h = 0;

#pragma omp parallel shared(h_a, h_b, h_result) private(i,j,h)
	{
	#pragma omp for schedule(static, chunk)
			for (i =0; i < m; ++i) 
			{
				for (j = 0; j < k; ++j) 
				{
					h_result[i*k + j] = 0.0;
					for (h = 0; h < n; ++h) 
					{   
						h_result[i*k+j]= h_result[i*k+j] +  h_a[i * n + h] * h_b[h * k + j];
					}
				}
			}
	}
} 

void printMatrix(double *C, int M, int N)
{
	assert(C);
	for(int i = 0; i < M; i++)
	{
		for(int j = 0; j < N; j++)
		{
            if(C[i*N+j] != C[i*N+j])
            {
                printf("\n nandetected");
            }
			printf("%f ", *((C+i*N) + j));
		}
		printf("\n");
	}
}

double *transposeMatrix(double *matrix, int row, int column)
{
	double* temp = (double*)malloc(column*row*sizeof(double*));
	
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

void combine3Matrix(double*a, double*b,double*c, double*combine, int rowa, int rowb, int rowc)
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

void mm(double *h_a, double *h_b, double *h_result, int m, int n, int k) 
{
    for (int i = 0; i < m; ++i) 
    {
        for (int j = 0; j < k; ++j) 
        {
            double tmp = 0.0;
            for (int h = 0; h < n; ++h) 
            {
                tmp += h_a[i * n + h] * h_b[h * k + j];
            }
            h_result[i * k + j] = tmp;
        }
    }
}


void TMatrix(double *A, double *B, int m, int n)
{
    /* set loop iteration chunk size */
#pragma omp parallel
	{
	#pragma omp for schedule(static, 1) 
		for(int i =0; i < m; i++)
		{
			for(int j = 0; j < n; j++)
			{
				B[j*m + i] = A[i*n + j];
			}
		}
	}
}


void matrixSubtraction(double *h_a, double *h_b, double *h_result, int m, int n) 
{

#pragma omp parallel
	{
	#pragma omp for schedule(static, 1)
			for (int i = 0; i < m; ++i) 
			{
				for (int j = 0; j < n; ++j) 
				{
				    h_result[i*n+j] = h_a[i*n+j] - h_b[i*n+j];
				}
			}
	}
} 

void matrixScalarMul(double *h_a, double *h_b, double *h_result, int m, int n) 
{
	int	tid, nthreads, chunk;
	tid = nthreads = chunk = 0;
	chunk = 10;                    /* set loop iteration chunk size */

#pragma omp parallel
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

void matrixDelta(double *e, double *layer, double *delta, int m, int n)
{
#pragma omp parallel
	{
	#pragma omp for schedule(static, 1)
			for (int i = 0; i < m; ++i) 
			{
                
				for (int j = 0; j < n; ++j) 
				{
				    delta[i*n+j]  = e[i*n+j] * reluActivation(layer[i*n+j],1);
				}
			}
	}
}


void autoencoderE(double *a, double *b) 
{
    double te = 0.0;
    int j;


    #pragma omp parallel for reduction(+:te)    
	for ( j = 0; j < 773; ++j) 
	{
		te += (1.0/2.0) * pow((a[j] - b[j]),2);
	}

    printf("\n %.5f",te);

} 

// not real matirx add has a multiplier
void matrixAddition(double *h_a, double *h_b, int m, int n) 
{
#pragma omp parallel
	{
	#pragma omp for schedule(static, 1)
			for (int i = 0; i < m; ++i) 
			{
				for (int j = 0; j < n; ++j) 
				{
				    h_a[i*n+j] = h_a[i*n+j] + (1.0/200.0) *  h_b[i*n+j];
				}
			}
	}
} 


void matrixZero(double *h_a,  int m, int n) 
{

#pragma omp parallel
	{
	#pragma omp for schedule(static, 1)
			for (int i = 0; i < m; ++i) 
			{
				for (int j = 0; j < n; ++j) 
				{
				    h_a[i*n+j] = 0.0;
				}
			}
	}
}


void matrixCopy(double *h_a, double *h_b, int m, int n) 
{
#pragma omp parallel
	{
	#pragma omp for schedule(static, 1)
			for (int i = 0; i < m; ++i) 
			{
				for (int j = 0; j < n; ++j) 
				{
				    h_a[i*n+j] = h_b[i*n+j];
				}
			}
	}
}
