#include <stdio.h>
#include <stdlib.h>
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
int checkLegal(BSTATE *board, int cloc, int nloc);

#define DEPTH 7

int main()
{   
    MLIST *legal;
    int play = 1;
    int p, bw;
    printf("0 for P vs AI, 1 for P vs P, 2 for AI vs AI: ");
    scanf("%d", &p);
    BSTATE* board;
    board = createBstate();
    loadStart(board);
    GUI(board->boardarray);
    int score = basicEvaluation(board);
    printf("Eval score = %d\n", score);
    if(p == 1)
    {
        while(play)
        {
            printf("White's turn\n");
            playerMove(board);
            GUI(board->boardarray);
            score = basicEvaluation(board);
            printf("Current Board Value = %d\n", score);
            legal = createMovelist();
            allLegal(legal, board);
            if(legal->movenum == 0)
            {
                printf("Checkmate!\n");
                deleteMovelist(legal);
                break;
            }
            deleteMovelist(legal);
            printf("Black's turn\n");
            playerMove(board);
            GUI(board->boardarray);
            score = basicEvaluation(board);
            printf("Current Board Value = %d\n", score);
            legal = createMovelist();
            allLegal(legal, board);
            if(legal->movenum == 0)
            {
                printf("Checkmate!\n");
                deleteMovelist(legal);
                break;
            }
            deleteMovelist(legal); 
            printf("Enter 1 to continue, 0 to exit: ");
            scanf("%d", &play);
        }
    }
    else if (p == 0)
    {
        printf("Enter Black(1) or White(0)?: ");
        scanf("%d",&bw);
        if(bw)
        {
            printf("You are playing as black\n");
            while(play)
            {
                aiMove(board);
                GUI(board->boardarray);
                score = basicEvaluation(board);
                printf("Current Board Value = %d\n", score);
                legal = createMovelist();
                allLegal(legal, board);
                if(legal->movenum == 0)
                {
                    printf("Checkmate!\n");
                    deleteMovelist(legal);
                    break;
                }
                deleteMovelist(legal);
                playerMove(board);
                GUI(board->boardarray);
                score = basicEvaluation(board);
                printf("Current Board Value = %d\n", score);
                legal = createMovelist();
                allLegal(legal, board);
                if(legal->movenum == 0)
                {
                    printf("Checkmate!\n");
                    deleteMovelist(legal);
                    break;
                }
                deleteMovelist(legal);
                printf("Enter 1 to continue, 0 to exit: ");
                scanf("%d", &play);
            }
        }
        else
        {
            printf("You are playing as white\n");
            while(play)
            {
                playerMove(board);
                GUI(board->boardarray);
                score = basicEvaluation(board);
                printf("Current Board Value = %d\n", score);
                legal = createMovelist();
                allLegal(legal, board);
                if(legal->movenum == 0)
                {
                    printf("Checkmate!\n");
                    deleteMovelist(legal);
                    break;
                }
                deleteMovelist(legal);
                aiMove(board);
                GUI(board->boardarray);
                score = basicEvaluation(board);
                printf("Current Board Value = %d\n", score);
                legal = createMovelist();
                allLegal(legal, board);
                if(legal->movenum == 0)
                {
                    printf("Checkmate!\n");
                    deleteMovelist(legal);
                    break;
                }
                deleteMovelist(legal);
                printf("Enter 1 to continue, 0 to exit: ");
                scanf("%d", &play);
            }
        }
    }
    else if (p == 2)
    {
        while(play)
        {
            aiMove(board);
            GUI(board->boardarray);
            score = basicEvaluation(board);
            printf("Current Board Value = %d\n", score);
            legal = createMovelist();
            allLegal(legal, board);
            if(legal->movenum == 0)
            {
                printf("Checkmate!\n");
                printf("Enter 1 to simulate another game, 0 to exit: ");
                scanf("%d", &play);
                if(play)
                {
                    deleteBstate(board);
                    board = createBstate();
                    loadStart(board);
                }
            }
            deleteMovelist(legal);
        }
    }   
    deleteBstate(board); 
    return 0;
}

void playerMove(BSTATE *board)
{
     assert(board);
     int iCloc, iNloc;
     char cloc[2], nloc[2];
     int legal = 0;
     do
     {
     printf("Enter location of piece to move: ");
     scanf("%s", cloc);
     printf("Enter destination location: ");
     scanf("%s", nloc);
     iCloc = strToInt(cloc);
     iNloc = strToInt(nloc);
     if(iCloc == 60 && iNloc == 62 && board->boardarray[iCloc/8][iCloc%8] == 6)
     {
        iCloc = 1;
        iNloc = 1;
     }
     if(iCloc == 60 && iNloc == 58 && board->boardarray[iCloc/8][iCloc%8] == 6)
     {
        iCloc = 2;
        iNloc = 2;
     }
     if(iCloc == 4 && iNloc == 6 && board->boardarray[iCloc/8][iCloc%8] == 16)
     {
        iCloc = 3;
        iNloc = 3;
     }
     if(iCloc == 4 && iNloc == 2 && board->boardarray[iCloc/8][iCloc%8] == 16)
     {
        iCloc = 4;
        iNloc = 4;
     }
     legal = checkLegal(board, iCloc, iNloc);
     if(!legal)
     {
        printf("The move entered is not legal, try again.\n");
     }
     }while(!legal);
     mov(board->boardarray, iCloc, iNloc);
     updateBstate(board);
     changeSide(board);
}

void aiMove(BSTATE *board)
{
    assert(board);
    MENTRY *move;
    move = minmax(board);
    assert(move);
    int legal = checkLegal(board, move->CLOC, move->NLOC);
    if(!legal)
    {
        printf("AI made invalid move\n");
    }
    mov(board->boardarray, move->CLOC, move->NLOC);
    updateBstate(board);
    changeSide(board);  
    free(move);
    move = NULL; 
    
    // new minmax using depth
/*  
    MINI *mini;
    BSTATE *temp;
    float score;
    temp = createBstate();
    copyBstate(board, temp);
    mini = createMini(temp, DEPTH);
    score = altMax(mini, -3.4E38, 3.4E38, DEPTH);
    assert(mini->move);
    int legal = checkLegal(board, mini->move->CLOC, mini->move->NLOC);
    if(!legal)
    {
        printf("AI made invalid move\n");
    }
    mov(board->boardarray, mini->move->CLOC, mini->move->NLOC);
    changeSide(board);
    removeMini(mini);
    deleteBstate(temp); 
*/
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

/* return 1 if a move is legal, 0 if not */
int checkLegal(BSTATE *board, int cloc, int nloc)
{
    MLIST *legal;
    MENTRY *current;
    legal = createMovelist();
    allLegal(legal, board);
    assert(legal->First);
    current = legal->First;
    while(current)
    {
        if(current->CLOC == cloc && current->NLOC == nloc)
        {
            deleteMovelist(legal);
            return 1;
        }
        current = current->Next;
    }
    deleteMovelist(legal);
    return 0;
}
