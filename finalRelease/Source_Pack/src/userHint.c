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
#include "SDL/SDL.h"

void giveHint(BSTATE *board, SDL_Surface *LoadingHint, SDL_Surface *screen)
{ 
    SDL_BlitSurface(LoadingHint, NULL, screen, NULL);
    SDL_Flip(screen);
    assert(board);
    MENTRY* bestMove=minmax(board);
    if(bestMove==NULL)
    {
        printf("\nYou are about to lose\n");
        return;
    }

    
    // offset by 1 when printing out
    // offset by 65 to go from 0 to 'A'
    // take 9 and subtract map from 
    // one range to another
    int desrow=9-(bestMove->NLOC/8 +1);
    char descol=bestMove->NLOC%8;
    descol+=65;

    int sourrow=9-(bestMove->CLOC/8 +1);
    char sourcol=bestMove->CLOC%8;
    sourcol+=65;

    printf("The position that you should move is from %c%d to %c%d\n",sourcol,sourrow, descol, desrow);
}

//used only for testing
