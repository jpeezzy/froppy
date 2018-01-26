#ifndef _DATA_ENTRY_H
#define _DATA_ENTRY_H

#include "boardstate.h"
#include "database.h"

// structure used for representing an entry of a data base containing the next,
// prev move and the state of the move it represents as well as the pt to
// database
typedef struct en    ENTRY;
typedef struct datab DATABASE;
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

void   readFenfile(FILE** fenFilehandle, DATABASE* dataMain);
BSTATE pickRandMove(DATABASE* dataMain);

#endif