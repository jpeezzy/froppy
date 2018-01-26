#include "dataEntry.h"
#include <stdio.h>
#include <stdlib.h>
#include "database.h"
#include "fenToBoardState.h"

// read a fen file and returns a database comprising of all the moves
void readFenfile(FILE* fenFilehandle, DATABASE* dataMain)
{
  char curLine[100];
  dataMain->totalData = 0;
  dataMain->firstMove = NULL;
  dataMain->lastMove  = NULL;
  ENTRY* curEntry     = NULL;
  // read a file line by line and records the result into a database
  while (fgets(curLine, 100, fenFilehandle) != NULL)
    {
      if (dataMain->totalData == 0)
        {
          dataMain->firstMove = malloc(sizeof(ENTRY));
          fenToBoardState(curLine, dataMain->firstMove->state);
          curEntry = dataMain->firstMove;
        }
      else
        {
          curEntry->next = malloc(sizeof(ENTRY));
          fenToBoardState(curLine, curEntry->state);
          curEntry = curEntry->next;
        }
      dataMain->totalData += 1;
    }
  dataMain->lastMove = curEntry;
}