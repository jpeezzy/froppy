#ifndef MINMAX_H
#define MINMAX_H

#include "movelist.h"
#include "boardstate.h"

typedef struct node NODE;
typedef struct head HEAD;
typedef struct mini MINI;

/* node structure to create n-ary trees for minmax */
struct node
{
    float value;
    MENTRY *move;
    BSTATE *board;
    MLIST *legal; /* legal move list for current board */
    NODE *parent;
    NODE *child;
    NODE *next;
    int children; /* number of children nodes of current node */
    HEAD *head; 
};

/* structure to keep track of total number of nodes in a current depth */
struct head
{
    int length;
    NODE *first; /* pointer to first node in current depth */
  	HEAD *next; /* pointer to the head struct of next depth */
};

/* structure to store board and best move for new implementation of minimax */
struct mini
{
   BSTATE *board;
   MENTRY *move;
   int store;      
};

/* enum to define minimizers and maximizers */
typedef enum player{Min, Max} PLAYER; 

/* create a Node with input float value */
NODE *createNode(float value, MENTRY *move, BSTATE *board);

/* create a head structure */
void *createHead(NODE *root);

/* create mini structure */
MINI *createMini(BSTATE *board);

/* add a child to the parent node with input float value */
NODE *addChild(NODE* parent, float value, MENTRY *move, BSTATE *board);

/* add sibling next to the child node with input float value */
NODE *addSibling(NODE *child, float value, MENTRY *move, BSTATE *board);

/* remove the node along with its siblings and children and head struct*/
void removeNode(NODE *node);

/* remove the head struct */
void removeHead(HEAD *head);

/* remove mini struct */
void removeMini(MINI *mini);

/* generate a layer and a pointer to the first child */
NODE *generateLayer(NODE *parent);

/* finds the best worst value in the tree */
float alphabeta(NODE* node, float alpha, float beta, PLAYER minmax);

/* finds the best worst move for the AI to make */
MENTRY *minmax (BSTATE *currentBoard);

/* maximizer function for minimax with alpha-beta pruning that uses depth */
float alphaBetaMax(BSTATE* board, float alpha, float beta, int depth);
 
/* minimizer function for minimax with alpha-beta pruning that uses depth */ 
float alphaBetaMin(BSTATE *board, float alpha, float beta, int depth); 

#endif
