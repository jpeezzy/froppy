#include "ChessGUI.h"

#include <stdio.h>
#include <assert.h>
#include "SDL/SDL.h"
#include "movelist.h"
#include "boardstate.h"
#include "minmax.h"

void DisplayWindow(char *file, SDL_Surface *screen, int delay)
{
    /* Creating a temporary surface */
    SDL_Surface *image = NULL;
    
    /* Loading image to temporary surface */
    image = SDL_LoadBMP(file);
    if (image == NULL)
    {
        printf("Image did not load successfully \n");
    }
    
    /* Copying image to displayable screen */
    SDL_BlitSurface(image, NULL, screen, NULL);
    
    /* Updating screen */
    SDL_Flip(screen);
    
    /* Determines how long image stays up for */
    SDL_Delay(delay);
    
    
    /* Deleting temporary surface */
    SDL_FreeSurface(image);
}


void Add_Box(SDL_Surface *image, SDL_Surface *screen, int x, int y)
{
    /* Coordinates for where the box will go on the screen */
    SDL_Rect coordinates;   
    coordinates.x = x;
    coordinates.y = y;

    /* Adding box to screen */
    SDL_BlitSurface(image, NULL, screen, &coordinates);
}
    
void Add_BoxFile(char *file, SDL_Surface *screen, int x, int y)     
{
    SDL_Surface *image = NULL;
    
    image = SDL_LoadBMP(file);

    /* Coordinates for where the box will go on the screen */
    SDL_Rect coordinates;
    coordinates.x = x;
    coordinates.y = y;

    /* Adding box to the screen */
    SDL_BlitSurface(image, NULL, screen, &coordinates);

    SDL_FreeSurface(image);
}

void CreateBoard(SDL_Surface *baseBoard, SDL_Surface *tileBoard, SDL_Surface *screen)
{
    Add_Box(baseBoard, screen, 0, 0);   /* Make whole screen white */
    
    SDL_Rect coordinates;   /* Declaring and initializing rectangle */
    coordinates.x = 0;
    coordinates.y = 0;
    coordinates.w = 80;
    coordinates.h = 80;

    for (int i = 0; i < 4; i++)     /* Pasting green squares over white board */
    {   for (int j = 0; j < 4; j++)
        { 
    
            coordinates.x = 160*i;
            coordinates.y = + 160*j;
            SDL_BlitSurface(tileBoard, &coordinates, screen, &coordinates);        
        }
    }
    
    coordinates.x = 0;
    coordinates.y = 0;
    
    for (int i = 0; i < 4; i++)     /* Pasting green squares over white board again */
    {   for (int j = 0; j < 4; j++)
        {
            coordinates.x = 80 + 160*i;
            coordinates.y = 80 + 160*j;
            SDL_BlitSurface(tileBoard, &coordinates, screen, &coordinates);
        }
    }
}

SDL_Surface *BaseBoardCopy(SDL_Surface *screen, SDL_Rect boardArray[8][8])
{
    SDL_Surface *copy = NULL;
    copy = SDL_DisplayFormat(screen); 
    for (int i = 0; i < 8; i ++)
    {   for (int j = 0; j < 8; j++)
        {
            SDL_BlitSurface(screen, &boardArray[i][j], copy, &boardArray[i][j]);
        }
    }
    return copy;
}

void Move(SDL_Surface *piece, SDL_Rect sprite, SDL_Surface *screen, SDL_Rect rect)
{
    SDL_BlitSurface(piece, &sprite, screen, &rect);
} 

void  MovePiece(int selectX, int selectY, int destX, int destY, SDL_Surface *baseBoard, SDL_Surface *chessPieces, SDL_Rect pieceArray[8][8], SDL_Rect boardArray[8][8], SDL_Surface *screen, SDL_Rect empty, SDL_Surface *effect, SDL_Rect animation[10], int effectNumber, int lastMove[4])
{ 
    SDL_BlitSurface(baseBoard, &boardArray[selectX][selectY], screen, &boardArray[selectX][selectY]);   /* replace old square with base */  
#if animations == 1    
    if (pieceArray[destX][destY].h != 1)   /* If destination square is not empty */
    {
        if ( effectNumber == 1) /* Explosion */
        {
            for (int i = 0; i < 4; i ++)    /* Play first 4 frames of the explosion animation */
            {
                SDL_BlitSurface(effect, &animation[i], screen, &boardArray[destX][destY]);    
                SDL_Flip(screen);
            }  

            for (int i = 4; i < 9; i ++)    /* Play last 5 frames of explosion animation */
            {
                SDL_BlitSurface(baseBoard, &boardArray[destX][destY], screen, &boardArray[destX][destY]);           /* paste base square */  
                SDL_BlitSurface(chessPieces, &pieceArray[selectX][selectY], screen, &boardArray[destX][destY]);     /* place piece */
                SDL_BlitSurface(effect, &animation[i], screen, &boardArray[destX][destY]);
                SDL_Flip(screen);
            }
            SDL_BlitSurface(baseBoard, &boardArray[destX][destY], screen, &boardArray[destX][destY]);           /* paste base square */  
            SDL_BlitSurface(chessPieces, &pieceArray[selectX][selectY], screen, &boardArray[destX][destY]);     /* place piece */
            SDL_Flip(screen);
        }
        else if (effectNumber == 2) /* Lightning */
        {
            for (int i = 0; i < 6; i ++)
            {
                SDL_BlitSurface(baseBoard, &boardArray[destX][destY], screen, &boardArray[destX][destY]);   /* paste base square */
                SDL_BlitSurface(chessPieces, &pieceArray[destX][destY], screen, &boardArray[destX][destY]); /* re-place old piece */
                SDL_BlitSurface(effect, &animation[i], screen, &boardArray[destX][destY]);                   /* draw animation on top */
                SDL_Flip(screen);
            }
            for (int i = 6; i < 10; i++)
            {
                SDL_BlitSurface(baseBoard, &boardArray[destX][destY], screen, &boardArray[destX][destY]);       /* paste base square */
                SDL_BlitSurface(chessPieces, &pieceArray[selectX][selectY], screen, &boardArray[destX][destY]); /* place new piece */
                SDL_BlitSurface(effect, &animation[i], screen, &boardArray[destX][destY]);                       /* draw animation on top */
                SDL_Flip(screen);
            }               
            SDL_BlitSurface(baseBoard, &boardArray[destX][destY], screen, &boardArray[destX][destY]);           /* paste base square */  
            SDL_BlitSurface(chessPieces, &pieceArray[selectX][selectY], screen, &boardArray[destX][destY]);     /* place piece */ 
            SDL_Flip(screen);
        }   
    }


    else if (pieceArray[destX][destY].h == 1)  /* If destination square is empty */
    {
        SDL_BlitSurface(baseBoard, &boardArray[destX][destY], screen, &boardArray[destX][destY]);           /* paste base square */  
        SDL_BlitSurface(chessPieces, &pieceArray[selectX][selectY], screen, &boardArray[destX][destY]);     /* place piece */
    } 

#endif 

    SDL_BlitSurface(baseBoard, &boardArray[destX][destY], screen, &boardArray[destX][destY]);           /* paste base square */  
    SDL_BlitSurface(chessPieces, &pieceArray[selectX][selectY], screen, &boardArray[destX][destY]);     /* place piece */

    lastMove[0] = selectX;  
    lastMove[1] = selectY;
    lastMove[2] = destX;
    lastMove[3] = destY;
    
         
    pieceArray[destX][destY] = pieceArray[selectX][selectY];                                            /* move piece on piece array */
    pieceArray[selectX][selectY] = empty;                                                               /* set old square to empty */
    SDL_Flip(screen);   /* update screen */
}


void PrintBoard(BSTATE *board, SDL_Surface *baseBoard, SDL_Rect pieces[17], SDL_Surface *chessPieces, SDL_Surface *screen, SDL_Rect boardArray[8][8]) 
{
    for (int i = 0; i < 8; i++)
    {   for (int j = 0; j < 8; j++)
        {
            if (board->boardarray[i][j] != 0)   /* if position is not empty, print the piece */
            {
                SDL_BlitSurface(baseBoard, &boardArray[j][i], screen, &boardArray[j][i]);   /* paste base square */
                SDL_BlitSurface(chessPieces, &pieces[board->boardarray[i][j]], screen, &boardArray[j][i]);  /* paste piece */
            }
            else 
            {
                SDL_BlitSurface(baseBoard, &boardArray[j][i], screen, &boardArray[j][i]);   /* paste base square */
            }
        }
    }

    SDL_Flip(screen);
}		

/****************** MiniMax Functions ***************************************/

int playerMove(BSTATE *board, int selectX, int selectY, int destX, int destY)
{
	assert(board);
	int iCloc, iNloc;		
//  char cloc[2], nloc[2];
	int legal = 0;
/*	do
	{
	printf("Enter location of piece to move: ");
	scanf("%s", cloc);
	printf("Enter destination location: ");
	scanf("%s", nloc);
	iCloc = strToInt(cloc);
	iNloc = strToInt(nloc);	
*/

	iCloc = selectX + selectY*8;		 /* Converting click coordinates to board coordinates */	
	iNloc = destX +   destY*8;		 /*													 */
     		
	legal = checkLegal(board, iCloc, iNloc); /* Checking if it is a legal move */
	if(!legal)
	{
		printf("The move entered is not legal, try again.\n");	/* If not legal, return fail */
		return 0;
	}
//	}while(!legal);
	mov(board->boardarray, iCloc, iNloc);	/* Move piece */
	changeSide(board);						/* Change turn */
	return 1;								/* Return success */
}

void changeSide(BSTATE *board)
{
    assert(board);
    if(board->sidetomove)
    {
        board->sidetomove = 0;   /* switches from black to white */
        printf("It is now WHITE side's turn! \n");        
	} 
    else
    {   
        board->sidetomove = 1;   /* switches from white to black */
        printf("It is now BLACK side's turn! \n");
    }
}

int checkLegal(BSTATE *board, int cloc, int nloc)
{
	
    MLIST *legal;
    MENTRY *current;
    legal = createMovelist();
    allLegal(legal, board);     /* fills list with all legal moves */
    assert(legal->First);
    current = legal->First;

    while(current)
    {
        if(current->CLOC == cloc && current->NLOC == nloc)  /* if the inputted move matches a legal move on the list */
        {
            deleteMovelist(legal);                          
            return 1;   /* give the green light to complete the move */
        }
        current = current->Next;
    }
    deleteMovelist(legal);
    return 0;   /* if not legal, return Fail and prevent the move */
}

void aiMove(BSTATE *board, int AIMove[2])
{
    assert(board);
   
    MENTRY *move;
    move = minmax(board);
    assert(move);
    int legal = checkLegal(board, move->CLOC, move->NLOC);
    if(!legal)
    {
        printf("AI made invalid move\n");
    }
    mov(board->boardarray, move->CLOC, move->NLOC); 
    changeSide(board);  

    assert(move);
    assert(move->CLOC);

    AIMove[0] = move->CLOC;
    AIMove[1] = move->NLOC;
    free(move);
    move = NULL; 
 
}

void UpdateWindow(SDL_Surface *screen, int delay)
{
    SDL_Flip(screen);
    SDL_Delay(delay);
}

void Exit(SDL_Surface *screen)
{
    SDL_FreeSurface(screen);
    SDL_Quit();
}
