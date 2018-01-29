#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "dataEntry.h"
#include "fenToBoardState.h"
#include "randGen.h"

DATABASE* createDataB(void)
{
    DATABASE* temp  = malloc(sizeof(DATABASE));
    temp->totalData = 0;
    return temp;
}

ENTRY* createEntry(DATABASE* datab, BSTATE* state)
{
    ENTRY* temp = malloc(sizeof(ENTRY));
    temp->base  = datab;
    temp->state = state;
    return temp;
}

// read a fen file and returns a database comprising of all the moves
void readFenfile(FILE* fenFilehandle, DATABASE* dataMain)
{
    assert(dataMain);
    assert(fenFilehandle);
    char curLine[100];
    dataMain->totalData = 0;
    dataMain->firstMove = NULL;
    dataMain->lastMove  = NULL;
    ENTRY* curEntry     = NULL;
    // read a file line by line and records the result into a database
    while (fgets(curLine, 100, fenFilehandle) != NULL)
        {
            if (dataMain->totalData == 0)
                {
                    dataMain->firstMove =
                        createEntry(dataMain, malloc(sizeof(BSTATE)));
                    assert(dataMain->firstMove);
                    fenToBoardState(curLine, dataMain->firstMove->state);
                    curEntry = dataMain->firstMove;
                }
            else
                {
                    curEntry->next =
                        createEntry(dataMain, malloc(sizeof(BSTATE)));
                    fenToBoardState(curLine, curEntry->state);
                    curEntry = curEntry->next;
                }
            dataMain->totalData = dataMain->totalData + 1;
        }
    dataMain->lastMove = curEntry;
}

// pick and return a random move
BSTATE pickRandMove(DATABASE* dataMain)
{
    assert(dataMain->totalData);
    assert(dataMain);
    srand(time(NULL));
    ENTRY* curEntry = NULL;
    int    num      = (int)randGenerate() % (dataMain->totalData);
    int    count    = 0;
    curEntry        = dataMain->firstMove;
    while (count < num)
        {
            curEntry = curEntry->next;
            ++count;
        }
    return *(curEntry->state);
}