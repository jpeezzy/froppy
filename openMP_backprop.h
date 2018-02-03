#ifndef _OPENMP_BACKPROP_
#define _OPENMP_BACKPROP_

// calculate the derivative of error with respect to the output in the last
// layer
void calerrorOuputO(double* output, double* truth, double* res, int length);

// calculate derivative of error with repsect to output, take in weights and
// derivative of error with respect to input of next layer as well as length of
// the current node and length of the next node
void calerrorOuput(
    double* weight, double* nexterrorVAl, double* res, int lenCur, int lenNex);

// calculate the derivative of error w.r.t to value of a node
// parameters: the current layer output, derivative of error w.r.t output
void calerrorVal(double* layerVal, double* errorOutput, double* res, int length);

// calculate the gradient descent
// parameters: output and weight of the current layer and derivative of error
// w.r.t to val of next layer
void calgrad(
    double* prevOutput, double* errorVal, double* res, int lenCur, int lenPrev);

void printOutError(double* error, int lenght);
#endif