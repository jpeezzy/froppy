#ifndef _NEURALSAVE_H
#define _NEURALSAVE_H
#include <stdio.h>
#include <stdlib.h>

// check how many files we have had then create new data files for other files
// to write in, return an array of file handle
FILE** createNewset(FILE* numFile);

// do post processing tasks for file after we are done with training
void closeNewset(FILE** handleList);

// save everything inlcuding t in NADAM
// save only weights if too much work
void SaveNN(AUTOW*   autoweights,
            AUTOL*   autolayer,
            DECODEW* decodeweights,
            DECODEL* decodelayer,
            AUTOW*   autograd,
            DECODEW* decodegrad);

// load everything if possible
// load on weights if too much work
void LoadNN();

#endif