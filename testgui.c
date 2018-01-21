#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "testgui.h"

void pcon(int input)
{
    char tempc[2] = "  ";
    int tempi,tempi2;
    tempi = input%10;
    if(tempi == 0)
    {
        tempc[1] = ' ';
        tempc[0] = ' ';
    }
    else if(tempi == 1)
    {
        tempc[1] = 'P';
    }
    else if(tempi == 2)
    {
        tempc[1] = 'N';
    }
    else if(tempi == 3)
    {
        tempc[1] = 'B';
    }
    else if(tempi == 4)
    {
        tempc[1] = 'R';
    }
    else if(tempi == 5)
    {
        tempc[1] = 'Q';
    }
    else if(tempi == 6)
    {
        tempc[1] = 'K';
    }
    tempi2 = (input/10)%10;
    if(tempi2 == 0 && tempi != 0)
    {
        tempc[0] = 'w';
    }
    else if(tempi2 == 1 && tempi != 0)
    {
        tempc[0] = 'b';
    }
    printf("  %c%c  ##",tempc[0],tempc[1]);
}

void GUI(int barr[8][8])
{
    int i,j;
    printf("\n        A       B       C       D       E       F       G       H");
    for(i = 0; i < 8; i++)
    {
        printf("\n   ##################################################################");
        printf("\n   ##      ##      ##      ##      ##      ##      ##      ##      ##");
        for(j = 0; j < 8; j++)
        {   
            if( j == 0)
            {
                printf("\n %d ##", 9-(i+1));
            }
            pcon((barr[i][j]));
        }
        printf("\n   ##      ##      ##      ##      ##      ##      ##      ##      ##");
    }
    printf("\n   ##################################################################\n");
}
