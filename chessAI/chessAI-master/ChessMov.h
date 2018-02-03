#ifndef CHESSMOV_H
#define CHESSMOV_H

typedef struct MoveList MOVE;

struct MoveList
{
    MOVE *Next;
    MOVE *Prev;
    int ML; 
};
void mov(int barr[8][8], int ploc, int pmov);

int bcon(char a, int b);

int * pawnmov(int barr[8][8], int ploc);

int * nmov(int barr[8][8], int ploc);

int * bmov(int barr[8][8], int ploc);

int * rmov(int barr[8][8], int ploc);

int * qmov(int barr[8][8], int ploc);

int * kmov(int barr[8][8], int ploc);

#endif
