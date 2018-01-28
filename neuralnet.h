#ifndef NEURALNET_H
#define NEURALNET_H

extern double eta;
extern double epsilon;
extern double beta1;
extern double beta2;

typedef struct AUTOENCODERWEIGHTS AUTOW;
typedef struct DECODERWEIGHTS     DECODEW;
typedef struct AUTOENCODERLAYER   AUTOL;
typedef struct DECODERLAYER       DECODEL;

// Struct for the auto encoder weight information
struct AUTOENCODERWEIGHTS
{
  double weight0[773][600];
  double weight1[600][400];
  double weight2[400][200];
  double weight3[200][100];
};

// Struct for auto encoder layers
struct AUTOENCODERLAYER
{
  double input[1][773];
  double layer1[1][600];
  double layer2[1][400];
  double layer3[1][200];
  double output[1][100];
};

// Struct for the auto encode weight information
struct DECODERWEIGHTS
{
  double weight0[100][200];
  double weight1[200][400];
  double weight2[400][600];
  double weight3[600][773];
};

// Struct for the decoder layers
struct DECODERLAYER
{
  // input of decoder is the output of the autoencoder
  double layer1[1][200];
  double layer2[1][400];
  double layer3[1][600];
  double output[1][773];
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
double reluActivation(double x, int flag);

// Tanh Activation
double tanhActivation(double x, int flag);

// Apply Activation Function on an array
void tanhArray(double *array, int m, int n, int flag);

// Apply Activation Function on an array
void reluArray(double *array, int m, int n, int flag);

// nadam update
void nadam(double *w, double *g, double *m, double *v, int t);

// nadam on an a array
void nadamArray(double *W, double *G, double *M, double *V, int m, int n, int t);

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
void randReluArray(double *A, int m, int n, int f);

#endif
