#ifndef NEURALNET_H
#define NEURALNET_H

double reluActivation(double x, int flag);
double tanhActivation(double x, int flag);
void reluArray(double *A, int m, int n, int flag);
void tanhArray(double *A, int m, int n, int flag);
void nadamArray( double *W, double *V, double *M, double *G, int m, int n, int t);
void nadam(double *w, double *v, double *m, double *g, int t);

#endif
