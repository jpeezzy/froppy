#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>
#include <curses.h>

#include "ChessGUI.h"

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

void unicodeChess(int input)
{
	wchar_t ac = 0x2654;
	int type = input / 10;
	int class = input % 10; 
	if (type == 1)
	{
		ac =((class == 1) ? 0x2659: /*chess piece is black */
		(class == 2) ? 0x2658 :
		(class == 3) ? 0x2657 :
		(class == 4) ? 0x2656 : 
		(class == 5) ? 0x2655 : 
		(class == 6) ? 0x2654 :
		0x0020); 
	}
	else /*if piece is black */
	{
		ac = (class == 1) ? 0x265F :
		(class == 2) ? 0x265E :
		(class == 3) ? 0x265D :
		(class == 4) ? 0x265C : 
		(class == 5) ? 0x265B : 
		(class == 6) ? 0x265A : 0x0020;
	}
		/*(class == 2 && ac = 0x2658) || 
		(class == 3 && ac = 0x2657) || 
		(class == 4 && ac = 0x2656) ||
		(class == 5 && ac = 0x2655) ||
		(class == 6 && ac = 0x2654); */
	/*use freopen to reset stdout, using wprintf with printf leads to undefined behavior */
//	printf("printin unicode \n");
	freopen(NULL, "w", stdout);
	//wprintf(L"%lc", ac);
	setlocale(LC_CTYPE,"");
    wprintf(L"   %lc  ##", ac);
	freopen(NULL, "w", stdout);
	setlocale(LC_ALL,"");
//	printf("doen printing \n");

	return;
}
void GUI(int barr[8][8])
{
	unicodeChess(11);
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
            //pcon((barr[i][j]));
           unicodeChess((barr[i][j]));
        }
        printf("\n   ##      ##      ##      ##      ##      ##      ##      ##      ##");
    }
    printf("\n   ##################################################################\n");
}
