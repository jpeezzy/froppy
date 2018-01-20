#include <stdio.h>
#include <stdlib.h>
#include "boardstate.h"
#include "movelist.h"
#include "testgui.h"
#include <assert.h>

void seemoves(MLIST *movelist, BSTATE *board);

int main()
{
    BSTATE *cboard;
    cboard = createBstate();
    loadStart(cboard);

    MLIST *list;
    list = createMovelist();
    
    //GUI(cboard->boardarray);
    knightmove(list, cboard,57);
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




