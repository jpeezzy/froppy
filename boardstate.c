#include <stdio.h>
#include <stdlib.h>
#include "boardcontrol.h"

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
    
    p->boardarray[8][8] = {0};
    p->WQCFlag = NULL;
    p->WKCFlag = NULL;
    p->BQCFlag = NULL;
    p->BKCFLag = NULL;
    p->sidetomove = NULL;
}

// This will delete all the pointers for the board
void deleteBstate(BSTATE *board)
{
    assert(board);

}

// This will load the start of the game to the inputted board
void loadStart(BSTATE *currentboard)
{
    assert(currentboard);

    //setting the start of the game flags
    currentboard->WQCFLAG = 1;
    currentboard->WKCFlag = 1;
    currentboard->BQCFlag = 1;
    currentboard->BKCFLag = 1;
    currentboard->sidetoimove = 0;

    //put in the start of the board 
    

}

