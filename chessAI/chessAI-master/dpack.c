#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <string.h>
/* test */
int saveArray(double *A, char *fname, int m, int n, int tnum);
int main()
{
    double A[400][200]; 
    int l1,l2;
    srand(1);
    for(l1 = 0; l1 < 400; ++l1)
    {
        for(l2 = 0; l2 < 200; ++l2)
        {
            A[l1][l2] = ((double)rand()/(double)(RAND_MAX)) * 50;
        }
        
    }
    char fname[400] = "testA";
    printf("\nflag 0\n");
    struct timespec tstart={0,0}, tend={0,0};
    clock_gettime(CLOCK_MONOTONIC, &tstart);
    saveArray((double *)A, fname, 400, 200, 4);
    clock_gettime(CLOCK_MONOTONIC, &tend);
    printf("P took about %.5f seconds\n",((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
    return 0;
}

/* Rows is m and N is colums */
/* if the array is small tnum should be 1*/
int saveArray(double *A, char *fname, int m, int n, int tnum)
{
    FILE *file;
    char ftype[] = ".dank";
    char fmaster[400];
    strcpy(fmaster,fname);
    strcat(fmaster,"master");
    strcat(fmaster,ftype);
    int mode = 0;
    file = fopen(fmaster,"w");

    if (!file)
    {
        printf("Cannot open file \"%s\" for writing!\n", fname);
        return 1;
    }
    
    /* All the master information for the files goes here */
    if( n > m)
    {
        mode = 1;
    }
    fprintf(file,"%d\n", mode);
    fprintf(file,"%d\n", tnum);
    fprintf(file,"%d %d\n", m, n);
    
    if (ferror(file))
    {
        printf("\nFile error while writing to file!\n");
        return 2;
    }
    fclose(file);
    int i;
    int tid, nthreads, chunk;
    tid = nthreads = chunk = 0;
    chunk = 1;           
    omp_set_dynamic(0);
    omp_set_num_threads(tnum);
    if(mode == 0)
    {
        #pragma omp parallel 
        {            
            tid = omp_get_thread_num();
            if (tid == 0)
            {
                nthreads = omp_get_num_threads();
            }

            #pragma omp for schedule(static, chunk)
            for( i = 0; i < tnum; i++)
            {
                {
                    FILE *fpointer;
                    char newf[400];
                    char fnumber[400];
                    strcpy(newf,fname);
                    sprintf(fnumber, "%d", i);
                    strcat(newf,fnumber);
                    strcat(newf,ftype);
                    fpointer  = fopen(newf,"w");
                    int xx,yy;
                    int starta = i*(m/tnum);
                    int enda = (i+1) * (m/tnum);
                    if(i == (tnum-1))
                    {
                        enda = m;
                    }
                    for(xx = starta; xx < enda; ++xx)
                    {
                        for(yy = 0; yy < n; ++yy)
                        {
                            fprintf(fpointer,"%f ",A[xx*n+yy]);    
                        }
                        fprintf(fpointer,"\n");
                    }
                    fclose(fpointer);
                }

            }
        }
    }
    return 0;
}
