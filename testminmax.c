#include <stdio.h>
#include <assert.h>
#include "testgui.h"
#include "minmax.h"
#include "boardstate.h"
#include "basic_eval.h"
#include "movelist.h"

void playerMove(BSTATE *board);
void aiMove(BSTATE *board);
void changeSide(BSTATE *board);
int strToInt(char loc[]);

int main()
{   
    MLIST *legal;
    int play = 1;
    BSTATE* board;
    board = createBstate();
    loadStart(board);
    GUI(board->boardarray);
    int score = basicEvaluation(board);
    printf("Eval score = %d\n", score);
    printf("You are playing as white\n");
    while(play)
    {
        playerMove(board);
        GUI(board->boardarray);
        legal = createMovelist();
        allLegal(legal, board);
        printf("legal movenum = %d\n", legal->movenum);
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
     char cloc[2], nloc[2];
     printf("Enter location of piece to move: ");
     scanf("%s", cloc);
     printf("Enter destionation location: ");
     scanf("%s", nloc);
     mov(board->boardarray, strToInt(cloc), strToInt(nloc));
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

int strToInt(char loc[])
{
    int x,y;
    switch(loc[0])
    {
        case 'a':
            x = 0;
            break;
        case 'b':
            x = 1;
            break;
        case 'c':
            x = 2;
            break;
        case 'd':
            x = 3;
            break;
        case 'e':
            x = 4;
            break;
        case 'f':
            x = 5;
            break;
        case 'g':
            x = 6;
            break;
        case 'h':
            x = 7;
            break;
    }
    switch(loc[1])
    {
        case '1':
            y = 7;
            break;
        case '2':
            y = 6;
            break;
        case '3':
            y = 5;
            break;
        case '4':
            y = 4;
            break;
        case '5':
            y = 3;
            break;
        case '6':
            y = 2;
            break;
        case '7':
            y = 1;
            break;
        case '8':
            y = 0;
            break;
    }
    return y*8+x;               
}
