#ifndef NEURALNET_H
#define NEURALNET_H

extern long double eta;
extern long double epsilon;
extern long double beta1;
extern long double beta2;

typedef struct AUTOENCODERWEIGHTS AUTOW;
typedef struct DECODERWEIGHTS     DECODEW;
typedef struct AUTOENCODERLAYER   AUTOL;
typedef struct DECODERLAYER       DECODEL;

// Struct for the auto encoder weight information
struct AUTOENCODERWEIGHTS
{
  long double weight0[773][600];
  long double weight1[600][400];
  long double weight2[400][200];
  long double weight3[200][100];
};

// Struct for auto encoder layers
struct AUTOENCODERLAYER
{
  long double input[1][773];
  long double layer1[1][600];
  long double layer2[1][400];
  long double layer3[1][200];
  long double output[1][100];
};

// Struct for the auto encode weight information
struct DECODERWEIGHTS
{
  long double weight0[100][200];
  long double weight1[200][400];
  long double weight2[400][600];
  long double weight3[600][773];
};

// Struct for the decoder layers
struct DECODERLAYER
{
  // input of decoder is the output of the autoencoder
  long double layer1[1][200];
  long double layer2[1][400];
  long double layer3[1][600];
  long double output[1][773];
};

AUTOW *  createAutoW(void);
DECODEW *createDecoW(void);
AUTOL *  createAutoL(void);
DECODEL *createDecoL(void);

// Stage starts from 1 and goesfrom 773 to 600 to 773
// Foward prop for auto encoder changes depending on stage
void fowardpropAuto(AUTOW *  autoweights,
                    AUTOL *  autolayer,
                    DECODEW *decodeweights,
                    DECODEL *decodelayer,
                    int      stage);

// Backprop for Auto Encoder Changes Depending on
void backpropAuto(AUTOW *  autoweights,
                  AUTOL *  autolayer,
                  DECODEW *decodeweights,
                  DECODEL *decodelayer,
                  AUTOW *  autograd,
                  DECODEW *decodegrad,
                  int      stage);

void backpropAutoN(AUTOW *  autoweights,
                  AUTOL *  autolayer,
                  DECODEW *decodeweights,
                  DECODEL *decodelayer,
                  AUTOW *  autograd,
                  DECODEW *decodegrad,
                  int      stage);

// RELU Activation
long double reluActivation(long double x, int flag);

// Tanh Activation
long double tanhActivation(long double x, int flag);

// Apply Activation Function on an array
void tanhArray(long double *array, int m, int n, int flag);

// Apply Activation Function on an array
void reluArray(long double *array, int m, int n, int flag);

// nadam update
void nadam(long double *w, long double *g, long double *m, long double *v, int t);

// nadam on an a array
void nadamArray(long double *W, long double *G, long double *M, long double *V, int m, int n, int t);

// nadam optimzation algorithm
void nadamAuto(AUTOW *  autoweights,
               DECODEW *decodeweights,
               AUTOW *  autograd,
               DECODEW *decodegrad,
               AUTOW *  autoM,
               AUTOW *  autoV,
               DECODEW *decodeM,
               DECODEW *decodeV,
               int      t,
               int      stage);

// intialize the relu
void randReluArray(long double *A, int m, int n, int f);

#endif
