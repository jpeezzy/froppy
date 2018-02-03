#include "ChessGUI.h" 
#include <stdio.h>


int main(int argc, char *args[])
{
    SDL_Surface *screen = NULL;     /* Main surface to be displayed */
    SDL_Surface *baseBoard = NULL;  /* Surface that will be the reference chess board */
 

    SDL_Init(SDL_INIT_VIDEO);   /* Initializes SDL Environment */

    screen = SDL_SetVideoMode(WIDTH, HEIGHT, BPP, SDL_SWSURFACE); /* Inputs settings for screen */
    DisplayWindow("Intro.bmp", screen, 1000);    

    SDL_WM_SetCaption("Froppy Chess Game", NULL);   /* Giving the window a title */

    /* Surface initializations and assignments */
    SDL_Surface *greenBoard = NULL;      /* Making surface for green part of board */
    greenBoard = SDL_LoadBMP("Green.bmp");
    if ( greenBoard == NULL)    /* Checking if it loaded */
    {
        printf("Green Board didn't load \n");
        Exit(greenBoard);
        return 0;
    }
    
    SDL_Surface *whiteBoard = NULL;     /* Making surface for white part of board */
    whiteBoard = SDL_LoadBMP("White.bmp");
    if ( whiteBoard == NULL)    /* Checking if it loaded */
    {
        printf("White Board didn't load \n");
        Exit(whiteBoard);
        return 0;
    }

    SDL_Surface *chessPieces = NULL;         /* Making surface for chess pieces */
    chessPieces = SDL_LoadBMP("ChessPiecesRedraw.bmp");
    Uint32 color = SDL_MapRGB(chessPieces->format, 0xFF,0xFF,0xFF); /* Filtering out the unwanted background color */
    SDL_SetColorKey(chessPieces, SDL_SRCCOLORKEY, color);    
    
    SDL_Surface *highlights = NULL;         /* Making surface for square highlight */
    highlights = SDL_LoadBMP("SquareHighlight.bmp");
    color = SDL_MapRGB(highlights->format, 0xFF, 0xFF, 0xFF);/* Filtering out all white color */
    SDL_SetColorKey(highlights, SDL_SRCCOLORKEY, color);
    /* Setting rectangles for highlight outlines */
    SDL_Rect yellow;  /* yellow outline definitions */  
    yellow.x = 0;
    yellow.y = 0;
    yellow.w = 80;
    yellow.h = 80;
    SDL_Rect red;     /* red outline definitons */
    red.x = 80; 
    red.y = 0;
    red.w = 80;
    red.h = 80;

    
    /* Creating array of SDL_Rects to keep track of chess board. 
              Each SDL_Rect correlates to one square on the board      */
    SDL_Rect boardArray[8][8];
    for (int i = 0; i < 8; i ++)
    {   for (int j = 0; j <8; j++)
        {
            boardArray[i][j].x = i*80;
            boardArray[i][j].y = j*80;
            boardArray[i][j].w = 80;
            boardArray[i][j].h = 80;
        }
    }

    /* Creating array for piece positions */
    SDL_Rect pieceArray[8][8];
    SDL_Rect empty; /* placeholder variable to denote an empty square */
        empty.x = 0; empty.y = 0; empty.w = 1; empty.h = 1; 
  
    for (int i = 0; i < 8; i++)
    {   for (int j = 0; j < 8; j++)
        {
            pieceArray[i][j] = empty;
        }
    }
     
    /******** Creating SDL_Rects for each individual piece from the sprite sheet ************/
    /* Pieces:      King         Queen        Rook        Bishop       Horse        Pawn    */
    /* Black: */ SDL_Rect bK; SDL_Rect bQ; SDL_Rect bR; SDL_Rect bB; SDL_Rect bH; SDL_Rect bP;
    /* White: */ SDL_Rect wK; SDL_Rect wQ; SDL_Rect wR; SDL_Rect wB; SDL_Rect wH; SDL_Rect wP;
    /****************************************************************************************/
                 bK.x = 0;    bQ.x = 80;   bR.x = 160;  bB.x = 240;  bH.x = 320;  bP.x = 400;    
                 bK.y = 0;    bQ.y = 0;    bR.y = 0;    bB.y = 0;    bH.y = 0;    bP.y = 0;
                 bK.w = 80;   bQ.w = 80;   bR.w = 80;   bB.w = 80;   bH.w = 80;   bP.w = 80;
                 bK.h = 80;   bQ.h = 80;   bR.h = 80;   bB.h = 80;   bH.h = 80;   bP.h = 80;

                 wK.x = 0;    wQ.x = 80;   wR.x = 160;  wB.x = 240;  wH.x = 320;  wP.x = 400;
                 wK.y = 80;   wQ.y = 80;   wR.y = 80;   wB.y = 80;   wH.y = 80;   wP.y = 80;
                 wK.w = 80;   wQ.w = 80;   wR.w = 80;   wB.w = 80;   wH.w = 80;   wP.w = 80;
                 wK.h = 80;   wQ.h = 80;   wR.h = 80;   wB.h = 80;   wH.h = 80;   wP.h = 80;
    /****************************************************************************************/


/** Start of user interaction loop  **********************************************************/
    SDL_Event introEvent;
    SDL_Event event; 
   
 
    int quit = 0;       /* Flag for ending menu while loop */
    while (quit != 1)    /* Intro screen loop */
    { 
        while (SDL_PollEvent(&introEvent))
        {
            if (introEvent.type == SDL_MOUSEBUTTONDOWN)  
            {
                Add_BoxFile("Menu.bmp", screen, 0,0);   /* change to menu screen */
                SDL_Flip(screen);
printf("intro click in \n");
                quit = 1;  
                break;
            }
        }
    }         

    quit = 0; /* resetting quit flag for next loop */    

    while (quit != 1)
    {
        while(SDL_PollEvent(&event))    /* Starting poll for Menu */
        {
            switch(event.type)
            {
                case SDL_MOUSEBUTTONDOWN: 
                    printf("Coordinates of your click: %d %d \n", event.motion.x, event.motion.y);
                    if (event.motion.x >= 176 && event.motion.x <= 468)
                    { 
                        if ( event.motion.y >= 475 && event.motion.y <= 578)    /* Quit button */
                        {
                            Add_BoxFile("MenuQuit.bmp", screen, 0,0);
                            SDL_Flip(screen);
                            break;
                        }
                        else if (event.motion.y >= 346 && event.motion.y <= 449) /* Play button */
                        {
                            Add_BoxFile("MenuPlay.bmp", screen, 0,0);
                            SDL_Flip(screen);
                            break;
                        }
                    }
                    else 
                    {
                        Add_BoxFile("Menu.bmp", screen, 0,0);
                        SDL_Flip(screen);
                    } 
    
 
                case SDL_MOUSEBUTTONUP:
                    if (event.motion.x >= 176 && event.motion.x <= 468)
                    { 
                        if ( event.motion.y >= 475 && event.motion.y <= 578)    /* Quit button */
                        {
                            printf("You chose to quit! \n");
                            Exit(screen);
                            return 0;
                        }
                    
                        else if (event.motion.y >= 346 && event.motion.y <= 449)     /* Play button */    
                        {    
                            printf("You chose to play! \n");
                            CreateBoard(greenBoard, whiteBoard, screen);    /* Making board graphics */ 
        
                            baseBoard = SDL_DisplayFormat(screen);          /* Copying board for reference */ 
                            SDL_BlitSurface(screen, NULL, baseBoard, NULL); 
                        
                    

                    
                        /* Putting initial pieces on the board */
                 	          /* Pawns */
                 	    for (int i = 0; i < 8; i++) 
                	    { 
                 	        SDL_BlitSurface(chessPieces, &bP, screen, &boardArray[i][1]); 
                                pieceArray[i][1] = bP; 
                	        SDL_BlitSurface(chessPieces, &wP, screen, &boardArray[i][6]);
                                pieceArray[i][6] = wP;
                 	    }
        
	                         /* Rooks */
        	            SDL_BlitSurface(chessPieces, &bR, screen, &boardArray[0][0]);
                            pieceArray[0][0] = bR;   
        	            SDL_BlitSurface(chessPieces, &bR, screen, &boardArray[7][0]);
                            pieceArray[7][0] = bR;
               	        SDL_BlitSurface(chessPieces, &wR, screen, &boardArray[0][7]);
                            pieceArray[0][7] = wR;
               		    SDL_BlitSurface(chessPieces, &wR, screen, &boardArray[7][7]);
                            pieceArray[7][7] = wR;

			            	/* Knights */
               	 	    SDL_BlitSurface(chessPieces, &bH, screen, &boardArray[1][0]);
                            pieceArray[1][0] = bH;
               	        SDL_BlitSurface(chessPieces, &bH, screen, &boardArray[6][0]);
                 	        pieceArray[6][0] = bH;
                        SDL_BlitSurface(chessPieces, &wH, screen, &boardArray[1][7]);
                	        pieceArray[1][7] = wH;
                        SDL_BlitSurface(chessPieces, &wH, screen, &boardArray[6][7]);
                            pieceArray[6][7] = wH;                       
         
		      	    	    /* Bishops */
             		    SDL_BlitSurface(chessPieces, &bB, screen, &boardArray[2][0]);
              	            pieceArray[2][0] = bB;
                        SDL_BlitSurface(chessPieces, &bB, screen, &boardArray[5][0]);
             	            pieceArray[5][0] = bB;
                        SDL_BlitSurface(chessPieces, &wB, screen, &boardArray[2][7]);
               	            pieceArray[2][7] = wB;
                        SDL_BlitSurface(chessPieces, &wB, screen, &boardArray[5][7]);
                            pieceArray[5][7] = wB;	

                    		/* Queens */
                   	    SDL_BlitSurface(chessPieces, &bQ, screen, &boardArray[3][0]);
                            pieceArray[3][0] = bQ;
                   	    SDL_BlitSurface(chessPieces, &wQ, screen, &boardArray[3][7]);
                            pieceArray[3][7] = wQ;

				            /* Kings */
                   	    SDL_BlitSurface(chessPieces, &bK, screen, &boardArray[4][0]);
                            pieceArray[4][0] = bK;
                   	    SDL_BlitSurface(chessPieces, &wK, screen, &boardArray[4][7]);
                            pieceArray[4][7] = wK;

                            quit = 1;
                     	} /* end elseif */
                    }
                    break;
           
                 case SDL_QUIT:     /* Handles if user presses "x" button on window */
                    printf("You exited out of the window. \n");
                    Exit(screen);
                    quit = 1;
                    break;
            }
        }    
    }   

    SDL_Flip(screen);

    quit = 0;       /* Starting game loop. Resetting quit flag */
    int select = 0; /* flag for initial selection of a piece */

    int selectX = 0;/* storing selection coordinate   (x) */
    int selectY = 0;/* storing selection coordinate   (y) */
    int destX = 0;  /* storing destination coordinate (x) */
    int destY = 0;  /* storing destination coordinate (y) */


    while (quit !=1)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_MOUSEBUTTONDOWN: /* clicking on a piece */
                    if (select == 0)        /******* initial click to select piece ********/
                    {
                        selectX = event.motion.x/80;    /* assigning the square coordinate of the click to SELECTION */
                        selectY = event.motion.y/80; 

                        if (pieceArray[selectX][selectY].h == 1)    /* if the selection is empty, do not highlight */
                        {
                            break;
                        }
                        else    /* otherwise, highlight the selection */
                        {
                            SDL_BlitSurface(highlights, &yellow, screen, &boardArray[selectX][selectY]);    /* highlight selected piece */
                            SDL_Flip(screen);   /* update display */
                            select = 1;         /* set select flag up */
                            break;
                        }
                    }
                    else if (select == 1)   /***** second click to decide destination *****/
                    {
                        destX = event.motion.x/80;  /* assigning the square coordinates of the click to DESTINATION */
                        destY = event.motion.y/80;
                        if (&pieceArray[selectX][selectY] ==  &pieceArray[destX][destY])    /* if user clicks on the same square as desination */
                        {
                            SDL_BlitSurface(baseBoard, &boardArray[selectX][selectY], screen, &boardArray[selectX][selectY]);   /* removes the yellow highlight */
                            SDL_BlitSurface(chessPieces, &pieceArray[selectX][selectY], screen, &boardArray[selectX][selectY]); /*                              */
                            SDL_Flip(screen);   /* update screen*/
                            select = 0;         /* select down */
                            break;
                        }
                        else if (pieceArray[selectX][selectY].h != 1)    /* in the case of eating a piece */ 
                        {
                            SDL_BlitSurface(baseBoard, &boardArray[destX][destY], screen, &boardArray[destX][destY]);           /* paste base square */  
                            SDL_BlitSurface(chessPieces, &pieceArray[selectX][selectY], screen, &boardArray[destX][destY]);     /* place piece */
                            SDL_BlitSurface(baseBoard, &boardArray[selectX][selectY], screen, &boardArray[selectX][selectY]);   /* replace old square with base */
                            pieceArray[destX][destY] = pieceArray[selectX][selectY];                            /* set new square on piece array */
                            pieceArray[selectX][selectY] = empty;                                       /* set old square to empty */
                            SDL_Flip(screen);   /* update screen */
                            select = 0;         /* select down */
                            break;
                        }
                    }/* end of first if */
                    break;
                
                case SDL_QUIT:  /* Handles if user presses "x" button on window */
                    printf("You exited out of the window. \n");
                    Exit(screen);
                    quit = 1;
                    break;
            }
        }    
    }       


    /* Freeing used surfaces */
    SDL_FreeSurface(baseBoard);  /* board copy */
    SDL_FreeSurface(whiteBoard); /* white board surface */ 
    SDL_FreeSurface(greenBoard); /* green board surface */
    SDL_FreeSurface(chessPieces);/* chess pieces surface */ 
    SDL_FreeSurface(highlights); /* square outlines */
        
    return 0;
}

