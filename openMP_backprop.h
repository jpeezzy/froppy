#ifndef _OPENMP_BACKPROP_
#define _OPENMP_BACKPROP_

// calculate the derivative of error with respect to the output in the last
// layer
void calerrorOuputO(float* output, float* truth, float* res);

// calculate derivative of error with repsect to output, take in weights and
// derivative of error with respect to input of next layer
void calerrorOuput(float* weight, float* errorVAl, float* res);

#endif