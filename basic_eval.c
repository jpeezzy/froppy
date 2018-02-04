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
static int piece_weight[6] = {1, 3, 3, 5, 9, 200};

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
    int whitePiecenum[6]={0,0,0,0,0,0};
    int blackPiecenum[6]={0,0,0,0,0,0};
    MLIST* all_moves  = NULL;
    all_moves         = createMovelist();

    allLegal(all_moves, currentboard);
    assert(all_moves);

    for (int board_index = 0; board_index < 64; ++board_index)
        {
            // black case
            // 63-board_index will flip the board

            if (currentboard->boardarray[board_index / 8][board_index % 8] > 10)
                {
                    eval_score +=
                        piece_square_table[currentboard
                                               ->boardarray[board_index / 8]
                                                           [board_index % 8] -
                                           11][63 - board_index];
                
                //piece value feature
                eval_score+=piece_value[currentboard->boardarray[board_index / 8][board_index % 8]-11];
                }

            else if (currentboard->boardarray[board_index / 8]
                                             [board_index % 8] < 9 &&
                     currentboard
                             ->boardarray[board_index / 8][board_index % 8] > 0)
                {
                    eval_score +=

                        (-1) *
                        piece_square_table[currentboard
                                               ->boardarray[board_index / 8]
                                                           [board_index % 8] -
                                           1][board_index];
                    
                    //piece value feature
                    eval_score+=(-1)*piece_value[currentboard->boardarray[board_index / 8][board_index % 8]-1];
                }
        }
    
    //flip if on whiteside
    if (!currentboard->sidetomove)
        {
            eval_score*=(-1);
        }

        //mobility feature 
        eval_score+= all_moves->movenum;
        deleteMovelist(all_moves);

        return eval_score;
}

// int main(void)
// {
//   return 0;
// }
