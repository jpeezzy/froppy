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

//move a piece no checks for error
void mov(int barr[8][8], int ploc, int pmov);

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

//This will add all the possible pawn moves at that location
void knightmove(MLIST *list, BSTATE *board, int currentlocation);

//This will add all the possible rook moves at that location
void rookmove(MLIST *list, BSTATE *board, int currentlocation);

//This will add all the possible bishop moves at that location
void bishopmove(MLIST *list, BSTATE *board, int currentlocation);

//This will add all the possible bishop moves at that location
void queenmove(MLIST *list, BSTATE *board, int currentlocation);

//This will add all teh possible king moves at that location
void kingmove(MLIST *list, BSTATE *board, int currentlocation);

//Checks for checks on the king
int checkchecker(BSTATE *board);

//return location of black or white king based off flag
int serachforking(BSTATE *board, int flag); 

//adds all legal moves to the list
void allLegal(MLIST *list, BSTATE *board);

#endif
