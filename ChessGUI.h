#ifndef CHESSGUI_H
#define CHESSGUI_H


#include "SDL/SDL.h"

/* Global Variables for Screen settings */
#define  WIDTH   640 /* width of screen */
#define  HEIGHT  540 /* height of screen*/
#define  BPP     24   /* bits per pixel  */

void DisplayWindow(char *file, SDL_Surface *screen, int delay);     /* Displays one screen */
void Add_Box(SDL_Surface *image, SDL_Surface *screen, int x, int y);    /*Copies an image to another screen */
void Add_BoxFile(char *file, SDL_Surface *screen, int x, int y);    /* Adds to screen directly from a file */
void UpdateWindow(SDL_Surface *screen, int delay);  /* Updates and keeps window open */
void Exit(SDL_Surface *screen); /* Exit protocol */

#endif
