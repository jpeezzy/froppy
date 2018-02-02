#ifndef CHESSGUI_H
#define CHESSGUI_H


#include "SDL/SDL.h"

/* Global Variables for Screen settings */
#define  WIDTH   640 /* width of screen */
#define  HEIGHT  640 /* height of screen*/
#define  BPP     24   /* bits per pixel  */

    

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

/* Adds the pieces in the intial position onto the board */
//void InitializeBoard(SDL_Surface *pieces, SDL_Surface *screen, SDL_Rect boardArray[8][8]);

/* Exit protocol */
void Exit(SDL_Surface *screen);

#endif
