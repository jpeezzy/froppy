#include <assert.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "chessAI/chessAI-master/matrix.h"
#include "neuralnet.h"

#define CORE_NUM 56

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
  temp        = transposeMatrix(weight, lenNex, lenCur);
  matrixMultiplication(temp, nexterrorVAl, res, lenNex, lenCur, 1);
  free(temp);
}

// calculate the derivative of error w.r.t to value of a node
// parameters: the current layer value, derivative of error w.r.t output
void calerrorVal(float* layerVal, float* errorOutput, float* res, int length)
{
  assert(layerVal);
  assert(errorOutput);
  assert(res);
  reluArray(layerVal, length, 1, 1);
#pragma omp parallel num_threads(CORE_NUM)
  for (int i = 0; i < length; ++i)
    {
      res[i] = layerVal[i] * errorOutput[i];
    }
}

// calculate the gradient descent
// parameters: output of previous layer, derivative of error w.r.t value
void calgrad(
    float* prevOutput, float* errorVal, float* res, int lenCur, int lenPrev)
{
  assert(prevOutput);
  assert(errorVal);
  assert(res);
#pragma omp parallel num_threads(CORE_NUM)
  for (int i = 0; i < lenCur; ++i)
    {
      for (int j = 0; j < lenPrev; ++j)
        {
          res[i * lenPrev + j] = prevOutput[j] * errorVal[i];
        }
    }
}

void backpropAuto(AUTOW*   autoweights,
                  AUTOL*   autolayer,
                  DECODEW* decodeweights,
                  DECODEL* decodelayer,
                  AUTOW*   autograd,
                  DECODEL* decodegrad,
                  int      stage)
{
  // temporary variables for storing numbers between layers
  float* derErrorOutput0 = NULL;
  float* derErrorOutput  = NULL;
  float* derErrorVal     = NULL;
  float* derErrorOutput2 = NULL;
  float* derErrorVal2    = NULL;
  switch (stage)
    {
      case 1:
        // train decoder layer output
        derErrorOutput0 = (float*)malloc(773 * sizeof(float));
        calerrorOuputO(
            decodelayer->output, autolayer->input, derErrorOutput0, 773);
        derErrorVal = (float*)malloc(773 * sizeof(float));
        calerrorVal(decodelayer->output, derErrorOutput0, derErrorVal, 773);
        calgrad(decodelayer->layer3, derErrorVal, decodegrad->output, 773, 600);

        // train input layer of encoder
        derErrorOutput = (float*)malloc(600 * sizeof(float));
        calerrorOuput(
            autoweights->weight0, derErrorVal, derErrorOutput, 600, 773);
        free(derErrorVal);
        derErrorVal = NULL;
        derErrorVal = (float*)malloc(600 * sizeof(float));
        calerrorVal(autolayer->input, derErrorOutput, derErrorVal, 600);
        calgrad(derErrorOutput0, derErrorVal, autograd->weight0, 600, 773);

        free(derErrorOutput0);
        free(derErrorOutput);
        free(derErrorVal);

        break;
      case 2:
        // train decoder layer output
        derErrorOutput0 = (float*)malloc(773 * sizeof(float));
        calerrorOuputO(
            decodelayer->output, autolayer->input, derErrorOutput0, 773);
        derErrorVal = (float*)malloc(773 * sizeof(float));
        calerrorVal(decodelayer->output, derErrorOutput0, derErrorVal, 773);

        // train layer 3 of decoder
        derErrorOutput = (float*)malloc(400 * sizeof(float));
        calerrorOuput(
            autoweights->weight1, derErrorVal, derErrorOutput, 400, 600);
        free(derErrorVal);
        derErrorVal = NULL;
        derErrorVal = (float*)malloc(600 * sizeof(float));
        calerrorVal(autolayer->layer1, derErrorOutput, derErrorVal, 400);
        calgrad(derErrorOutput0, derErrorVal, autograd->weight1, 400, 600);
        free(derErrorOutput);

        // train layer 1 of encoder
        derErrorOutput = (float*)malloc(400 * sizeof(float));
        calerrorOuput(
            autoweights->weight1, derErrorVal, derErrorOutput, 400, 600);
        free(derErrorVal);
        derErrorVal = NULL;
        derErrorVal = (float*)malloc(600 * sizeof(float));
        calerrorVal(autolayer->layer1, derErrorOutput, derErrorVal, 400);
        calgrad(derErrorOutput0, derErrorVal, autograd->weight1, 400, 600);

        // train input layer of encoder
        free(derErrorOutput);
        derErrorOutput = (float*)malloc(600 * sizeof(float));
        calerrorOuput(
            autoweights->weight0, derErrorVal, derErrorOutput, 600, 773);
        free(derErrorVal);
        derErrorVal = NULL;
        derErrorVal = (float*)malloc(600 * sizeof(float));
        calerrorVal(autolayer->input, derErrorOutput, derErrorVal, 600);
        calgrad(derErrorOutput0, derErrorVal, autograd->weight0, 600, 773);

        free(derErrorOutput0);
        free(derErrorOutput);
        free(derErrorVal);

        break;
      case 3:
        // train layer 2 of decoder
        derErrorOutput0 = (float*)malloc(400 * sizeof(float));
        calerrorOuputO(
            decodelayer->layer2, autolayer->layer2, derErrorOutput0, 400);
        derErrorVal = (float*)malloc(400 * sizeof(float));
        calerrorVal(decodelayer->layer2, derErrorOutput0, derErrorVal, 400);
        calgrad(decodelayer->layer1, derErrorVal, decodegrad->layer2, 400, 200);

        derErrorOutput = (float*)malloc(200 * sizeof(float));
        calerrorOuput(
            autoweights->weight3, derErrorVal, derErrorOutput, 200, 400);
        free(derErrorVal);
        derErrorVal = NULL;
        derErrorVal = (float*)malloc(200 * sizeof(float));
        calerrorVal(autolayer->layer2, derErrorOutput, derErrorVal, 200);
        calgrad(derErrorOutput0, derErrorVal, autograd->weight2, 200, 400);

        free(derErrorOutput0);
        free(derErrorOutput);
        free(derErrorVal);
        break;
      case 4:
        // train layer 1 of decoder
        derErrorOutput0 = (float*)malloc(200 * sizeof(float));
        calerrorOuputO(
            decodelayer->layer1, autolayer->layer3, derErrorOutput0, 200);
        derErrorVal = (float*)malloc(200 * sizeof(float));
        calerrorVal(decodelayer->layer1, derErrorOutput0, derErrorVal, 200);
        calgrad(autolayer->output, derErrorVal, decodegrad->layer1, 200, 100);

        derErrorOutput = (float*)malloc(100 * sizeof(float));
        calerrorOuput(
            autoweights->weight3, derErrorVal, derErrorOutput, 100, 200);
        free(derErrorVal);
        derErrorVal = NULL;
        derErrorVal = (float*)malloc(100 * sizeof(float));
        calerrorVal(autolayer->layer3, derErrorOutput, derErrorVal, 100);
        calgrad(derErrorOutput0, derErrorVal, autograd->weight3, 100, 200);

        free(derErrorOutput0);
        free(derErrorOutput);
        free(derErrorVal);
        break;
      default:
        break;
    }
}