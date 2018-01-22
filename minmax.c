#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include "minmax.h"
#include "movelist.h"
#include "basic_eval.h"
#include "boardstate.h"

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
    node->head = NULL;
    return node;
}

/* create a head structure*/
HEAD *createHead(NODE *root)
{
    assert(root);
    HEAD *head = NULL;
    head = malloc(sizeof(HEAD));
    if(head == NULL)
    {
        perror("Out of memory aborting...");
        exit(10);
    }
    head->length = 1;
    head->root = root;
    root->head = head;
    return head;
}


/* create node with float value and add it to the parent node */
NODE *addChild(NODE* parent, HEAD *head, float value, MENTRY *move, BSTATE *board)
{
    assert(parent);
    assert(head);
    assert(parent->child == NULL);
    assert(move);
    assert(board);
    NODE *child = NULL;
    child = createNode(value, move, board);
    parent->child = child;
    child->parent = parent;
    child->head = head;
    head->length++;
    return child;     
}

/* create node with float value and add it to child */
NODE *addSibling(NODE *child, HEAD *head, float value, MENTRY *move, BSTATE *board)
{
    assert(child);
    assert(head);
    assert(child->next == NULL);
    assert(move);
    assert(board);
    NODE *sibling = NULL;
    sibling = createNode(value, move, board);
    child->next = sibling;
    sibling->parent = child->parent;
    sibling->head = head;
    head->length++;
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

/* remove the head struct */
void removeHead(HEAD *head)
{
    assert(head);
    head->root = NULL;
    free(head);   
}


/* generate a layer and a pointer to the first child */
NODE* generateLayer(NODE *parent, HEAD *head)
{
    MLIST* legalMLIST = NULL;
    MENTRY* currentMove = NULL; 
    NODE* currentNode = NULL;   
    
    legalMLIST = findlegalmoves(parent->board);
    currentMove = legalMLIST->start;
    currentNode = addChild(parent, head, 0, currentMove, mov(parent->board->boardarray, currentMove->CLOC, currentMove->NLOC));
    while(currentMove != NULL)
    {
        currentNode = addSibling(currentNode, head, 0, currentMove, mov(parent->board->boardarray, currentMove->CLOC, currentMove->NLOC));  
        currentMove = currentMove->Next;       
    }   
    return parent->child;
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
    MENTRY *bestMove;
    NODE *tree;
    HEAD *head;
    NODE *current;
    NODE *start;   

    int time = 40000; 
    clock_t start_time = clock();
    clock_t time_elapsed; 
    
    // creates first 2 levels of the tree
    tree = createNode(0, NULL, currentBoard);
    head = createHead(tree);
    current = tree;
    start = generateLayer(current);
    
    // create one level of tree per loop, checks for time at the end of each loop
    do
    {
        current = start;
        while(current != NULL)
        {
            start = generateLayer(current, head); 
            if(current == NULL && current->parent->next)
            {
                current = current->parent->next->child;
            }
            current = current->next;
        }
        start = start->parent->parent->child->child;
        time_elapsed = clock() - start_time;
    } while(time_elapsed < time);
    
    bestMove = alphabeta(tree, weights, -3.4E38, 3.4E38, Max);
    return bestMove;
}   
