/*********************************************************************/
/*						                     */
/* randGen.h: header file for randGen.c                              */
/*                                                                   */
/* Author: JAB      Date: 1/24/2018                                  */
/*                                                                   */
/* Comments: edited 1/26/2018 for neuralnet compatibility (nc)       */
/*                                                                   */
/*********************************************************************/
	
#ifndef RANDGEN_H
#define RANDGEN_H

/* other header files req'd for corres. C code */
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h> 
#include <time.h>

/* randGen() global variables */
extern uint64_t multVal, seedVal, incrVal;
extern uint32_t rnd;

/* function(s) */
double randGen();
double randGenerate();

#endif
