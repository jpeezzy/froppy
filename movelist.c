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


