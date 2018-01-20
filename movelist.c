#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "movelist.h"
#include "boardstate.h"
 
//Move a piece doesnt check anything
void mov(int barr[8][8], int ploc, int pmov)
{
    int temp = barr[ploc/8][ploc%8];
    barr[ploc/8][ploc%8] = 0;
    barr[pmov/8][pmov%8] = temp;
}
  
//Create the move List
MLIST *createMovelist()
{
    MLIST *p;
    p = malloc(sizeof(MLIST));

    if(!p)
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

    while(e)
    {
        n = e->Next;
        free(e);
        e = n;
    }

    free(list);
    list = NULL;
}

//This will create the struct for the new Entry
MENTRY *createMentry(int currentlocation, int movelocation)
{
    MENTRY *p;
    p = malloc(sizeof(MENTRY));
    if(!p)
    {
        perror("Memory has run out aborting");
        exit(10);
    }
    p->List = NULL;
    p->CLOC = currentlocation;
    p->NLOC = movelocation;
    p->Prev = NULL;
    p->Next = NULL;

    return p;
}

//This will add new moves to the movelist      
void appendMove(MLIST *Movelist, MENTRY *Move)
{
    assert(Movelist);
    assert(Move);
    
    if(Movelist->Last)
    {
        Move->List = Movelist;
        Move->Prev = Movelist->Last;
        Move->Next = NULL;
        Move->Prev->Next = Move;
        Movelist->Last = Move;
        Movelist->movenum++;
    }
    else
    {
        Move->List = Movelist;
        Move->Prev = NULL;
        Move->Next = NULL;
        Movelist->First = Move;
        Movelist->Last = Move;
        Movelist->movenum = 1;
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
        //double step at begining of game
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
                appendMove(list, createMentry(pawnlocation, newlocation));
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
                    appendMove(list, createMentry(pawnlocation, newlocation));
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
                    appendMove(list, createMentry(pawnlocation, newlocation));
                }
            }
        }

        //need to add en passant later 

    }

    //for black pawns
    if(type == 1)
    {
        //double step at begining of game
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
                appendMove(list, createMentry(pawnlocation, newlocation));
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
                    appendMove(list, createMentry(pawnlocation, newlocation));
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
                    appendMove(list, createMentry(pawnlocation, newlocation));
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

    int knightx = currentlocation/8;
    int knighty = currentlocation%8;
    int newlocation;
    int type = (board->boardarray[knightx][knighty])/10;

    if(knightx>2 && knighty != 0 && ((board->boardarray[knightx-2][knighty-1])/10 != type || board->boardarray[knightx-2][knighty-1] == 0))
    {
        newlocation = 8*(knightx-2)+(knighty-1);
        appendMove(list, createMentry(currentlocation, newlocation));
    }
    if(knightx<6 && knighty != 0 && ((board->boardarray[knightx+2][knighty-1])/10 != type || board->boardarray[knightx+2][knighty-1] == 0))
    {
        newlocation = 8*(knightx+2)+(knighty-1);
        appendMove(list, createMentry(currentlocation, newlocation));
    }
    if(knightx<6 && knighty != 7 && ((board->boardarray[knightx+2][knighty+1])/10 != type || board->boardarray[knightx+2][knighty+1] == 0))
    {
        newlocation = 8*(knightx+2)+(knighty+1);
        appendMove(list, createMentry(currentlocation, newlocation));
    }
    if(knightx>2 && knighty != 7 && ((board->boardarray[knightx-2][knighty+1])/10 != type || board->boardarray[knightx-2][knighty+1] == 0))
    {
        newlocation = 8*(knightx-2)+(knighty+1);
        appendMove(list, createMentry(currentlocation, newlocation));
    }

    if(knightx != 0 && knighty > 2 && ((board->boardarray[knightx-1][knighty-2])/10 != type || board->boardarray[knightx-1][knighty-2] == 0))
    {
        newlocation = 8*(knightx-1)+(knighty-2);
        appendMove(list, createMentry(currentlocation, newlocation));
    }
    if(knightx != 0 && knighty < 6 && ((board->boardarray[knightx+1][knighty-2])/10 != type || board->boardarray[knightx+1][knighty-2] == 0))
    {
        newlocation = 8*(knightx+1)+(knighty-2);
        appendMove(list, createMentry(currentlocation, newlocation));
    }
    if(knightx != 7 && knighty < 6 && ((board->boardarray[knightx+1][knighty+2])/10 != type || board->boardarray[knightx+1][knighty+2] == 0))
    {
        newlocation = 8*(knightx+1)+(knighty+2);
        appendMove(list, createMentry(currentlocation, newlocation));
    }
    if(knightx != 7 && knighty > 2 && ((board->boardarray[knightx-1][knighty+2])/10 != type || board->boardarray[knightx-1][knighty+2] == 0))
    {
        newlocation = 8*(knightx-1)+(knighty+2);
        appendMove(list, createMentry(currentlocation, newlocation));
    }

}


void rookmove(MLIST *list, BSTATE *board, int currentlocation)
{

    assert(list);
    assert(board);
    assert(currentlocation);

    int rookx = currentlocation/8;
    int rooky = currentlocation%8;
    int type = board->boardarray[rookx][rooky]/10;
    int newlocation;
    int i;

    //go left
    if(rooky != 0)
    {
        for(i=1; i<=(rooky); ++i)
        {
            newlocation = 8*(rookx)+(rooky-i);
            if( (board->boardarray[rookx][rooky-i]/10 != type) && (board->boardarray[rookx][rooky-i] != 0))
            {
                appendMove(list, createMentry(currentlocation, newlocation));
                break;
            }
            else if( (board->boardarray[rookx][rooky-i]/10 == type) && (board->boardarray[rookx][rooky-i] != 0))
            {
                break;
            }
            else
            {
                appendMove(list, createMentry(currentlocation, newlocation));
            }
        }
     }

    //go right
    if(rooky != 7)
    {
        for(i=1; i<=7-(rooky); ++i)
        {
            newlocation = 8*(rookx)+(rooky+i);
            if( (board->boardarray[rookx][rooky+i]/10 != type) && (board->boardarray[rookx][rooky+i] != 0))
            {
                appendMove(list, createMentry(currentlocation, newlocation));
                break;
            }
            else if( (board->boardarray[rookx][rooky+i]/10 == type) && (board->boardarray[rookx][rooky+i] != 0))
            {
                break;
            }
            else
            {
                appendMove(list, createMentry(currentlocation, newlocation));
            }
        }
    }

    //go down
    if(rookx != 7)
    {
        for(i=1; i<=7-(rookx); ++i)
        {
            newlocation = 8*(rookx+i)+(rooky);
            if( (board->boardarray[rookx+i][rooky]/10 != type) && (board->boardarray[rookx+i][rooky] != 0))
            {
                appendMove(list, createMentry(currentlocation, newlocation));
                break;
            }
            else if( (board->boardarray[rookx+i][rooky]/10 == type) && (board->boardarray[rookx+i][rooky] != 0))
            {
                break;
            }
            else
            {
                appendMove(list, createMentry(currentlocation, newlocation));
            }
        }
    }

    //go up
    if(rookx != 0)
    {
        for(i=1; i<=(rookx); ++i)
        {
            newlocation = 8*(rookx-i)+(rooky);
            if( (board->boardarray[rookx-i][rooky]/10 != type) && (board->boardarray[rookx-i][rooky] != 0))
            {
                appendMove(list, createMentry(currentlocation, newlocation));
                break;
            }
            else if( (board->boardarray[rookx-i][rooky]/10 == type) && (board->boardarray[rookx-i][rooky] != 0))
            {
                break;
            }
            else
            {
                appendMove(list, createMentry(currentlocation, newlocation));
            }
        }
    }
}

void bishopmove(MLIST *list, BSTATE *board, int currentlocation)
{
    assert(board);
    assert(list);

    int bishopx = currentlocation/8;
    int bishopy = currentlocation%8;
    int type = board->boardarray[bishopx][bishopy]/10;
    int newlocation;
    int i,j;

    // got up to the left
    if(1)
    {
        if(bishopx < bishopy)
        {
            j = bishopx;
        }
        else
        {
            j = bishopy;
        }

        for(i=1; i<=j; ++i)
        {
            newlocation = 8*(bishopx-i)+(bishopy-i);
            if( (board->boardarray[bishopx-i][bishopy-i]/10 != type) && (board->boardarray[bishopx-i][bishopy-i] != 0))
            {
                appendMove(list, createMentry(currentlocation, newlocation));
                break;
            }
            else if( (board->boardarray[bishopx-i][bishopy-i]/10 == type) && (board->boardarray[bishopx-i][bishopy-i] != 0))
            {
                break;
            }
            else
            {
                appendMove(list, createMentry(currentlocation, newlocation));
            }
        }
     }

    // got up to the right
    if(1)
    {
        if(bishopx < 7 - bishopy)
        {
            j = bishopx;
        }
        else
        {
            j = 7 - bishopy;
        }

        for(i=1; i<=j; ++i)
        {
            newlocation = 8*(bishopx-i)+(bishopy+i);
            if( (board->boardarray[bishopx-i][bishopy+i]/10 != type) && (board->boardarray[bishopx-i][bishopy+i] != 0))
            {
                appendMove(list, createMentry(currentlocation, newlocation));
                break;
            }
            else if( (board->boardarray[bishopx-i][bishopy+i]/10 == type) && (board->boardarray[bishopx-i][bishopy+i] != 0))
            {
                break;
            }
            else
            {
                appendMove(list, createMentry(currentlocation, newlocation));
            }
        }
     }

    // go down to the right
    if(1)
    {
        if(7-bishopx < 7 - bishopy)
        {
            j = 7 - bishopx;
        }
        else
        {
            j = 7 - bishopy;
        }

        for(i=1; i<=j; ++i)
        {
            newlocation = 8*(bishopx+i)+(bishopy+i);
            if( (board->boardarray[bishopx+i][bishopy+i]/10 != type) && (board->boardarray[bishopx+i][bishopy+i] != 0))
            {
                appendMove(list, createMentry(currentlocation, newlocation));
                break;
            }
            else if( (board->boardarray[bishopx+i][bishopy+i]/10 == type) && (board->boardarray[bishopx+i][bishopy+i] != 0))
            {
                break;
            }
            else
            {
                appendMove(list, createMentry(currentlocation, newlocation));
            }
        }
     }

    // go down to the left
    if(1)
    {
        if(7-bishopx <  bishopy)
        {
            j = 7- bishopx;
        }
        else
        {
            j = bishopy;
        }

        for(i=1; i<=j; ++i)
        {
            newlocation = 8*(bishopx+i)+(bishopy-i);
            if( (board->boardarray[bishopx+i][bishopy-i]/10 != type) && (board->boardarray[bishopx+i][bishopy-i] != 0))
            {
                appendMove(list, createMentry(currentlocation, newlocation));
                break;
            }
            else if( (board->boardarray[bishopx+i][bishopy-i]/10 == type) && (board->boardarray[bishopx+i][bishopy-i] != 0))
            {
                break;
            }
            else
            {
                appendMove(list, createMentry(currentlocation, newlocation));
            }
        }
     }
}



