#ifndef MINMAX_H
#define MINMAX_H

typedef struct node NODE;

/* node structure to create n-ary trees for minmax */
struct node
{
    float value;
    MENTRY *move;
    BSTATE *board;
    NODE *parent;
    NODE *child;
    NODE *next;
};

/* enum to define minimizers and maximizers */
typedef enum player{Min, Max} PLAYER; 

/* create a Node with input float value */
NODE *createNode(float value, MENTRY *move);

/* add a child to the parent node with input float value */
NODE *addChild(NODE* parent, float value);

/* add sibling next to the child node with input float value */
NODE *addSibling(NODE *child, float value);

/* remove the node along with its siblings and children */
void removeNode(NODE *node);

/* finds the best worst value in the tree*/
NODE *alphabeta(NODE* node, float alpha, float beta, PLAYER minmax);

/* finds the best worst move for the AI to make */
MENTRY *minmax (BSTATE *currentBoard, WEIGHTS *weights);

#endif
