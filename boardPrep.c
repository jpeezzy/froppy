/**
 * Written to load/save board state to fen files 
 * 
 */
 

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "boardPrep.h"
#include "fenToBoardState.h"
#include "boardstate.h"

// name of save file, how many saves we have and the character size 
// of each fen line 
#define SAVEFILE "savefile.txt"
#define NUMSAVEFILES 4
#define SAVESIZE 100

// Saving will overwite the current save
// CURRENT SUPPORT 2 SAVES MAX
// saveNum is which save file to load/save from 

// basically read the content of the old file and then write 
// it back the same except for the line that needs to be saved
void boardSave(BSTATE* board, int saveNum)
{
    assert(board);
    FILE* savefile=fopen(SAVEFILE, "r");
    assert(savefile);
    char* saveList[NUMSAVEFILES];

    for(int j=0; j<NUMSAVEFILES; ++j)
    {
        saveList[j]=malloc(SAVESIZE*sizeof(char));
        assert(saveList[j]);
        strcpy(saveList[j],"");
    }

    // read the files to know the original content
    int i=0;
    while(i<NUMSAVEFILES)
    {
        fgets(saveList[i], SAVESIZE, savefile);
        ++i;
    }

    fclose(savefile);
    
    // write it back with the change
    boardToFen(saveList[saveNum-1],board);
    savefile=fopen(SAVEFILE, "w+");
    
    for(int k=0; k<NUMSAVEFILES; ++k)
    {
        if(k==saveNum-1){
            fprintf(savefile, "%s\n", saveList[k]);
        }
        else
        {
        fprintf(savefile, "%s", saveList[k]);
        }
    }

    // cleanup
    fclose(savefile);
        for(int j=0; j<NUMSAVEFILES; ++j)
    {
        free(saveList[j]);
    }
}


//saveNum starts at 1, not zero
void boardLoad(BSTATE* board,int saveNum)
{
    assert(board);
    FILE* savefile=fopen(SAVEFILE, "r");
    char* saveList[NUMSAVEFILES];

    for(int j=0; j<NUMSAVEFILES; ++j){
        saveList[j]=malloc(SAVESIZE*sizeof(char));
        assert(saveList[j]);
    }

    //read the files to know the original content
    int i=0;
    while(i<NUMSAVEFILES)
    {
        fgets(saveList[i], SAVESIZE, savefile);
        ++i;
    }

    if(i<saveNum)
    {
        perror("\nThere is nothing to load\n");
        return ;
    }
    fenToBoardState(saveList[saveNum-1], board);

    // cleanup
    for(int j=0; j<NUMSAVEFILES; ++j)
    {
        free(saveList[j]);
    }
    fclose(savefile);
}