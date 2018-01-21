#include "ChessGUI.h" 
#include <stdio.h>


int main(int argc, char *args[])
{
    SDL_Surface *screen = NULL;
    
    SDL_Init(SDL_INIT_VIDEO);   /* Initializes SDL Environment */

    screen = SDL_SetVideoMode(WIDTH, HEIGHT, BPP, SDL_SWSURFACE); /* Inputs settings for screen */
    DisplayWindow("TempMenu.bmp", screen, 1000);    


/*    SDL_Surface *image = NULL;
    image = SDL_LoadBMP("button.bmp");
    if ( image == NULL)
    {
        printf("Button didn't load \n");
        Exit(image);
        return 0;
    }

    Add_Box(image, screen, 100, 100); 
    SDL_Flip(screen);
    SDL_Delay(1000);
*/
/** Start of user interaction  **********************************************************/
    SDL_Event event; 
    int quit = 0;   /* Flag for ending menu while loop */

    while (quit != 1)
    {
        SDL_Flip(screen);   /* keeping window open while waiting for input */ 
        SDL_Delay(100);     

        while(SDL_PollEvent(&event))    /* Starting poll for Menu */
        {
            switch(event.type)
            {
                case SDL_MOUSEBUTTONDOWN: 
                    printf("Coordinates of your click: %d %d \n", event.motion.x, event.motion.y);
                    if (event.motion.x >= 120 && event.motion.x <= 470)
                    { 
                        if ( event.motion.y >= 400 && event.motion.y >= 477)    /* Quit button */
                        {
                            printf("You chose to quit! \n");
                            Exit(screen);
                            quit = 1;
                        }
                        else if (event.motion.y >= 263 && event.motion.y <= 350) /* Play button */
                        {
                            printf("You chose to play! \n");
                            Exit(screen);
                            quit = 1;
                        }
                    } 
                    break;
            }
        }    
    }   

 
//    SDL_FreeSurface(image);  
    return 0;
}

