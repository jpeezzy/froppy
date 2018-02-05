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
        "I hope you code better than you play chess",
	"Are you watson? cause you look hard coded",
        "You're the type of player to get 3rd place in a 1v1 match",
        "I would give you a free piece, but you would still miss",
        "You're impossible to underestimate",
        "Is your AI missing a chromosome?",
        "You would be better off making random moves",
        "My 'Easy' difficulty chess program can do a better move than that",
        "You are just a budget version of a bronze 5 chess engine",
        "Congratulations, your probablity of losing is now higher than your IQ",
        "Calling your AI trash would be an understatement"};

    char* spicyListGood[] = {
        "What a spicy move",
        "Deep blue, is that you?",
        "What does the eval function return for the move? It's over 9000!!!!!!!!!",
        "It's either you are really good or this eval function is really bad",
        "I'm gonna need more features to make it through this game",
        "Finally, a decent move...",
        "Didn't know you are capapble of doing such a move",
        "Suprised you managed find something useful out of a trashcan"};

    // don't forget to adjust the total number of comments here
    const int spicyListBadLen  = 14;
    const int spicyListGoodLen = 8;

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
