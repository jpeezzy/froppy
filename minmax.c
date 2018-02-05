#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <unistd.h>
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
    node->cousin = NULL;
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
    head->last = first;
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
        // cousin pointer of last node in current depth points to child
        assert(parent->head->next->last->next == NULL);
        parent->head->next->last->cousin = child;
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
    // last pointer of head points to sibling node
    child->head->last = sibling;
    sibling->head->length++;
    return sibling;
}

/* remove node along with its child and siblings recursively */
void removeNode(NODE *node)
{
    assert(node);
    if(node->head)
    {
        if(node->head->first == node)
        {
            removeHead(node->head);
            node->head = NULL;
        }
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
    head->last = NULL;
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
    currentMove = currentMove->Next;
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
    BSTATE *cpyBoard = NULL;
    MENTRY *bestMove = NULL;
    NODE *tree = NULL; 
    float score;

    // copies current board to avoid changing it  
    cpyBoard = createBstate();
    copyBstate(currentBoard, cpyBoard);
    // creates the root of tree
    tree = createNode(NULL, cpyBoard);
    createHead(tree);
    /* creates the first depth of tree */
    MLIST *legal = NULL;
    MENTRY *currentMove = NULL;
    NODE *currentNode = NULL;
    // find legal moves
    legal = createMovelist();
    allLegal(legal, cpyBoard);        
    assert(legal->movenum);
    tree->legal = legal;
    currentMove = legal->First;
    // create new board, copy and move
    cpyBoard = createBstate();
    copyBstate(currentBoard, cpyBoard);
    mov(cpyBoard->boardarray, currentMove->CLOC, currentMove->NLOC);
    // switch sides
    if (cpyBoard->sidetomove)
    {
        cpyBoard->sidetomove = 0;
    }
    else
    {
        cpyBoard->sidetomove = 1;
    }
    currentNode = addChild(tree, currentMove, cpyBoard); 
    currentMove = currentMove->Next;
    while(currentMove != NULL)
    {
        // create new board, copy and move
        cpyBoard = createBstate();
        copyBstate(currentBoard, cpyBoard);
        mov(cpyBoard->boardarray, currentMove->CLOC, currentMove->NLOC);
        // switch sides
        if (cpyBoard->sidetomove)
        {
            cpyBoard->sidetomove = 0;
        }
        else
        {
            cpyBoard->sidetomove = 1;
        }
        currentNode = addSibling(currentNode, currentMove, cpyBoard);
        // this is done to make each node in 1st depth act like root of their own tree for parallelization
        createHead(currentNode);
        currentMove = currentMove->Next;
    }
    /* finished creating first depth */
    
    
    // initialize timer
    time_t endwait;
    time_t start_time = time(NULL);
    time_t seconds = 3;
    endwait = start_time + seconds;
    int threads = omp_get_max_threads();
    if(tree->legal->movenum > threads)
    {
        endwait = endwait / 2.0;
    }
    else if(tree->legal->movenum > threads * 2)
    {
        endwait = endwait / 3.0;
    }



/* start parallelization */
currentNode = tree->child;

#pragma omp parallel 
    {
    #pragma omp for schedule(dynamic, 1)
        for(int i = 0; i < tree->legal->movenum; i++)
        {
            NODE *current;
            NODE *start;
            time_t time_elapsed = time(NULL);
            current = currentNode;
            for(int j = 0; j < i; j++)
            {
                current = current->next;
            }
            assert(current);
            start = generateLayer(current);
            // current has no legal moves
            if (start != NULL)
            {
                do
                {
                    current = start;
                    assert(current->child == NULL);
                    while(current)
                    {
                        time_elapsed = time(NULL);
                        if(time_elapsed < endwait)
                        {
                            break;
                        }
                        start = generateLayer(current);
                        time_elapsed = time(NULL);
                        if(time_elapsed < endwait)
                        {
                            break;
                        }
                        if (current->next)
                        {
                            // moves onto next sibling node
                            current = current->next;
                        }
                        else
                        {
                            assert(current->next == NULL);
                            current = current->cousin;
                        }
                    }
                    /* not able to generate next depth */
                    if (start == NULL)
                    {
                        break;
                    }
                    time_elapsed = time(NULL);
                }while (time_elapsed < endwait);
            }       
        }
    }
    // if AI is black
    if (currentBoard->sidetomove)
    {
        score = min(tree, -3.4E38, 3.4E38);
    }
    else // if AI is white
    {
        score = max(tree, -3.4E38, 3.4E38);
    }
    printf("score = %f\n", score);
    bestMove = tree->move;
    removeNode(tree);
    cpyBoard = NULL;
    tree = NULL;
    currentMove = NULL;
    currentNode = NULL;
    legal = NULL;
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
                    assert(current->move);
                    if(node->move)
                    {
                        node->move->CLOC = current->move->CLOC;
                        node->move->NLOC = current->move->NLOC;
                    }
                    else
                    {
                        node->move = createMentry(current->move->CLOC, current->move->NLOC);       
                    }
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
         assert(temp->move);
         if(node->move)
         {
            node->move->CLOC = temp->move->CLOC;
            node->move->NLOC = temp->move->NLOC;
         }
         else
         {            
            node->move = createMentry(temp->move->CLOC, temp->move->NLOC);    
         }
    }
    current = NULL;
    return alpha;
}
 
/* minimizer function for minimax with alpha-beta pruning that uses depth */
float min(NODE *node, float alpha, float beta) 
{
    assert(node);
    NODE *current;
    NODE *temp;
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
                // check if node is root node, then stores best move
                if (node->parent == NULL)
                {    
                    assert(current->move);
                    // if MENTRY is already created for node
                    if (node->move)
                    {
                        node->move->CLOC = current->move->CLOC;   
                        node->move->NLOC = current->move->NLOC;
                    }
                    else
                    {
                        node->move = createMentry(current->move->CLOC, current->move->NLOC);
                    }
                }
                return alpha; // fail hard alpha-cutoff
	    }
            // temp stores the pointer to potential best move
            temp = current;
	    beta = value; // beta acts like min in MiniMax
	}
	current = current->next;
    }
    // check if node is root node, then stores best move
    if (node->parent == NULL)
    {    
        assert(temp->move);
        // if MENTRY is already created for node
        if (node->move)
        {
            node->move->CLOC = temp->move->CLOC;   
            node->move->NLOC = temp->move->NLOC;
        }
        else
        {
            node->move = createMentry(temp->move->CLOC, temp->move->NLOC);
        }
    }
    current = NULL;
    return beta;
}

/* alternative way of implementing minmax using depth, mov and undo mov */
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

/* remove mini struct */
void removeMini(MINI *mini)
{
    assert(mini);
    mini->board = NULL;
    if(mini->move)
    {
        free(mini->move);
    }
    mini->move = NULL;
    free(mini);
}

/* maximizer function for minimax with alpha-beta pruning that uses depth */
float altMax(MINI *mini, float alpha, float beta, int depth) 
{
    assert(mini);
    MLIST *legal;
    MENTRY *current;
    MENTRY *temp;
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
    	value = altMin(mini, alpha, beta, depth - 1);
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
                deleteMovelist(legal);
		return beta;   // fail hard beta-cutoff
	    }
            temp = current;
	    alpha = value; // alpha acts like max in MiniMax
	}
        current = current->Next;
    }
    if(mini->depth == depth)
    {
        mini->move->CLOC = temp->CLOC;
        mini->move->NLOC = temp->NLOC;                
    }
    assert(legal);
    deleteMovelist(legal);
    legal = NULL;
    current = NULL;
    return alpha;
}
 
/* minimizer function for minimax with alpha-beta pruning that uses depth */ 
float altMin(MINI* mini, float alpha, float beta, int depth) 
{
    assert(mini);
    MLIST *legal;
    MENTRY *current;
    MENTRY *temp;
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
    	value = altMax(mini, alpha, beta, depth - 1);
    	mov(mini->board->boardarray, current->NLOC, current->CLOC);
	if (value < beta)
	{
	    if (value <= alpha)
	    {
	        if(mini->depth == depth)
                {
                    mini->move->CLOC = current->CLOC;
                    mini->move->NLOC = current->NLOC;
                }   
                deleteMovelist(legal);
                return alpha; // fail hard alpha-cutoff
	    }
	    temp = current;
            beta = value; // beta acts like min in MiniMax
	}
	current = current->Next;
    }
    if(mini->depth == depth)
    {
        mini->move->CLOC = temp->CLOC;
        mini->move->NLOC = temp->NLOC;
    }
    assert(legal);
    deleteMovelist(legal);
    legal = NULL;
    current = NULL;
    return beta;
}
