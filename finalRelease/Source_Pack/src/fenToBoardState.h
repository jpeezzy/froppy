
#ifndef FENTOBOARDSTATE_H
#define FENTOBOARDSTATE_H
#include "boardstate.h"

/*Converts a fen string to the board Structure*/
void fenToBoardState(char *fen, BSTATE *b);

/*Converts board structure to fen*/
void boardToFen(char *fen, BSTATE *b);

//appends the game to a file (will have 5 save game states)
void saveGame(char *fen);

//converts each line to string and sends it to a function that converts it to an array based on fen input
char* textConverterToEight(char* stringT, BSTATE* b, int index);

//converts chess rank to char
char pieceToChar(int input);

//converts the char to chess rank
int charToPiece(char input);

#endif 
