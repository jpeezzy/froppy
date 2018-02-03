#ifndef BOARD_STATE_H
#define BOARD_STATE_H

// This is the struct that hold all the board information
typedef struct
{
    // we still have the right to castle, but may not be able to castle right
    // now
    // side to move is 0 for white
    int boardarray[8][8];
    int WKCFlag;
    int WQCFlag;
    int BQCFlag;
    int BKCFlag;
    int sidetomove;

} BSTATE;

// This will allocate the memory for the board
BSTATE *createBstate(void);

// This will delete all the pointers for the board
void deleteBstate(BSTATE *board);

// This will load the start of the game to the inputted board
void loadStart(BSTATE *currentboard);

// This will copy the board to another board
void copyBstate(BSTATE *old, BSTATE *new);

#endif
