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
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "randGen.h"

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

/*********************************************************************/
/*********************************************************************/
/*
int main() 
{
	int iters = 10;
	
	printf("\n");
	for(int i=0; i<iters; i++)
	{   
        printf("iter. %2d: randGen() = %0.12f\n", i+1, randGen() );
	} 
	
	return 0;

}*/ /* end main() */
/*********************************************************************/
/*********************************************************************/

/* EOF */
