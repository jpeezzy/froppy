#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "boardstate.h"
#include "fenToBoardState.h"
char* textConverterToEight(char* stringT, BSTATE* b, int index);
void  fenToBoardState(char* fen, BSTATE* b)
{
	/* cheess board pieces based off the positions 1-8, where black king is at
	 * board1 */
	assert(b);
	assert(fen);
	char* board[13];
	int   n     = 0;
	char* buf   = malloc(sizeof(char*) * 30);
	if(!buf){
		perror("Out of Memory aborting");
		exit(10);
	}
	int   count = 0;
	/*tolkenizing the input */
	while (count < 13)
	{
		if (count >= 7)
		{
			sscanf(fen, "%s%n", buf, &n);
		}
		else
		{
			sscanf(fen, "%[^/]%n", buf, &n);
		}

		board[count] = malloc(sizeof(char*) * n);
		if(!board[count]){
			perror("Out of Memory aborting");
			exit(10);
		}
		memcpy(board[count], buf, n);

		fen += n;
		if (*fen == '\0')
			break;
		count++;
		fen++;
	}

	if(board[8] == 'w')
	{
		b->sidetomove = 0;
	}
	else if(board[8] == 'b')
	{
		b->sidetomove = 1;
	}
	else
		b->sidetomove = 3;
	//b->sidetomove = atoi(board[8]);
	/*parse for castling flags for
	 * board[9] */
	for (int i = 0; i < sizeof(board[9]) / sizeof(char); i++)
	{
		if (board[9][i] == 'K')
		{
			b->WKCFlag = 1;
		}
		else if (board[9][i] == 'Q')
		{
			b->WQCFlag = 1;
		}
		else if (board[9][i] == 'k')
		{
			b->BKCFlag = 1;
		}
		else if (board[9][i] == 'q')
		{
			b->BQCFlag = 1;
		}
	}
	/* now lets add the states to the array */
	for (int i = 0; i < 8; i++)
	{
#ifdef DEBUG
		// printf("parsing checker %s \n", board[i]);
#endif
		textConverterToEight(board[i], b, i);
	}
}

int charToPiece(char input)
{
	switch (input)
	{
		case 'P':
			return 1;
		case 'N':
			return 2;
		case 'B':
			return 3;
		case 'R':
			return 4;
		case 'Q':
			return 5;
		case 'K':
			return 6;
		case 'p':
			return 11;
		case 'n':
			return 12;
		case 'b':
			return 13;
		case 'r':
			return 14;
		case 'q':
			return 15;
		case 'k':
			return 16;
			// case '1': return 0;
	}
	return 0;
}
//Converts intnumber to char
char pieceToChar(int input)
{
	switch (input)
	{
		case 1:
			return 'P';
		case 2:
			return 'N';
		case 3:
			return 'B';
		case 4:
			return 'R';
		case 5:
			return 'Q';
		case 6:
			return 'K';
		case 11:
			return 'p';
		case 12:
			return 'n';
		case 13:
			return 'b';
		case 14:
			return 'r';
		case 15:
			return 'q';
		case 16:
			return 'k';
			// case '1': return 0;
	}
	return '0';

}

void boardToFen(char *fen, BSTATE *b);

/*
int main()
{
	BSTATE test;
	//fenToBoardState("K2Q4/pppppppp/3b4/3b2r1/1n1P1k2/7N/PPP1PPPP/RNB2B1R wKQkq -", &test);
	//fenToBoardState("K2Q4/pppppppp/3b4/3br3/1n1P1k2/7N/PPP1PPPP/RNB2B1R w KQkq	-", &test); 
	//fenToBoardState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -", &test);
	fenToBoardState("r1bqkbnr/pppp1p1p/8/NP2pn1p/2P5/3B4/P2PPPPP/R1BQK1NR b KQkq -", &test);
	char *fen;
	fen = malloc(sizeof(char)*80);
	boardToFen(fen, &test);
	free(fen);
	return 0;
}*/

char* textConverterToEight(char* stringT, BSTATE* b, int index)
{
	// printf("string is %s\n", stringT);
	int temp[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	// printf("temp is %s\n", temp);
	/*getting size of string */
	int count        = 0;
	int temp2        = 0;
	int positionTemp = 0;
	//	int spaceCounter = textConverterToEight0;
	while (stringT[count] != NULL || stringT[count] != '\0')
	{
		count++;
	}
	if (count == 8)
	{
		for (int i = 0; i < 8; i++)
		{
			temp[i] = charToPiece(stringT[i]);
			if (stringT[i] == '1')
			{
				temp[i] = 0;
			}
		}
	}
	else
	{
		for (int i = 0; i < count; i++)
		{
			if (stringT[i] >= '1' && stringT[i] <= '8')
			{
				temp2 = (int)(stringT[i] - '0');

				if (stringT[i] != '1')
					positionTemp = i;

				for (int j = 0; j < (temp2); j++)
				{
					positionTemp++;
				}
			}
			else
			{
				temp[positionTemp] = charToPiece(stringT[i]);
				positionTemp++;
			}
		}
	}

	for (int i = 0; i < 8; i++)
	{
		b->boardarray[index][i] = temp[i];
	}
	return "";
}

void boardToFen(char *fen, BSTATE *b)
{
	assert(fen);
	assert(b);
	// now parse it to fen from this format
	// making a counter for every time we encounter 0;
	// rowCount is the count for each "row" in fen
	int count  = 0;
	int prevCount = 0;
	for(int i = 0; i < 8; i++)
	{
		for(int j =0; j< 8; j++)
		{
			printf("%d", b->boardarray[i][j]);
		}
		printf("\n");
	}
	for(int i = 0; i < 8; i++)
	{
		count = 0;
		for(int j =0; j < 8; j++)
		{
			if(b->boardarray[i][j] == 0)
			{
				count++;

				if (j == 7 && b->boardarray[i][j] == 0)
				{
					//printf("count is %d \n", count);
					fen[count + prevCount] = count + '0';
					//count = 0;
				}
			}
			else
			{
				if(count > 0)
				{
					if(i == 3)
					{
						printf("count is %c \n", count + '0');
						printf("char is %d \n", b->boardarray[i][j]);
					}
					prevCount +=count;
					fen[prevCount] = count+'0';
					prevCount++;
					fen[prevCount] = pieceToChar(b->boardarray[i][j]);
					count = 0;
				}
				else
				{
					prevCount++;
					fen[prevCount] = pieceToChar(b->boardarray[i][j]);
				}
			}
			//printf("%d",b->boardarray[i][j]);
		}
		fen[count + prevCount + 1] = '/';
		prevCount += count + 1;
	}


	for(int i = 0; i < 80; i++)
	{
		//fen[i*8+j] = (char)b->boardarray[i][j];
		printf("%c", fen[i]);
		//printf("%d ",b->boardarray[i][j]);
	}
	printf("\n");
	return;

}

void saveGame(char *fen)
{
	//append text file based on save state
}
