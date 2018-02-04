/**
 * Load: Used for creating a board state from a fen file, used Jusint fenToBoard
 * Save: Used board to fen to save to a fen file
 * 
 */

#ifndef _BOARD_PREP_H
#define _BOARD_PREP_H

#include "boardstate.h"

void boardSave(BSTATE* board,int saveNum);

void boardLoad(BSTATE* board,int saveNum);

#endif