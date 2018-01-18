#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "boardstate.h"
void fenToBoardState(char * fen, BSTATE *b)
{
	/* cheess board pieces based off the positions 1-8, where black king is at board1 */
	char* board[13];
	int n = 0;
	char* buf = malloc(sizeof(char*) * 30);
	int count = 0;
	/*tolkenizing the input */ 
	while (count < 13)
	{
		if(count >= 7) {sscanf(fen, "%s%n", buf, &n);}
		else{sscanf(fen, "%[^/]%n", buf, &n);}

		board[count] = malloc(sizeof(char*) * n);
		memcpy(board[count], buf, n);

		fen += n;
		if (*fen == '\0')
			break;
		count++;
		fen++;
	}
	/* now lets add the states to the array */
	int blankFlag = 0; /*adds blankspacess when flag is 1 */
	int numRepeat = 0; /*number of times to add blank space */
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			if((board[i][j] > '1' && board[i][j] <= '8') && blankFlag == 0)
			{
				blankFlag = 1;
				numRepeat = board[i][j] - '0';
			}
			if(blankFlag == 1 && numRepeat > 0)
			{
				b->boardarray[i][j] = '0'; 
				numRepeat--;
				if(numRepeat == 0) {printf("reset ! \n"); blankFlag = 0;}
				continue;
			}
			else
			{
				b->boardarray[i][j] = board[i][j];
			}
			/*chess peices between 3-6 done! */
		}
	}
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			printf("%c ", b->boardarray[i][j]);
		}
		printf("\n");
	}
}

int main()
{
	BSTATE test;
	fenToBoardState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", &test);
	return 0;
}
