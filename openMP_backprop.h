#ifndef _OPENMP_BACKPROP_
#define _OPENMP_BACKPROP_

// calculate the derivative of error with respect to the output in the last
// layer
void calerrorOuputO(float* output, float* truth, float* res);

// calculate derivative of error with repsect to output, take in weights and
// derivative of error with respect to input of next layer
void calerrorOuput(float* weight, float* nexterrorVAl, float* res);

// calculate the derivative of error w.r.t to value of a node
// parameters: the current layer value, derivative of error w.r.t output
void calerrorVal(float* layerVal, float* errorOutput, float* res);

// calculate the gradient descent
// parameters: output of previous layer, derivative of error w.r.t value
void calgrad(float* prevOutput, float* errorVal, float* res);
#endif