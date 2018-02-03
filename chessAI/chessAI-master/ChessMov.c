#include <stdio.h>
#include <stdlib.h>
#include "ChessMov.h"

void mov(int barr[8][8], int ploc, int pmov)
{
    int temp = barr[ploc/8][ploc%8];
    barr[ploc/8][ploc%8] = 0;
    barr[pmov/8][pmov%8] = temp;
}
/* Character Conversion*/ 
int ccon(char a)
{
    if(a == 'a')
    {
        return 0;
    }
    else if(a == 'b')
    {
        return 1;
    }
    else if(a == 'c')
    {
        return 2;
    }
    else if(a == 'd')
    {
        return 3;
    }
    else if(a == 'e')
    {
        return 4;
    }
    else if(a == 'f')
    {
        return 5;
    }
    else if(a == 'g')
    {
        return 6;
    }
    else if(a == 'h')
    {
        return 7;
    }
}
/*Board Conversion*/
int bcon(char a, int b)
{
    int temp;
    temp = (((8-(b)) * 8) + (ccon(a)));
    return temp;
}

MOVE *CreateMoveList(void)
{
    MOVE *move;
    move = malloc(sizeof(MOVE));
    if(!move)
    {
        perror("Out of Memory");
        exit(10);
    }
    move->Next = NULL;
    move->Prev = NULL;
    move->ML = NULL;
    return move;
}

