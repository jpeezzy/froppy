#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
void fenToBoardState(char * fen) //, BSTATE *b)
{
	/* cheess board pieces based off the positions 1-8, where black king is at board1 */
	char* board[13];
	int n = 0;
	char* buf = malloc(sizeof(char*) * 30);
	int count = 0;
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
	for(int i = 0; i < 13; i++)
	{
		printf("%s\n", board[i]);
	}
}

int main()
{
	fenToBoardState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	return 0;
}
