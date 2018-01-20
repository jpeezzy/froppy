#ifndef MOVELIST_H
#define MOVELIST_H
#include "boardstate.h"
typedef struct MOVELIST MLIST;
typedef struct MOVEENTRY MENTRY;  

//The struct is for the list of move
struct MOVELIST
{
    int movenum;
    MENTRY *First;
    MENTRY *Last;
} ;

//The struct that contains each move 
struct MOVEENTRY
{
    MLIST *List;
    //Current location in the array
    int CLOC;
    //New Location in the array
    int NLOC;
    MENTRY *Prev;
    MENTRY *Next;
};

//Create the move List
MLIST *createMovelist();

//Delete MoveList and all move
void deleteMovelist(MLIST *list);

//This will create the struct for the new Entry
MENTRY *createMentry(int currentlocation, int newlocation); 

//This will add new moves to the movelist      
void appendMove(MLIST *Movelist, MENTRY *Move);

//This will add the the possible pawn moves at that location
void pawnmove(MLIST *list, BSTATE *board, int pawnlocation);


#endif
