#include "ChessGUI.h" 
#include <assert.h>
#include "testgui.h"
#include "minmax.h"
#include "boardstate.h"
#include "basic_eval.h"
#include "movelist.h"
#include <stdio.h>
#include <stdlib.h>
#include "spicyComments.h"
#include "fenToBoardState.c"

#include "userHint.h"
#include "boardPrep.h"
#include "spicyComments.h"
int main(int argc, char *args[])
{
    SDL_Surface *screen = NULL;     /* Main surface to be displayed */
    SDL_Surface *baseBoard = NULL;  /* Surface that will be the reference chess board */

    

    int AI = 0;         /* Flag to turn on/off AI. 0 = off, 1 = on */
//    int lastMove[4];    /* Array to save the last move's coordinates in serial */ 
//    int lastPieces[2];  /* Array to save the last 2 pieces that moved */
    
    SDL_Init(SDL_INIT_VIDEO);   /* Initializes SDL Environment */
    
    screen = SDL_SetVideoMode(WIDTH, HEIGHT + 80, BPP, SDL_SWSURFACE); /* Inputs settings for screen */
    DisplayWindow("Intro.bmp", screen, 1000);    
    
    Add_BoxFile("BottomButtons1.bmp", screen, 0,640);

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

    SDL_Surface *chooseOpponent = NULL;     /* Making surface for opponent-choosing screen */
    chooseOpponent = SDL_LoadBMP("ChooseOpponent.bmp");

    SDL_Surface *chessPieces = NULL;         /* Making surface for chess pieces */
    chessPieces = SDL_LoadBMP("ChessPiecesRedraw.bmp");
    Uint32 color = SDL_MapRGB(chessPieces->format, 0xFF,0xFF,0xFF); /* Filtering out the unwanted background color */
    SDL_SetColorKey(chessPieces, SDL_SRCCOLORKEY, color);    
    
    SDL_Surface *highlights = NULL;         /* Making surface for square highlight */
    highlights = SDL_LoadBMP("SquareHighlight.bmp");
    color = SDL_MapRGB(highlights->format, 0xFF, 0xFF, 0xFF);/* Filtering out all white color */
    SDL_SetColorKey(highlights, SDL_SRCCOLORKEY, color);

    SDL_Surface *loadingHint = NULL;        /* Making surface for hint loading popup */
    loadingHint = SDL_LoadBMP("LoadingHint.bmp");
    color = SDL_MapRGB(loadingHint->format, 0xFF, 0xFF, 0xFF);
    SDL_SetColorKey(loadingHint, SDL_SRCCOLORKEY, color);

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

    SDL_Surface *explosion = NULL;          /* Making explosion animation */
    explosion = SDL_LoadBMP("Explosion.bmp");
    color = SDL_MapRGB(explosion->format, 0xFF,0xFF,0xFF);/* Filtering out white color */
    SDL_SetColorKey(explosion, SDL_SRCCOLORKEY, color);

    SDL_Surface *lightning = NULL;          /* Making lightning animation */
    lightning = SDL_LoadBMP("Lightning.bmp");
    color = SDL_MapRGB(lightning->format, 0xFF, 0xFF,0xFF);/* Filtering out white color */
    SDL_SetColorKey(lightning, SDL_SRCCOLORKEY, color);


    SDL_Surface *saveMenu = NULL;       /* Save Menu Menu */
    saveMenu = SDL_LoadBMP("SaveMenu.bmp");
    color = SDL_MapRGB(saveMenu->format, 0xFF, 0xFF, 0xFF);
    SDL_SetColorKey(saveMenu, SDL_SRCCOLORKEY, color);

    SDL_Surface *loadMenu = NULL;       /* Load Menu Menu */
    loadMenu = SDL_LoadBMP("LoadMenu.bmp");
    color = SDL_MapRGB(loadMenu->format, 0xFF, 0xFF, 0xFF);
    SDL_SetColorKey(loadMenu, SDL_SRCCOLORKEY, color);



    /* setting rectangles for  animation */
    SDL_Rect animation[10];
    for (int i = 0; i < 10; i ++)
    {
        animation[i].h = 80; animation[i].w = 80;    
        animation[i].x = 80*i; animation[i].y = 0;
    }
    
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
  
    for (int i = 0; i < 8; i++)     /* initializing the pieceArray */
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

    SDL_Rect pieces[17];    /* array to hold piece rectangles */
    for (int i = 0; i < 17; i++)
    {
        pieces[i] = empty;
    }
    pieces[1] = wP;     pieces[11] = bP;
    pieces[2] = wH;     pieces[12] = bH;
    pieces[3] = wB;     pieces[13] = bB;
    pieces[4] = wR;     pieces[14] = bR;
    pieces[5] = wQ;     pieces[15] = bQ;
    pieces[6] = wK;     pieces[16] = bK;
        

    


    printf("--------FROPPY CHESS GAME---------- \n"); 

/****************************************** START OF USER INTERACTION  **********************************************************/
    SDL_Event introEvent;
    SDL_Event event; 
   
 
    int quit = 0;        /* Flag for ending menu while loop */
    while (quit != 1)    /******** INTRO SCREEN LOOP ********* */
    { 
        while (SDL_PollEvent(&introEvent))
        {
            if (introEvent.type == SDL_MOUSEBUTTONUP)  
            {
                Add_BoxFile("Menu.bmp", screen, 0,0);   /* change to menu screen */
                SDL_Flip(screen);
                quit = 1;  
                break;
            }
            if (introEvent.type == SDL_QUIT)
            {
                printf("You exited out of the window. \n");
                Exit(screen);
                quit = 1;
                return 0;
            }
        }
    }         

    quit = 0; /* resetting quit flag for next loop */    

    while (quit != 1)
    {
        while(SDL_PollEvent(&event))    /******* MENU SCREEN LOOP ********/
        {
            switch(event.type)
            {
                case SDL_MOUSEBUTTONDOWN: 
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
                        if ( event.motion.y >= 475 && event.motion.y <= 578)        /* Quit button */
                        {
                            printf("You chose to quit! \n");
                            Exit(screen);
                            return 0;
                        }
                    
                        else if (event.motion.y >= 346 && event.motion.y <= 449)    /* Play button */    
                        {    
                            printf("You chose to play! \n");
                            Add_BoxFile("ChooseOpponent.bmp", screen, 0,0);    /* moves to colorchoice screen  */
                            SDL_Flip(screen);
                            quit = 1;
                            break;
                        }
                    }
                    break;

                 case SDL_QUIT:     /* Handles if user presses "x" button on window */
                    printf("You exited out of the window. \n");
                    Exit(screen);
                    quit = 1;
                    break;

            }/* end switch */
        }/* end PollEvent loop */
    }/* end main loop */
       
    quit = 0; /* resetting quit flag */

    while (quit != 1)   /************* Opponent Choice Loop ******************/
    {
        while (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_MOUSEBUTTONUP:
                    if (event.motion.x >= 162 && event.motion.x <= 480)
                    {
                        if (event.motion.y >= 321 && event.motion.y <= 430)      /* if "HUMAN" button is pressed */
                        {
                            printf("You chose to play Human vs. Human! \n");
                            AI = 0;
                            quit = 1;
                            break;
                        }
                        else if ( event.motion.y >= 441 && event.motion.y <= 550)/* if "COMPUTER" button is pressed */
                        {
                            printf("You chose to play Human vs. Computer! \n");
                            AI = 1;
                            quit = 1;
                            break;
                        }
                    }
                case SDL_QUIT:
                    quit = 1;
                    return 0;

            }
        }
    }           
        
    Add_BoxFile("ChooseColor.bmp", screen, 0,0); 
    SDL_Flip(screen);


    int AIMove[2];  /* Array to save the AI */
    int turn = 0;   /* variable to decide who goes goes first */
    quit = 0; /* resetting quit flag */
    int go = 0; /* dirty workaround to fix a loop issue */
    
    
    while (quit != 1)
    {
        while (SDL_PollEvent(&event))   /******* COLORCHOICE & BOARD INSTANTIATION  LOOP *********/
        {
            switch (event.type)
            {
                
                case SDL_MOUSEBUTTONUP:
                    if (event.motion.x >= 182 && event.motion.x <= 458      /* if "BLACK" button is pressed */
                     && event.motion.y >= 58  && event.motion.y <= 161)
                    {
                        if (AI != 0)
                        {
                            turn = 1;  /* setting side color to black */
                            printf("You chose to play as BLACK side \n\n");
                        }
                        go = 1;
                    }
                    else if (event.motion.x >= 179 && event.motion.x <= 515 /* if "WHITE" button is pressed */
                          && event.motion.y >= 478 && event.motion.y <= 581)
                    {
                        if (AI != 0)
                        {
                            turn = 0;  /* seting side color to white */
                            printf("You chose to play as WHITE side. \n\n");
                        }
                        go = 1;
                    }    

                    if (go == 1)               
                    {  
                        Add_BoxFile("BottomButtons.bmp", screen, 0,640); 
                        CreateBoard(greenBoard, whiteBoard, screen);    /* Making board graphics */ 
                        baseBoard = SDL_DisplayFormat(screen);          /* Copying board for reference */ 
                        SDL_BlitSurface(screen, NULL, baseBoard, NULL); 


                        /* Putting initial pieces on the board */
                        printf("[ Creating & Initializing the Board ] \n");
            
                            /* Pawns */
                     	for (int i = 0; i < 8; i++) 
                    	{ 
                 	        SDL_BlitSurface(chessPieces, &bP, screen, &boardArray[i][1]); 
                                pieceArray[i][1] = bP; 
                	        SDL_BlitSurface(chessPieces, &wP, screen, &boardArray[i][6]);
                                pieceArray[i][6] = wP;
                 	    }
                        printf("    Initializing: Pawns   ... \n");

	                        /* Rooks */
        	            SDL_BlitSurface(chessPieces, &bR, screen, &boardArray[0][0]);
                            pieceArray[0][0] = bR;   
        	            SDL_BlitSurface(chessPieces, &bR, screen, &boardArray[7][0]);
                            pieceArray[7][0] = bR;
               	        SDL_BlitSurface(chessPieces, &wR, screen, &boardArray[0][7]);
                            pieceArray[0][7] = wR;
               		    SDL_BlitSurface(chessPieces, &wR, screen, &boardArray[7][7]);
                            pieceArray[7][7] = wR;
                        printf("    Initializing: Rooks   ... \n");

			                /* Knights */
               	 	    SDL_BlitSurface(chessPieces, &bH, screen, &boardArray[1][0]);
                            pieceArray[1][0] = bH;
               	        SDL_BlitSurface(chessPieces, &bH, screen, &boardArray[6][0]);
                 	        pieceArray[6][0] = bH;
                        SDL_BlitSurface(chessPieces, &wH, screen, &boardArray[1][7]);
                	        pieceArray[1][7] = wH;
                        SDL_BlitSurface(chessPieces, &wH, screen, &boardArray[6][7]);
                            pieceArray[6][7] = wH;                       
                        printf("    Initializing: Knights ... \n");         

		      	    	    /* Bishops */
             		    SDL_BlitSurface(chessPieces, &bB, screen, &boardArray[2][0]);
              	            pieceArray[2][0] = bB;
                        SDL_BlitSurface(chessPieces, &bB, screen, &boardArray[5][0]);
             	            pieceArray[5][0] = bB;
                        SDL_BlitSurface(chessPieces, &wB, screen, &boardArray[2][7]);
               	            pieceArray[2][7] = wB;
                        SDL_BlitSurface(chessPieces, &wB, screen, &boardArray[5][7]);
                            pieceArray[5][7] = wB;	
                        printf("    Initializing: Bishops ... \n");

                    	    /* Queens */
                   	    SDL_BlitSurface(chessPieces, &bQ, screen, &boardArray[3][0]);
                            pieceArray[3][0] = bQ;
                   	    SDL_BlitSurface(chessPieces, &wQ, screen, &boardArray[3][7]);
                            pieceArray[3][7] = wQ;
                        printf("    Initializing: Queens  ... \n");

				            /* Kings */
                   	    SDL_BlitSurface(chessPieces, &bK, screen, &boardArray[4][0]);
                            pieceArray[4][0] = bK;
                   	    SDL_BlitSurface(chessPieces, &wK, screen, &boardArray[4][7]);
                            pieceArray[4][7] = wK;
                        printf("    Initializing: Kings   ... \n");
                        printf("[               Done!               ]\n");
                        quit = 1;
                        SDL_Flip(screen);
                    }
                    break;

           
                 case SDL_QUIT:     /* Handles if user presses "x" button on window */
                    printf("You exited out of the window. \n");
                    Exit(screen);
                    quit = 1;
                    break;
            }/* end switch */
        }/* end PollEvent while loop */
    }/* end main while loop */

    SDL_Flip(screen);

    quit = 0;       /* Starting game loop. Resetting quit flag */
    int select = 0; /* flag for initial selection of a piece */

    int selectX = 0;/* storing selection coordinate   (x) */
    int selectY = 0;/* storing selection coordinate   (y) */
    int destX = 0;  /* storing destination coordinate (x) */
    int destY = 0;  /* storing destination coordinate (y) */

    int effectNumber = 0;  /* random number for picking kill animation */

	/* Copied and paste the testminmax.c code over */
	MLIST *legal;
    BSTATE *board;
    BSTATE *undoBoard;
    board = createBstate();
    
    loadStart(board);
    copyBstate(board,undoBoard);

    int score = basicEvaluation(board);
    printf("Eval score = %d\n", score);
    if (turn == 0)
    {
        printf("You are playing as WHITE\n");
	}
    else if (turn == 1)
    {
        printf("You are playing as BLACK \n");
    }

    int menu = 0;       /* variable for bottom menu selections */
    int menuSelect = 0; /* variable for deciding which save select button is pressed */

    while (quit !=1)
    {
       if (AI == 1 && turn == 1)
        {    
            aiMove(board, AIMove);     
                            
            selectX = AIMove[0]%8;   /* Converting 1D array to 2D array for GUI update */
            selectY = AIMove[0]/8;
            destX   = AIMove[1]/8;
            destY   = AIMove[1]/8; 
            PrintBoard(board, baseBoard, pieces, chessPieces, screen, boardArray);
 
            legal = createMovelist();
            allLegal(legal, board);
            if(legal->movenum == 0)
            {
                printf("Checkmate!\n");
                deleteMovelist(legal);
                break;
            }
            deleteMovelist(legal);
            turn = 0;	
        }
        else if (turn == 0)
        {
        while (SDL_PollEvent(&event))   /****** MAIN GAME LOOP ******/
        {
            switch (event.type)
            {
                case SDL_MOUSEBUTTONDOWN: /* clicking on a piece */
                    if (menu == 2)      /* Load menu button choices */
                    {
                        if (event.motion.x >= 225 && event.motion.x <= 416)
                        {
                            if (event.motion.y >= 280 && event.motion.y <= 361)
                            {
                                menuSelect = 1;

                            }
                            else if (event.motion.y >= 377 && event.motion.y <= 457)
                            {
                                menuSelect = 2;
                            }
                            boardLoad(board, menuSelect);   /* Loading board with selection number */
                            PrintBoard(board, baseBoard, pieces, chessPieces, screen, boardArray);  /* Printing new board */
                            menu = 0;   /* menu flag down */
                            copyBstate(board,undoBoard);
                        }
                        break;
                    }
       
                   else if (menu == 3)  /* Save menu button choices */
                    {
                        if (event.motion.x >= 225 && event.motion.x <= 416)
                        {
                            if (event.motion.y >= 280 && event.motion.y <= 361)
                            {
                                menuSelect = 1;
                            }
                            else if (event.motion.y >= 377 && event.motion.y <= 457)
                            {
                                menuSelect = 2;
                            }
                            boardSave(board, menuSelect);   /* Saving board with selection number */
                            PrintBoard(board, baseBoard, pieces, chessPieces, screen, boardArray);  /* Printing new board */
                            Exit(screen);
                            menu = 0;   /* menu flag down */
                            return 0;
                       }
                        break;
                    }
 
                    else if (event.motion.y <= 640)
                    {
                        if (select == 0)        /* initial click to select piece */
                        {
                            selectX = event.motion.y/80;    /* assigning the square coordinate of the click to SELECTION */
                            selectY = event.motion.x/80; 

                            if (board->boardarray[selectX][selectY] == 0/*pieceArray[selectX][selectY].h == 1*/)    /* if the selection is empty, do not highlight */
                            {
                                break;
                            }
                            else                                        /* otherwise, highlight the selection */
                            {
                                SDL_BlitSurface(highlights, &yellow, screen, &boardArray[selectY/*selectX*/][selectX/*selectY*/]);    /* highlight selected piece */
                                SDL_Flip(screen);   /* update display */
                                select = 1;         /* set select flag up */
                                break;
                            }
                        }
                        else if (select == 1)   /***** second click to decide destination *****/
                        {
                            destX = event.motion.y/80;  /* assigning the square coordinates of the click to DESTINATION */
                            destY = event.motion.x/80;
                            if (selectX == destX && selectY == destY)    /* if user clicks on the same square as desination */
                            {
                                SDL_BlitSurface(baseBoard, &boardArray[selectY][selectX], screen, &boardArray[selectY][selectX]);   /* removes the yellow highlight */
                                SDL_BlitSurface(chessPieces, &pieces[board->boardarray[selectX][selectY]], screen, &boardArray[selectY][selectX]);
                                SDL_Flip(screen);   /* update screen*/
                                select = 0;         /* select flag down */
                                break;
                            }
                            else    /* in the case of clicking on a new square  */ 
                            {
                                effectNumber = rand()%2 + 1;    /* picking random number to pick the animation */

                      //      lastPieces[0] = board->boardarray[selectX][selectY];    /* Saves the selected square    */
                      //      lastPieces[1] = board->boardarray[destX][destY];        /* Saves the destination square */

			/******************** Beginning of MinMax Integration ****************************************************/
                                
                                if (playerMove(board, selectY, selectX, destY, destX) != 1)     /* If invalid move */
                                {
                                    select = 0; /* restart loop to make user select another piece to move */

                                    SDL_BlitSurface(baseBoard, &boardArray[selectY][selectX], screen, &boardArray[selectY][selectX]);   /* removes the yellow highlight */                            
                                    SDL_BlitSurface(chessPieces, &pieces[board->boardarray[selectX][selectY]], screen, &boardArray[selectY][selectX]);
 
                                    SDL_Flip(screen);   /* update screen*/
                                    break;   
                                }
                                spicyAdd(board);
                                PrintBoard(board, baseBoard, pieces, chessPieces, screen, boardArray);
                                
                                legal = createMovelist();
                                allLegal(legal, board);
                                if(legal->movenum == 0)     /* Checkmate checker */
                                {   
                                    printf("Checkmate!\n");
                                    deleteMovelist(legal);
                                    break;
                                }
                                deleteMovelist(legal);
            /******************** End of MinMax Integration ***********************************************************/
                                if (AI != 0)
                                {
                                    turn = 1;  /* set turn back to AI */
                                }
                                select = 0;         /* select flag down */
                                break;
                            }
                        }
                    }
                    else if (event.motion.y > 640)   /* if user clicks on any bottom menu buttons */
                    {
                        if (event.motion.x >= 479)                                  /* Hint button */
                        {
                            /* Provide Hint to users */
                            giveHint(board, loadingHint, screen);
                            PrintBoard(board, baseBoard, pieces, chessPieces, screen, boardArray);                            
                            break;
                        }
                        else if (event.motion.x <= 478 && event.motion.x >= 320)    /* Save button */
                        {
                            SDL_BlitSurface(saveMenu, NULL, screen, NULL);
                            SDL_Flip(screen);
                            menu = 3;
                            break;
                        }
                        else if (event.motion.x <= 318 && event.motion.x >= 160)    /* Load button */
                        {
                            SDL_BlitSurface(loadMenu, NULL, screen, NULL);
                            SDL_Flip(screen);
                            menu = 2;
                            break;
                        }
                        else if (event.motion.x <= 159)                             /* Undo button */
                        {
                            copyBstate(undoBoard,board);
                            break;
                        }
                    copyBstate(board,undoBoard);
                    }
                    break;
                
                case SDL_QUIT:  /* Handles if user presses "x" button on window */
                    printf("You exited out of the game. \n");
                    Exit(screen);
                    quit = 1;
                    break;
                }/* end switch */
            }/* end PollEvent loop */
        }/* end main while loop */       
    }/* end of first if */

    /* Freeing used surfaces */
    SDL_FreeSurface(baseBoard);         /* board copy            */
    SDL_FreeSurface(whiteBoard);        /* white board surface   */ 
    SDL_FreeSurface(greenBoard);        /* green board surface   */
    SDL_FreeSurface(chessPieces);       /* chess pieces surface  */ 
    SDL_FreeSurface(highlights);        /* square outlines       */
    SDL_FreeSurface(explosion);         /* explosion animation   */    
    SDL_FreeSurface(lightning);         /* lightning animation   */        
    SDL_FreeSurface(chooseOpponent);    /* choose opponent screen*/
    SDL_FreeSurface(saveMenu);          /* save menu             */
    SDL_FreeSurface(loadMenu);          /* load menu             */

//    SDL_FreeSurface(screen1);           /* intro screens         */
    return 0;
}

