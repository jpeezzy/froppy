/**
 * Simple evaluation function used for early prototyping
 * Author: Khoi Trinh
 * the score is relative to the other side
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

// weight coeeficient for each type of chess piece

#define MOBILITY_WEIGHT 0.2

// material value of a piece
static int piece_value[6]  = {100, 280, 320, 479, 929, 60000};
static int piece_weight[6] = {1, 3, 3, 5, 9, 200};

// position score is used for evaluation piece based on their coordinates
// piece square tables
// source: https://github.com/thomasahle/sunfish
// Pawn, Knight, Bishop, Rook Queen, King

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

    {-30, -40, -40, -50, -50, -40, -40, -30, -30, -40, -40, -50, -50,
     -40, -40, -30, -30, -40, -40, -50, -50, -40, -40, -30, -30, -40,
     -40, -50, -50, -40, -40, -30, -20, -30, -30, -40, -40, -30, -30,
     -20, -10, -20, -20, -20, -20, -20, -20, -10, 20,  20,  0,   0,
     0,   0,   20,  20,  20,  30,  10,  0,   0,   10,  30,  20}};

int basicEvaluation(BSTATE* currentboard)
{
    double eval_score = 0.0;
    MLIST* all_moves  = NULL;
    all_moves         = createMovelist();
    allLegal(all_moves, currentboard);
    assert(all_moves);
    MENTRY* current_move = all_moves->First;
    // int     piece_count[12] = {0};
    // compute mobility score
    eval_score += all_moves->movenum * MOBILITY_WEIGHT;

    int temp   = 0;
    int change = 0;
    while (current_move != NULL)
        {
            for (int board_index = 0; board_index < 64; ++board_index)
                {
                    // check if the current position is changed on next move
                    if (board_index == current_move->CLOC)
                        {
                            change      = 1;
                            temp        = board_index;
                            board_index = current_move->NLOC;
                        }

                    // black case
                    if (!currentboard->sidetomove)
                        {
                            if (currentboard->boardarray[board_index / 8]
                                                        [board_index % 8] > 10)
                                {
                                    //++piece_count[boardarray[board_index] -
                                    // 5];

                                    eval_score += piece_square_table
                                        [currentboard
                                             ->boardarray[board_index / 8]
                                                         [board_index % 8] -
                                         11][63 - board_index];
                                    // compute material value
                                    eval_score +=
                                        piece_value[currentboard->boardarray
                                                        [board_index / 8]
                                                        [board_index % 8] -
                                                    11] *
                                        piece_weight[currentboard->boardarray
                                                         [board_index / 8]
                                                         [board_index % 8] -
                                                     11];
                                }
                        }

                    // white case
                    else if (currentboard->sidetomove)
                        {
                            if (currentboard->boardarray[board_index / 8]
                                                        [board_index % 8] < 9)
                                {
                                    //++piece_count[boardarray[board_index] -
                                    // 1];
                                    // use piece square table to evaluate based
                                    // on position
                                    eval_score += piece_square_table
                                        [currentboard
                                             ->boardarray[board_index / 8]
                                                         [board_index % 8] -
                                         1][board_index];

                                    // compute material value
                                    eval_score +=
                                        piece_value[currentboard->boardarray
                                                        [board_index / 8]
                                                        [board_index % 8] -
                                                    1] *
                                        piece_weight[currentboard->boardarray
                                                         [board_index / 8]
                                                         [board_index % 8] -
                                                     1];
                                }
                        }
                    if (change)
                        {
                            change      = 0;
                            board_index = temp;
                        }
                }
            current_move = current_move->Next;
        }
    deleteMovelist(all_moves);
    return eval_score;
}

// int main(void)
// {
//   return 0;
// }
