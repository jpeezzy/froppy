#include "neuralnet.h"
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix.h"
#include "randGen.h"

long double eta     = 0.001;
long double epsilon = 0.00000001;
long double beta1   = 0.9;
long double beta2   = 0.999;

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
long double reluActivation(long double x, int flag)
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

long double tanhActivation(long double x, int flag)
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
void reluArray(long double *array, int m, int n, int flag)
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
            //printf(" \n %d %d", i, j);
            array[i * n + j] = reluActivation(array[i * n + j], flag);
          }
      }
  }
}

// Applies tanh activation to an array
void tanhArray(long double *array, int m, int n, int flag)
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
            array[i * n + j] = tanhActivation(array[i * n + j], flag);
          }
      }
  }
}

// Nadam for a single part
void nadam(long double *w, long double *g, long double *m, long double *v, int t)
{
  // Update the Momentum and Variance Vectors
  *(m) = (*(m)) * beta1 + (1.0 - beta1) * (*(g));
  *(v) = (*(v)) * beta2 + (1.0 - beta2) * (*(g)) * (*(g));
  
  long double vt = (*(v)) / (1.0 - pow(beta2,t));
  long double mt = (*(m)) / (1.0 - pow(beta1,t)); 

  // Calculate the new weight
  *(w) = *(w) + .001* (*(g));
  //*(w) = *(w) + ( (eta/(sqrt(vt) + epsilon)) * ((beta1*mt) + (((1.0-beta1)*(*(g)))/(1.0-pow(beta1,t))))); 
    //- (eta / ((sqrt((*(v)) / (1.0 - pow(beta1, t)))) + epsilon)) *
                    //(beta1 * ((*(m)) / (1.0 - pow(beta1, t))) +
                     //((1.0 - beta1) * (*(g))) / (1.0 - pow(beta1, t)));
  //printf("\n %.5f", *w);
}

// nadam Array
void nadamArray(long double *W, long double *G, long double *M, long double *V, int m, int n, int t)
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
          {/*
            nadam(&(W[i * n + j]),
                  &(G[i * n + j]),
                  &(M[i * n + j]),
                  &(V[i * n + j]),
                  t);*/
            W[i*n+j] = W[i*n+j] + .001 * G[i*n+j];
            //printf("\n %.5f",W[i*n+j]);
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
        nadamArray((long double *)autoweights->weight0,
                   (long double *)autograd->weight0,
                   (long double *)autoM->weight0,
                   (long double *)autoV->weight0,
                   773,
                   600,
                   t);
        nadamArray((long double *)decodeweights->weight3,
                   (long double *)decodegrad->weight3,
                   (long double *)decodeM->weight3,
                   (long double *)decodeV->weight3,
                   600,
                   773,
                   t);
      }

    // stage 2 of the auto encoder
    if (stage == 2)
      {
        nadamArray((long double *)autoweights->weight1,
                   (long double *)autograd->weight1,
                   (long double *)autoM->weight1,
                   (long double *)autoV->weight1,
                   600,
                   400,
                   t);
        nadamArray((long double *)decodeweights->weight2,
                   (long double *)decodegrad->weight2,
                   (long double *)decodeM->weight2,
                   (long double *)decodeV->weight2,
                   400,
                   600,
                   t);
      }

    // stage 3 of the auto encoder
    if (stage == 3)
      {
        nadamArray((long double *)autoweights->weight2,
                   (long double *)autograd->weight2,
                   (long double *)autoM->weight2,
                   (long double *)autoV->weight2,
                   400,
                   200,
                   t);
        nadamArray((long double *)decodeweights->weight1,
                   (long double *)decodegrad->weight1,
                   (long double *)decodeM->weight1,
                   (long double *)decodeV->weight1,
                   200,
                   400,
                   t);
      }

    // stage 4 of the auto encoder
    if (stage == 4)
      {
        nadamArray((long double *)autoweights->weight3,
                   (long double *)autograd->weight3,
                   (long double *)autoM->weight3,
                   (long double *)autoV->weight3,
                   200,
                   100,
                   t);
        nadamArray((long double *)decodeweights->weight0,
                   (long double *)decodegrad->weight0,
                   (long double *)decodeM->weight0,
                   (long double *)decodeV->weight0,
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
      matrixMultiplication((long double *)autolayer->input,
                           (long double *)autoweights->weight0,
                           (long double *)autolayer->layer1,
                           1,
                           773,
                           600);
      reluArray((long double *)autolayer->layer1, 1, 600, 0);

      matrixMultiplication((long double *)autolayer->layer1,
                           (long double *)decodeweights->weight3,
                           (long double *)decodelayer->output,
                           1,
                           600,
                           773);
      reluArray((long double *)decodelayer->output, 1, 773, 0);
      //int ii;
      //for(ii=0; ii < 773; ++ii)
      //{
      //    printf("%.4f \n",decodelayer->output[0][ii]);
      //}
    }

  // foward prop when the autoencoder is in stage 2
  if (stage == 2)
    {
      matrixMultiplication((long double *)autolayer->input,
                           (long double *)autoweights->weight0,
                           (long double *)autolayer->layer1,
                           1,
                           773,
                           600);
      reluArray((long double *)autolayer->layer1, 1, 600, 0);

      matrixMultiplication((long double *)autolayer->layer1,
                           (long double *)autoweights->weight1,
                           (long double *)autolayer->layer2,
                           1,
                           600,
                           400);
      reluArray((long double *)autolayer->layer2, 1, 400, 0);

      matrixMultiplication((long double *)autolayer->layer2,
                           (long double *)decodeweights->weight2,
                           (long double *)decodelayer->layer3,
                           1,
                           400,
                           600);
      reluArray((long double *)decodelayer->layer3, 1, 600, 0);

      matrixMultiplication((long double *)decodelayer->layer3,
                           (long double *)decodeweights->weight3,
                           (long double *)decodelayer->output,
                           1,
                           600,
                           773);
      reluArray((long double *)decodelayer->output, 1, 773, 0);
    }

  // foward prop at stage 3
  if (stage == 3)
    {
      matrixMultiplication((long double *)autolayer->input,
                           (long double *)autoweights->weight0,
                           (long double *)autolayer->layer1,
                           1,
                           773,
                           600);
      reluArray((long double *)autolayer->layer1, 1, 600, 0);

      matrixMultiplication((long double *)autolayer->layer1,
                           (long double *)autoweights->weight1,
                           (long double *)autolayer->layer2,
                           1,
                           600,
                           400);
      reluArray((long double *)autolayer->layer2, 1, 400, 0);

      matrixMultiplication((long double *)autolayer->layer2,
                           (long double *)autoweights->weight2,
                           (long double *)autolayer->layer3,
                           1,
                           400,
                           200);
      reluArray((long double *)autolayer->layer3, 1, 200, 0);

      matrixMultiplication((long double *)autolayer->layer3,
                           (long double *)decodeweights->weight1,
                           (long double *)decodelayer->layer2,
                           1,
                           200,
                           400);
      reluArray((long double *)decodelayer->layer2, 1, 400, 0);

      matrixMultiplication((long double *)decodelayer->layer2,
                           (long double *)decodeweights->weight2,
                           (long double *)decodelayer->layer3,
                           1,
                           400,
                           600);
      reluArray((long double *)decodelayer->layer3, 1, 600, 0);

      matrixMultiplication((long double *)decodelayer->layer3,
                           (long double *)decodeweights->weight3,
                           (long double *)decodelayer->output,
                           1,
                           600,
                           773);
      reluArray((long double *)decodelayer->output, 1, 773, 0);
    }

  // foward prop at stage 4
  if (stage == 4)
    {
      matrixMultiplication((long double *)autolayer->input,
                           (long double *)autoweights->weight0,
                           (long double *)autolayer->layer1,
                           1,
                           773,
                           600);
      reluArray((long double *)autolayer->layer1, 1, 600, 0);

      matrixMultiplication((long double *)autolayer->layer1,
                           (long double *)autoweights->weight1,
                           (long double *)autolayer->layer2,
                           1,
                           600,
                           400);
      reluArray((long double *)autolayer->layer2, 1, 400, 0);

      matrixMultiplication((long double *)autolayer->layer2,
                           (long double *)autoweights->weight2,
                           (long double *)autolayer->layer3,
                           1,
                           400,
                           200);
      reluArray((long double *)autolayer->layer3, 1, 200, 0);

      matrixMultiplication((long double *)autolayer->layer3,
                           (long double *)autoweights->weight3,
                           (long double *)autolayer->output,
                           1,
                           200,
                           100);
      reluArray((long double *)autolayer->output, 1, 100, 0);

      matrixMultiplication((long double *)autolayer->output,
                           (long double *)decodeweights->weight0,
                           (long double *)decodelayer->layer1,
                           1,
                           100,
                           200);
      reluArray((long double *)decodelayer->layer1, 1, 200, 0);

      matrixMultiplication((long double *)decodelayer->layer1,
                           (long double *)decodeweights->weight1,
                           (long double *)decodelayer->layer2,
                           1,
                           200,
                           400);
      reluArray((long double *)decodelayer->layer2, 1, 400, 0);

      matrixMultiplication((long double *)decodelayer->layer2,
                           (long double *)decodeweights->weight2,
                           (long double *)decodelayer->layer3,
                           1,
                           400,
                           600);
      reluArray((long double *)decodelayer->layer3, 1, 600, 0);

      matrixMultiplication((long double *)decodelayer->layer3,
                           (long double *)decodeweights->weight3,
                           (long double *)decodelayer->output,
                           1,
                           600,
                           773);
      reluArray((long double *)decodelayer->output, 1, 773, 0);
    }
}

void randReluArray(long double *A, int m, int n, int f)
{
  int i, j;
  for (i = 0; i < m; ++i)
    {
      for (j = 0; j < n; ++j)
        {
          A[i * n + j] = randGen() * (sqrt(6.0) / f);
        }
    }
}

void backpropAutoN(AUTOW *  autoweights,
                  AUTOL *  autolayer,
                  DECODEW *decodeweights,
                  DECODEL *decodelayer,
                  AUTOW *  autograd,
                  DECODEW *decodegrad,
                  int      stage)
{
    if(stage == 1)
    {
        long double totalerror[1][773];
        long double outputdelta[1][773];
        long double weight3T[773][600];

        long double layer1E[1][600];
        long double layer1delta[1][600];
        long double inputT[773][1];
        long double layer1T[600][1];
        
        autoencoderE((long double *) autolayer->input, (long double *) decodelayer->output);

        matrixSubtraction((long double *) autolayer->input, (long double *) decodelayer->output, (long double *) totalerror, 1, 773);
        matrixDelta((long double *) totalerror, (long double *) decodelayer->output, (long double *) outputdelta, 1, 773);

        
        TMatrix((long double *) decodeweights->weight3, (long double *) weight3T, 600, 773);
        matrixMultiplication((long double *) outputdelta, (long double *) weight3T, (long double *) layer1E, 1, 773,600);

        //not yet tested pass here
        matrixDelta((long double *) layer1E, (long double *) autolayer->layer1, (long double *) layer1delta, 1, 600);
        TMatrix((long double *) autolayer->layer1, (long double *) layer1T, 1, 600);
        TMatrix((long double *) autolayer->input, (long double *) inputT, 1, 773);

        //calucalate the weights 
        matrixMultiplication((long double *) layer1T, (long double *) outputdelta, (long double *) decodegrad->weight3, 600, 1, 773);
        matrixMultiplication((long double *) inputT, (long double *) layer1delta, (long double *) autograd->weight0, 773, 1, 600);

        //printMatrix((long double *) decodegrad->weight3, 600, 773);
        printMatrix((long double *) layer1E, 1, 600);
               
    }


}


