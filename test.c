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
    mov(cboard->boardarray, 52, 36);
    //mov(cboard->boardarray, 12, 28);
    cboard->sidetomove = 1;
    allLegal(list,cboard);
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

    BSTATE *copy;
    copy = createBstate();
    copyBstate(board, copy);

    while(p)
    {
        mov(copy->boardarray,p->CLOC, p->NLOC);
        GUI(copy->boardarray);
        copyBstate(board, copy);
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


