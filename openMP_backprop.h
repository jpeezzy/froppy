#ifndef _OPENMP_BACKPROP_
#define _OPENMP_BACKPROP_

// calculate the derivative of error with respect to the output in the last
// layer
void calerrorOuputO(float* output, float* truth, float* res, int length);

// calculate derivative of error with repsect to output, take in weights and
// derivative of error with respect to input of next layer as well as length of
// the current node and length of the next node
void calerrorOuput(
    float* weight, float* nexterrorVAl, float* res, int lenCur, int lenNex);

// calculate the derivative of error w.r.t to value of a node
// parameters: the current layer output, derivative of error w.r.t output
void calerrorVal(float* layerVal, float* errorOutput, float* res, int length);

// calculate the gradient descent
// parameters: output of previous layer, derivative of error w.r.t value
void calgrad(
    float* prevOutput, float* errorVal, float* res, int lenCur, int lenPrev);

void printOutError(float* error, int lenght);
#endif