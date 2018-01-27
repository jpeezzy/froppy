#include "neuralnet.h"
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix.h"
#include "randGen.h"

float eta     = 0.001;
float epsilon = 10e-8;
float beta1   = 0.9;
float beta2   = 0.999;

AUTOW *createAutoW(void)
{
  AUTOW *temp = malloc(sizeof(AUTOW));
  return temp;
}
DECODEW *createDecoW(void)
{
  DECODEW *temp = malloc(sizeof(DECODEW));
  return temp;
}
AUTOL *createAutoL(void)
{
  AUTOL *temp = malloc(sizeof(AUTOL));
  return temp;
}
DECODEL *createDecoL(void)
{
  DECODEL *temp = malloc(sizeof(DECODEL));
  return temp;
}
// Calculates the relu activation of a single input 0 is normal 1 is deriv
float reluActivation(float x, int flag)
{
  if (flag == 0)
    {
      if (x > 0)
        {
          return x;
        }
      else
        {
          return 0.0;
        }
    }
  else if (flag == 1)
    {
      if (x > 0)
        {
          return 1.0;
        }
      else
        {
          return 0.0;
        }
    }
  return 10;
}

float tanhActivation(float x, int flag)
{
  if (flag == 0)
    {
      return (1.0 / (1.0 + exp(-x)));
    }
  else if (flag == 1)
    {
      return (x * (1.0 - x));
    }
  return 10;
}

// Applies relu activation to an array
void reluArray(float *array, int m, int n, int flag)
{
  int i, j;
  int tid, nthreads, chunk;
  chunk = 10;
#pragma omp parallel
  {
    tid = omp_get_thread_num();
    if (tid == 0)
      {
        nthreads = omp_get_num_threads();
      }
#pragma omp for schedule(static, chunk)
    for (i = 0; i < m; ++i)
      {
        for (j = 0; j < n; ++j)
          {
            array[i * j + n] = reluActivation(array[i * j + n], flag);
          }
      }
  }
}

// Applies tanh activation to an array
void tanhArray(float *array, int m, int n, int flag)
{
  int i, j;
  int tid, nthreads, chunk;
  chunk = 10;
#pragma omp parallel
  {
    tid = omp_get_thread_num();
    if (tid == 0)
      {
        nthreads = omp_get_num_threads();
      }
#pragma omp for schedule(static, chunk)
    for (i = 0; i < m; ++i)
      {
        for (j = 0; j < n; ++j)
          {
            array[i * j + n] = tanhActivation(array[i * j + n], flag);
          }
      }
  }
}

// Nadam for a single part
void nadam(float *w, float *g, float *m, float *v, int t)
{
  // Update the Momentum and Variance Vectors
  *(m) = (*(m)) * beta1 + (1 - beta1) * (*(g));
  *(v) = (*(v)) * beta2 + (1 - beta2) * (*(g)) * (*(g));

  // Calculate the new weight
  *(w) = *(w) - (eta / (sqrt((*(v)) / (1 - pow(beta1, t)))) + epsilon) *
                    (beta1 * ((*(m)) / (1 - pow(beta1, t))) +
                     ((1 - beta1) * (*(g))) / (1 - pow(beta1, t)));
}

// nadam Array
void nadamArray(float *W, float *G, float *M, float *V, int m, int n, int t)
{
  int i, j;
  int chunk, nthreads, tid;
  chunk = 10;
#pragma omp parallel
  {
    tid = omp_get_thread_num();
    if (tid == 0)
      {
        nthreads = omp_get_num_threads();
      }
#pragma omp for schedule(static, chunk)
    for (i = 0; i < m; ++i)
      {
        for (j = 0; j < n; ++j)
          {
            nadam(&(W[i * n + j]),
                  &(G[i * n + j]),
                  &(M[i * n + j]),
                  &(V[i * n + j]),
                  t);
          }
      }
  }
}

void nadamAuto(AUTOW *  autoweights,
               DECODEW *decodeweights,
               AUTOW *  autograd,
               DECODEW *decodegrad,
               AUTOW *  autoM,
               AUTOW *  autoV,
               DECODEW *decodeM,
               DECODEW *decodeV,
               int      t,
               int      stage)
{
  int tid, nthreads, chunk;
  chunk = 10;
#pragma omp parallel
  {
    tid = omp_get_thread_num();
    if (tid == 0)
      {
        nthreads = omp_get_num_threads();
      }

    // stage 1 of the auto encoder
    if (stage == 1)
      {
        nadamArray((float *)autoweights->weight0,
                   (float *)autograd->weight0,
                   (float *)autoM->weight0,
                   (float *)autoV->weight0,
                   773,
                   600,
                   t);
        nadamArray((float *)decodeweights->weight3,
                   (float *)decodegrad->weight3,
                   (float *)decodeM->weight3,
                   (float *)decodeV->weight3,
                   600,
                   773,
                   t);
      }

    // stage 2 of the auto encoder
    if (stage == 2)
      {
        nadamArray((float *)autoweights->weight1,
                   (float *)autograd->weight1,
                   (float *)autoM->weight1,
                   (float *)autoV->weight1,
                   600,
                   400,
                   t);
        nadamArray((float *)decodeweights->weight2,
                   (float *)decodegrad->weight2,
                   (float *)decodeM->weight2,
                   (float *)decodeV->weight2,
                   400,
                   600,
                   t);
      }

    // stage 3 of the auto encoder
    if (stage == 3)
      {
        nadamArray((float *)autoweights->weight2,
                   (float *)autograd->weight2,
                   (float *)autoM->weight2,
                   (float *)autoV->weight2,
                   400,
                   200,
                   t);
        nadamArray((float *)decodeweights->weight1,
                   (float *)decodegrad->weight1,
                   (float *)decodeM->weight1,
                   (float *)decodeV->weight1,
                   200,
                   400,
                   t);
      }

    // stage 4 of the auto encoder
    if (stage == 4)
      {
        nadamArray((float *)autoweights->weight3,
                   (float *)autograd->weight3,
                   (float *)autoM->weight3,
                   (float *)autoV->weight3,
                   200,
                   100,
                   t);
        nadamArray((float *)decodeweights->weight0,
                   (float *)decodegrad->weight0,
                   (float *)decodeM->weight0,
                   (float *)decodeV->weight0,
                   100,
                   200,
                   t);
      }
  }
}

// foward propagation
void fowardpropAuto(AUTOW *  autoweights,
                    AUTOL *  autolayer,
                    DECODEW *decodeweights,
                    DECODEL *decodelayer,
                    int      stage)
{
  // foward prop when the autoencoder is in stage 1
  if (stage == 1)
    {
      matrixMultiplication((float *)autolayer->input,
                           (float *)autoweights->weight0,
                           (float *)autolayer->layer1,
                           1,
                           773,
                           600);
      reluArray((float *)autolayer->layer1, 1, 600, 0);

      matrixMultiplication((float *)autolayer->layer1,
                           (float *)decodeweights->weight3,
                           (float *)decodelayer->output,
                           1,
                           600,
                           773);
      reluArray((float *)decodelayer->output, 1, 773, 0);
      int ii;
      for(ii=0; ii < 773; ++ii)
      {
          printf("%.4f \n",decodelayer->output[0][ii]);
      }
    }

  // foward prop when the autoencoder is in stage 2
  if (stage == 2)
    {
      matrixMultiplication((float *)autolayer->input,
                           (float *)autoweights->weight0,
                           (float *)autolayer->layer1,
                           1,
                           773,
                           600);
      reluArray((float *)autolayer->layer1, 1, 600, 0);

      matrixMultiplication((float *)autolayer->layer1,
                           (float *)autoweights->weight1,
                           (float *)autolayer->layer2,
                           1,
                           600,
                           400);
      reluArray((float *)autolayer->layer2, 1, 400, 0);

      matrixMultiplication((float *)autolayer->layer2,
                           (float *)decodeweights->weight2,
                           (float *)decodelayer->layer3,
                           1,
                           400,
                           600);
      reluArray((float *)decodelayer->layer3, 1, 600, 0);

      matrixMultiplication((float *)decodelayer->layer3,
                           (float *)decodeweights->weight3,
                           (float *)decodelayer->output,
                           1,
                           600,
                           773);
      reluArray((float *)decodelayer->output, 1, 773, 0);
    }

  // foward prop at stage 3
  if (stage == 3)
    {
      matrixMultiplication((float *)autolayer->input,
                           (float *)autoweights->weight0,
                           (float *)autolayer->layer1,
                           1,
                           773,
                           600);
      reluArray((float *)autolayer->layer1, 1, 600, 0);

      matrixMultiplication((float *)autolayer->layer1,
                           (float *)autoweights->weight1,
                           (float *)autolayer->layer2,
                           1,
                           600,
                           400);
      reluArray((float *)autolayer->layer2, 1, 400, 0);

      matrixMultiplication((float *)autolayer->layer2,
                           (float *)autoweights->weight2,
                           (float *)autolayer->layer3,
                           1,
                           400,
                           200);
      reluArray((float *)autolayer->layer3, 1, 200, 0);

      matrixMultiplication((float *)autolayer->layer3,
                           (float *)decodeweights->weight1,
                           (float *)decodelayer->layer2,
                           1,
                           200,
                           400);
      reluArray((float *)decodelayer->layer2, 1, 400, 0);

      matrixMultiplication((float *)decodelayer->layer2,
                           (float *)decodeweights->weight2,
                           (float *)decodelayer->layer3,
                           1,
                           400,
                           600);
      reluArray((float *)decodelayer->layer3, 1, 600, 0);

      matrixMultiplication((float *)decodelayer->layer3,
                           (float *)decodeweights->weight3,
                           (float *)decodelayer->output,
                           1,
                           600,
                           773);
      reluArray((float *)decodelayer->output, 1, 773, 0);
    }

  // foward prop at stage 4
  if (stage == 4)
    {
      matrixMultiplication((float *)autolayer->input,
                           (float *)autoweights->weight0,
                           (float *)autolayer->layer1,
                           1,
                           773,
                           600);
      reluArray((float *)autolayer->layer1, 1, 600, 0);

      matrixMultiplication((float *)autolayer->layer1,
                           (float *)autoweights->weight1,
                           (float *)autolayer->layer2,
                           1,
                           600,
                           400);
      reluArray((float *)autolayer->layer2, 1, 400, 0);

      matrixMultiplication((float *)autolayer->layer2,
                           (float *)autoweights->weight2,
                           (float *)autolayer->layer3,
                           1,
                           400,
                           200);
      reluArray((float *)autolayer->layer3, 1, 200, 0);

      matrixMultiplication((float *)autolayer->layer3,
                           (float *)autoweights->weight3,
                           (float *)autolayer->output,
                           1,
                           200,
                           100);
      reluArray((float *)autolayer->output, 1, 100, 0);

      matrixMultiplication((float *)autolayer->output,
                           (float *)decodeweights->weight0,
                           (float *)decodelayer->layer1,
                           1,
                           100,
                           200);
      reluArray((float *)decodelayer->layer1, 1, 200, 0);

      matrixMultiplication((float *)decodelayer->layer1,
                           (float *)decodeweights->weight1,
                           (float *)decodelayer->layer2,
                           1,
                           200,
                           400);
      reluArray((float *)decodelayer->layer2, 1, 400, 0);

      matrixMultiplication((float *)decodelayer->layer2,
                           (float *)decodeweights->weight2,
                           (float *)decodelayer->layer3,
                           1,
                           400,
                           600);
      reluArray((float *)decodelayer->layer3, 1, 600, 0);

      matrixMultiplication((float *)decodelayer->layer3,
                           (float *)decodeweights->weight3,
                           (float *)decodelayer->output,
                           1,
                           600,
                           773);
      reluArray((float *)decodelayer->output, 1, 773, 0);
    }
}

void randReluArray(float *A, int m, int n, int f)
{
  int i, j;
  srand(time(NULL));
  for (i = 0; i < m; ++i)
    {
      for (j = 0; j < n; ++j)
        {
          A[i * n + j] = ((float)rand()/(float)(RAND_MAX)) * (sqrt(6.0) / f);
        }
    }
}
