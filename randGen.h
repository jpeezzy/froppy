/*********************************************************************/
/*																	 */
/* randGen.h: header file for randGen.c                              */
/*                                                                   */
/* Author: JAB      Date: 1/24/2018                                  */
/*                                                                   */
/* Comments: edited 1/26/2018 for neuralnet compatibility (nc)       */
/*                                                                   */
/*********************************************************************/
	
#ifndef RANDGEN_H
#define RANDGEN_H

/* randGen() global variables */
extern uint64_t multVal, seedVal, incrVal;
extern uint32_t rnd;

/* neuralnet pseudo-global-input variables (nc testing)*/
extern int m, n, f;
extern float *A;

/* function(s) */
float randGen();
void randReluArray_edited(float *A, int m, int n, int f);


#endif