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
static int piece_value[6]  = {100, 280, 320, 479, 929, 60000};

// Pawn, Knight, Bishop, Rook Queen, King is the order in the 
// piece square table

// 63- to flip
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

float basicEvaluation(BSTATE* currentboard)
{
    assert(currentboard);

    float  eval_score = 0.0;
    MLIST* all_moves  = NULL;
    all_moves         = createMovelist();

    const float mobilWeight=10;
    allLegal(all_moves, currentboard);
    assert(all_moves);

    //find total number of pieces
    int Bpawn, Bknight, Bbishop, Brook, Bqueen, Bking;
    int Wpawn, Wknight, Wbishop, Wrook, Wqueen, Wking;
    Bpawn = Bknight = Bbishop = Brook = Bqueen = Bking = 0;
    Wpawn = Wknight = Wbishop = Wrook = Wqueen = Wking = 0;;

    int a,c,d;
    //two sides
    for(a=0; a<2; ++a)
    {
        //8 by 8 matrix
        for(c=0; c<8; ++c)
        {
            for(d=0; d<8; ++d)
            {
                //white then black
                if(a == 0)
                {
                    if(currentboard->boardarray[c][d] == 1)
                    {
                        Wpawn++;           
                    }
                    else if(currentboard->boardarray[c][d] == 2)
                    {
                        Wknight++;
                    }
                    else if(currentboard->boardarray[c][d] == 3)
                    {
                        Wbishop++;
                    }
                    else if(currentboard->boardarray[c][d] == 4)
                    {
                        Wrook++;
                    }
                    else if(currentboard->boardarray[c][d] == 5)
                    {
                        Wqueen++;
                    }
                    else if(currentboard->boardarray[c][d] == 6)
                    {
                        Wking++;
                    }
                }
                else if(a==1)
                {
                    if(currentboard->boardarray[c][d] == 11)
                    {
                        Bpawn++;           
                    }
                    else if(currentboard->boardarray[c][d] == 12)
                    {
                        Bknight++;
                    }
                    else if(currentboard->boardarray[c][d] == 13)
                    {
                        Bbishop++;
                    }
                    else if(currentboard->boardarray[c][d] == 14)
                    {
                        Brook++;
                    }
                    else if(currentboard->boardarray[c][d] == 15)
                    {
                        Bqueen++;
                    }
                    else if(currentboard->boardarray[c][d] == 16)
                    {
                        Bking++;
                    }

                }
            }
        }
        
    }
    
    eval_score += piece_value[0]*(Wpawn - Bpawn);
    eval_score += piece_value[1]*(Wknight - Bknight);
    eval_score += piece_value[2]*(Wbishop - Bbishop);
    eval_score += piece_value[3]*(Wrook - Brook);
    eval_score += piece_value[4]*(Wqueen - Bqueen);
    eval_score += piece_value[5]*(Wking - Bking);
    //printf("\n %f \n",eval_score);
    //now adding the score for each piece
    
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
                
                //piece value feature
                //eval_score+=piece_value[currentboard->boardarray[board_index / 8][board_index % 8]-11];
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
                    
                    //piece value feature
                    //eval_score+=(-1)*piece_value[currentboard->boardarray[board_index / 8][board_index % 8]-1];
                }
        }
    
    //flip if on whiteside
  /*if (!currentboard->sidetomove)
        {
            eval_score*=(-1);
        }

        //mobility feature 
        eval_score+= all_moves->movenum*mobilWeight;*/
        deleteMovelist(all_moves);

        return eval_score;
}

// int main(void)
// {
//   return 0;
// }
