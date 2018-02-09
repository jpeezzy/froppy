/**
 * Simple evaluation function used for early prototyping
 * Author: Khoi Trinh
 * the score depends only on the current board state
 * the evaluation function provides these features:
 * Material
 * Mobility
 * Piece Square Tables
 *
 */
#include "basic_eval.h"
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "boardstate.h"
#include "movelist.h"

// material value of a piece
static int piece_value[6] = {100, 280, 320, 479, 929, 60000};

// Pawn, Knight, Bishop, Rook Queen, King is the order in the
// piece square table

// 63- to flip
// sources for piece_square_table unmodified
// sources:
// https://chessprogramming.wikispaces.com/Simplified+evaluation+function
static int piece_square_table[6][64] = {
    {0,   0,   0,   0,   0,   0,   0,   0,
            78,  83,  86,  73, 102,  82,  85,  90,
             7,  29,  21,  44,  40,  31,  44,   7,
           -17,  16,  -2,  15,  14,   0,  15, -13,
           -26,   3,  10,   9,   6,   1,   0, -23,
           -22,   9,   5, -11, -10,  -2,   3, -19,
           -31,   8,  -7, -37, -36, -14,   3, -31,
             0,   0,   0,   0,   0,   0,   0,   0},

    {-66, -53, -75, -75, -10, -55, -58, -70,
            -3,  -6, 100, -36,   4,  62,  -4, -14,
            10,  67,   1,  74,  73,  27,  62,  -2,
            24,  24,  45,  37,  33,  41,  25,  17,
            -1,   5,  31,  21,  22,  35,   2,   0,
           -18,  10,  13,  22,  18,  15,  11, -14,
           -23, -15,   2,   0,   2,   0, -23, -20,
           -74, -23, -26, -24, -19, -35, -22, -69},

    {-59, -78, -82, -76, -23,-107, -37, -50,
           -11,  20,  35, -42, -39,  31,   2, -22,
            -9,  39, -32,  41,  52, -10,  28, -14,
            25,  17,  20,  34,  26,  25,  15,  10,
            13,  10,  17,  23,  17,  16,   0,   7,
            14,  25,  24,  15,   8,  25,  20,  15,
            19,  20,  11,   6,   7,   6,  20,  16,
            -7,   2, -15, -12, -14, -15, -10, -10},

    {35,  29,  33,   4,  37,  33,  56,  50,
            55,  29,  56,  67,  55,  62,  34,  60,
            19,  35,  28,  33,  45,  27,  25,  15,
             0,   5,  16,  13,  18,  -4,  -9,  -6,
           -28, -35, -16, -21, -13, -29, -46, -30,
           -42, -28, -42, -25, -25, -35, -26, -46,
           -53, -38, -31, -26, -29, -43, -44, -53,
           -30, -24, -18,   5,  -2, -18, -31, -32},

    {6,   1,  -8,-104,  69,  24,  88,  26,
            14,  32,  60, -10,  20,  76,  57,  24,
            -2,  43,  32,  60,  72,  63,  43,   2,
             1, -16,  22,  17,  25,  20, -13,  -6,
           -14, -15,  -2,  -5,  -1, -10, -20, -22,
           -30,  -6, -13, -11, -16, -11, -16, -27,
           -36, -18,   0, -19, -15, -15, -21, -38,
           -39, -30, -31, -13, -31, -36, -34, -42},

    { 4,  54,  47, -99, -99,  60,  83, -62,
           -32,  10,  55,  56,  56,  55,  10,   3,
           -62,  12, -57,  44, -67,  28,  37, -31,
           -55,  50,  11,  -4, -19,  13,   0, -49,
           -55, -43, -52, -28, -51, -47,  -8, -50,
           -47, -42, -43, -79, -64, -32, -29, -32,
            -4,   3, -14, -50, -57, -18,  13,   4,
            17,  30,  -3, -14,   6,  -1,  40,  18}};

float basicEvaluation(BSTATE *currentboard)
{
    assert(currentboard);

    float  eval_score = 0.0;
    MLIST *all_moves  = NULL;
    MLIST *all_O = NULL;
    all_O = createMovelist();
    all_moves         = createMovelist();

    allLegal(all_moves, currentboard);
    if(currentboard->sidetomove == 1)
    {
        currentboard->sidetomove = 0;
        allLegal(all_O,currentboard);
        currentboard->sidetomove = 1;
    }
    else if(currentboard->sidetomove == 0)
    {
        currentboard->sidetomove = 1;
        allLegal(all_O,currentboard);
        currentboard->sidetomove = 0;
    }

    //assert(all_moves);

    // find total number of pieces
    int tb, tw;
    tb = tw = 0;
    int Bpawn, Bknight, Bbishop, Brook, Bqueen, Bking;
    int Wpawn, Wknight, Wbishop, Wrook, Wqueen, Wking;
    Bpawn = Bknight = Bbishop = Brook = Bqueen = Bking = 0;
    Wpawn = Wknight = Wbishop = Wrook = Wqueen = Wking = 0;
    

    int a, c, d;
    // two sides
    for (a = 0; a < 2; ++a)
        {
            // 8 by 8 matrix
            for (c = 0; c < 8; ++c)
                {
                    for (d = 0; d < 8; ++d)
                        {
                            // white then black
                            if (a == 0)
                                {
                                    if (currentboard->boardarray[c][d] == 1)
                                        {
                                            Wpawn++;
                                            tw++;
                                        }
                                    else if (currentboard->boardarray[c][d] ==
                                             2)
                                        {
                                            Wknight++;
                                            tw++;
                                        }
                                    else if (currentboard->boardarray[c][d] ==
                                             3)
                                        {
                                            Wbishop++;
                                            tw++;
                                        }
                                    else if (currentboard->boardarray[c][d] ==
                                             4)
                                        {
                                            Wrook++;
                                            tw++;
                                        }
                                    else if (currentboard->boardarray[c][d] ==
                                             5)
                                        {
                                            Wqueen++;
                                            tw++;
                                        }
                                    else if (currentboard->boardarray[c][d] ==
                                             6)
                                        {
                                            Wking++;
                                            tw++;
                                        }
                                }
                            else if (a == 1)
                                {
                                    if (currentboard->boardarray[c][d] == 11)
                                        {
                                            Bpawn++;
                                            tb++;
                                        }
                                    else if (currentboard->boardarray[c][d] ==
                                             12)
                                        {
                                            Bknight++;
                                            tb++;
                                        }
                                    else if (currentboard->boardarray[c][d] ==
                                             13)
                                        {
                                            Bbishop++;
                                            tb++;
                                        }
                                    else if (currentboard->boardarray[c][d] ==
                                             14)
                                        {
                                            Brook++;
                                            tb++;
                                        }
                                    else if (currentboard->boardarray[c][d] ==
                                             15)
                                        {
                                            Bqueen++;
                                            tb++;
                                        }
                                    else if (currentboard->boardarray[c][d] ==
                                             16)
                                        {
                                            Bking++;
                                            tb++;
                                        }
                                }
                        }
                }
        }

    eval_score += piece_value[0] * (Wpawn - Bpawn);
    eval_score += piece_value[1] * (Wknight - Bknight);
    eval_score += piece_value[2] * (Wbishop - Bbishop);
    eval_score += piece_value[3] * (Wrook - Brook);
    eval_score += piece_value[4] * (Wqueen - Bqueen);
    eval_score += piece_value[5] * (Wking - Bking);
    // printf("\n %f \n",eval_score);
    // now adding the score for each piece
if((tb+tw)>7)
{
    for (int board_index = 0; board_index < 64; ++board_index)
        {
            // black case
            // 63-board_index will flip the board

            if (currentboard->boardarray[board_index / 8][board_index % 8]/10 == 1)
                {
                    eval_score -= piece_square_table[currentboard->boardarray[board_index / 8][board_index % 8] -11][63 - board_index];

                    // piece value feature
                    // eval_score+=piece_value[currentboard->boardarray[board_index
                    // / 8][board_index % 8]-11];
                }

            else if (currentboard->boardarray[board_index / 8]
                                             [board_index % 8] < 9 &&
                     currentboard
                             ->boardarray[board_index / 8][board_index % 8] > 0)
                {
                    eval_score += piece_square_table[currentboard->boardarray[board_index / 8][board_index % 8] -1][board_index];

                    // piece value feature
                    // eval_score+=(-1)*piece_value[currentboard->boardarray[board_index
                    // / 8][board_index % 8]-1];
                }
        }

    // flip if on whiteside
    /*if (!currentboard->sidetomove)
        {
            eval_score*=(-1);
        }

        //mobility feature
        eval_score+= all_moves->movenum*mobilWeight;*/
    
        // pawn strcutre bonus
        int i,j;
        for(i=0; i<8; ++i)
        {
            for(j=0; j<8; ++j)
            {
                if(currentboard->boardarray[i][j] == 1)
                {
                    if(j != 0 && i != 7 && currentboard->boardarray[i+1][j-1] == 1)
                    {
                        eval_score += 2;   
                    }
                    if(j != 7 && i != 7 && currentboard->boardarray[i+1][j+1] == 1)
                    {
                        eval_score += 2;
                    }

                }
                if(currentboard->boardarray[i][j] == 11)
                {
                    if(j != 0 && i != 0 && currentboard->boardarray[i-1][j-1] == 11)
                    {
                        eval_score -= 2;   
                    }
                    if(j != 7 && i != 0 && currentboard->boardarray[i-1][j+1] == 11)
                    {
                        eval_score -= 2;
                    }

                }

            }
        }
}
if((tb+tw) < 7)
{
    for (int board_index = 0; board_index < 64; ++board_index)
        {
            // black case
            // 63-board_index will flip the board

            if (currentboard->boardarray[board_index / 8][board_index % 8] == 11)
                {
                    eval_score -= piece_square_table[currentboard->boardarray[board_index / 8][board_index % 8] -11][63 - board_index];

                    // piece value feature
                    // eval_score+=piece_value[currentboard->boardarray[board_index
                    // / 8][board_index % 8]-11];
                }

            else if (currentboard->boardarray[board_index / 8]
                                             [board_index % 8] == 1)
                {
                    eval_score += piece_square_table[currentboard->boardarray[board_index / 8][board_index % 8] -1][board_index];

                    // piece value feature
                    // eval_score+=(-1)*piece_value[currentboard->boardarray[board_index
                    // / 8][board_index % 8]-1];
                }
        }

    //king distance
    int wx,wy,bx,by;
    int xx,yy;
    for(xx=0; xx<8; ++xx)
    {
        for(yy=0; yy<8; ++yy)
        {
            if(currentboard->boardarray[xx][yy] == 6)
            {
                wx = xx;
                wy = yy;
            }
            if(currentboard->boardarray[xx][yy] == 16)
            {
                bx = xx;
                by = yy;
            }
        }

    }
    int kingd = sqrt(pow(wx-bx,2) + pow(wy-by,2)); 

    if(tw > tb)
    {
        eval_score -= .5*kingd; 
    }
    if(tb > tw)
    {
        eval_score += .5*kingd;
    }

}

    //mobility
    int Wmob, Bmob;
    Wmob = Bmob = 0;
    int px, py;
    MENTRY *temp;
    temp = all_moves->First;
    while(temp)
    {
        px = temp->CLOC/8;
        py = temp->CLOC%8;
        if(currentboard->boardarray[px][py]/10 == 1)
        {
            Bmob++;
        }
        if(currentboard->boardarray[px][py]/10 == 0)
        {
            Wmob++;
        }
        temp = temp->Next;
    }
    temp = all_O->First;
    while(temp)
    {
        px = temp->CLOC/8;
        py = temp->CLOC%8;
        if(currentboard->boardarray[px][py]/10 == 1)
        {
            Bmob++;
        }
        if(currentboard->boardarray[px][py]/10 == 0)
        {
            Wmob++;
        }
        temp = temp->Next;
    }
    //printf(" Wmob: %d Bmob: %d \n ", Wmob, Bmob);
    eval_score += (Wmob - Bmob) *  .5; 

    if(currentboard->sidetomove == 0)
    {
        if(Wmob == 0)
        {
            eval_score -= 1000000; 
        }
    }
    if(currentboard->sidetomove == 1)
    {
        if(Bmob == 0)
        {
            eval_score += 1000000;
        }
    }



    deleteMovelist(all_moves);
    deleteMovelist(all_O);
    return eval_score;
}

// int main(void)
// {
//   return 0;
// }
