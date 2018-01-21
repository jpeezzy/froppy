#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include "minmax.h"

/* create Node */
NODE *createNode(float value, MENTRY *move, BSTATE *board)
{
    NODE *node = NULL;
    node = malloc(sizeof(NODE));
    if(node == NULL)
    {
        perror("Out of memory aborting...");
        exit(10);
    }   
    node->value = value;
    node->move = move;
    node->board = board;
    node->parent = NULL;
    node->child = NULL;
    node->next = NULL;
    return node;
}

/* create node with float value and add it to the parent node */
NODE *addChild(NODE* parent, float value, MENTRY *move, BSTATE *board)
{
    assert(parent);
    assert(parent->child == NULL);
    assert(move);
    assert(board);
    NODE *child = NULL;
    child = createNode(value, move, board);
    parent->child = child;
    child->parent = parent;
    return child;     
}

/* create node with float value and add it to child */
NODE *addSibling(NODE *child, float value, MENTRY *move, BSTATE *board)
{
    assert(child);
    assert(child->next == NULL);
    assert(move);
    assert(board);
    NODE *sibling = NULL;
    sibling = createNode(value, move, board);
    child->next = sibling;
    sibling->parent = child->parent;
    return sibling;
}

/* remove node along with its child and siblings */
void removeNode(NODE *node)
{
    assert(node);
    if(node->child)
    {
        removeNode(node->child);
    }
    if(node->next)
    {
        removeNode(node->next);
    }
    node->move = NULL;
    node->board = NULL;
    node->parent = NULL;
    node->child = NULL;
    node->next = NULL;
    free(node);
}

/* finds the best worst value in the tree using minmax with alphabeta pruning */
float *alphabeta(NODE *node, WEIGHTS *weights, float alpha, float beta, PLAYER minmax)
{
    assert(node);
    assert(weights);
    NODE *current = NULL;
    float temp; 
    
    // if current node is the a node, returns its value;
    if(node->child == NULL)
    {
        node->value = basicEvaluation(node->board, weights);
        return node->value;
    }
    
    // if current node is a maximizer
    if(minmax == Max)
    {
        node->value = -3.4E38;
        current = node->child;
        while(current)
        {
            temp = alphabeta(current, alpha, beta, Min);
            if(temp > node->value)
            {
                node->value = temp;
                // if node is the root(top) node, store the pointer to the moveEntry Struct of current in it. 
                if(node->parent = NULL)
                {
                    node->move = current->move;       
                }
            }
            alpha = (alpha > node->value) ? alpha : node->value;
            if(beta <= alpha)
            {
                // pruning the tree
                break;
            }
            current = current->next;
        }
        current = NULL;
        return node->value;
    }       
    
    // if current node is a minimizer 
    else
    {
        node->value = 3.4E38;
        current = node->child;
        while(current)
        {
            temp = alphabeta(current, alpha, beta, Max);
            node->value = (node->value < temp) ? node->value : temp;
            beta = (beta < node->value) ? beta : node->value;
            if(beta <= alpha)
            {
                // pruning the tree
                break;
            }
            current = current->next;
        }
        current = NULL;
        return node->value;   
    }
}
 
/* finds the best worst move for the AI to make, returns pointer to MENTRY*/
MENTRY *minmax(BSTATE *currentBoard, WEIGHTS *weights)
{
    MLIST *legalMLIST = NULL;
    MENTRY *bestMove = NULL;
    MENTRY *currentMove = NULL;
    NODE *tree = NULL;
    NODE *currentNode = NULL;
    BSTATE *board = NULL;
    
    int time = 40000; 
    clock_t start = clock();
    clock_t time_elapsed; 
    
    tree = createNode(0, NULL, currentBoard);
    currentNode = tree;
    int new_layer;
    
    // first layer of the tree
    legalMLIST = findlegalmoves(currentNode->board);
    currentMove = legalMLIST->start;
    board = currentNode->board;
    new_layer = 1;
    while(currentMove != NULL)
    {
        if(new_layer)
        {
            currentNode = addChild(currentNode, 0, currentMove, makeamove(board, currentMove));
            new_layer = 0;
        }
        else
        {
            currentNode = addSibling(currentNode, 0, currentMove, makeamove(board, currentMove));
        }  
        currentMove = currentMove->Next;       
    }   
    
}   

/* generate a layer and a pointer to the first child */
NODE* generateLayer(NODE *parent)
{
    MLIST* legalMLIST = NULL;
    MENTRY* currentMove = NULL; 
    NODE* currentNode = NULL;   
    
    legalMLIST = findlegalmoves(parent->board);
    currentMove = legalMLIST->start;
    currentNode = addChild(parent, 0, currentMove, makeamove(parent->board, currentMove));
    while(currentMove != NULL)
    {
        currentNode = addSibling(currentNode, 0, currentMove, makeamove(board, currentMove));  
        currentMove = currentMove->Next;       
    }   
    return parent->child;
}

int main()
{
    NODE *tree = NULL;
    NODE *current = NULL;
    NODE *add = NULL;
    float x;
    int i;
    srand(time(NULL));
    tree = createNode(0, NULL);
    current = addChild(tree, 0, NULL);
    current = tree->child;
    for(i = 0; i < 19; i++)
    {
        current = addSibling(current, 0, NULL);
    }
    current = tree->child;
    add = addChild(current, 2.0);
    while(current)
    {
        for(i = 0; i < 19; i++)
        {
            x = ((float)rand()/(float)(RAND_MAX)) * 5.0;
            add = addSibling(add, x);                
        }
        current = current->next;
    }
        float value = alphabeta(tree, -3.4E38, 3.4E38, Max);
        printf("%2.6f\n",value);
        removeNode(tree);
        return 0;
}
