#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "movelist.h"
#include "boardstate.h"
 
//Move a piece doesnt check anything
void mov(int barr[8][8], int ploc, int pmov)
{
    if(ploc == 0 && pmov == 0)
    {
        barr[7][6] = 6;
        barr[7][5] = 4;
        barr[7][4] = 0;
        barr[7][7] = 0;
    }
    else if(ploc == 1 && pmov == 1)
    {
        barr[7][2] = 6;
        barr[7][3] = 4;
        barr[7][4] = 0;
        barr[7][0] = 0;
    }
    else if(ploc == 2 && pmov == 2)
    {
        barr[0][2] = 16;
        barr[0][3] = 14;
        barr[0][0] = 0;
        barr[0][4] = 0;
    }
    else if(ploc == 3 && pmov == 3)
    {
        barr[0][6] = 16;
        barr[0][5] = 14;
        barr[0][7] = 0;
        barr[0][4] = 0;
    }
    else
    {
        int temp = barr[ploc/8][ploc%8];
        barr[ploc/8][ploc%8] = 0;
        barr[pmov/8][pmov%8] = temp;
    }
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
                if(checkmove(board, pawnlocation, newlocation))
                {
                    appendMove(list, createMentry(pawnlocation,newlocation));
                }
            }                   
        }

        //single step 
        if(pawnx != 0)
        {
            newlocation = 8*(pawnx-1)+pawny;
            if(board->boardarray[pawnx-1][pawny] == 0)
            {
                if(checkmove(board, pawnlocation, newlocation))
                {
                    appendMove(list, createMentry(pawnlocation,newlocation));
                }
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
                    if(checkmove(board, pawnlocation, newlocation))
                    {
                        appendMove(list, createMentry(pawnlocation,newlocation));
                    }
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
                    if(checkmove(board, pawnlocation, newlocation))
                    {
                        appendMove(list, createMentry(pawnlocation,newlocation));
                    }
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
                if(checkmove(board, pawnlocation, newlocation))
                {
                  appendMove(list, createMentry(pawnlocation,newlocation));
                }
            }                   
        }

        //single step
        if(pawnx != 7)
        {
            newlocation = 8*(pawnx+1)+pawny;
            if(board->boardarray[pawnx+1][pawny] == 0)
            {
                if(checkmove(board, pawnlocation, newlocation))
                {
                  appendMove(list, createMentry(pawnlocation,newlocation));
                }
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
                    if(checkmove(board, pawnlocation, newlocation))
                    {
                        appendMove(list, createMentry(pawnlocation,newlocation));
                    }
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
                    if(checkmove(board, pawnlocation, newlocation))
                    {
                      appendMove(list, createMentry(pawnlocation,newlocation));
                    }
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

    int knightx = currentlocation/8;
    int knighty = currentlocation%8;
    int newlocation;
    int type = (board->boardarray[knightx][knighty])/10;

    if(knightx>2 && knighty != 0 && ((board->boardarray[knightx-2][knighty-1])/10 != type || board->boardarray[knightx-2][knighty-1] == 0))
    {
        newlocation = 8*(knightx-2)+(knighty-1);
        if(checkmove(board, currentlocation, newlocation))
        {
            appendMove(list, createMentry(currentlocation, newlocation));
        }
    }
    if(knightx<6 && knighty != 0 && ((board->boardarray[knightx+2][knighty-1])/10 != type || board->boardarray[knightx+2][knighty-1] == 0))
    {
        newlocation = 8*(knightx+2)+(knighty-1);
        if(checkmove(board, currentlocation, newlocation))
        {
            appendMove(list, createMentry(currentlocation, newlocation));
        }
    }
    if(knightx<6 && knighty != 7 && ((board->boardarray[knightx+2][knighty+1])/10 != type || board->boardarray[knightx+2][knighty+1] == 0))
    {
        newlocation = 8*(knightx+2)+(knighty+1);
        if(checkmove(board, currentlocation, newlocation))
        {
            appendMove(list, createMentry(currentlocation, newlocation));
        }
    }
    if(knightx>2 && knighty != 7 && ((board->boardarray[knightx-2][knighty+1])/10 != type || board->boardarray[knightx-2][knighty+1] == 0))
    {
        newlocation = 8*(knightx-2)+(knighty+1);
        if(checkmove(board, currentlocation, newlocation))
        {
            appendMove(list, createMentry(currentlocation, newlocation));
        }
    }

    if(knightx != 0 && knighty > 2 && ((board->boardarray[knightx-1][knighty-2])/10 != type || board->boardarray[knightx-1][knighty-2] == 0))
    {
        newlocation = 8*(knightx-1)+(knighty-2);
        if(checkmove(board, currentlocation, newlocation))
        {
            appendMove(list, createMentry(currentlocation, newlocation));
        }
    }
    if(knightx != 0 && knighty < 6 && ((board->boardarray[knightx+1][knighty-2])/10 != type || board->boardarray[knightx+1][knighty-2] == 0))
    {
        newlocation = 8*(knightx+1)+(knighty-2);
        if(checkmove(board, currentlocation, newlocation))
        {
            appendMove(list, createMentry(currentlocation, newlocation));
        }
    }
    if(knightx != 7 && knighty < 6 && ((board->boardarray[knightx+1][knighty+2])/10 != type || board->boardarray[knightx+1][knighty+2] == 0))
    {
        newlocation = 8*(knightx+1)+(knighty+2);
        if(checkmove(board, currentlocation, newlocation))
        {
            appendMove(list, createMentry(currentlocation, newlocation));
        }
    }
    if(knightx != 7 && knighty > 2 && ((board->boardarray[knightx-1][knighty+2])/10 != type || board->boardarray[knightx-1][knighty+2] == 0))
    {
        newlocation = 8*(knightx-1)+(knighty+2);
        if(checkmove(board, currentlocation, newlocation))
        {
            appendMove(list, createMentry(currentlocation, newlocation));
        }
    }

}


void rookmove(MLIST *list, BSTATE *board, int currentlocation)
{

    assert(list);
    assert(board);

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
                if(checkmove(board, currentlocation, newlocation))
                {
                    appendMove(list, createMentry(currentlocation, newlocation));
                }
                break;
            }
            else if( (board->boardarray[rookx][rooky-i]/10 == type) && (board->boardarray[rookx][rooky-i] != 0))
            {
                break;
            }
            else
            {
                if(checkmove(board, currentlocation, newlocation))
                {
                    appendMove(list, createMentry(currentlocation, newlocation));
                }
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
                if(checkmove(board, currentlocation, newlocation))
                {
                    appendMove(list, createMentry(currentlocation, newlocation));
                }
                break;
            }
            else if( (board->boardarray[rookx][rooky+i]/10 == type) && (board->boardarray[rookx][rooky+i] != 0))
            {
                break;
            }
            else
            {
                if(checkmove(board, currentlocation, newlocation))
                {
                    appendMove(list, createMentry(currentlocation, newlocation));
                }
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
                if(checkmove(board, currentlocation, newlocation))
                {
                    appendMove(list, createMentry(currentlocation, newlocation));
                }
                break;
            }
            else if( (board->boardarray[rookx+i][rooky]/10 == type) && (board->boardarray[rookx+i][rooky] != 0))
            {
                break;
            }
            else
            {
                if(checkmove(board, currentlocation, newlocation))
                {
                    appendMove(list, createMentry(currentlocation, newlocation));
                }
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
                if(checkmove(board, currentlocation, newlocation))
                {
                    appendMove(list, createMentry(currentlocation, newlocation));
                }
                break;
            }
            else if( (board->boardarray[rookx-i][rooky]/10 == type) && (board->boardarray[rookx-i][rooky] != 0))
            {
                break;
            }
            else
            {
                if(checkmove(board, currentlocation, newlocation))
                {
                    appendMove(list, createMentry(currentlocation, newlocation));
                }
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
                if(checkmove(board, currentlocation, newlocation))
                {
                    appendMove(list, createMentry(currentlocation, newlocation));
                }
                break;
            }
            else if( (board->boardarray[bishopx-i][bishopy-i]/10 == type) && (board->boardarray[bishopx-i][bishopy-i] != 0))
            {
                break;
            }
            else
            {
                if(checkmove(board, currentlocation, newlocation))
                {
                    appendMove(list, createMentry(currentlocation, newlocation));
                }
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
                if(checkmove(board, currentlocation, newlocation))
                {
                    appendMove(list, createMentry(currentlocation, newlocation));
                }
                break;
            }
            else if( (board->boardarray[bishopx-i][bishopy+i]/10 == type) && (board->boardarray[bishopx-i][bishopy+i] != 0))
            {
                break;
            }
            else
            {
                if(checkmove(board, currentlocation, newlocation))
                {
                    appendMove(list, createMentry(currentlocation, newlocation));
                }
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
                if(checkmove(board, currentlocation, newlocation))
                {
                    appendMove(list, createMentry(currentlocation, newlocation));
                }
                break;
            }
            else if( (board->boardarray[bishopx+i][bishopy+i]/10 == type) && (board->boardarray[bishopx+i][bishopy+i] != 0))
            {
                break;
            }
            else
            {
                if(checkmove(board, currentlocation, newlocation))
                {
                    appendMove(list, createMentry(currentlocation, newlocation));
                }
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
                if(checkmove(board, currentlocation, newlocation))
                {
                    appendMove(list, createMentry(currentlocation, newlocation));
                }
                break;
            }
            else if( (board->boardarray[bishopx+i][bishopy-i]/10 == type) && (board->boardarray[bishopx+i][bishopy-i] != 0))
            {
                break;
            }
            else
            {
                if(checkmove(board, currentlocation, newlocation))
                {
                    appendMove(list, createMentry(currentlocation, newlocation));
                }
            }
        }
     }
}

void queenmove(MLIST *list, BSTATE *board, int currentlocation)
{
    assert(list);
    assert(board);

    int queenx = currentlocation/8;
    int queeny = currentlocation%8;
    int newlocation,i,j;
    int type = board->boardarray[queenx][queeny]/10;

    // got up to the left
    if(1)
    {
        if(queenx < queeny)
        {
            j = queenx;
        }
        else
        {
            j = queeny;
        }

        for(i=1; i<=j; ++i)
        {
            newlocation = 8*(queenx-i)+(queeny-i);
            if( (board->boardarray[queenx-i][queeny-i]/10 != type) && (board->boardarray[queenx-i][queeny-i] != 0))
            {
                if(checkmove(board, currentlocation, newlocation))
                {
                    appendMove(list, createMentry(currentlocation, newlocation));
                }
                break;
            }
            else if( (board->boardarray[queenx-i][queeny-i]/10 == type) && (board->boardarray[queenx-i][queeny-i] != 0))
            {
                break;
            }
            else
            {
                if(checkmove(board, currentlocation, newlocation))
                {
                    appendMove(list, createMentry(currentlocation, newlocation));
                }
            }
        }
     }

    // got up to the right
    if(1)
    {
        if(queenx < 7 - queeny)
        {
            j = queenx;
        }
        else
        {
            j = 7 - queeny;
        }

        for(i=1; i<=j; ++i)
        {
            newlocation = 8*(queenx-i)+(queeny+i);
            if( (board->boardarray[queenx-i][queeny+i]/10 != type) && (board->boardarray[queenx-i][queeny+i] != 0))
            {
                if(checkmove(board, currentlocation, newlocation))
                {
                    appendMove(list, createMentry(currentlocation, newlocation));
                }
                break;
            }
            else if( (board->boardarray[queenx-i][queeny+i]/10 == type) && (board->boardarray[queenx-i][queeny+i] != 0))
            {
                break;
            }
            else
            {
                if(checkmove(board, currentlocation, newlocation))
                {
                    appendMove(list, createMentry(currentlocation, newlocation));
                }
            }
        }
     }

    // go down to the right
    if(1)
    {
        if(7-queenx < 7 - queeny)
        {
            j = 7 - queenx;
        }
        else
        {
            j = 7 - queeny;
        }

        for(i=1; i<=j; ++i)
        {
            newlocation = 8*(queenx+i)+(queeny+i);
            if( (board->boardarray[queenx+i][queeny+i]/10 != type) && (board->boardarray[queenx+i][queeny+i] != 0))
            {
                if(checkmove(board, currentlocation, newlocation))
                {
                    appendMove(list, createMentry(currentlocation, newlocation));
                }
                break;
            }
            else if( (board->boardarray[queenx+i][queeny+i]/10 == type) && (board->boardarray[queenx+i][queeny+i] != 0))
            {
                break;
            }
            else
            {
                if(checkmove(board, currentlocation, newlocation))
                {
                    appendMove(list, createMentry(currentlocation, newlocation));
                }
            }
        }
     }

    // go down to the left
    if(1)
    {
        if(7-queenx <  queeny)
        {
            j = 7- queenx;
        }
        else
        {
            j = queeny;
        }

        for(i=1; i<=j; ++i)
        {
            newlocation = 8*(queenx+i)+(queeny-i);
            if( (board->boardarray[queenx+i][queeny-i]/10 != type) && (board->boardarray[queenx+i][queeny-i] != 0))
            {
                if(checkmove(board, currentlocation, newlocation))
                {
                    appendMove(list, createMentry(currentlocation, newlocation));
                }
                break;
            }
            else if( (board->boardarray[queenx+i][queeny-i]/10 == type) && (board->boardarray[queenx+i][queeny-i] != 0))
            {
                break;
            }
            else
            {
                if(checkmove(board, currentlocation, newlocation))
                {
                    appendMove(list, createMentry(currentlocation, newlocation));
                }
            }
        }
     }

    //go left
    if(queeny != 0)
    {
        for(i=1; i<=(queeny); ++i)
        {
            newlocation = 8*(queenx)+(queeny-i);
            if( (board->boardarray[queenx][queeny-i]/10 != type) && (board->boardarray[queenx][queeny-i] != 0))
            {
                if(checkmove(board, currentlocation, newlocation))
                {
                    appendMove(list, createMentry(currentlocation, newlocation));
                }
                break;
            }
            else if( (board->boardarray[queenx][queeny-i]/10 == type) && (board->boardarray[queenx][queeny-i] != 0))
            {
                break;
            }
            else
            {
                if(checkmove(board, currentlocation, newlocation))
                {
                    appendMove(list, createMentry(currentlocation, newlocation));
                }
            }
        }
     }

    //go right
    if(queeny != 7)
    {
        for(i=1; i<=7-(queeny); ++i)
        {
            newlocation = 8*(queenx)+(queeny+i);
            if( (board->boardarray[queenx][queeny+i]/10 != type) && (board->boardarray[queenx][queeny+i] != 0))
            {
                if(checkmove(board, currentlocation, newlocation))
                {
                    appendMove(list, createMentry(currentlocation, newlocation));
                }
                break;
            }
            else if( (board->boardarray[queenx][queeny+i]/10 == type) && (board->boardarray[queenx][queeny+i] != 0))
            {
                break;
            }
            else
            {
                if(checkmove(board, currentlocation, newlocation))
                {
                    appendMove(list, createMentry(currentlocation, newlocation));
                }
            }
        }
    }

    //go down
    if(queenx != 7)
    {
        for(i=1; i<=7-(queenx); ++i)
        {
            newlocation = 8*(queenx+i)+(queeny);
            if( (board->boardarray[queenx+i][queeny]/10 != type) && (board->boardarray[queenx+i][queeny] != 0))
            {
                if(checkmove(board, currentlocation, newlocation))
                {
                    appendMove(list, createMentry(currentlocation, newlocation));
                }
                break;
            }
            else if( (board->boardarray[queenx+i][queeny]/10 == type) && (board->boardarray[queenx+i][queeny] != 0))
            {
                break;
            }
            else
            {
                if(checkmove(board, currentlocation, newlocation))
                {
                    appendMove(list, createMentry(currentlocation, newlocation));
                }
            }
        }
    }

    //go up
    if(queenx != 0)
    {
        for(i=1; i<=(queenx); ++i)
        {
            newlocation = 8*(queenx-i)+(queeny);
            if( (board->boardarray[queenx-i][queeny]/10 != type) && (board->boardarray[queenx-i][queeny] != 0))
            {
                if(checkmove(board, currentlocation, newlocation))
                {
                    appendMove(list, createMentry(currentlocation, newlocation));
                }
                break;
            }
            else if( (board->boardarray[queenx-i][queeny]/10 == type) && (board->boardarray[queenx-i][queeny] != 0))
            {
                break;
            }
            else
            {
                if(checkmove(board, currentlocation, newlocation))
                {
                    appendMove(list, createMentry(currentlocation, newlocation));
                }
            }
        }
    }

}

void kingmove(MLIST *list, BSTATE *board, int currentlocation)
{
    assert(list);
    assert(board);

    int kingx = currentlocation/8;
    int kingy = currentlocation%8;
    int type = board->boardarray[kingx][kingy]/10;
    int newlocation, i, j;

    for(i=-1; i<=1; ++i)
    {
        for(j=-1; j<=1; ++j)
        {
            newlocation = 8*(kingx+i)+(kingy+j);
            
            if((kingx+i >= 0) && (kingx+i <= 7) && (kingy+j >= 0) && (kingy+j<=7))
            {
                if( ((board->boardarray[kingx+i][kingy+j]/10  != type) && (board->boardarray[kingx+i][kingy+j] != 0)) || (board->boardarray[kingx+i][kingy+j] == 0))
                {
                    if(checkmove(board, currentlocation, newlocation))
                    {
                        appendMove(list, createMentry(currentlocation, newlocation));
                    }
                }
            }
        }
    }
    
    //castling
    if(board->WKCFlag == 1)
    {

    }
}

int searchforking(BSTATE *board, int flag)
{
    //search for king 
    int i,j;

    for(i=0; i<8; ++i)
    {
        for(j=0; j<8; ++j)
        {
            if( board->boardarray[i][j]%10 == 6)    
            {
                if(flag == 0 && (board->boardarray[i][j]/10 == 0 && board->boardarray[i][j] != 0))
                {
                    return (8*j+i);
                }
                if(flag == 1 && (board->boardarray[i][j]/10 == 1 && board->boardarray[i][j] != 0))
                {
                    return (8*j+i);
                }
            }
        }
    }
    return 0;
}


int checkmove(BSTATE *board, int currentlocation, int newlocation)
{
    assert(board);
    
    BSTATE *temp;
    temp = createBstate();
    copyBstate(board, temp);
    mov(temp->boardarray, currentlocation, newlocation);
    int flag = board->sidetomove;
    int result = checkchecker(temp, flag);
    deleteBstate(temp);
    if( result == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }

}
 
//return 0 if not in check
int checkchecker(BSTATE *board, int flag)
{
    int s = searchforking(board, flag);
    int kingx = s/8;
    int kingy = s%8;
    int i,j;

    
        //Check along the horizontal lines

        //UP
        if(kingx != 0)
        {
            for(i=1; i<(7-kingx); ++i)
            {
                if(board->boardarray[kingx-i][kingy]/10 == flag && board->boardarray[kingx-i][kingy] != 0)
                {
                    break;
                }
                else if(board->boardarray[kingx-i][kingy]/10 != flag)
                {
                    if(board->boardarray[kingx-i][kingy]%10 == 5 || board->boardarray[kingx-i][kingy]%10 == 4)
                    {
                        return 1;
                    }
                }
            }
        }

        //DOWN
        if(kingx != 7)
        {
            for(i=1; i<(kingx); ++i)
            {
                if(board->boardarray[kingx+i][kingy] == flag && board->boardarray[kingx+i][kingy] != 0)
                {
                    break;
                }
                else if(board->boardarray[kingx+i][kingy]/10 != flag)
                {
                    if(board->boardarray[kingx+i][kingy]%10 == 5 || board->boardarray[kingx+i][kingy]%10 == 4)
                    {
                        return 1;
                    }
                }
            }
        }

        //Left
        if(kingy != 0)
        {
            for(i=1; i<(kingy); ++i)
            {
                if(board->boardarray[kingx][kingy-i]/10 == flag && board->boardarray[kingx][kingy-i] != 0)
                {
                    break;
                }
                else if(board->boardarray[kingx][kingy-i]/10 != flag)
                {
                    if(board->boardarray[kingx][kingy-i]%10 == 5 || board->boardarray[kingx][kingy-i]%10 == 4)
                    {
                        return 1;
                    }
                }
            }
        }

        //RIGHT
        if(kingy != 7)
        {
            for(i=1; i<(7-kingy); ++i)
            {
                if(board->boardarray[kingx][kingy+i]/10 == flag && board->boardarray[kingx][kingy+i] != 0)
                {
                    break;
                }
                else if(board->boardarray[kingx][kingy+i]/10 != flag)
                {
                    if(board->boardarray[kingx][kingy+i]%10 == 5 || board->boardarray[kingx][kingy+i]%10 == 4)
                    {
                        return 1;
                    }
                }
            }
        }
     
    //horse moves 
    if(kingx>2 && kingy != 0 && ((board->boardarray[kingx-2][kingy-1])/10 != flag ) && board->boardarray[kingx-2][kingy-1] %10 == 2 )
    {
        return 1;
    }
    if(kingx<6 && kingy != 0 && ((board->boardarray[kingx+2][kingy-1])/10 != flag ) && board->boardarray[kingx+2][kingy-1] %10 == 2)
    {
        return 1;
    }
    if(kingx<6 && kingy != 7 && ((board->boardarray[kingx+2][kingy+1])/10 != flag ) && board->boardarray[kingx+2][kingy+1] %10 == 2)
    {
        return 1;
    }
    if(kingx>2 && kingy != 7 && ((board->boardarray[kingx-2][kingy+1])/10 != flag ) && board->boardarray[kingx-2][kingy+1] %10 == 2)
    {
        return 1;
    }

    if(kingx != 0 && kingy > 2 && ((board->boardarray[kingx-1][kingy-2])/10 != flag ) && board->boardarray[kingx-1][kingy-2] %10 == 2)
    {
        return 1;
    }
    if(kingx != 0 && kingy < 6 && ((board->boardarray[kingx+1][kingy-2])/10 != flag ) && board->boardarray[kingx+1][kingy-2] %10 == 2)
    {
        return 1;
    }
    if(kingx != 7 && kingy < 6 && ((board->boardarray[kingx+1][kingy+2])/10 != flag ) && board->boardarray[kingx+1][kingy+2] %10 == 2)
    {
        return 1;
    }
    if(kingx != 7 && kingy > 2 && ((board->boardarray[kingx-1][kingy+2])/10 != flag ) && board->boardarray[kingx-1][kingy+2] %10 == 2)
    {
        return 1;
    }
        
/*
    // diagonal
    
    // got up to the left
    
        if(kingx < kingy)
        {
            j = kingx;
        }
        else
        {
            j = kingy;
        }

        for(i=1; i<=j; ++i)
        {
            if( (board->boardarray[kingx-i][kingy-i]/10 == flag) && (board->boardarray[kingx-i][kingy-i] != 0))
            {
                return 1;
            }
            else if( (board->boardarray[kingx-i][kingy-i]/10 != flag) && (board->boardarray[kingx-i][kingy-i] != 0))
            {
                break;
            }
            
        }
     

    // got up to the right
    
        if(kingx < 7 - kingy)
        {
            j = kingx;
        }
        else
        {
            j = 7 - kingy;
        }

        for(i=1; i<=j; ++i)
        {
            if( (board->boardarray[kingx-i][kingy+i]/10 == flag) && (board->boardarray[kingx-i][kingy+i] != 0))
            {
                return 1;
            }
            else if( (board->boardarray[kingx-i][kingy+i]/10 == flag) && (board->boardarray[kingx-i][kingy+i] != 0))
            {
                break;
            }
        }
     

    // go down to the right
    
        if(7-kingx < 7 - kingy)
        {
            j = 7 - kingx;
        }
        else
        {
            j = 7 - kingy;
        }

        for(i=1; i<=j; ++i)
        {
            if( (board->boardarray[kingx+i][kingy+i]/10 == flag) && (board->boardarray[kingx+i][kingy+i] != 0))
            {
                return 1;
                
            }
            else if( (board->boardarray[kingx+i][kingy+i]/10 == flag) && (board->boardarray[kingx+i][kingy+i] != 0))
            {
                break;
            }
        }
     

    // go down to the left
    
        if(7-kingx <  kingy)
        {
            j = 7- kingx;
        }
        else
        {
            j = kingy;
        }

        for(i=1; i<=j; ++i)
        {
            if( (board->boardarray[kingx+i][kingy-i]/10 == flag) && (board->boardarray[kingx+i][kingy-i] != 0))
            {
                return 1;
            }
            else if( (board->boardarray[kingx+i][kingy-i]/10 == flag) && (board->boardarray[kingx+i][kingy-i] != 0))
            {
                break;
            }
        }
  
*/
  
        //pawn check
        
        //for white king
        if(flag == 0)
        {
            if(kingx != 0)
            {
                //left
                if(kingy > 0)
                {
                    if(board->boardarray[kingx-1][kingy-1]/10 != flag && board->boardarray[kingx-1][kingy-1] != 0)
                    {
                        if(board->boardarray[kingx-1][kingy-1]%10 == 1)
                        {
                            return 1;
                        }
                    }
                }
                //right
                if(kingy < 7)
                {
                    if(board->boardarray[kingx-1][kingy+1]/10 != flag && board->boardarray[kingx-1][kingy+1] != 0)
                    {
                        if(board->boardarray[kingx-1][kingy+1]%10 == 1)
                        {
                            return 1;
                        }
                    }
                }

            }
        }
        
        //for black king
        if(flag == 1)
        {
            if(kingx != 0)
            {
                //left
                if(kingy > 0)
                {
                    if(board->boardarray[kingx+1][kingy-1]/10 != flag && board->boardarray[kingx+1][kingy-1] != 0)
                    {
                        if(board->boardarray[kingx+1][kingy-1]%10 == 1)
                        {
                            return 1;
                        }
                    }
                }
                //right
                if(kingy < 7)
                {
                    if(board->boardarray[kingx+1][kingy+1]/10 != flag && board->boardarray[kingx+1][kingy+1] != 0)
                    {
                        if(board->boardarray[kingx+1][kingy+1]%10 == 1)
                        {
                            return 1;
                        }
                    }
                }

            }
        }


    return 0;
}

void allLegal(MLIST *list, BSTATE *board)
{
    assert(list);
    assert(board);

    int stm = board->sidetomove;
    int i,j,type;
    int cloc;

    for(i = 0; i<8; ++i)
    {
        for(j = 0; j<8; ++j)
        {
            if( board->boardarray[i][j]/10 == stm && board->boardarray != 0)
            {
                type = board->boardarray[i][j]%10;
                cloc = 8*i+j;;
                if(type == 1)
                {
                    pawnmove(list,board, cloc);
                }
                else if(type == 2)
                {
                    knightmove(list,board, cloc);
                }
                else if(type == 3)
                {
                    bishopmove(list,board,cloc);
                }
                else if(type == 4)
                {
                    rookmove(list,board,cloc);
                }
                else if(type == 5)
                {
                    queenmove(list, board, cloc);
                }
                else if(type == 6)
                {
                    kingmove(list, board ,cloc);
                }
            }

        }

    }

}

