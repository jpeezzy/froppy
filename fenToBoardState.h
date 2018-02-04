
#ifndef FENTOBOARDSTATE_H
#define FENTOBOARDSTATE_H
#include "boardstate.h"

/*Converts a fen string to the board Structure*/
void fenToBoardState(char *fen, BSTATE *b);

/*Converts board structure to fen*/
void boardToFen(char *fen, BSTATE *b);

//appends the game to a file (will have 5 save game states)
void saveGame(char *fen);
#endif 