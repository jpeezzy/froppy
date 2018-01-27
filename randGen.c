/*********************************************************************/
/* randGen_fini.c: random number generator (RNG) from the PCG family */
/*                                                                   */
/* Author: JAB      Date: 1/24/2018                                  */
/*                                                                   */
/* Comments: 100% of this code is the reimplementation of the 	     */
/*			 PCG_XSH_RR RNG presented on pg 44 of this paper: 		 */
/*			 http://www.pcg-random.org/pdf/hmc-cs-2014-0905.pdf      */
/*           as well as the minimal C implementation package 	     */
/*			 provided for download via     						     */
/*           http://www.pcg-random.org/download.html			     */
/*      	                                                         */
/*           edited 1/26/2018 for neuralnet compatibility (nc)       */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <math.h> /* for neuralnet compatibility testing (nct) */
#include "randGen.h"

/* global variables */
uint64_t multVal =  6364136223846793005ULL;
uint64_t seedVal =  9600629759793949339ULL;  
uint64_t incrVal = 15726070495360670683ULL;

uint32_t rnd = 0;

/* neuralnet pseudo-global-input variables (nc) */
int m = 8, n = 8, f = 773+600;
float *A = NULL;


/* ----------------------------------------------------------------- */
/* ----------------------------------------------------------------- */
float randGen()
{
    /* generate (sequential) seed value */
    seedVal = seedVal > UINT64_MAX ? UINT64_MAX : seedVal+time(NULL); 

	/* lin congr gen (LCG) part -  var 'state' on pg 44 of PCG paper */
	uint64_t lcgVal = multVal*seedVal + incrVal;
	
	/* rest of PCG_XSH_RR from minimal C implementation PCG package */
	uint32_t xorShiftVal = ((lcgVal >> 18u) ^ lcgVal) >> 27u;
	uint32_t rotVal = lcgVal >> 59u;

	rnd = (xorShiftVal >> rotVal) | (xorShiftVal << ((-rotVal) & 31));
	
	return (float) rnd/UINT32_MAX;
	
} /* end uint32_t rand_PCG_XSH_RR() */
/* ----------------------------------------------------------------- */
/* ----------------------------------------------------------------- */

/* ----------------------------------------------------------------- */
/* ----------------------------------------------------------------- */
void randReluArray_edited(float *A, int m, int n, int f)/* (for nct) */
{
	int i, j;
	A = malloc(m*n*sizeof(float));
	/*srand(time(NULL));*/
	printf("\nA:\n");
	for (i = 0; i < m; ++i)
    {
		for (j = 0; j < n; ++j)
        {
			A[i * n + j] = randGen() * (sqrt(6.0) / f);
			/*printf("A[i*n+j] = A[%2d*%2d+%2d] = A[%2d] = %8f\n",
			       i, n, j, i*n+j, A[i*n+j] );*/
		    printf("%8f ", A[i*n+j]);
        }
		printf("\n");
    }
	
	free(A); /* passes valgrind */
	
}
/* ----------------------------------------------------------------- */
/* ----------------------------------------------------------------- */

/*********************************************************************/
/*********************************************************************/
int main() 
{
	int iters = 10;
	
	printf("\n");
	for(int i=0; i<iters; i++)
	{   
        printf("iter. %2d: randGen() = %0.12f\n", i+1, randGen() );
	}
	randReluArray_edited(A, m, n, f);
	
	/*free(A); oops, wrong place */
	
	printf("\n");
	
	return 0;

} /* end main() */
/*********************************************************************/
/*********************************************************************/

/* EOF */
