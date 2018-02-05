#ifndef CHESSGUI_H
#define CHESSGUI_H

#include "boardstate.h"
#include "SDL/SDL.h"
#include "movelist.h"

/* Global Variables for Screen settings */
#define  WIDTH   640 /* width of screen */
#define  HEIGHT  640 /* height of screen*/
#define  BPP     24  /* bits per pixel  */

/* Variables for MiniMax */
#define DEPTH 5

#define animations 0 /* animation control: 0 = off, 1 = on */
    

/* Displays one screen */
void DisplayWindow(char *file, SDL_Surface *screen, int delay);

/* Copies an image to another screen at a certain coordinate */
void Add_Box(SDL_Surface *image, SDL_Surface *screen, int x, int y);

/* Adds to screen directly from a file without using a surface first */
void Add_BoxFile(char *file, SDL_Surface *screen, int x, int y);

/* Updates and keeps window open for while loops */
void UpdateWindow(SDL_Surface *screen, int delay);

/* Creates the tiled board graphic */
void CreateBoard(SDL_Surface *baseBoard, SDL_Surface *tileBoard, SDL_Surface *screen); 

/* Copies the tiled board surface to another surface to use as reference */
SDL_Surface *BaseBoardCopy(SDL_Surface *screen, SDL_Rect boardArray[8][8]); 

/* Renaming SDL_BlitSurface(), so that it'll be easier for me to re-type it in chunks */
void Move(SDL_Surface *piece, SDL_Rect sprite, SDL_Surface *screen, SDL_Rect rect); 

/* Function to move a piece both on the board array and the GUI */
void MovePiece(int selectX, int selectY, int destX, int destY, SDL_Surface *baseBoard, 
SDL_Surface *chessPieces, SDL_Rect pieceArray[8][8], SDL_Rect boardArray[8][8], SDL_Surface *screen, 
SDL_Rect empty, SDL_Surface *effect, SDL_Rect animation[10], int effectNumber, int lastMove[4]);

/* Printing board based on the BSTATE instead of the GUI board */
void PrintBoard(BSTATE *board, SDL_Surface *baseBoard, SDL_Rect pieces[17], SDL_Surface *chessPieces, SDL_Surface *screen, SDL_Rect boardArray[8][8]);

/*************** MiniMax Functions ************************/
/* Moves the pieces on the minimax board */
int playerMove(BSTATE *board, int selectX, int selectY, int destX, int destY);

/* Changes turns */	
void changeSide(BSTATE *board);

/* Checks if a move is legal. Returns 1 if a move is legal, 0 if not */
int checkLegal(BSTATE *board, int cloc, int nloc);

/* Prompts the AI to move */
void aiMove(BSTATE *board, int AIMove[2]);

/* Exit protocol */
void Exit(SDL_Surface *screen);

#endif
