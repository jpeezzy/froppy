#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

//Calculates the relu activation of a single input 0 is normal 1 is deriv
float reluActivation(float x, int flag)
{
    if(flag == 0)
    {
        if(x>0)
        {
            return x;
        }
        else
        {
            return 0.0;
        }
    }
    else if( flag == 1)
    {
        if(x>0)
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

float tanhActivation(float x, int flag)
{
    if(flag == 0)
    {
        return(1.0/(1.0+exp(-x)));
    }
    else if(flag == 1)
    {
        return(x*(1.0-x));
    }
    return 10;   
}


//Applies relu activation to an array 
void reluArray(float *array, int m, int n, int flag)
{
    int i,j;
    int tid, nthreads, chunk;
    chunk = 10;
    #pragma omp parallel
        {
            tid = omp_get_thread_num();
            if(tid == 0)
            {
                nthreads = omp_get_num_threads();
            }
            #pragma omp for schedule(static, chunk)
                for(i = 0; i<m; ++i)
                {
                    for(j = 0; j<n; ++j)
                    {
                        array[i*j+n] = reluActivation(array[i*j+n], flag);                       
                    }
                }
        }
}

//Applies tanh activation to an array 
void tanhArray(float *array, int m, int n, int flag)
{
    int i,j;
    int tid, nthreads, chunk;
    chunk = 10;
    #pragma omp parallel
        {
            tid = omp_get_thread_num();
            if(tid == 0)
            {
                nthreads = omp_get_num_threads();
            }
            #pragma omp for schedule(static, chunk)
                for(i = 0; i<m; ++i)
                {
                    for(j = 0; j<n; ++j)
                    {
                        array[i*j+n] = tanhActivation(array[i*j+n], flag);                       
                    }
                }
        }
}

//Nadam for a single part
void nadam(float *w, float *g, float *m, float *v, int t)
{
    //Update the Momentum and Variance Vectors
    *(m) = (*(m))*b1 + (1-b1)*(*(g));
    *(v) = (*(v))*b2 + (1-b2)*(*(g))*(*(g));
    
    //Calculate the new weight
    *(w) = *(w) - (eta/(sqrt((*(v))/(1-pow(b1,t))))+e)*(b1*((*(m))/(1-pow(b1,t)))+ ((1-b1)*(*(g)))/(1-pow(b1,t)));  
}

//nadam Array 
void nadamArray( float *W, float *V, float *M, float *G, int m, int n, int t)
{
    int i,j;
    
    for(i = 0; i < m; ++i)
    {
        for(j = 0; j < n; ++j)
        {
             nadam( &(W[i*n+j]), &(V[i*n+j]), &(M[i*n+j]), &(G[i*n+j]), t);         
        }
    }
}








