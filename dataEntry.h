#ifndef _DATA_ENTRY_H
#define _DATA_ENTRY_H

#include "boardstate.h"
#include "database.h"

// structure used for representing an entry of a data base containing the next,
// prev move and the state of the move it represents as well as the pt to
// database
typedef struct
{
  DATABASE* base;
  BSTATE*   state;
  ENTRY*    next;
  ENTRY*    prev;
} ENTRY;

#endif