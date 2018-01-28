/**
 * Author: Khoi Trinh
 * Written for saving and loading layers of neural net
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "neuralnet.h"
#include "neuralsave.h"

#define FILENUM 11

// check how many files we have had then create new data files for other files
// to write in, return an array of file handle
// take in the handle to the file that tells how many files we have had
FILE** createNewset(FILE* numFile)
{
    int    numberofFile;
    char   tempnum[5];
    char   dirName[30] = "~/";
    FILE** handleList  = malloc(sizeof(FILE*) * FILENUM);
    // wl is the last weight, which will be loaded to the next iteration
    const char* nameAll[] = {
        "aw", "al", "dw", "dl", "ag", "dg", "am", "av", "dm", "dv", "wl"};
    char temp[20];

    // findout how many files have we stored so far
    fscanf(numFile, "%d", &numberofFile);
    // new file will have new serial number to distinguish
    ++numberofFile;
    // write new number into the file
    sprintf(tempnum, "%d", numberofFile);
    fputs(tempnum, numFile);

    // create the directory name to be saved to

    mkdir(strcat(dirName, dirName), 0700);

    for (int i = 0; i < FILENUM; ++i)
        {
            sprintf(tempnum, "%d", numberofFile);
            handleList[i] = fopen(strcat(nameAll[i], tempnum), "w");
        }

    return handleList;
}

// do post processing tasks for files after we are done wwriting
void closeNewset(FILE** handleList)
{
    for (int i = 0; i < FILENUM; ++i)
        {
            fclose(handleList[i]);
            free(handleList[i]);
        }
    free(handleList);
}
