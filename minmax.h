#ifndef MINMAX_H
#define MINMAX_H

typedef struct node NODE;
typedef struct head HEAD;

/* structure to keep track of total number of nodes in a tree */
struct head
{
    int length;
    NODE *root;
};

/* node structure to create n-ary trees for minmax */
struct node
{
    float value;
    MENTRY *move;
    BSTATE *board;
    NODE *parent;
    NODE *child;
    NODE *next;
    int length;
    HEAD *head;
};

/* enum to define minimizers and maximizers */
typedef enum player{Min, Max} PLAYER; 

/* create a Node with input float value */
NODE *createNode(float value, MENTRY *move, BSTATE *board);

/* create a head structure*/
HEAD *createHead(NODE *root);

/* add a child to the parent node with input float value */
NODE *addChild(NODE* parent, HEAD* head, float value, BSTATE *board);

/* add sibling next to the child node with input float value */
NODE *addSibling(NODE *child, HEAD * head, float value, BSTATE *board);

/* remove the node along with its siblings and children */
void removeNode(NODE *node);

/* remove the head struct */
void removeHead(HEAD *head);

/* generate a layer and a pointer to the first child */
NODE *generateLayer(NODE *parent, NODE *head);

/* finds the best worst value in the tree*/
NODE *alphabeta(NODE* node, WEIGHTS *weights, float alpha, float beta, PLAYER minmax);

/* finds the best worst move for the AI to make */
MENTRY *minmax (BSTATE *currentBoard, WEIGHTS *weights);

#endif
