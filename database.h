#ifndef _DATA_BASE_H
#define _DATA_BASE_H
#include "dataEntry.h"

// struct representing a database of information extracted from fen file
typedef struct
{
  int        totalData;
  struct en* firstMove;
  struct en* lastMove;
} DATABASE;

#endif