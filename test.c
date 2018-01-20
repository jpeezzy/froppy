#include <stdio.h>
#include <stdlib.h>
#include "boardstate.h"
#include "movelist.h"
#include "testgui.h"
#include <assert.h>

void seemoves(MLIST *movelist, BSTATE *board);
void empty(BSTATE *board);

int main()
{
    BSTATE *cboard;
    cboard = createBstate();
    loadStart(cboard);

    MLIST *list;
    list = createMovelist();
    
    //GUI(cboard->boardarray);
    empty(cboard);
    cboard->boardarray[0][0] = 3;
    bishopmove(list, cboard, 0);
    seemoves(list,cboard);
    deleteMovelist(list);

    deleteBstate(cboard);
}

void seemoves(MLIST *movelist, BSTATE *board)
{
    assert(movelist);
    assert(board);

    MENTRY *p;
    p = movelist->First;

    while(p)
    {
        mov(board->boardarray,p->CLOC, p->NLOC);
        GUI(board->boardarray);
        mov(board->boardarray, p->NLOC, p->CLOC);
        p = p->Next;
    }
}

void empty(BSTATE *board)
{
    int i,j;
    for(i= 0; i<8; ++i)
    {
        for(j=0; j<8; ++j)
        {
            board->boardarray[i][j] = 0;
        }
    }

}


