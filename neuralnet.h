#ifndef NEURALNET_H
#define NEURALNET_H

extern float eta;
extern float epsilon;
extern float beta1;
extern float beta2;

typedef struct AUTOENCODERWEIGHTS AUTOW;
typedef struct DECODERWEIGHTS     DECODEW;
typedef struct AUTOENCODERLAYER   AUTOL;
typedef struct DECODERLAYER       DECODEL;

// Struct for the auto encoder weight information
struct AUTOENCODERWEIGHTS
{
  float weight0[773][600];
  float weight1[600][400];
  float weight2[400][200];
  float weight3[200][100];
};

// Struct for auto encoder layers
struct AUTOENCODERLAYER
{
  float input[1][773];
  float layer1[1][600];
  float layer2[1][400];
  float layer3[1][200];
  float output[1][100];
};

// Struct for the auto encode weight information
struct DECODERWEIGHTS
{
  float weight0[100][200];
  float weight1[200][400];
  float weight2[400][600];
  float weight3[600][773];
};

// Struct for the decoder layers
struct DECODERLAYER
{
  // input of decoder is the output of the autoencoder
  float layer1[1][200];
  float layer2[1][400];
  float layer3[1][600];
  float output[1][773];
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
float reluActivation(float x, int flag);

// Tanh Activation
float tanhActivation(float x, int flag);

// Apply Activation Function on an array
void tanhArray(float *array, int m, int n, int flag);

// Apply Activation Function on an array
void reluArray(float *array, int m, int n, int flag);

// nadam update
void nadam(float *w, float *g, float *m, float *v, int t);

// nadam on an a array
void nadamArray(float *W, float *G, float *M, float *V, int m, int n, int t);

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
void randReluArray(float *A, int m, int n, int f);

#endif
