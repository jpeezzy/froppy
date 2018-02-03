#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include "minmax.h"
#include "basic_eval.h"

/* create Node */
NODE *createNode(MENTRY *move, BSTATE *board)
{
    NODE *node = NULL;
    node = malloc(sizeof(NODE));
    if(node == NULL)
    {
        perror("Out of memory aborting...");
        exit(10);
    }   
    node->value = 0;
    node->move = move;
    node->board = board;
    node->legal = NULL;
    node->parent = NULL;
    node->child = NULL;
    node->next = NULL;
    node->children = 0;
    node->head = NULL;
    return node;
}

/* create a head structure*/
void createHead(NODE *first)
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
    head->next = NULL;
    first->head = head;
}

/* create node with float value and add it to the parent node */
NODE *addChild(NODE* parent, MENTRY *move, BSTATE *board)
{
    assert(parent);
    assert(parent->child == NULL);
    assert(move);
    assert(board);
    NODE *child = NULL;
    child = createNode(move, board);
    parent->child = child;
    parent->children = 1;
    child->parent = parent;
    /* check if the child is the first node in current depth */
    if (parent->head->next == NULL)
    {
        // creates head struct if the child is first node
        createHead(child);
	parent->head->next = child->head;
    }
    else
    {
        child->head = parent->head->next;
        child->head->length++;
    }
    return child;     
}

/* create node with float value and add it to child */
NODE *addSibling(NODE *child, MENTRY *move, BSTATE *board)
{
    assert(child);
    assert(child->next == NULL);
    assert(move);
    assert(board);
    NODE *sibling = NULL;
    sibling = createNode(move, board);
    child->next = sibling;
    sibling->parent = child->parent;
    sibling->parent->children++;
    sibling->head = child->head;
    sibling->head->length++;
    return sibling;
}

/* remove node along with its child and siblings recursively */
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
    if(node->board)
    {
        deleteBstate(node->board);
        node->board = NULL;
    }
    if(node->legal)
    {
        deleteMovelist(node->legal);
        node->legal = NULL;
    }
    node->move = NULL;
    node->parent = NULL;
    node->child = NULL;
    node->next = NULL;
    node->head = NULL;
    free(node);
}

/* remove the head struct */
void removeHead(HEAD *head)
{
    assert(head);
    head->first = NULL;
    head->next = NULL;
    free(head);   
}

/* generate a layer and a pointer to the first node in next depth */
NODE* generateLayer(NODE *parent)
{
    assert(parent);
    assert(parent->board);
    int length;
    MLIST* legalMLIST = NULL;
    MENTRY* currentMove = NULL; 
    NODE* currentNode = NULL;
    BSTATE *board = NULL;   
    legalMLIST = createMovelist();
    allLegal(legalMLIST, parent->board);
    parent->legal = legalMLIST;
    length = legalMLIST->movenum;
    // condition for which the input parent node has no legal moves 
    if (length == 0)
    {
        // return the pointer to the first node in next depth if it exits
        if(parent->head->next != NULL)
        {	
            return parent->head->next->first;
	}
        // else return NULL
	else
	{
	    return NULL;
	}
    }
    currentMove = legalMLIST->First;
    board = createBstate();
    copyBstate(parent->board, board);
    mov(board->boardarray, currentMove->CLOC, currentMove->NLOC);
    // switch sides
    if (board->sidetomove)
    {
        board->sidetomove = 0;
    }
    else
    {
        board->sidetomove = 1;
    }
    // creates the first child node
    currentNode = addChild(parent, currentMove, board);
    
    // loop that creates the remaining children nodes 
    while(currentMove != NULL)
    {
        board = createBstate();
        copyBstate(parent->board, board);
        mov(board->boardarray, currentMove->CLOC, currentMove->NLOC);
        //switch sides
        if (board->sidetomove)
        {
            board->sidetomove = 0;
        }
        else
        {
            board->sidetomove = 1;
        }
        currentNode = addSibling(currentNode, currentMove, board);  
        currentMove = currentMove->Next;       
    }
    // return pointer to first node in the depth containing children   
    return parent->head->next->first;
}
 
/* finds the best worst move for the AI to make, returns pointer to MENTRY */
MENTRY *minmax(BSTATE *currentBoard)
{   
    assert(currentBoard);
    BSTATE *cpyBoard;
    MENTRY *bestMove = NULL;
    NODE *tree = NULL;
    NODE *current = NULL;
    NODE *temp = NULL;
    NODE *start = NULL;
    int length;  

    // initialize timer
    int time = 60000; 
    clock_t start_time = clock();
    clock_t time_elapsed; 
    
    // copies current board to avoid changing it  
    cpyBoard = createBstate();
    copyBstate(currentBoard, cpyBoard);
    // creates the root of tree
    tree = createNode(NULL, cpyBoard);
    createHead(tree);
    // creates the first depth of tree
    start = generateLayer(tree);
    // condition for which first depth cannot be created i.e input board has no legal moves
    if(start == NULL)
    {
        return NULL;
    }
 
    /* increases depth of tree by 1 per loop, checks for time at the end of each loop */
    do
    {
        current = start;
        // this length can be used to change the while loop below to "for loop" for openmp
        /* length = current->head->length; */
        while(current)
        {
            start = generateLayer(current);
            if (current->next)
	    {
		// moves on to next sibling node
                current = current->next;
	    }
	    else
	    {	
                // condition to move to the child of parent's next sibling node       	
		temp = current->parent->next;
		// find parent's next sibling that has a child 
                while (temp != NULL && temp->child == NULL)
                {
		    temp = temp->next;
		}
                if (temp != NULL)
                {
                    assert(temp->child);
		    current = temp->child;
	        }
                else
                {
                    current = temp;
                }
            }
        }
        
        /* not able to generate next depth */
        if(start == NULL)
        {
            break;
	}
        time_elapsed = clock() - start_time;
        printf("time elapsed = %Lf\n", (long double)time_elapsed);
    } while(time_elapsed < time); 
    float score = max(tree, -3.4E38, 3.4E38);
    printf("score = %f\n", score);
    bestMove = tree->move;
    removeNode(tree);
    tree = NULL;
    current = NULL;
    temp = NULL;
    start = NULL;
    return bestMove;
}   


/* maximizer function for minimax with alpha-beta pruning */
float max(NODE* node, float alpha, float beta) 
{
    assert(node);
    NODE *current;
    NODE *temp;
    float value;
    // if node is leaf node, evaluate the board
    if (node->child == NULL)
    {	
        value = basicEvaluation(node->board);
        return value;
    }
    current = node->child;
    // loop through the children nodes
    while (current != NULL)
    {
    	value = min(current, alpha, beta);
    	if (value > alpha)
    	{	
   	    if(value >= beta)
	    {
                // check if node is root node, then stores the best move
                if(node->parent == NULL)
                {
                    if(node->move)
                    {
                        free(node->move);
                        node->move = NULL;
                    }
                    assert(current->move);
                    node->move = createMentry(current->move->CLOC, current->move->NLOC);       
                }
		return beta;   // fail hard beta-cutoff
	    }
            // temp stores the pointer to potential best move
	    temp = current;
            alpha = value; // alpha acts like max in MiniMax
        }
        current = current->next;
    }
    // check if node is root node, then stores the best move
    if(node->parent == NULL)
    {
         if(node->move)
         {
            free(node->move);
            node->move = NULL;
         }
         assert(temp->move);
         node->move = createMentry(temp->move->CLOC, temp->move->NLOC);   
    }
    current = NULL;
    return alpha;
}
 
/* minimizer function for minimax with alpha-beta pruning that uses depth */
float min(NODE *node, float alpha, float beta) 
{
    NODE *current;
    float value;
    if (node->child == NULL)
    {
        value = basicEvaluation(node->board);	
        return value;
    }
    current = node->child;
    // loop through the children nodes
    while (current != NULL)
    {
    	value = max(current, alpha, beta);
	if (value < beta)
	{
	    if (value <= alpha)
	    {
	        return alpha; // fail hard alpha-cutoff
	    }
	    beta = value; // beta acts like min in MiniMax
	}
	current = current->next;
    }
    current = NULL;
    return beta;
}


