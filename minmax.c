#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include "minmax.h"

/* create Node */
NODE *createNode(float value, MENTRY *move)
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
    node->board = NULL;
    node->parent = NULL;
    node->child = NULL;
    node->next = NULL;
    return node;
}

/* create node with float value and add it to the parent node */
NODE *addChild(NODE* parent, float value, MENTRY *move)
{
    assert(parent);
    assert(parent->child == NULL);
    NODE *child = NULL;
    child = createNode(value, move);
    parent->child = child;
    child->parent = parent;
    return child;     
}

/* create node with float value and add it to child */
NODE *addSibling(NODE *child, float value, MENTRY *move)
{
    assert(child);
    assert(child->next == NULL);
    NODE *sibling = NULL;
    sibling = createNode(value, move);
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
    BSTATE *board = NULL;
    MLIST *mlist = NULL;
    MENTRY *move = NULL;
    float temp; 
    
    // if current node is the a node, returns its value;
    if(node->child == NULL)
    {
        current = node;
        mlist = createMoveList();
        while(current-> parent != NULL)
        {
          appendMove(mlist, current->move);
          current = current->parent; 
        }
        assert(current->board);
        board = current->board;
        move = mlist->last;
        while(move != NULL)
        {
          board = makemove(board, move);
          move = move->Prev;
        }
        node->value = basicEvaluation(board, weights);
        deleteMoveList(mlist);
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
    BSTATE *board = NULL;
    NODE *tree = NULL;
    NODE *currentNode = NULL;
    int time = 45000; 
    clock_t start = clock();
    clock_t time_elapsed; 
    tree = createNode(0 ,NULL);
    tree->board = currentBoard;
    currentNode = tree;
    int new_layer;
    board = currentBoard;
    do
    {
        legalMLIST = findlegalmoves(board);
        currentMove = legalMLIST->start;
        new_layer = 1;
        while(currentMove != NULL)
        {
            if(new_layer)
            {
                currentNode = addChild(currentNode, 0, currentMove);
                new_layer = 0;
            }
            else
            {
                currentNode = addSibling(currentNode, 0, currentMove);
            }   
            currentMove = currentMove->Next;   
        }
        time_elapsed = clock() - start;
    }while(time_elapsed < time);
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
