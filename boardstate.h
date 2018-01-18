#ifndef BOARD_CONTROL_H
#define BOARD_CONTROL_H

// This is the struct that hold all the board information
typedef struct
{
  int boardarray[8][8];
  // we still have the right to castle, but may not be able to castle right now
  int WKCFlag;
  int WQCFLag;
  int BQCFlag;
  int BKCFLag;

} BSTATE;

// This will allocate the memory for the board
BSTATE *createBstate(void);

// This will delete all the pointers for the board
void deleteBstate(BSTATE *board);

// This will load the start of the game to the inputted board
void loadStart(BSTATE *currentboard);

#endif
