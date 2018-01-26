#include <assert.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "chessAI/chessAI-master/matrix.h"
#include "neuralnet.h"

#define CORE_NUM 56

void backpropAuto(AUTOW*   autoweights,
                  AUTOL*   autolayer,
                  DECODEW* decodeweights,
                  DECODEL* decodelayer,
                  AUTOW*   autograd,
                  DECODEL* decodegrad,
                  int      stage);

void calerrorOuputO(float* output, float* truth, float* res, int length)
{
  assert(output);
  assert(truth);
  assert(res);
#pragma omp parallel num_threads(CORE_NUM)
  for (int i = 0; i < length; ++i)
    {
      res[i] = output[i] - truth[i];
    }
}

// calculate derivative of error with repsect to output, take in weights and
// derivative of error with respect to input of next layer as well as length of
// the current node and length of the next node
void calerrorOuput(
    float* weight, float* nexterrorVAl, float* res, int lenCur, int lenNex)
{
  assert(nexterrorVAl);
  assert(res);
  float* temp = NULL;
  transposeMatrix(weight, lenNex, lenCur);
}

// calculate the derivative of error w.r.t to value of a node
// parameters: the current layer value, derivative of error w.r.t output
void calerrorVal(float* layerVal, float* errorOutput, float* res, int length)
{
  assert(layerVal);
  assert(errorOutput);
  assert(res);
}

// calculate the gradient descent
// parameters: output of previous layer, derivative of error w.r.t value
void calgrad(
    float* prevOutput, float* errorVal, float* res, int lenCur, int lenPrev)
{
  assert(prevOutput);
  assert(errorVal);
  assert(res);
}