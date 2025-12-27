#ifndef MP3_H
#define MP3_H

#include "types.h"


typedef struct musicfile
{
    char Title[100];
    char Artist[100];
    char Album[100];
    char Year[10];
    char Comment[100];
    char Genre[100]; 

    FILE *fptr_audio;
    //int tag_size;
    FILE *dup_mp3;

}playlist;

//Checking opeation types
operation_type check_operation_type(char *argv[]);

/*---------------------VIEW-----------------------*/

// For viewing 
status viewing(playlist *mp3);

// Open files for fetching
status open_file_for_fetching(playlist *mp3);

// Read ID3V1 tag from frame 
status read_id3v2_tag(playlist *mp3);

// Extracted the version, flag, and tag size from header
status read_tag_frames(playlist *mp3 ,int tag_size );

/*  -----------------EDIT--------------------*/

//For editing
status editing(playlist *mp3,char *option, char *new_value,char *filename);

//opening file for fetching(Edit)
status open_file_for_fetching_edit(playlist *mp3,char *filename);


//for editing(For Editing)
status read_tag_frames_for_editing(playlist *mp3,char *option, char *new_value,char *filename);


#endif