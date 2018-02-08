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
#include "boardstate.h"
#include "movelist.h"

// material value of a piece
static int piece_value[6] = {100, 320, 330, 500, 900, 20000};

// Pawn, Knight, Bishop, Rook Queen, King is the order in the
// piece square table

// 63- to flip
// sources for piece_square_table unmodified
// sources:
// https://chessprogramming.wikispaces.com/Simplified+evaluation+function
static int piece_square_table[6][64] = {
    {0,  0,  0,  0,   0,   0,  0,  0,  50, 50, 50,  50, 50, 50,  50, 50,
     10, 10, 20, 30,  30,  20, 10, 10, 5,  5,  10,  25, 25, 10,  5,  5,
     0,  0,  0,  20,  20,  0,  0,  0,  5,  -5, -10, 0,  0,  -10, -5, 5,
     5,  10, 10, -20, -20, 10, 10, 5,  0,  0,  0,   0,  0,  0,   0,  0},

    {-50, -40, -30, -30, -30, -30, -40, -50, -40, -20, 0,   0,   0,
     0,   -20, -40, -30, 0,   10,  15,  15,  10,  0,   -30, -30, 5,
     15,  20,  20,  15,  5,   -30, -30, 0,   15,  20,  20,  15,  0,
     -30, -30, 5,   10,  15,  15,  10,  5,   -30, -40, -20, 0,   5,
     5,   0,   -20, -40, -50, -40, -30, -30, -30, -30, -40, -50},

    {-20, -10, -10, -10, -10, -10, -10, -20, -10, 0,   0,   0,   0,
     0,   0,   -10, -10, 0,   5,   10,  10,  5,   0,   -10, -10, 5,
     5,   10,  10,  5,   5,   -10, -10, 0,   10,  10,  10,  10,  0,
     -10, -10, 10,  10,  10,  10,  10,  10,  -10, -10, 5,   0,   0,
     0,   0,   5,   -10, -20, -10, -10, -10, -10, -10, -10, -20

    },

    {0,  0, 0, 0, 0, 0, 0, 0,  5,  10, 10, 10, 10, 10, 10, 5,
     -5, 0, 0, 0, 0, 0, 0, -5, -5, 0,  0,  0,  0,  0,  0,  -5,
     -5, 0, 0, 0, 0, 0, 0, -5, -5, 0,  0,  0,  0,  0,  0,  -5,
     -5, 0, 0, 0, 0, 0, 0, -5, 0,  0,  0,  5,  5,  0,  0,  0},

    {-20, -10, -10, -5,  -5,  -10, -10, -20, -10, 0,   0,   0,  0,
     0,   0,   -10, -10, 0,   5,   5,   5,   5,   0,   -10, -5, 0,
     5,   5,   5,   5,   0,   -5,  0,   0,   5,   5,   5,   5,  0,
     -5,  -10, 5,   5,   5,   5,   5,   0,   -10, -10, 0,   5,  0,
     0,   0,   0,   -10, -20, -10, -10, -5,  -5,  -10, -10, -20

    },

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

float basicEvaluation(BSTATE *currentboard)
{
    assert(currentboard);

    float  eval_score = 0.0;
    MLIST *all_moves  = NULL;
    all_moves         = createMovelist();

    allLegal(all_moves, currentboard);
    assert(all_moves);

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

            if (currentboard->boardarray[board_index / 8][board_index % 8] > 10)
                {
                    eval_score +=
                        (-1) *
                        piece_square_table[currentboard
                                               ->boardarray[board_index / 8]
                                                           [board_index % 8] -
                                           11][63 - board_index];

                    // piece value feature
                    // eval_score+=piece_value[currentboard->boardarray[board_index
                    // / 8][board_index % 8]-11];
                }

            else if (currentboard->boardarray[board_index / 8]
                                             [board_index % 8] < 9 &&
                     currentboard
                             ->boardarray[board_index / 8][board_index % 8] > 0)
                {
                    eval_score +=

                        (1) *
                        piece_square_table[currentboard
                                               ->boardarray[board_index / 8]
                                                           [board_index % 8] -
                                           1][board_index];

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





    deleteMovelist(all_moves);

    return eval_score;
}

// int main(void)
// {
//   return 0;
// }
