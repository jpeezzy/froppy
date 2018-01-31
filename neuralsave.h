#ifndef _NEURALSAVE_H
#define _NEURALSAVE_H
#include <stdio.h>
#include <stdlib.h>

// check how many files we have had then create new data files for other files
// to write in, return an array of file handle
FILE** createNewset(char* fileName, int mode);

// do post processing tasks for file after we are done with training
void closeNewset(FILE** handleList);

// save everything inlcuding t in NADAM
// save only weights if too much work
void SaveNN(AUTOW*   aw,
            AUTOL*   al,
            DECODEW* dw,
            DECODEL* dl,
            AUTOW*   ag,
            DECODEW* dg,
            AUTOW*   am,
            AUTOW*   av,
            DECODEW* dm,
            DECODEW*   dv);

// load everything if possible
// load on weights if too much work
void LoadNN(AUTOW*   aw,
            AUTOL*   al,
            DECODEW* dw,
            DECODEL* dl,
            AUTOW*   ag,
            DECODEW* dg,
            AUTOW*   am,
            AUTOW*   av,
            DECODEW* dm,
            DECODEW*   dv);

#endif
