#include <stdio.h>
#include <stdlib.h>
#include "ChessGUI.h"
#include "ChessMov.h"

int main(void)
{
    int start[8][8] = {
        {14,12,13,15,16,13,12,14},
        {11,11,11,11,11,11,11,11},
        {0,0,0,0,0,0,0,0}, 
        {0,0,0,0,0,0,0,0}, 
        {0,0,0,0,0,0,0,0}, 
        {0,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,1},
        {4,2,3,5,6,3,2,4}};
    int input1,input2,ploc,pmov;
    char c1,c2;
    char line[100];
   
    while(1)
    {
        /*mov(start,9,17);
        mov(start,10,17);*/
        GUI(start);
        fgets(line, sizeof(line), stdin);
        sscanf(line,"%c%d %c%d", &c1, &input1, &c2, &input2);
        printf("\n%d\n", bcon(c1,input1));
        ploc = bcon(c1,input1);
        printf("\n%d\n", bcon(c2,input2));
        pmov = bcon(c2,input2);
        mov(start,ploc,pmov); 
    }

    return 0;
}
