/**
 * Give user hints based on the best move possible 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "userHint.h"
#include "minmax.h"
#include "movelist.h"
#include "boardstate.h"

void giveHint(BSTATE *board)
{  
    assert(board);
    MENTRY* bestMove=minmax(board);
    if(bestMove==NULL)
    {
        printf("\nYou are about to lose\n");
        return;
    }
    int row;
    char col;

    //offset by 1 when printing out
    //offset by 65 to go from 0 to 'A'
    row=bestMove->NLOC/8 +1;
    col=bestMove->NLOC%8;
    col+=65;

    printf("The position that you should move is %c%d\n",col,row);
}
