#include<stdio.h>
#include<string.h>
#include "types.h"
#include "mp3_tag_reader.h"
#include "mp3_tag_editor.h"

void print_help()
{
    printf("\nMP3 TAG READER AND EDITOR\n\n");
    printf("-v    View MP3 file contents\n");
    printf("-e -t <new title> <mp3 file>\n");
    printf("-e -a <new artist> <mp3 file>\n");
    printf("-e -A <new album> <mp3 file>\n");
    printf("-e -y <new year> <mp3 file>\n");
    printf("-e -m <new music> <mp3 file>\n");
    printf("-e -c <new comment> <mp3 file>\n");
}

void print_usage()
{
    printf("ERROR: Invalid arguments\n\n");
    printf("Usage:\n");
    printf("./a.out -v <mp3 file>\n");
    printf("./a.out -e -t <new title> <mp3 file>\n");
    printf("./a.out -e -a <new artist> <mp3 file>\n");
    printf("./a.out -e -A <new album> <mp3 file>\n");
    printf("./a.out -e -y <new year> <mp3 file>\n");
    printf("./a.out -e -m <new music> <mp3 file>\n");
    printf("./a.out -e -c <new comment> <mp3 file>\n");
}

int main(int argc,char *argv[])
{
    if(argc==1)
    {
        print_usage();
        return 0;
    }
    if(strcmp(argv[1],"--help")==0)
    {
        print_help();
        return 0;
    }

    if(strcmp(argv[1],"-v")==0)
    {
        if(argc!=3)
        {
            print_usage();
            return 0;
        }
        printf("Selected View Operation\n");
        view_mp3(argv[2]);
        return 0;
    }

    if(strcmp(argv[1],"-e")==0)
    {
        if(argc!=5)
        {
            print_usage();
            return 0;
        }

        printf("Selected Edit Operation\n");
        if (strcmp(argv[2],"-t")==0)
        {
            edit_mp3(argv[4],"TIT2",argv[3]);
        }
        else if(strcmp(argv[2],"-a")==0)
        {
            edit_mp3(argv[4],"TPE1",argv[3]);
        }
        else if(strcmp(argv[2],"-A")==0)
        {
            edit_mp3(argv[4],"TALB",argv[3]);
        }
        else if(strcmp(argv[2],"-y")==0)
        {
            edit_mp3(argv[4],"TYER",argv[3]);
        }
        else if(strcmp(argv[2],"-m")==0)
        {
            edit_mp3(argv[4],"TCON",argv[3]);
        }
        else if(strcmp(argv[2],"-c")==0)
        {
            edit_mp3(argv[4],"COMM",argv[3]);
        }
        else
        {
            print_usage();
        }

        return 0;
    }
    print_usage();
    return 0;
}