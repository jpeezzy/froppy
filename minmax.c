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

/* create mini structure */
MINI *createMini(BSTATE *board, int depth)
{
    assert(board);
    MINI *mini = NULL;
    mini = malloc(sizeof(MINI));
    if(mini == NULL)
    {
        perror("Out of memory aborting...");
        exit(10);
    }
    mini->board = board;
    mini->move = createMentry(0, 0);
    mini->depth = depth;
    return mini;
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
    /* check if the child is the first node */
    if (parent->head->next == NULL)
    {
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

/* remove mini struct */
void removeMini(MINI *mini)
{
    assert(mini);
    mini->board = NULL;
    mini->move = NULL;
    free(mini);
}

/* generate a layer and a pointer to the first node in next depth */
NODE* generateLayer(NODE *parent)
{
    int length;
    MLIST* legalMLIST = NULL;
    MENTRY* currentMove = NULL; 
    NODE* currentNode = NULL;
    BSTATE *board = NULL;   
    legalMLIST = createMovelist();
    allLegal(legalMLIST, parent->board);
    parent->legal = legalMLIST;
    length = legalMLIST->movenum;
    if (length == 0)
    {
        if(parent->head->next != NULL)
        {	
            return parent->head->next->first;
	}
	else
	{
	    return NULL;
	}
    }
    currentMove = legalMLIST->First;
    board = createBstate();
    copyBstate(parent->board, board);
    mov(board->boardarray, currentMove->CLOC, currentMove->NLOC);
    if (board->sidetomove)
    {
        board->sidetomove = 0;
    }
    else
    {
        board->sidetomove = 1;
    }
    currentNode = addChild(parent, currentMove, board);

#pragma omp parallel num_threads(56)
    {
    #pragma omp for schedule(static)
        for(int i = 0; i < length; i++)
        {
            board = createBstate();
            copyBstate(parent->board, board);
            mov(board->boardarray, currentMove->CLOC, currentMove->NLOC);
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
    }   
    return parent->head->next->first;
}

/* finds the best worst value in the tree using minmax with alphabeta pruning */
float alphabeta(NODE *node, float alpha, float beta, PLAYER minmax)
{
    assert(node);
    NODE *current = NULL;
    int children;
    float temp;
    
    // if current node is a maximizer
    if(minmax == Max)
    {
        // if current node is the a node, returns its value;
        if(node->child == NULL)
        {
            node->value = basicEvaluation(node->board);
            return node->value;
        }
        node->value = -3.4E38;
        current = node->child;
        children = node->children;
         
        for(int i = 0; i < children; i++)
        {
            temp = alphabeta(current, alpha, beta, Min);
            if(temp > node->value)
            {
                node->value = temp;
                /* if node is the root(top) node, store the pointer to the moveEntry Struct of current in it */ 
                if(node->parent == NULL)
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
        
        current = NULL;
        return node->value;
    }       
    
    // if current node is a minimizer 
    else
    {
        // if current node is the a node, returns its value;
        if(node->child == NULL)
        {
            node->value = basicEvaluation(node->board);
            return node->value;
        }
        node->value = 3.4E38;
        current = node->child;
        children = node->children;
    
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
        current = NULL;
        return node->value;   
    }
}
 
/* finds the best worst move for the AI to make, returns pointer to MENTRY */
MENTRY *minmax(BSTATE *currentBoard)
{   
    assert(currentBoard);
    MENTRY *bestMove = NULL;
    NODE *tree = NULL;
    NODE *current = NULL;
	  NODE *temp = NULL;
    NODE *start = NULL;
    int length;  

    int time = 40000; 
    clock_t start_time = clock();
    clock_t time_elapsed; 
    
    /* creates first 2 depths of the tree */
    tree = createNode(NULL, currentBoard);
    current = tree;
    start = generateLayer(current);
    
    /* increases depth of tree by 1 per loop, checks for time at the end of each loop */
    do
    {
        current = start;
        length = current->head->length;
    #pragma omp parallel num_threads(56)
        { 
        #pragma omp for schedule(static)
            for(int i = 0; i < length; i++)
            {
                start = generateLayer(current);
                if (current->next)
	        {
		    current = current->next;
		}
		else
		{	        	
		    temp = current->parent->next;
		    while (temp != NULL && temp->child == NULL)					    {
		        temp = temp->next;
		    }
			current = temp->child;
		}
            }
        }
        /* not able to generate next depth */
        if(start == NULL)
        {
            break;
	}
        time_elapsed = clock() - start_time;
    } while(time_elapsed < time);
    
    alphabeta(tree, -3.4E38, 3.4E38, Max);
    bestMove = tree->move;
    removeNode(tree);
    tree = NULL;
    current = NULL;
    temp = NULL;
    start = NULL;
    return bestMove;
}   

/* maximizer function for minimax with alpha-beta pruning that uses depth */
float max(MINI *mini, float alpha, float beta, int depth) 
{
    MLIST *legal;
    MENTRY *current;
    float value;
    if (depth == 0)
    {	
	return basicEvaluation(mini->board);
    }
    legal = createMovelist();
    allLegal(legal, mini->board);
    current = legal->First;
    while (current != NULL)
    {
    	mov(mini->board->boardarray, current->CLOC, current->NLOC);
    	value = min(mini, alpha, beta, depth - 1);
    	mov(mini->board->boardarray, current->NLOC, current->CLOC);
    	if (value > alpha)
    	{	
   	    if(value >= beta)
	    {
                if(mini->depth == depth)
                {
                    mini->move->CLOC = current->CLOC;
                    mini->move->NLOC = current->NLOC;
                }
		return beta;   // fail hard beta-cutoff
	    }
	    alpha = value; // alpha acts like max in MiniMax
	}
        current = current->Next;
    }
    if(mini->depth == depth)
    {
        mini->move->CLOC = current->CLOC;
        mini->move->NLOC = current->NLOC;                
    }
    deleteMovelist(legal);
    legal = NULL;
    current = NULL;
    return alpha;
}
 
/* minimizer function for minimax with alpha-beta pruning that uses depth */ 
float min(MINI* mini, float alpha, float beta, int depth) 
{
    MLIST *legal;
    MENTRY *current;
    float value;
    if (depth == 0)
    {	
        return basicEvaluation(mini->board);
    }
    legal = createMovelist();
    allLegal(legal, mini->board);
    current = legal->First;
    while (current != NULL)
    {
        mov(mini->board->boardarray, current->CLOC, current->NLOC);
    	value = max(mini, alpha, beta, depth - 1);
    	mov(mini->board->boardarray, current->NLOC, current->CLOC);
	if (value < beta)
	{
	    if (value <= alpha)
	    {
	        return alpha; // fail hard alpha-cutoff
	    }
	    beta = value; // beta acts like min in MiniMax
	}
	current = current->Next;
    }
    deleteMovelist(legal);
    legal = NULL;
    current = NULL;
    return beta;
}
