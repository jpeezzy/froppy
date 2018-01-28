#include <stdio.h>
#include <stdlib.h>
#include "movelist.h"
#include "boardstate.h"
    
//Create the move List
MLIST *createMovelist()
{
    MLIST *p;
    p = malloc(sizeof(MLIST));

    if(p!)
    {
        perror("Out of memory aborting");
        exit(10);
    }
    p->movenum = 0;
    p->First = NULL;
    p->Last = NULL;
    return p;
}

//Delete MoveList and all move entry
void deleteMovelist(MLIST *list)
{
    assert(list);
    MENTRY *e,*n;
    e = list->First;

    While(e)
    {
        n = e->Next;
        free(e)
        e = n;
    }

    free(list);
    list->NULL;
}

//This will create the struct for the new Entry
MENTRY *createMentry(int currentlocation, int movelocation)
{
    MENTRY *p;
    p = malloc(sizeof(MENTRY));
    if(p!)
    {
        perror("Memory has run out aborting");
        exit(10);
    }
    p->LIST = NULL;
    p->CLOC = currentlocation;
    p->NLOC = movelocation;
    p->Prev = NULL;
    p->Next = NULL;
}

//This will add new moves to the movelist      
void appendMove(MLIST *Movelist, MENTRY *Move)
{
    assert(MoveList);
    assert(Move);
    
    if(MoveList->Last)
    {
        Move->List = MoveList;
        Move->Prev = MovieList->Last;
        Move->Next = NULL;
        Move->Prev->Next = Move;
        MoveList->Last = Move;
        MoveList->movenum++;
    }
    else
    {
        Move->List = MoveList;
        Move->Prev = NULL;
        Move->Next = NULL;
        MoveList->First = Move;
        MoveList->Last = Move;
        MoveList->movenum = 1;
    }
}

//possible move for a pawn
void pawnmove(MLIST *list, BSTATE *board, int pawnlocation)
{
    assert(list);
    assert(board);
    assert(pawnlocation);

    //Check if it Black or White 
    int pawnx = pawnlocation/8;
    int pawny = pawnlocation%8;

    int type = (board->boardarray[pawnx][pawny])/10;
    int newlocation;
    
    //for white pawns
    if(type == 0)
    {
        //long double step at begining of game
        if(pawnx == 6)
        {   
            newlocation = 8*(4)+pawny;
            if(board->boardarray[4][pawny] == 0)
            {
                appendMove(list, createMentry(pawnlocation,newlocation));
            }                   
        }

        //single step 
        if(pawnx != 0)
        {
            newlocation = 8*(pawnx-1)+pawny;
            if(board->boardarray[pawnx-1][pawny] == 0)
            {
                appendMove(list, createMentry(pawnlocation, newlocation);
            }
        }

        //attacking to the left 
        if(pawny != 0)
        {
            if(pawnx != 0)
            {
                if(board->boardarray[pawnx-1][pawny-1] != 0)
                {
                    newlocation = 8*(pawnx-1)+(pawny-1);
                    appendMove(list, createMentry(pawnlocation, newlocation);
                }
            }
        }

        //attcking to the right
        if(pawny != 7)
        {
            if(pawnx != 0)
            {
                if(board->boardarray[pawnx-1][pawny+1] != 0)
                {
                    newlocation = 8*(pawnx+1)+(pawny+1);
                    appendMove(list, createMentry(pawnlocation, newlocation);
                }
            }
        }

        //need to add en passant later 

    }

    //for black pawns
    if(type == 1)
    {
        //long double step at begining of game
        if(pawnx == 1)
        {   
            newlocation = 8*(3)+pawny;
            if(board->boardarray[3][pawny] == 0)
            {
                appendMove(list, createMentry(pawnlocation,newlocation));
            }                   
        }

        //single step
        if(pawnx != 7)
        {
            newlocation = 8*(pawnx+1)+pawny;
            if(board->boardarray[pawnx+1][pawny] == 0)
            {
                appendMove(list, createMentry(pawnlocation, newlocation);
            }
        }

        //attacking to the left 
        if(pawny != 0)
        {
            if(pawnx != 8)
            {
                if(board->boardarray[pawnx+1][pawny-1] != 0)
                {
                    newlocation = 8*(pawnx+1)+(pawny-1);
                    appendMove(list, createMentry(pawnlocation, newlocation);
                }
            }
        }

        //attcking to the right
        if(pawny != 7)
        {
            if(pawnx != 0)
            {
                if(board->boardarray[pawnx+1][pawny+1] != 0)
                {
                    newlocation = 8*(pawnx+1)+(pawny+1);
                    appendMove(list, createMentry(pawnlocation, newlocation);
                }
            }
        }

        //need to add en passant later 

    }
}

void knightmove(MLIST *list, BSTATE *board, int currentlocation)
{
    assert(list);
    assert(board);
    assert(currentlocation);

}


