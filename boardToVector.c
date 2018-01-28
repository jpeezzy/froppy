/** Function: boardToVector.c
 * Author: Justin Lee
 * Function: allocates a vector
 * */
#include "boardToVector.h"
#include <malloc.h>
#include "boardstate.h"
#include "fenToBoardState.h"
void boardToVector(BSTATE *board, long double *vector)
{
  vector[0] = board->WKCFlag;
  vector[1] = board->WQCFlag;
  vector[2] = board->BQCFlag;
  vector[3] = board->WKCFlag;
  vector[4] = board->sidetomove;
  /*we have flags created to determine if black or white
   */

  /*white is 1, black is 0 */
  int shift = 5; /** shifts array by 5 b/c first 5 inputs
                   are used up */
  int piece = 0;
  for (int color = 0; color < 2; color++)
    {
      for (int rank = 0; rank < 6; rank++)
        {
          for (int i = 0; i < 8; i++)
            {
              for (int j = 0; j < 8; j++)
                {
                  piece = board->boardarray[i][j];
                  if (piece / 10 == (color) && piece % 10 == (rank + 1) &&
                      piece != 0)
                    {
                      //  printf("the piece is %d, position is %d and %d !!!\n
                      //  ",
                      //        piece,
                      //        i,
                      //        j);
                      // printf("Rank is  %d, Color is %d!!!\n ", rank,
                      // color);
                      vector[shift + color * 6 * 8 * 8 + rank * 8 * 8 + i * 8 +
                             j] = 1;
                    }
                  else
                    {
                      vector[shift + color * 6 * 8 * 8 + rank * 8 * 8 + i * 8 +
                             j] = 0;
                    }
                }
            }
        }
    }
}

/*
int main()
{
    BSTATE test;
    fenToBoardState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1",
&test); int *vectorTest = malloc(sizeof(int)*774); boardToVector(&test,
vectorTest);
    //testing vector
    int shift = 5;
    int color = 0;
    int rank = 3;
    printf("\n");
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            printf("%d ", vectorTest[shift+color*6*8*8+rank*8*8+i*8+j]);
        }
        printf("\n");
    }
    printf("\n");
    free(vectorTest);
    return 0;
}
*/
