#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "boardstate.h"

char* textConverterToEight(char* stringT, BSTATE *b, int index);
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

	b->sidetomove = atoi(board[8]);
	/*parse for castling flags for 
	 * board[9] */
	for(int i = 0; i < sizeof(board[9])/sizeof(char); i++)
	{
		if(board[9][i] == 'K')
		{
			b->WKCFlag = 1;
		}
		else if(board[9][i] == 'Q')
		{
			b->WQCFlag = 1;
		}
		else if(board[9][i] == 'k')
		{
			b->BKCFlag = 1;
		}
		else if(board[9][i] == 'q')
		{
			b->BQCFlag = 1;
		}
	}
	/* now lets add the states to the array */
	for(int i = 0; i < 8; i++)
	{
		printf("parsing checker %s \n", board[i]);
		textConverterToEight(board[i],b, i);
	}

	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			printf("%d ", b->boardarray[i][j]);
		}
		printf("\n");
	} 
}


int charToPiece(char input)
{
	switch(input)
	{
		case 'P': return 1;
		case 'N': return 2;
		case 'B': return 3;
		case 'R': return 4;
		case 'Q': return 5;
		case 'K': return 6;
		case 'p': return 11;
		case 'n': return 12;
		case 'b': return 13;
		case 'r': return 14;
		case 'q': return 15;
		case 'k': return 16;
		//case '1': return 0;
	}
}
/*
int main()
{
	BSTATE test;
	//	fenToBoardState("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1", &test);
	//	fenToBoardState("rnbqkbnr/p1pppppp/8/1p6/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq -", &test);
	//	fenToBoardState("rnbqkbnr/p1pppppp/8/1p6/3PP3/2P2N2/PP3PPP/RNBQKB1R w KQkq -", &test);
	//	fenToBoardState("rn1qkbnr/pppppppp/1B6/1P1R3N/1b2K3/3N4/P1PPPPPP/R2Q1B2 w KQkq -", &test);
	//	fenToBoardState("rnbqkbnr/pppppppp/8/8/3Q1K2/1P4B1/P1PPPPPP/RN3BNR w KQkq -", &test);
	fenToBoardState("rnbqkbnr/pppppppp/8/8/1K2R3/1P4B1/8/8 w KQkq -", &test);
	fenToBoardState("rnbqkbnr/pppppppp/8/2R5/1K2R3/1P1R2B1/5K2/2B5 w KQkq -", &test);
	return 0;
}
*/
char* textConverterToEight(char* stringT, BSTATE *b, int index)
{
	//printf("string is %s\n", stringT);
	int temp[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	//printf("temp is %s\n", temp);
	/*getting size of string */
	int count = 0;
	int temp2 = 0;
	int positionTemp = 0;
	//	int spaceCounter = 0;
	while (stringT[count] != NULL || stringT[count] != '\0')
	{
		count++;
	}
	if(count == 8)
	{
		for(int i = 0; i< 8; i++)
		{
			temp[i] = charToPiece(stringT[i]);
			if(stringT[i] == '1')
			{
				temp[i] = 0;
			}
		}
	}
	else
	{
		for(int i = 0; i < count; i++)
		{
			if(stringT[i] >= '1' && stringT[i] <='8')
			{
				printf("stringT[i] is %c \n", stringT[i]);
				temp2 = (int)(stringT[i] - '0');
				positionTemp = i;
				printf("i is %d \n", i);
				for(int j = 0; j < (temp2); j++)
				{
					positionTemp++;
				}
			}
			else
			{
				//printf("piece being movie is %c at index %d \n", stringT[i], temp2);
				//temp[i+temp2 -1] = 'a';
				temp[positionTemp] = charToPiece(stringT[i]);
			}
		}
	}
	for(int i = 0; i< 8; i++)
	{
		b->boardarray[index][i] =temp[i];
	}
	return "";
}
