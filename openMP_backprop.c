#include <assert.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "chessAI/matrix.h"
#include "neuralnet.h"

void backpropAuto(AUTOW *  weights,
                  AUTOL *  autolayer,
                  DECODEW *decodeweights,
                  DECODEL *decodeweights,
                  AUTOW *  autograd,
                  DECODE * decodegrad,
                  int      stage)
{
}