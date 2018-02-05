#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "boardstate.h"

// This will allocate the memory for the board
BSTATE *createBstate(void)
{
    BSTATE *p;
    p = malloc(sizeof(BSTATE));

    if(!p)
    {
        perror("Out of memory Aborting");
        exit(10);
    }   
    
    p->WQCFlag = 0;
    p->WKCFlag = 0;
    p->BQCFlag = 0;
    p->BKCFlag = 0;
    p->sidetomove = 0;

    return p;
}

// This will delete all the pointers for the board
void deleteBstate(BSTATE *board)
{
    assert(board);
    free(board);
}

// This will load the start of the game to the inputted board
void loadStart(BSTATE *currentboard)
{
    assert(currentboard);

    //setting the start of the game flags
    currentboard->WQCFlag = 1;
    currentboard->WKCFlag = 1;
    currentboard->BQCFlag = 1;
    currentboard->BKCFlag = 1;
    currentboard->sidetomove = 0;

    //put in the start of the board 
    int i,j;

    for(i=0; i<8; ++i)
    {
        for(j=0; j<8; ++j)
        {
            currentboard->boardarray[i][j] = 0;
        }
    }

    for(i=0; i<8; ++i)
    {
        currentboard->boardarray[6][i] = 1;
        currentboard->boardarray[1][i] = 11;
    }
    currentboard->boardarray[0][0] = 14;
    currentboard->boardarray[0][1] = 12;
    currentboard->boardarray[0][2] = 13;
    currentboard->boardarray[0][3] = 15;
    currentboard->boardarray[0][4] = 16;
    currentboard->boardarray[0][5] = 13;
    currentboard->boardarray[0][6] = 12;
    currentboard->boardarray[0][7] = 14;
    currentboard->boardarray[7][0] = 4;
    currentboard->boardarray[7][1] = 2;
    currentboard->boardarray[7][2] = 3;
    currentboard->boardarray[7][3] = 5;
    currentboard->boardarray[7][4] = 6;
    currentboard->boardarray[7][5] = 3;
    currentboard->boardarray[7][6] = 2;
    currentboard->boardarray[7][7] = 4;

}

void copyBstate(BSTATE *old, BSTATE *new)
{
    assert(old);
    assert(new);

    new->WQCFlag = old->WQCFlag;
    new->WKCFlag = old->WKCFlag;
    new->BQCFlag = old->BQCFlag;
    new->BKCFlag = old->BKCFlag;
    new->sidetomove = old->sidetomove;

    int i,j;

    for(i=0; i<8; ++i)
    {
        for(j=0; j<8; ++j)
        {
            new->boardarray[i][j] = old->boardarray[i][j];
        }
    }
}

void updateBstate(BSTATE *board)
{
    assert(board);
    if(board->WKCFlag == 1)
    {
        if(board->boardarray[7][4] != 6 || board->boardarray[7][7] != 4)
        {
            board->WKCFlag = 0;
        }
    }
    if(board->WQCFlag == 1)
    {
        if(board->boardarray[7][4] != 6 || board->boardarray[7][0] != 4)
        {
            board->WQCFlag = 0;
        }
    }
    if(board->BKCFlag == 1)
    {
        if(board->boardarray[0][4] != 16 || board->boardarray[0][7] != 14)
        {
            board->BKCFlag = 0;
        }
    }
    if(board->BQCFlag == 1)
    {
        if(board->boardarray[0][4] != 6 || board->boardarray[0][0] != 4)
        {
            board->BQCFlag = 0;
        }
    }
    
}




