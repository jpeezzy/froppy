#ifndef _BOARD_PREP_H
#define _BOARD_PREP_H

#include "boardstate.h"

void boardSave(BSTATE* board,int saveNum);

void boardLoad(BSTATE* board,int saveNum);

#endif