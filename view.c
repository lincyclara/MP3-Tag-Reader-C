

#include<stdio.h>
#include<string.h>
#include<stdlib.h> 

#include "mp3.h"
#include "types.h"

status viewing(playlist *mp3)
{
    //open files for fetching
    if(open_file_for_fetching(mp3) == e_success)
    {
        //printf("File opened\n");
    }
    else
    {
        printf("File is not opened\n");
        return e_failure;
    }

    //Reading header 10 bytes from the framework
    if(read_id3v2_tag(mp3) == e_success)
    {
        //printf("Tag is read from header\n");
    }
    else
    {
        printf("Tag not present\n");
        return e_failure;
    }
    
    fclose(mp3->fptr_audio);
    return e_success;
}


status open_file_for_fetching(playlist *mp3)
{
    //FILE *fp = mp3->fptr_audio ;
    mp3->fptr_audio = fopen("sample.mp3","rb");
    if(mp3->fptr_audio == NULL)
    {
        printf("File is not found\n");
        return e_failure;
    }
    return e_success;   
}

status read_id3v2_tag(playlist *mp3)
{
    FILE *fp = mp3 ->fptr_audio;
    char Tag[10];
    if(fread(Tag,1,10,fp) != 10)
    {
    
        return e_failure;
    }
    if(Tag[0] == 'I' && Tag[1] == 'D' && Tag[2] == '3')
    {
        //printf("Tag is present\n");
        
    }
    else
    {
        printf("Tag is not present\n");
        return e_failure;
    }
    int version = Tag[3];
    int revision = Tag[4];
    unsigned char flags = Tag[5];
    //int tag_size = Tag[6] - Tag[9];
    int tag_size = ((Tag[6] &0x7F) << 21) | ((Tag[7] &0x7F) << 14) | ((Tag[8] & 0x7F) << 7) | (Tag[9] & 0x7F) ;
    //printf("Version : %d\nRevision : %d\nFlags : %u\nTag Size : %d\n",version,revision,flags,tag_size);
    read_tag_frames(mp3,tag_size);
    return e_success;

}
status read_tag_frames(playlist *mp3 ,int tag_size )
{
    //int Tag_ID[4]; 
    fseek(mp3->fptr_audio,10,SEEK_SET);
    int bytes_read = 0;
    while(bytes_read < tag_size)
    {

    unsigned char Tag_ID[4] =  {0};
    if(fread(Tag_ID,1,4,mp3->fptr_audio) != 4)
    {
        printf("Tag ID is not read successfully\n");
        return e_failure;
    }
    bytes_read +=4;

    if(Tag_ID[0] == 0)
    {
        break;
    }

    unsigned char size_bytes[4];
    if(fread(size_bytes,1,4,mp3->fptr_audio) != 4)
    {
        printf("Content size is not correct\n");
        return e_failure;
    }
    bytes_read += 4;

    unsigned int frame_size = (size_bytes[0] << 24) | (size_bytes[1] << 16) | (size_bytes[2] << 8) | (size_bytes[3]);

    unsigned char flags[2];
    if(fread(flags,1,2,mp3->fptr_audio) != 2)
    {
        printf("Flag is not read\n");
        return e_failure;
    }
    bytes_read += 2;

    if(frame_size ==0 ||  frame_size > tag_size)
    {
        printf("Invalid frame size\n");
        return e_failure;
    }

    unsigned char *content = malloc(frame_size + 1);
    if(content == NULL)
    {
        printf("Memory allocation failed\n");
        return e_failure;
    }
    if(fread(content,1,frame_size,mp3->fptr_audio) != frame_size)
    {
        printf("Content read failed\n");
        free(content);
        return e_failure; 
    }
    bytes_read += frame_size;
    content[frame_size] ='\0';

    char *data = (char *)(content + 1);

    if(strcmp((char*)Tag_ID,"TIT2") == 0)
    {
        strncpy(mp3->Title,data,sizeof(mp3->Title)-1);
        //printf("Title : %s\n",data);
    }
    else if(strcmp((char*)Tag_ID,"TPE1") == 0)
    {
        strncpy(mp3->Artist,data,sizeof(mp3->Artist)-1);
        //printf("Artist : %s\n",data);
    }
    else if(strcmp((char*)Tag_ID,"TALB") == 0)
    {
        strncpy(mp3->Album,data,sizeof(mp3->Album)-1);
        //printf("Album : %s\n",data);
    }
    else if(strcmp((char*)Tag_ID,"TYER") == 0)
    {
        strncpy(mp3->Year,data,sizeof(mp3->Year)-1);
        //printf("Year : %s\n",data);
    }
    else if(strcmp((char*)Tag_ID,"COMM") == 0)
    {
        strncpy(mp3->Comment,data,sizeof(mp3->Comment)-1);
        //printf("Comment : %s\n",data);
    }
    else if(strcmp((char*)Tag_ID,"TCON") == 0)
    {
        strncpy(mp3->Genre,data,sizeof(mp3->Genre)-1);
        //printf("Genre : %s\n",data);
    }
    
    free(content);
    }
    return e_success;

}