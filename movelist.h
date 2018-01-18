#ifndef MOVELIST_H
#define MOVELIST_H

typedef struct MOVELIST MLIST;
typedef struct MOVEENTRY MENTRY;  

//The struct is for the list of move
typedef struct MOVELIST
{
    int movenum;
    MENTRY *First;
    MENTRY *Last;
} ;

//The struct that contains each move 
typedef struct MOVEENTRY
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
int MLIST *createMovelist();

//Delete MoveList and all move
int MLIST * 

//This will add new moves to the movelist      
void appendMove(MLIST *Movelist, MENTRY *Move);



#endif
