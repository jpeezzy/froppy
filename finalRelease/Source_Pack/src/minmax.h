#ifndef MINMAX_H
#define MINMAX_H
#include "movelist.h"
#include "boardstate.h"

typedef struct node NODE;
typedef struct head HEAD;

/* node structure to create n-ary trees for minmax */
struct node
{
    float value;
    MENTRY *move;  /* pointer to the move */
    BSTATE *board; /* pointer to board */
    MLIST *legal; /* legal move list for current board */
    NODE *parent; /* pointer to parent node */
    NODE *child; /* pointer to first child node */
    NODE *next; /* pointer to next sibling node */
    NODE *cousin; /* pointer to next cousin node */
    int children; /* number of children nodes of current node */
    HEAD *head; /* pointer to head struct of current depth*/
};

/* structure to keep track of total number of nodes and the pointer to first node in a current depth */
struct head
{
    int length;
    NODE *first; /* pointer to first node in current depth */
    NODE *last; /*pointer to last node in current depth */
    HEAD *next; /* pointer to the head struct of next depth */
};

/* create a Node with input float value */
NODE *createNode(MENTRY *move, BSTATE *board);

/* create a head structure */
void createHead(NODE *first);

/* add a child to the parent node with input float value */
NODE *addChild(NODE* parent, MENTRY *move, BSTATE *board);

/* add sibling next to the child node with input float value */
NODE *addSibling(NODE *child, MENTRY *move, BSTATE *board);

/* remove the node along with its siblings and children and head struct*/
void removeNode(NODE *node);

/* remove the head struct */
void removeHead(HEAD *head);

/* generate a layer and a pointer to the first child */
NODE *generateLayer(NODE *parent);

/* finds the best worst move for the AI to make */
MENTRY *minmax (BSTATE *currentBoard);

/* maximizer function for minimax with alpha-beta pruning */
float max(NODE *node, float alpha, float beta);
 
/* minimizer function for minimax with alpha-beta pruning */ 
float min(NODE *node, float alpha, float beta); 




/* alternative way of implementing minmax using depth and mov, undo mov */
typedef struct mini MINI;
/* structure to store board and best move for new implementation of minimax */

struct mini
{
   BSTATE *board;
   MENTRY *move; 
   int depth; /* depth used to call minimax function */   
};

/* create mini structure */
MINI *createMini(BSTATE *board, int depth);

/* remove mini struct */
void removeMini(MINI *mini);

/* maximizer function for minimax with alpha-beta pruning that uses depth */
float altMax(MINI *mini, float alpha, float beta, int depth);

/* minimizer function for minimax with alpha-beta pruning that uses depth */ 
float altMin(MINI* mini, float alpha, float beta, int depth);
 
#endif
