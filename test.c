#include <stdio.h>
#include <stdlib.h>
#include "boardstate.h"
#include "movelist.h"

int main()
{
    BSTATE *cboard;
    cboard = createBstate();
    loadStart(cboard);

    MLIST *list;
    list = createMovelist();
    
   // pawnmove(list, cboard,8*1+0);
    
    deleteMovelist(list);

    deleteBstate(cboard);
}
