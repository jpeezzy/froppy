#ifndef NEURALNET_H
#define NEURALNET_H

long double reluActivation(long double x, int flag);
long double tanhActivation(long double x, int flag);
void reluArray(long double *A, int m, int n, int flag);
void tanhArray(long double *A, int m, int n, int flag);
void nadamArray( long double *W, long double *V, long double *M, long double *G, int m, int n, int t);
void nadam(long double *w, long double *v, long double *m, long double *g, int t);

#endif
