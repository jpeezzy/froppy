/** Function: boardToVector.c
 * Author: Justin Lee 
 * Function: allocates a vector 
 * */
#include "boardToVector.h"
#include "boardstate.h"
#include <malloc.h>
void boardToVector(BSTATE *board, int *vector)
{
	vector = malloc(sizeof(int)*774);
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
	for(int color = 0; color < 2; color++)
	{
		for(int rank = 0; rank < 6; rank++)
		{
			for(int i = 0; i < 8; i++)
			{
				for(int j = 0; j < 8; j++)
				{
					int piece = board->boardarray[i][j];
					if(piece% 10 == color)
					{
						if (piece/10 == rank)
						{
							vector[shift+(rank+1)*(color+1)*(i*8 + j)] = piece;
						}
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
	fenToBoardState("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1", &test);
	int *vectorTest;
	boardToVector(&test, vectorTest);
	return 0;	
}
*/
