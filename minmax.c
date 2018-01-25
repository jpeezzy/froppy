#include <omp.h>
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
    node->children = 0;
    node->head = NULL;
    return node;
}

/* create a head structure*/
void *createHead(NODE *first)
{
    assert(first);
    HEAD *head = NULL;
    head = malloc(sizeof(HEAD));
    if(head == NULL)
    {
        perror("Out of memory aborting...");
        exit(10);
    }
    head->length = 1;
    head->first = first;
    first->head = head;
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
    parent->children = 1;
    child->parent = parent;
    /* check if the parent is the first node */
    if(parent->head->first == parent)
    {
        createHead(child);
    }
    else
    {
        child->head = parent->head->first->child->head;
    }
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
    sibling->parent->children++;
    sibling->head = child->head;
    return sibling;
}

/* remove node along with its child and siblings */
void removeNode(NODE *node)
{
    assert(node);
    if(node->head->first == node)
    {
        removeHead(node->head);
        node->head = NULL;
    }
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
    head->first = NULL;
    free(head);   
}

/* generate a layer and a pointer to the first child */
NODE* generateLayer(NODE *parent)
{
    int length;
    MLIST* legalMLIST = NULL;
    MENTRY* currentMove = NULL; 
    NODE* currentNode = NULL;   
    allLegal(legalMLIST, parent->board);
    currentMove = legalMLIST->start;
    currentNode = addChild(parent, 0, currentMove, mov(parent->board->boardarray, currentMove->CLOC, currentMove->NLOC));
    length = legalMLIST->movenum;

#pragma omp parallel
    {
    #pragma omp for
        for(int i = 0; i < length; i++)
        {
            currentNode = addSibling(currentNode, 0, currentMove, mov(parent->board->boardarray, currentMove->CLOC, currentMove->NLOC));  
            currentMove = currentMove->Next;       
        }
    }   
    return parent->child;
}

/* finds the best worst value in the tree using minmax with alphabeta pruning */
float *alphabeta(NODE *node, WEIGHTS *weights, float alpha, float beta, PLAYER minmax)
{
    assert(node);
    assert(weights);
    NODE *current = NULL;
    int children;
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
        children = node->children;
        
    #pragma omp parallel
        {
        #pragma omp for 
            for(int i = 0; i < children; i++)
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
            }/* rof */
        }
        current = NULL;
        return node->value;
    }       
    
    // if current node is a minimizer 
    else
    {
        node->value = 3.4E38;
        current = node->child;
        children = node->children;
    
    #pragma omp parallel
        {
        #pragma omp for
            for(int i = 0; i < children; i++)
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
            }/* rof */
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
    NODE *current;
    NODE *start; 
    int length;  

    int time = 40000; 
    clock_t start_time = clock();
    clock_t time_elapsed; 
    
    // creates first 2 levels of the tree
    tree = createNode(0, NULL, currentBoard);
    current = tree;
    start = generateLayer(current);
    
    // create one level of tree per loop, checks for time at the end of each loop
    do
    {
        current = start;
        length = current->head->length;
    #pragma omp parallel
        { 
        #pragma omp for
            for(int i = 0; i < length; i++)
            {
                start = generateLayer(current);
                current = current->next; 
                if(current == NULL && current->parent->next)
                {
                    current = current->parent->next->child;
                }
            }
        }
        /* change start to pointer to the first node in current depth */
        start = start->head->first;
        time_elapsed = clock() - start_time;
    } while(time_elapsed < time);
    
    bestMove = alphabeta(tree, weights, -3.4E38, 3.4E38, Max);
    return bestMove;
}   
