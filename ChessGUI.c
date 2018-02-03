#include "ChessGUI.h"

#include <stdio.h>
#include "SDL/SDL.h"

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

void  MovePiece(int selectX, int selectY, int destX, int destY, SDL_Surface *baseBoard, SDL_Surface *chessPieces, SDL_Rect pieceArray[8][8], SDL_Rect boardArray[8][8], SDL_Surface *screen, SDL_Rect empty, SDL_Surface *explosion, SDL_Rect animation[10])
{ 
    SDL_BlitSurface(baseBoard, &boardArray[selectX][selectY], screen, &boardArray[selectX][selectY]);   /* replace old square with base */  

    for (int i = 0; i < 4; i ++)
    {
        SDL_BlitSurface(explosion, &animation[i], screen, &boardArray[destX][destY]);     /* Play first 4 frames of the explosion animation */
        SDL_Flip(screen);
        SDL_Delay(1);         /* Assert 10 frames per second */
    }  

    for (int i = 4; i < 9; i ++)
    {
        SDL_BlitSurface(baseBoard, &boardArray[destX][destY], screen, &boardArray[destX][destY]);           /* paste base square */  
        SDL_BlitSurface(chessPieces, &pieceArray[selectX][selectY], screen, &boardArray[destX][destY]);     /* place piece */
        SDL_BlitSurface(explosion, &animation[i], screen, &boardArray[destX][destY]);
        SDL_Flip(screen);
        SDL_Delay(1);
    }

    SDL_BlitSurface(baseBoard, &boardArray[destX][destY], screen, &boardArray[destX][destY]);           /* paste base square */  
    SDL_BlitSurface(chessPieces, &pieceArray[selectX][selectY], screen, &boardArray[destX][destY]);     /* place piece */
                
    pieceArray[destX][destY] = pieceArray[selectX][selectY];                                            /* move piece on piece array */
    pieceArray[selectX][selectY] = empty;                                                               /* set old square to empty */
    SDL_Flip(screen);   /* update screen */
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
