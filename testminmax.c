#include <stdio.h>
#include <assert.h>
#include "testgui.h"
#include "minmax.h"
#include "boardstate.h"
#include "basic_eval.h"

void playerMove(BSTATE *board);
void aiMove(BSTATE *board);
void changeSide(BSTATE *board);

int main()
{   
    int play;
    BSTATE* board;
    board = createBstate();
    loadStart(board);
    GUI(board->boardarray);
    int score = basicEvaluation(board);
    printf("Eval score = %d\n", score);
    printf("You are playing as white");
    while(play)
    {
        playerMove(board);
        GUI(board->boardarray);
        aiMove(board);
        GUI(board->boardarray);
        printf("Enter 1 to continue, 0 to exit");
        scanf("%d", &play);
    }    
    deleteBstate(board);
    return 0;
}

void playerMove(BSTATE *board)
{
     assert(board);
     int cloc, nloc;
     printf("Enter location of piece to move: ");
     scanf("%d", &cloc);
     printf("Enter destionation location: ");
     scanf("%d", &nloc);
     mov(board->boardarray, cloc, nloc);
     changeSide(board);
}

void aiMove(BSTATE *board)
{
    assert(board);
    MENTRY *move;
    move = minmax(board);
    mov(board->boardarray, move->CLOC, move->NLOC);
    changeSide(board);   
}

void changeSide(BSTATE *board)
{
     assert(board);
     if(board->sidetomove)
     {
        board->sidetomove = 0;
     } 
     else
     {   
        board->sidetomove = 1;   
     }
}
