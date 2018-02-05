#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "basic_eval.h"
#include "boardstate.h"
#include "spicyComments.h"

#define THRESHOLD -1000.0

void spicyAdd(BSTATE* board)
{
    int num = 0;
    srand(time(NULL));

    // list of comments to choose from based on how good the moves
    // that the user just made
    // add more spicy comments here
    char* spicyListBad[] = {
        "You can do better than this",
        "You shouldn't have picked this difficulty",
        "It definitely doesn't take 5 weeks to make an AI that can wreck you",
        "I hope you code better than you play chess"
		"Are you watson? cause you look hard coded"};

    char* spicyListGood[] = {
        "What a spicy move",
        "Deep blue, is that you?",
        "What does the eval function return for the move? It's over 9000!!!!!!!!!",
        "It's either you are really good or this eval function is really bad",
        "I'm gonna need more features to make it through this game"};

    // don't forget to adjust the total number of comments here
    const int spicyListBadLen  = 5;
    const int spicyListGoodLen = 5;

    if (basicEvaluation(board) <= THRESHOLD)
        {
            num = rand() % spicyListBadLen;
            printf("%s\n", spicyListBad[num]);
            return;
        }
    else
        {
            num = rand() % spicyListGoodLen;
            printf("%s\n", spicyListGood[num]);
            return;
        }
}
