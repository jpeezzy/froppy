#ifndef NEURALNET_H
#define NEURALNET_H

float reluActivation(float x, int flag);
float tanhActivation(float x, int flag);
void reluArray(float *A, int m, int n, int flag);
void tanhArray(float *A, int m, int n, int flag);
void nadamArray( float *W, float *V, float *M, float *G, int m, int n, int t);
void nadam(float *w, float *v, float *m, float *g, int t);

#endif
