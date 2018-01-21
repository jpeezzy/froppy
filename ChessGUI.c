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
}
    
void Exit(SDL_Surface *screen)
{
    SDL_FreeSurface(screen);
    SDL_Quit();
}
