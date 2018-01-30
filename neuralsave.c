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
#include <unistd.h>

#include "neuralnet.h"
#include "neuralsave.h"

#define FILENUM 10
#define NUMFILE "auto.txt"

// check how many files we have had then create new data files for other files
// to write in, return an array of file handle
// take in the handle to the file that tells how many files we have had
// mode 0 for load, 1 for save
FILE** createNewset(char* fileName, int mode)
{
    FILE* numFile = fopen(fileName, "r");
    assert(numFile);
    int    numberofFile = 0;
    char   tempnum[5];
    char   dirName[30] = "./";
    FILE** handleList  = malloc(sizeof(FILE*) * FILENUM);
    assert(handleList);
    // wl is the last weight, which will be loaded to the next iteration
    char* nameAll[] = {
        "aw", "al", "dw", "dl", "ag", "dg", "am", "av", "dm", "dv"};
    char temp[255];

    // findout how many files have we stored so far
    fscanf(numFile, "%d", &numberofFile);
    // new file will have new serial number to distinguish
    assert(numberofFile);
    if (mode)
        {
            ++numberofFile;

            // write new number into the file
            fclose(numFile);
            numFile = fopen(fileName, "w+");
            fprintf(numFile, "%d", numberofFile);

            // create the directory to be save and move the working directory
            // there
            sprintf(tempnum, "%d", numberofFile);
            mkdir(strcat(dirName, tempnum), 0777);
            chdir(dirName);
        }
    else
        {
            sprintf(tempnum, "%d", numberofFile);
            chdir(strcat(dirName, tempnum));
        }
    fclose(numFile);
    // make one file handle for every variable that needs to be saved in the
    // neural net

    for (int i = 0; i < FILENUM; ++i)
        {
            if (mode)
                {
                    handleList[i] =
                        fopen(strcat(strcpy(temp, nameAll[i]), tempnum), "w+");
                }
            else
                {
                    handleList[i] =
                        fopen(strcat(strcpy(temp, nameAll[i]), tempnum), "r");
                }
        }

    return handleList;
}

// do post processing tasks for files after we are done wwriting
void closeNewset(FILE** handleList)
{
    assert(handleList);
    for (int i = 0; i < FILENUM; ++i)
        {
            fclose(handleList[i]);
        }
    free(handleList);
}

// save all the components of the neural network
void SaveNN(AUTOW*   aw,
            AUTOL*   al,
            DECODEW* dw,
            DECODEL* dl,
            AUTOW*   ag,
            DECODEW* dg,
            AUTOW*   am,
            AUTOW*   av,
            DECODEW* dm,
            DECODEW*   dv)
{
    FILE** fileList = createNewset(NUMFILE, 1);

    /***************************EncoderWeights**********************************/
    for (int i = 0; i < 773; ++i)
        {
            for (int j = 0; j < 600; ++j)
                {
                    fprintf(fileList[0], "%f\n", aw->weight0[i][j]);
                }
        }

    for (int i = 0; i < 600; ++i)
        {
            for (int j = 0; j < 400; ++j)
                {
                    fprintf(fileList[0], "%f\n", aw->weight1[i][j]);
                }
        }

    for (int i = 0; i < 400; ++i)
        {
            for (int j = 0; j < 200; ++j)
                {
                    fprintf(fileList[0], "%f\n", aw->weight2[i][j]);
                }
        }

    for (int i = 0; i < 200; ++i)
        {
            for (int j = 0; j < 100; ++j)
                {
                    fprintf(fileList[0], "%f\n", aw->weight3[i][j]);
                }
        }

    /***************************EncoderLayers**********************************/
    for (int i = 0; i < 1; ++i)
        {
            for (int j = 0; j < 773; ++j)
                {
                    fprintf(fileList[1], "%f\n", al->input[i][j]);
                }
        }

    for (int i = 0; i < 1; ++i)
        {
            for (int j = 0; j < 600; ++j)
                {
                    fprintf(fileList[1], "%f\n", al->layer1[i][j]);
                }
        }

    for (int i = 0; i < 1; ++i)
        {
            for (int j = 0; j < 400; ++j)
                {
                    fprintf(fileList[1], "%f\n", al->layer2[i][j]);
                }
        }

    for (int i = 0; i < 1; ++i)
        {
            for (int j = 0; j < 200; ++j)
                {
                    fprintf(fileList[1], "%f\n", al->layer3[i][j]);
                }
        }

    for (int i = 0; i < 1; ++i)
        {
            for (int j = 0; j < 100; ++j)
                {
                    fprintf(fileList[1], "%f\n", al->output[i][j]);
                }
        }

    /***************************DecoderWeights**********************************/
    for (int i = 0; i < 100; ++i)
        {
            for (int j = 0; j < 200; ++j)
                {
                    fprintf(fileList[2], "%f\n", dw->weight0[i][j]);
                }
        }

    for (int i = 0; i < 200; ++i)
        {
            for (int j = 0; j < 400; ++j)
                {
                    fprintf(fileList[2], "%f\n", dw->weight1[i][j]);
                }
        }

    for (int i = 0; i < 400; ++i)
        {
            for (int j = 0; j < 600; ++j)
                {
                    fprintf(fileList[2], "%f\n", dw->weight2[i][j]);
                }
        }

    for (int i = 0; i < 600; ++i)
        {
            for (int j = 0; j < 773; ++j)
                {
                    fprintf(fileList[2], "%f\n", dw->weight3[i][j]);
                }
        }

    /***************************DecoderLayers**********************************/
    for (int i = 0; i < 1; ++i)
        {
            for (int j = 0; j < 200; ++j)
                {
                    fprintf(fileList[3], "%f\n", dl->layer1[i][j]);
                }
        }

    for (int i = 0; i < 1; ++i)
        {
            for (int j = 0; j < 400; ++j)
                {
                    fprintf(fileList[3], "%f\n", dl->layer2[i][j]);
                }
        }

    for (int i = 0; i < 1; ++i)
        {
            for (int j = 0; j < 600; ++j)
                {
                    fprintf(fileList[3], "%f\n", dl->layer3[i][j]);
                }
        }

    for (int i = 0; i < 1; ++i)
        {
            for (int j = 0; j < 773; ++j)
                {
                    fprintf(fileList[3], "%f\n", dl->output[i][j]);
                }
        }

    /***************************AutoGradients**********************************/
    for (int i = 0; i < 773; ++i)
        {
            for (int j = 0; j < 600; ++j)
                {
                    fprintf(fileList[4], "%f\n", ag->weight0[i][j]);
                }
        }

    for (int i = 0; i < 600; ++i)
        {
            for (int j = 0; j < 400; ++j)
                {
                    fprintf(fileList[4], "%f\n", ag->weight1[i][j]);
                }
        }

    for (int i = 0; i < 400; ++i)
        {
            for (int j = 0; j < 200; ++j)
                {
                    fprintf(fileList[4], "%f\n", ag->weight2[i][j]);
                }
        }

    for (int i = 0; i < 200; ++i)
        {
            for (int j = 0; j < 100; ++j)
                {
                    fprintf(fileList[4], "%f\n", ag->weight3[i][j]);
                }
        }

    /***************************DecodeGradients**********************************/
    for (int i = 0; i < 100; ++i)
        {
            for (int j = 0; j < 200; ++j)
                {
                    fprintf(fileList[5], "%f\n", dg->weight0[i][j]);
                }
        }

    for (int i = 0; i < 200; ++i)
        {
            for (int j = 0; j < 400; ++j)
                {
                    fprintf(fileList[5], "%f\n", dg->weight1[i][j]);
                }
        }

    for (int i = 0; i < 400; ++i)
        {
            for (int j = 0; j < 600; ++j)
                {
                    fprintf(fileList[5], "%f\n", dg->weight2[i][j]);
                }
        }

    for (int i = 0; i < 600; ++i)
        {
            for (int j = 0; j < 773; ++j)
                {
                    fprintf(fileList[5], "%f\n", dg->weight3[i][j]);
                }
        }

    /***************************AM**********************************/
    for (int i = 0; i < 773; ++i)
        {
            for (int j = 0; j < 600; ++j)
                {
                    fprintf(fileList[6], "%f\n", am->weight0[i][j]);
                }
        }

    for (int i = 0; i < 600; ++i)
        {
            for (int j = 0; j < 400; ++j)
                {
                    fprintf(fileList[6], "%f\n", am->weight1[i][j]);
                }
        }

    for (int i = 0; i < 400; ++i)
        {
            for (int j = 0; j < 200; ++j)
                {
                    fprintf(fileList[6], "%f\n", am->weight2[i][j]);
                }
        }

    for (int i = 0; i < 200; ++i)
        {
            for (int j = 0; j < 100; ++j)
                {
                    fprintf(fileList[6], "%f\n", am->weight3[i][j]);
                }
        }

    /***************************AV**********************************/
    for (int i = 0; i < 773; ++i)
        {
            for (int j = 0; j < 600; ++j)
                {
                    fprintf(fileList[7], "%f\n", av->weight0[i][j]);
                }
        }

    for (int i = 0; i < 600; ++i)
        {
            for (int j = 0; j < 400; ++j)
                {
                    fprintf(fileList[7], "%f\n", av->weight1[i][j]);
                }
        }

    for (int i = 0; i < 400; ++i)
        {
            for (int j = 0; j < 200; ++j)
                {
                    fprintf(fileList[7], "%f\n", av->weight2[i][j]);
                }
        }

    for (int i = 0; i < 200; ++i)
        {
            for (int j = 0; j < 100; ++j)
                {
                    fprintf(fileList[7], "%f\n", av->weight3[i][j]);
                }
        }

    /***************************DM**********************************/

    for (int i = 0; i < 100; ++i)
        {
            for (int j = 0; j < 200; ++j)
                {
                    fprintf(fileList[8], "%f\n", dm->weight0[i][j]);
                }
        }

    for (int i = 0; i < 200; ++i)
        {
            for (int j = 0; j < 400; ++j)
                {
                    fprintf(fileList[8], "%f\n", dm->weight1[i][j]);
                }
        }

    for (int i = 0; i < 400; ++i)
        {
            for (int j = 0; j < 600; ++j)
                {
                    fprintf(fileList[8], "%f\n", dm->weight2[i][j]);
                }
        }

    for (int i = 0; i < 600; ++i)
        {
            for (int j = 0; j < 773; ++j)
                {
                    fprintf(fileList[8], "%f\n", dm->weight3[i][j]);
                }
        }

    /***************************DV**********************************/
    for (int i = 0; i < 773; ++i)
        {
            for (int j = 0; j < 600; ++j)
                {
                    fprintf(fileList[9], "%f\n", dv->weight0[i][j]);
                }
        }

    for (int i = 0; i < 600; ++i)
        {
            for (int j = 0; j < 400; ++j)
                {
                    fprintf(fileList[9], "%f\n", dv->weight1[i][j]);
                }
        }

    for (int i = 0; i < 400; ++i)
        {
            for (int j = 0; j < 200; ++j)
                {
                    fprintf(fileList[9], "%f\n", dv->weight2[i][j]);
                }
        }

    for (int i = 0; i < 200; ++i)
        {
            for (int j = 0; j < 100; ++j)
                {
                    fprintf(fileList[9], "%f\n", dv->weight3[i][j]);
                }
        }

    closeNewset(fileList);
}

// used for completely recreate an entire encoder/decoder in case of power loss
void LoadNN(AUTOW*   aw,
            AUTOL*   al,
            DECODEW* dw,
            DECODEL* dl,
            AUTOW*   ag,
            DECODEW* dg,
            AUTOW*   am,
            AUTOW*   av,
            DECODEW* dm,
            DECODEW*   dv)
{
    FILE** fileList = createNewset(NUMFILE, 0);
    char   buffer[200];

    /***************************EncoderWeights**********************************/
    for (int i = 0; i < 773; ++i)
        {
            for (int j = 0; j < 600; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[0]),
                           "%lf",
                           &(aw->weight0[i][j]));
                }
        }

    for (int i = 0; i < 600; ++i)
        {
            for (int j = 0; j < 400; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[0]),
                           "%lf",
                           &(aw->weight1[i][j]));
                }
        }

    for (int i = 0; i < 400; ++i)
        {
            for (int j = 0; j < 200; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[0]),
                           "%lf",
                           &(aw->weight2[i][j]));
                }
        }

    for (int i = 0; i < 200; ++i)
        {
            for (int j = 0; j < 100; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[0]),
                           "%lf",
                           &(aw->weight3[i][j]));
                }
        }

    /***************************EncoderLayers**********************************/
    for (int i = 0; i < 1; ++i)
        {
            for (int j = 0; j < 773; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[1]),
                           "%lf",
                           &(al->input[i][j]));
                }
        }

    for (int i = 0; i < 1; ++i)
        {
            for (int j = 0; j < 600; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[1]),
                           "%lf",
                           &(al->layer1[i][j]));
                }
        }

    for (int i = 0; i < 1; ++i)
        {
            for (int j = 0; j < 400; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[1]),
                           "%lf",
                           &(al->layer2[i][j]));
                }
        }

    for (int i = 0; i < 1; ++i)
        {
            for (int j = 0; j < 200; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[1]),
                           "%lf",
                           &(al->layer3[i][j]));
                }
        }

    for (int i = 0; i < 1; ++i)
        {
            for (int j = 0; j < 100; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[1]),
                           "%lf",
                           &(al->output[i][j]));
                }
        }

    /***************************DecoderWeights**********************************/
    for (int i = 0; i < 100; ++i)
        {
            for (int j = 0; j < 200; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[2]),
                           "%lf",
                           &(dw->weight0[i][j]));
                }
        }

    for (int i = 0; i < 200; ++i)
        {
            for (int j = 0; j < 400; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[2]),
                           "%lf",
                           &(dw->weight1[i][j]));
                }
        }

    for (int i = 0; i < 400; ++i)
        {
            for (int j = 0; j < 600; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[2]),
                           "%lf",
                           &(dw->weight2[i][j]));
                }
        }

    for (int i = 0; i < 600; ++i)
        {
            for (int j = 0; j < 773; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[2]),
                           "%lf",
                           &(dw->weight3[i][j]));
                }
        }

    /***************************DecoderLayers**********************************/
    for (int i = 0; i < 1; ++i)
        {
            for (int j = 0; j < 200; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[3]),
                           "%lf",
                           &(dl->layer1[i][j]));
                }
        }

    for (int i = 0; i < 1; ++i)
        {
            for (int j = 0; j < 400; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[3]),
                           "%lf",
                           &(dl->layer2[i][j]));
                }
        }

    for (int i = 0; i < 1; ++i)
        {
            for (int j = 0; j < 600; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[3]),
                           "%lf",
                           &(dl->layer3[i][j]));
                }
        }

    for (int i = 0; i < 1; ++i)
        {
            for (int j = 0; j < 773; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[3]),
                           "%lf",
                           &(dl->output[i][j]));
                }
        }

    /***************************AutoGradients**********************************/
    for (int i = 0; i < 773; ++i)
        {
            for (int j = 0; j < 600; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[4]),
                           "%lf",
                           &(ag->weight0[i][j]));
                }
        }

    for (int i = 0; i < 600; ++i)
        {
            for (int j = 0; j < 400; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[4]),
                           "%lf",
                           &(ag->weight1[i][j]));
                }
        }

    for (int i = 0; i < 400; ++i)
        {
            for (int j = 0; j < 200; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[4]),
                           "%lf",
                           &(ag->weight2[i][j]));
                }
        }

    for (int i = 0; i < 200; ++i)
        {
            for (int j = 0; j < 100; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[4]),
                           "%lf",
                           &(ag->weight3[i][j]));
                }
        }

    /***************************DecodeGradients**********************************/
    for (int i = 0; i < 100; ++i)
        {
            for (int j = 0; j < 200; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[5]),
                           "%lf",
                           &(dg->weight0[i][j]));
                }
        }

    for (int i = 0; i < 200; ++i)
        {
            for (int j = 0; j < 400; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[5]),
                           "%lf",
                           &(dg->weight1[i][j]));
                }
        }

    for (int i = 0; i < 400; ++i)
        {
            for (int j = 0; j < 600; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[5]),
                           "%lf",
                           &(dg->weight2[i][j]));
                }
        }

    for (int i = 0; i < 600; ++i)
        {
            for (int j = 0; j < 773; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[5]),
                           "%lf",
                           &(dg->weight3[i][j]));
                }
        }

    /***************************AM**********************************/
    for (int i = 0; i < 773; ++i)
        {
            for (int j = 0; j < 600; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[6]),
                           "%lf",
                           &(am->weight0[i][j]));
                }
        }

    for (int i = 0; i < 600; ++i)
        {
            for (int j = 0; j < 400; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[6]),
                           "%lf",
                           &(am->weight1[i][j]));
                }
        }

    for (int i = 0; i < 400; ++i)
        {
            for (int j = 0; j < 200; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[6]),
                           "%lf",
                           &(am->weight2[i][j]));
                }
        }

    for (int i = 0; i < 200; ++i)
        {
            for (int j = 0; j < 100; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[6]),
                           "%lf",
                           &(am->weight3[i][j]));
                }
        }

    /***************************AV**********************************/
    for (int i = 0; i < 773; ++i)
        {
            for (int j = 0; j < 600; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[7]),
                           "%lf",
                           &(av->weight0[i][j]));
                }
        }

    for (int i = 0; i < 600; ++i)
        {
            for (int j = 0; j < 400; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[7]),
                           "%lf",
                           &(av->weight1[i][j]));
                }
        }

    for (int i = 0; i < 400; ++i)
        {
            for (int j = 0; j < 200; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[7]),
                           "%lf",
                           &(av->weight2[i][j]));
                }
        }

    for (int i = 0; i < 200; ++i)
        {
            for (int j = 0; j < 100; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[7]),
                           "%lf",
                           &(av->weight3[i][j]));
                }
        }

    /***************************DM**********************************/

    for (int i = 0; i < 100; ++i)
        {
            for (int j = 0; j < 200; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[8]),
                           "%lf",
                           &(dm->weight0[i][j]));
                }
        }

    for (int i = 0; i < 200; ++i)
        {
            for (int j = 0; j < 400; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[8]),
                           "%lf",
                           &(dm->weight1[i][j]));
                }
        }

    for (int i = 0; i < 400; ++i)
        {
            for (int j = 0; j < 600; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[8]),
                           "%lf",
                           &(dm->weight2[i][j]));
                }
        }

    for (int i = 0; i < 600; ++i)
        {
            for (int j = 0; j < 773; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[8]),
                           "%lf",
                           &(dm->weight3[i][j]));
                }
        }

    /***************************DV**********************************/
    for (int i = 0; i < 773; ++i)
        {
            for (int j = 0; j < 600; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[9]),
                           "%lf",
                           &(dv->weight0[i][j]));
                }
        }

    for (int i = 0; i < 600; ++i)
        {
            for (int j = 0; j < 400; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[9]),
                           "%lf",
                           &(dv->weight1[i][j]));
                }
        }

    for (int i = 0; i < 400; ++i)
        {
            for (int j = 0; j < 200; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[9]),
                           "%lf",
                           &(dv->weight2[i][j]));
                }
        }

    for (int i = 0; i < 200; ++i)
        {
            for (int j = 0; j < 100; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[9]),
                           "%lf",
                           &(dv->weight3[i][j]));
                }
        }
    closeNewset(fileList);
}

// used for loading the weight of the last match for training the next match
void RetrWeight(AUTOW* aw, DECODEW* dw)
{
    FILE** fileList = createNewset(NUMFILE, 0);
    char   buffer[200];

    /***************************EncoderWeights**********************************/
    for (int i = 0; i < 773; ++i)
        {
            for (int j = 0; j < 600; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[0]),
                           "%lf",
                           &(aw->weight0[i][j]));
                }
        }

    for (int i = 0; i < 600; ++i)
        {
            for (int j = 0; j < 400; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[0]),
                           "%lf",
                           &(aw->weight1[i][j]));
                }
        }

    for (int i = 0; i < 400; ++i)
        {
            for (int j = 0; j < 200; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[0]),
                           "%lf",
                           &(aw->weight2[i][j]));
                }
        }

    for (int i = 0; i < 200; ++i)
        {
            for (int j = 0; j < 100; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[0]),
                           "%lf",
                           &(aw->weight3[i][j]));
                }
        }

    /***************************DecoderWeights**********************************/
    for (int i = 0; i < 100; ++i)
        {
            for (int j = 0; j < 200; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[2]),
                           "%lf",
                           &(dw->weight0[i][j]));
                }
        }

    for (int i = 0; i < 200; ++i)
        {
            for (int j = 0; j < 400; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[2]),
                           "%lf",
                           &(dw->weight1[i][j]));
                }
        }

    for (int i = 0; i < 400; ++i)
        {
            for (int j = 0; j < 600; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[2]),
                           "%lf",
                           &(dw->weight2[i][j]));
                }
        }

    for (int i = 0; i < 600; ++i)
        {
            for (int j = 0; j < 773; ++j)
                {
                    sscanf(fgets(buffer, 100, fileList[2]),
                           "%lf",
                           &(dw->weight3[i][j]));
                }
        }
    closeNewset(fileList);
}

// USED FOR TESTING ONLY
int main(void)
{
    FILE*  allFiles = fopen("test.txt", "w");
    char   buffer[100];
    double data[2][5] = {{5.4, 3.8, 9.5, 8.55555, 9.12312321},
                         {9, 10, 5, 4.5, 2.3}};
    double received[2][5];
    for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 5; ++j)
                {
                    fprintf(allFiles, "%f\n", data[i][j]);
                    printf(" %f ", data[i][j]);
                    printf("\n");
                }
        }
    printf("\n");
    fclose(allFiles);
    allFiles = fopen("test.txt", "r");
    for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 5; ++j)
                {
                    sscanf(
                        fgets(buffer, 100, allFiles), "%lf", &received[i][j]);
                }
        }
    for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 5; ++j)
                {
                    printf(" %f ", received[i][j]);
                    printf("\n");
                }
        }
    return 0;
}
