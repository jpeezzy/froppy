#ifndef _DATA_ENTRY_H
#define _DATA_ENTRY_H

#include "boardstate.h"
#include "stdio.h"
#include "stdlib.h"
#include "movelist.h"

// structure used for representing an entry of a data base containing the next,
// prev move and the state of the move it represents as well as the pt to
// database
typedef struct en    ENTRY;
typedef struct datab DATABASE;
//data base of move used for machine learning, not neural network

struct en
{
  DATABASE*  base;
  BSTATE*    state;
  struct en* next;
  struct en* prev;
};

struct datab
{
  int    totalData;
  ENTRY* firstMove;
  ENTRY* lastMove;
};





DATABASE* createDataB(void);

ENTRY* createEntry(DATABASE* datab, BSTATE* state);

void   readFenfile(FILE* fenFilehandle, DATABASE* dataMain);
void freeFenfile(DATABASE* dataMain);

//add random states to a database of states
BSTATE pickRandMove(DATABASE* dataMain);

//add random move entry to a move list
void addRandom(MLIST* dataMain, BSTATE* board);
#endif
