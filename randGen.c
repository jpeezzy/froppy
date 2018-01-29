/*********************************************************************/
/* randGen.c: random number generator (RNG) from the PCG family      */
/*                                                                   */
/* Author: JAB      Date: 1/24/2018                                  */
/*                                                                   */
/* Comments: 100% of this code is the reimplementation of the 	     */
/*	     PCG_XSH_RR RNG presented on pg 44 of this paper:        */
/*	     http://www.pcg-random.org/pdf/hmc-cs-2014-0905.pdf      */
/*           as well as the minimal C implementation package 	     */
/*	     provided for download via     	                     */
/*           http://www.pcg-random.org/download.html		     */
/*      	                                                     */
/*           edited 1/26/2018 for neuralnet compatibility (nc)       */
/*                                                                   */
/*********************************************************************/

#include "randGen.h"

#define INCL_MAIN 0 /* toggle for including main */

/* global variables */
uint64_t multVal =  6364136223846793005ULL;
uint64_t seedVal =  9600629759793949339ULL;  
uint64_t incrVal = 15726070495360670683ULL;

uint32_t rnd = 0;

/* ----------------------------------------------------------------- */
/* ----------------------------------------------------------------- */
double randGen()
{
    /* generate (sequential) seed value */
    seedVal = seedVal > UINT64_MAX ? UINT64_MAX : seedVal+time(NULL); 

    /* lin congr gen (LCG) part -  var 'state' on pg 44 of PCG paper */
    uint64_t lcgVal = multVal*seedVal + incrVal;
	
    /* rest of PCG_XSH_RR from minimal C implementation PCG package */
    uint32_t xorShiftVal = ((lcgVal >> 18u) ^ lcgVal) >> 27u;
    uint32_t rotVal = lcgVal >> 59u;

    rnd = (xorShiftVal >> rotVal) | (xorShiftVal << ((-rotVal) & 31));
	
    return (double) rnd/UINT32_MAX;
	
} /* end uint32_t rand_PCG_XSH_RR() */


double randGenerate()
{
    /* generate (sequential) seed value */
    seedVal = seedVal > UINT64_MAX ? UINT64_MAX : seedVal+time(NULL); 

    /* lin congr gen (LCG) part -  var 'state' on pg 44 of PCG paper */
    uint64_t lcgVal = multVal*seedVal + incrVal;
	
    /* rest of PCG_XSH_RR from minimal C implementation PCG package */
    uint32_t xorShiftVal = ((lcgVal >> 18u) ^ lcgVal) >> 27u;
    uint32_t rotVal = lcgVal >> 59u;

    rnd = (xorShiftVal >> rotVal) | (xorShiftVal << ((-rotVal) & 31));
	
    return (double) rnd;
	
} /* end uint32_t rand_PCG_XSH_RR() */


/* ----------------------------------------------------------------- */
/* ----------------------------------------------------------------- */

/*********************************************************************/
/*********************************************************************/
#if INCL_MAIN == 1

int main() 
{
    int iters = 10;
	
    printf("\n");
    for(int i=0; i<iters; i++)
    {   
        printf("iter. %2d: randGen() = %0.12f\n", i+1, randGen() );
    }
    printf("\n");
	
    return 0;

} /* end main() */

#endif
/*********************************************************************/
/*********************************************************************/

/* EOF */
