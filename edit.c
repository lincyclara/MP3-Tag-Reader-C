#include<stdio.h>
#include<string.h>
#include<stdlib.h> 

#include "mp3.h"
#include "types.h"


status editing(playlist *mp3,char *option, char *new_value,char *filename)
{
    //opening file for fetching
    if(open_file_for_fetching_edit(mp3,filename) == e_success )
    {
        //printf("File opened\n");
    }
    else
    {
        printf("File is not opened\n");
        return e_failure;
    }

    if( read_tag_frames_for_editing(mp3,option,new_value,filename) == e_success)
    {
        //printf("Reading completed\n");
    }
    else
    {
        printf("Reading Tag_header is not completed\n");
        return e_failure;
    }

    return e_success;
}

status open_file_for_fetching_edit(playlist *mp3,char *filename)
{
    mp3->fptr_audio = fopen("sample.mp3","rb");
    if(mp3->fptr_audio == NULL)
    {
        printf("File not found\n");
        return e_failure;
    }
    return e_success;
}


status read_tag_frames_for_editing(playlist *mp3,char *option,char *new_value,char *filename)
{
    //creating a duplicate file
    mp3->dup_mp3 = fopen("duplicate.mp3","wb");
    if(mp3->dup_mp3 == NULL)
    {
        printf("Error opening the file\n");
        //fclose(mp3->dup_mp3);
        return e_failure;
    }

    // calculating the size of the original file
    fseek(mp3->fptr_audio,0,SEEK_END);
    long size = ftell(mp3->fptr_audio);
    rewind(mp3->fptr_audio);

    char *buffer = malloc(size);
    if(buffer ==NULL)
    {
        printf("Memory allocation failed\n");
        fclose(mp3->fptr_audio);
        fclose(mp3->dup_mp3);
        return e_failure;
    }

    // copying all data into duplicate file
    if(fread(buffer,1,size,mp3->fptr_audio) != size)
    {
        printf("Error reading the file\n");
        free(buffer);
        fclose(mp3->fptr_audio);
        fclose(mp3->dup_mp3);
        return e_failure;
    }
    if(fwrite(buffer,1,size,mp3->dup_mp3) != size)
    {
        printf("Error reading the file\n");
        free(buffer);
        fclose(mp3->fptr_audio);
        fclose(mp3->dup_mp3);
        return e_failure;
    }
    free(buffer);
    fclose(mp3->dup_mp3);
    mp3->dup_mp3 = fopen("duplicate.mp3","rb+");
    if(mp3->dup_mp3 == NULL)
    {
        printf("Error opening the file\n");
        //fclose(mp3->dup_mp3);
        return e_failure;
    }

    // reading frame id

    char ID[5];
    if(strcmp(option,"-t") == 0)
    {
        strcpy(ID,"TIT2");
    }
    else if(strcmp(option,"-a") == 0)
    {
        strcpy(ID,"TPE1");
    }
    else if(strcmp(option,"-A") == 0)
    {
        strcpy(ID,"TALB");
    }
    else if(strcmp(option,"-y") == 0)
    {
        strcpy(ID,"TYER");
    }
    else if(strcmp(option,"-c") == 0)
    {
        strcpy(ID,"COMM");
    }
    else if(strcmp(option,"-m") == 0)
    {
        strcpy(ID,"TCON");
    }
    else{
        printf("Invalid option\n");
        fclose(mp3->dup_mp3);
        return e_success;
    }


    // Read ID3v2 header to get tag size
    fseek(mp3->dup_mp3, 0, SEEK_SET);
    char header[10];
    if(fread(header, 1, 10, mp3->dup_mp3) != 10)
    {
        printf("Error reading header\n");
        fclose(mp3->dup_mp3);
        return e_failure;
    }
    
    int tag_size = ((header[6] & 0x7F) << 21) | ((header[7] & 0x7F) << 14) | ((header[8] & 0x7F) << 7) | (header[9] & 0x7F);

    printf("-----------------------------------------------------------\n");
    printf("                        UPDATED DETAILS                    \n");
    printf("-----------------------------------------------------------\n");


    // Read from file and getting the size of ID
    fseek(mp3->dup_mp3,10,SEEK_SET);
    int i;

    
    for(i=0;i<6;i++)
    {
        unsigned char id[5];
        if(fread(id,1,4,mp3->dup_mp3) != 4)
        {
            fclose(mp3->dup_mp3);
            return e_failure;
        }
        id[4] ='\0';
        //bytes_read +=4;
        unsigned char duplicate_size[4];
        if(fread(duplicate_size,1,4,mp3->dup_mp3) != 4)
        {
            fclose(mp3->dup_mp3);
            return e_failure;
        }
        //bytes_read +=4;

        unsigned int dup_frame_size;
        dup_frame_size = (duplicate_size[0] << 24) | (duplicate_size[1] << 16) | (duplicate_size[2] << 8) | duplicate_size[3];
        
        //skipping flags
        fseek(mp3->dup_mp3,2,SEEK_CUR);
        //bytes_read +=2;
        long pos = ftell(mp3->dup_mp3);

        if(strcmp((char*)id,ID) == 0)
        {
            if(strlen(new_value)+1 > dup_frame_size)
            {
                printf("New value is longer than the desired size\n");
                fclose(mp3->dup_mp3);
                return e_failure;
            }
            fseek(mp3->dup_mp3,pos+1,SEEK_SET);
            fwrite(new_value,1,strlen(new_value),mp3->dup_mp3);
            
            int end = dup_frame_size -1 - strlen(new_value);
            for(int j=0;j<end;j++)
            {
                fputc('\0',mp3->dup_mp3);
            }
        }
        fseek(mp3->dup_mp3,pos,SEEK_SET);

        // display frames

        char *dup_data = (char*)malloc(dup_frame_size+1);
        if(dup_data == NULL)
        {
            fclose(mp3->dup_mp3);
            return e_failure;
        }
        if(fread(dup_data,1,dup_frame_size,mp3->dup_mp3) != dup_frame_size)
        {
            free(dup_data);
            fclose(mp3->dup_mp3);
            return e_failure;
        }
        dup_data[dup_frame_size] ='\0';

        if(strcmp((char*)id,"TIT2") == 0)
        {
            printf("%s : %s\n","Title",dup_data +1);
        }
        else if(strcmp((char*)id,"TPE1") == 0)
        {
            printf("%s : %s\n","Artist",dup_data +1);
        }
        else if(strcmp((char*)id,"TALB") == 0)
        {
            printf("%s : %s\n","Album",dup_data +1);
        }
        else if(strcmp((char*)id,"TYER") == 0)
        {
            printf("%s : %s\n","Year",dup_data +1);
        }
        else if(strcmp((char*)id,"COMM") == 0)
        {
            printf("%s : %s\n","Content",dup_data +1);
        }
        if(strcmp((char*)id,"TCON") == 0)
        {
            printf("%s : %s\n","Coments",dup_data +1);
        }
        /*else
        {
            return e_failure;
        }*/
        free(dup_data);
    }
    fclose(mp3->dup_mp3);
    mp3->dup_mp3 = NULL;

    fclose(mp3->fptr_audio);
    mp3->fptr_audio = NULL;

    printf("------------------------------------------------------------\n");
    printf("              DETAILS DISPLAYED SUCCESSFULLY                \n");
    printf("------------------------------------------------------------\n");


    rename("duplicate.mp3","sample.mp3");

    return e_success;

}