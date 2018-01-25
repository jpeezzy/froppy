#ifndef NEURALNET_H
#define NEURALNET_H

typedef struct AUTOENCODERWEIGHTS AUTOW;
typedef struct DECODERWEIGHTS     DECODEW;
typedef struct AUTOENCODERLAYER   AUTOL;
typedef struct DECODERLAYER       DECODEL;

// Struct for the auto encoder weight information
struct
{
  float weight0[773][600];
  float weight1[600][400];
  float weight2[400][200];
  float weight3[200][100];

} AUTOENCODERWEIGHTS;

// Struct for auto encoder layers
struct
{
  float input[1][773];
  float layer1[1][600];
  float layer2[1][400];
  float layer3[1][200];
  float output[1][100];
} AUTOENCODERLAYER;

// Struct for the auto encode weight information
struct
{
  float weight0[100][200];
  float weight1[200][400];
  float weight2[400][600];
  float weight3[600][773];

} DECODERWEIGHTS;

// Struct for the decoder layers
struct
{
  // input of decoder is the output of the autoencoder
  float layer1[1][200];
  float layer2[1][400];
  float layer3[1][600];
  float output[1][773];

} DECODERLAYER;

// Stage starts from 1 and goesfrom 773 to 600 to 773
// Foward prop for auto encoder changes depending on stage
void fowardpropAuto(AUTOW *  weights,
                    AUTOL *  autolayer,
                    DECODEW *decodeweights,
                    DECODEL *decodelayer,
                    int      stage);

// Backprop for Auto Encoder Changes Depending on
void backpropAuto(AUTOW *  weights,
                  AUTOL *  autolayer,
                  DECODEW *decodeweights,
                  DECODEL *decodelayer,
                  AUTOW *  autograd,
                  DECODE * decodegrad,
                  int      stage);

// RELU Activation
float reluActivation(float x, int flag);

// Tanh Activation
float tanhActivation(float x, int flag);

// Apply Activation Function on an array
void tanhArray(float *array, int m, int n, int flag);

// Apply Activation Function on an array
void reluArray(float *array, int m, int n, int flag);

#endif
