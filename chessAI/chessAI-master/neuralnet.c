#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include "neuralnet.h"

/* Standard RelU */
double reluActivation(double x, int flag)
{   if(flag == 0)
    {
        if( x > 0)
        {
            return x;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if( x > 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

/* tanh function */
double tanhActivation(double x, int flag)
{
    if(flag == 0)
    {
        return tanh(x);
    }
    else
    {
        return (x*(1-x));
    }
}

/*Applay RElU to array*/
void reluArray(double *A, int m, int n, int flag)
{
    int i,j;
    for(i = 0; i < m; ++i)
    {
        for(j = 0; j < n; ++j)
        {
            A[i*n+j] = reluActivation(A[i*n+j], flag); 
        }
    }
}

/* apply tanh function to an Array */
void tanhArray(double *A, int m, int n, int flag)
{
    int i,j;
    for(i = 0; i < m; ++i)
    {
        for(j = 0; j < n; ++j)
        {
            A[i*n+j] = tanhActivation(A[i*n+j], flag); 
        }
    }
}

void nadamArray( double *W, double *V, double *M, double *G, int m, int n, int t)
{
    int i,j;
    for(i = 0; i < m; ++i)
    {
        for(j = 0; j < n; ++j)
        {
           /* nadam(W[i*n+j], V[i*n+j], M[i*n+j], G[i*n+j], t);*/         
        }
    }
    /*Update Timestep*/
    t = t+1;
}

void nadam(double *w, double *v, double *m, double *g, int t)
{
    /*maybe this should be global for a speed increase?*/
    double b1 = .9, b2 = .999, e = pow(10,-8), eta = .001;
    
    /* Update the Momentum and Variance Vectors */
    /*m = m*b1 + (1-b1)*g;
    v = v*b2 + (1-b2)*g*g;*/
    
    /* Calculate the new weight */
    /*w = w - (eta/(sqrt(v/(1-pow(b1,t))))+e)(b1*(m/(1-pow(b1,t)))+ ((1-b1)*g)/(1-pow(b1,t)));*/
}

/* only used once to intialize the weights*/
/* also on the the to do list get a better random number generater*/
/* Xavier Intialization */
void intializetanh(double *A, int m, int n, int f)
{
    int i,j;
    srand(time(NULL));
    for(i = 0; i < m; ++i)
    {
        for(j = 0; j < n; ++j)
        {
            A[i*n+j] = rand() * sqrt(1/f);                               

        }
    }
}

void intializerelu(double *A, int m, int n, int f)
{
    int i,j;
    srand(time(NULL));
    for(i = 0; i < m; ++i)
    {
        for(j = 0; j < n ; ++j)
        {
            A[i*n+j] = rand() * sqrt(2/f);
        }
    }
}












