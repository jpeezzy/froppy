#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "dataEntry.h"
#include "fenToBoardState.h"
#include "randGen.h"
#include "movelist.h"

DATABASE* createDataB(void)
{
    DATABASE* temp  = malloc(sizeof(DATABASE));
    temp->totalData = 0;
    temp->firstMove=NULL;
    temp->lastMove=NULL;
    return temp;
}

ENTRY* createEntry(DATABASE* datab, BSTATE* state)
{
    ENTRY* temp = malloc(sizeof(ENTRY));
    temp->base  = datab;
    temp->state = state;
    temp->next=NULL;
    temp->prev=NULL;
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

//free the fen file in the RAM
void freeFenfile(DATABASE* dataMain)
{
    assert(dataMain);
    assert(dataMain->totalData);
    ENTRY* curEntry     = NULL;
    ENTRY* temp=NULL;
    curEntry=dataMain->firstMove;
    assert(curEntry);
    while(curEntry!=NULL)
    {
        free(curEntry->state);
        if((curEntry->next)!=NULL)
        {
            temp=curEntry;
            curEntry=curEntry->next;
            free(temp);
        }
        else
        {
          free(curEntry);
          break;
        }
    }
    dataMain->totalData=0;
    free(dataMain);
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


// pick a random move and add it into the movelist database
void addRandom(MLIST* dataMain, BSTATE* board)
{
    assert(dataMain);
    assert(board);
    MLIST* listMoves=createMovelist();
    assert(listMoves);
    MENTRY* curEntry=NULL;
    int num;
    int count=0;

    //create a list of all legal moves to pick from
    allLegal(listMoves, board);
    if(listMoves->movenum==0)
    {
        #ifdef DEBUG
        printf("There is no move to pick from\n");
        #endif
        deleteMovelist(listMoves);
        return;
    }

    //pick a random number 
    num = (int)randGenerate() % (listMoves->movenum);
    curEntry = listMoves->First;
    assert(curEntry);

    //traversing the linked list until reaching the picked move
    while (count < num)
        {
            curEntry = curEntry->Next;
            assert(curEntry);
            ++count;
        }

    appendMove(dataMain, createMentry(curEntry->CLOC, curEntry->NLOC));
    deleteMovelist(listMoves);
}   