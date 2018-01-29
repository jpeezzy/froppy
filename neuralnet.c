#include "neuralnet.h"
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix.h"
#include "randGen.h"

double eta     = 0.001;
double epsilon = 0.00000001;
double beta1   = 0.9;
double beta2   = 0.999;

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
double reluActivation(double x, int flag)
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
  return 0.0 ;
}

double tanhActivation(double x, int flag)
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
void reluArray(double *array, int m, int n, int flag)
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
void tanhArray(double *array, int m, int n, int flag)
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
void nadam(double *w, double *g, double *m, double *v, int t)
{
  // Update the Momentum and Variance Vectors
  *(m) = (*(m)) * beta1 + (1.0 - beta1) * (*(g));
  *(v) = (*(v)) * beta2 + (1.0 - beta2) * (*(g)) * (*(g));
  
  double vt = (*(v)) / (1.0 - pow(beta2,t));
  double mt = (*(m)) / (1.0 - pow(beta1,t)); 

  // Calculate the new weight
  //*(w) = *(w) + .01* (*(g));
  *(w) = *(w) + ( (eta/(sqrt(vt) + epsilon)) * ((beta1*mt) + (((1.0-beta1)*(*(g)))/(1.0-pow(beta1,t))))); 
  //*(w) = *(w) - (eta / ((sqrt((*(vt)) / (1.0 - pow(beta1, t)))) + epsilon)) * (beta1 * ((*(mt)) / (1.0 - pow(beta1, t))) + ((1.0 - beta1) * (*(g))) / (1.0 - pow(beta1, t)));
  //printf("\n %.5f", *w);
}

// nadam Array
void nadamArray(double *W, double *G, double *M, double *V, int m, int n, int t)
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
            //W[i*n+j] = W[i*n+j] + .001 * G[i*n+j];
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
        nadamArray((double *)autoweights->weight0,
                   (double *)autograd->weight0,
                   (double *)autoM->weight0,
                   (double *)autoV->weight0,
                   773,
                   600,
                   t);
        nadamArray((double *)decodeweights->weight3,
                   (double *)decodegrad->weight3,
                   (double *)decodeM->weight3,
                   (double *)decodeV->weight3,
                   600,
                   773,
                   t);
      }

    // stage 2 of the auto encoder
    if (stage == 2)
      {
        nadamArray((double *)autoweights->weight1,
                   (double *)autograd->weight1,
                   (double *)autoM->weight1,
                   (double *)autoV->weight1,
                   600,
                   400,
                   t);
        nadamArray((double *)decodeweights->weight2,
                   (double *)decodegrad->weight2,
                   (double *)decodeM->weight2,
                   (double *)decodeV->weight2,
                   400,
                   600,
                   t);
      }

    // stage 3 of the auto encoder
    if (stage == 3)
      {
        nadamArray((double *)autoweights->weight2,
                   (double *)autograd->weight2,
                   (double *)autoM->weight2,
                   (double *)autoV->weight2,
                   400,
                   200,
                   t);
        nadamArray((double *)decodeweights->weight1,
                   (double *)decodegrad->weight1,
                   (double *)decodeM->weight1,
                   (double *)decodeV->weight1,
                   200,
                   400,
                   t);
      }

    // stage 4 of the auto encoder
    if (stage == 4)
      {
        nadamArray((double *)autoweights->weight3,
                   (double *)autograd->weight3,
                   (double *)autoM->weight3,
                   (double *)autoV->weight3,
                   200,
                   100,
                   t);
        nadamArray((double *)decodeweights->weight0,
                   (double *)decodegrad->weight0,
                   (double *)decodeM->weight0,
                   (double *)decodeV->weight0,
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
      matrixMultiplication((double *)autolayer->input,
                           (double *)autoweights->weight0,
                           (double *)autolayer->layer1,
                           1,
                           773,
                           600);
      reluArray((double *)autolayer->layer1, 1, 600, 0);

      matrixMultiplication((double *)autolayer->layer1,
                           (double *)decodeweights->weight3,
                           (double *)decodelayer->output,
                           1,
                           600,
                           773);
      reluArray((double *)decodelayer->output, 1, 773, 0);
      //printMatrix((double *) decodelayer->output, 1, 773);
      //int ii;
      //for(ii=0; ii < 773; ++ii)
      //{
      //    printf("%.4f \n",decodelayer->output[0][ii]);
      //}
    }

  // foward prop when the autoencoder is in stage 2
  if (stage == 2)
    {
      matrixMultiplication((double *)autolayer->input,
                           (double *)autoweights->weight0,
                           (double *)autolayer->layer1,
                           1,
                           773,
                           600);
      reluArray((double *)autolayer->layer1, 1, 600, 0);

      matrixMultiplication((double *)autolayer->layer1,
                           (double *)autoweights->weight1,
                           (double *)autolayer->layer2,
                           1,
                           600,
                           400);
      reluArray((double *)autolayer->layer2, 1, 400, 0);

      matrixMultiplication((double *)autolayer->layer2,
                           (double *)decodeweights->weight2,
                           (double *)decodelayer->layer3,
                           1,
                           400,
                           600);
      reluArray((double *)decodelayer->layer3, 1, 600, 0);

      matrixMultiplication((double *)decodelayer->layer3,
                           (double *)decodeweights->weight3,
                           (double *)decodelayer->output,
                           1,
                           600,
                           773);
      reluArray((double *)decodelayer->output, 1, 773, 0);
    }

  // foward prop at stage 3
  if (stage == 3)
    {
      matrixMultiplication((double *)autolayer->input,
                           (double *)autoweights->weight0,
                           (double *)autolayer->layer1,
                           1,
                           773,
                           600);
      reluArray((double *)autolayer->layer1, 1, 600, 0);

      matrixMultiplication((double *)autolayer->layer1,
                           (double *)autoweights->weight1,
                           (double *)autolayer->layer2,
                           1,
                           600,
                           400);
      reluArray((double *)autolayer->layer2, 1, 400, 0);

      matrixMultiplication((double *)autolayer->layer2,
                           (double *)autoweights->weight2,
                           (double *)autolayer->layer3,
                           1,
                           400,
                           200);
      reluArray((double *)autolayer->layer3, 1, 200, 0);

      matrixMultiplication((double *)autolayer->layer3,
                           (double *)decodeweights->weight1,
                           (double *)decodelayer->layer2,
                           1,
                           200,
                           400);
      reluArray((double *)decodelayer->layer2, 1, 400, 0);

      matrixMultiplication((double *)decodelayer->layer2,
                           (double *)decodeweights->weight2,
                           (double *)decodelayer->layer3,
                           1,
                           400,
                           600);
      reluArray((double *)decodelayer->layer3, 1, 600, 0);

      matrixMultiplication((double *)decodelayer->layer3,
                           (double *)decodeweights->weight3,
                           (double *)decodelayer->output,
                           1,
                           600,
                           773);
      reluArray((double *)decodelayer->output, 1, 773, 0);
    }

  // foward prop at stage 4
  if (stage == 4)
    {
      matrixMultiplication((double *)autolayer->input,
                           (double *)autoweights->weight0,
                           (double *)autolayer->layer1,
                           1,
                           773,
                           600);
      reluArray((double *)autolayer->layer1, 1, 600, 0);

      matrixMultiplication((double *)autolayer->layer1,
                           (double *)autoweights->weight1,
                           (double *)autolayer->layer2,
                           1,
                           600,
                           400);
      reluArray((double *)autolayer->layer2, 1, 400, 0);

      matrixMultiplication((double *)autolayer->layer2,
                           (double *)autoweights->weight2,
                           (double *)autolayer->layer3,
                           1,
                           400,
                           200);
      reluArray((double *)autolayer->layer3, 1, 200, 0);

      matrixMultiplication((double *)autolayer->layer3,
                           (double *)autoweights->weight3,
                           (double *)autolayer->output,
                           1,
                           200,
                           100);
      reluArray((double *)autolayer->output, 1, 100, 0);

      matrixMultiplication((double *)autolayer->output,
                           (double *)decodeweights->weight0,
                           (double *)decodelayer->layer1,
                           1,
                           100,
                           200);
      reluArray((double *)decodelayer->layer1, 1, 200, 0);

      matrixMultiplication((double *)decodelayer->layer1,
                           (double *)decodeweights->weight1,
                           (double *)decodelayer->layer2,
                           1,
                           200,
                           400);
      reluArray((double *)decodelayer->layer2, 1, 400, 0);

      matrixMultiplication((double *)decodelayer->layer2,
                           (double *)decodeweights->weight2,
                           (double *)decodelayer->layer3,
                           1,
                           400,
                           600);
      reluArray((double *)decodelayer->layer3, 1, 600, 0);

      matrixMultiplication((double *)decodelayer->layer3,
                           (double *)decodeweights->weight3,
                           (double *)decodelayer->output,
                           1,
                           600,
                           773);
      reluArray((double *)decodelayer->output, 1, 773, 0);
    }
}

void randReluArray(double *A, int m, int n, int f)
{
  int i, j;
  for (i = 0; i < m; ++i)
    {
      for (j = 0; j < n; ++j)
        {
          A[i * n + j] = randGen() * 2.0/f;
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
        double totalerror[1][773];
        double outputdelta[1][773];
        double weight3T[773][600];

        double layer1E[1][600];
        double layer1delta[1][600];
        double inputT[773][1];
        double layer1T[600][1];
        
        autoencoderE((double *) autolayer->input, (double *) decodelayer->output);

        matrixSubtraction((double *) autolayer->input, (double *) decodelayer->output, (double *) totalerror, 1, 773);
        matrixDelta((double *) totalerror, (double *) decodelayer->output, (double *) outputdelta, 1, 773);

        
        TMatrix((double *) decodeweights->weight3, (double *) weight3T, 600, 773);
        matrixMultiplication((double *) outputdelta, (double *) weight3T, (double *) layer1E, 1, 773,600);
		//mm((double *) outputdelta, (double *) weight3T, (double *) layer1E, 1, 773,600);

        //not yet tested pass here
        matrixDelta((double *) layer1E, (double *) autolayer->layer1, (double *) layer1delta, 1, 600);
        TMatrix((double *) autolayer->layer1, (double *) layer1T, 1, 600);
        TMatrix((double *) autolayer->input, (double *) inputT, 1, 773);

        //calucalate the weights 
        matrixMultiplication((double *) layer1T, (double *) outputdelta, (double *) decodegrad->weight3, 600, 1, 773);
        matrixMultiplication((double *) inputT, (double *) layer1delta, (double *) autograd->weight0, 773, 1, 600);
		//mm((double *) layer1T, (double *) outputdelta, (double *) decodegrad->weight3, 600, 1, 773);
       // mm((double *) inputT, (double *) layer1delta, (double *) autograd->weight0, 773, 1, 600);

        //printMatrix((double *) decodegrad->weight3, 600, 773);
        //printMatrix((double *) layer1delta, 1, 600);
               
    }


}


