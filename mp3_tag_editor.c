#include "mp3_tag_editor.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void edit_mp3(char *filename,char *frame_name,char *new_value)
{
    FILE *fp;
    char frame_id[5];
    unsigned char size[4];
    unsigned char flags[2];
    int frame_size;
    long data_position;
    char *data;
    int new_size;
    fp = fopen(filename, "r+b");

    if(fp==NULL)
    {
        printf("ERROR: Unable to open file\n");
        return;
    }
    char id[4]={0};
    unsigned char version[2];
    fread(id,1,3,fp);
    fread(version,1,2,fp);

    if(strcmp(id,"ID3")!=0||version[0]!=0x03||version[1]!=0x00)
    {
        printf("ERROR: Invalid ID3v2.3 file\n");
        fclose(fp);
        return;
    }

    fseek(fp,10,SEEK_SET);
    while(1)
    {
        if (fread(frame_id,1,4,fp)!=4)
            break;
        frame_id[4]='\0';
        if(frame_id[0]==0)
            break;

        if(fread(size,1,4,fp)!=4)
            break;

        if(fread(flags,1,2,fp)!=2)
            break;

        frame_size=(size[0]<<24)|(size[1]<<16)|(size[2]<<8)|size[3];
        if(frame_size<=0||frame_size>100000)
            break;

        data_position=ftell(fp);

        if(strcmp(frame_id,frame_name)==0)
        {
            data=malloc(frame_size);
            if(data==NULL)
            {
                fclose(fp);
                return;
            }

            fseek(fp,data_position,SEEK_SET);

            if(fread(data,1,frame_size,fp)!=(size_t)frame_size)
            {
                free(data);
                fclose(fp);
                return;
            }

            memset(data,0,frame_size);

            if(strcmp(frame_name,"COMM")==0)
            {
                new_size=strlen(new_value)+5;

                if(new_size>frame_size)
                {
                    printf("New comment is too long\n");
                    free(data);
                    fclose(fp);
                    return;
                }

                data[0]=0;

                data[1]='e';
                data[2]='n';
                data[3]='g';
                data[4]=0;

                strcpy(data+5,new_value);
            }
            else
            {
                new_size = strlen(new_value) + 1;

                if (new_size>frame_size)
                {
                    printf("New value is longer than existing frame.\n");
                    printf("Current basic editor cannot expand this frame safely.\n");
                    free(data);
                    fclose(fp);
                    return;
                }

                data[0]=0;
                strcpy(data+1,new_value);
            }

            fseek(fp,data_position,SEEK_SET);
            fwrite(data,1,frame_size,fp);
            free(data);
            printf("Tag updated successfully\n");
            fclose(fp);
            return;
        }

        fseek(fp,frame_size,SEEK_CUR);
    }
    fclose(fp);
    printf("ERROR: Tag not found\n");
}