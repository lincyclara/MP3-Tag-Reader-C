/*
Name : Lincy K
Date : 06/11/2025
Description :This project implements an MP3 Tag Reader focused on reading and displaying metadata stored in the ID3v2 tag format from MP3 audio files.
             The ID3v2 tag is a widely used standard for embedding detailed information—such as song title, artist, album, track number, year, genre,
             comments, lyrics, and even album artwork—directly within MP3 files.The application processes MP3 files, locates the ID3v2 header (typically 
             found at the beginning of the file), and parses various metadata frames contained within the tag. It extracts and presents this information
             in a structured, user-friendly way, greatly facilitating music library organization and management.
Key Features:
* Supports ID3v2 tag format (ID3v2.2, ID3v2.3, ID3v2.4)
* Parses and displays important fields like Title, Artist, Album, Track Number, Genre, Year, Comments, and Album Art.
* Works with both individual files and directories.
* Error handling for missing or corrupted tags

*/

#include<stdio.h>
#include<string.h>

#include "mp3.h"
#include "types.h"

int main(int argc, char *argv[])
{
    playlist mp3;
    if(argc < 2)
    {
        printf("--------------------------------------------------------------------------------------------------\n");
        printf("Insufficient arguments, please follow the format\n");
        printf("For viewing use format: ./a.out -v <mp3filename> \n");
        printf("For editing use format: ./a.out -e  -e/-a/-A/-y/-m/-c  <new_value> <mp3filename> \n");
        printf("For help use format: ./a.out --help \n");
        printf("--------------------------------------------------------------------------------------------------\n");
        return e_failure;
    }
    //playlist *mp3;
    if(check_operation_type(argv) == e_view)
    {
        //printf("You choosed viewing content\n");
        if(argc < 3 )
        {
            printf("--------------------------------------------------------------------------------------------------\n");
            printf("For viewing use format: ./a.out -v <mp3filename> \n");
            printf("--------------------------------------------------------------------------------------------------\n");
            return e_failure;
        }

        

        if(viewing(&mp3) == e_success )
        {
            
            printf("--------------------------------------------------------------------------------------------------\n");
            printf("                                      SELECTED VIEW DETAILS                                       \n");
            printf("--------------------------------------------------------------------------------------------------\n");
            printf("                                 MP3 TAG READER AND EDITOR FOR ID3V2                              \n");
            printf("--------------------------------------------------------------------------------------------------\n");
            printf("TITLE          :   %s\n",mp3.Title);
            printf("ARTIST         :   %s\n",mp3.Artist);
            printf("ALBUM          :   %s\n",mp3.Album);
            printf("YEAR           :   %s\n",mp3.Year);
            printf("COMMENT        :   %s\n",mp3.Comment);
            printf("GENRE          :   %s\n",mp3.Genre);
            printf("--------------------------------------------------------------------------------------------------\n");
            printf("                                DETAILS DISPLAYED SUCCESSFULLY                                    \n");
            printf("--------------------------------------------------------------------------------------------------\n");
        }
        else
        {
            printf("Failure\n");
            return e_failure;
        }

    }
    
    else if(check_operation_type(argv) == e_edit)
    {
        
        if(argc < 5)
        {
            printf("--------------------------------------------------------------------------------------------------\n");
            printf("For editing use format: ./a.out -e -t/-a/-A/-y/-m/-c  <new_value>  <mp3filename>\n");
            printf("--------------------------------------------------------------------------------------------------\n");

            return e_failure;
        }
        else if(strstr(argv[argc-1],".mp3") == 0)
        {
            printf(".mp3 file not present\n");
            return e_failure;
        }

        if(!(strcmp(argv[2],"-t")==0) || (strcmp(argv[2],"-a")==0) || (strcmp(argv[2],"-A")==0) || (strcmp(argv[2],"-y")==0) || (strcmp(argv[2],"-c")==0) || (strcmp(argv[2],"-m")==0))
        {
            printf("Incorrect arguements. Use -t/-a/-A/-y/-c/-m\n");
            return e_failure;
        }
        //printf("You choosed Edit contents \n");
        if(editing(&mp3, argv[2], argv[3], argv[argc-1]) != e_success)
        {
            printf("Couldn't viewed contents\n");
            return e_failure;
            //printf("Title   : %s\n",mp3.Title);    
        }
            
    }
    
    else if(check_operation_type(argv) == e_help)
    {
        
        printf("--------------------------------------------------------------------------------------------------------\n");
        printf("                                              HELP MENU                                                   \n");
        printf("--------------------------------------------------------------------------------------------------------\n");
        printf("1. -v -> to view mp3 file contents\n");
        printf("2. -e -> to edit mp3 file contents\n");
        printf("         2.1. -t -> to edit song title\n");
        printf("         2.2. -a -> to edit artist name\n");
        printf("         2.3. -A -> to edit album name \n");
        printf("         2.4. -y -> to edit year\n");
        printf("         2.5. -c -> to edit comment\n");
        printf("         2.6. -m -> to edit genre\n");
        printf("--------------------------------------------------------------------------------------------------------\n");
        printf("Example:\n");
        printf("./a.out -e <-t> <New_Song_Title> <filename.mp3>\n");
        printf("--------------------------------------------------------------------------------------------------------\n");

    }
    else
    {
        printf("--------------------------------------------------------------------------------------------------------\n");
        printf("It is unsupported\n");
        printf("--------------------------------------------------------------------------------------------------------\n");
        return e_failure;
    }
    return e_success;

}


operation_type check_operation_type(char *argv[])
{
    if(strcmp(argv[1],"-v") == 0 )
    {
        return e_view;
    }
    else if (strcmp(argv[1],"-e") ==0 )
    {
        return e_edit;
    }
    else if(strcmp(argv[1],"--help") == 0)
    {
        return e_help;
    }
    else
    {
        return e_unsupported;
    }
}