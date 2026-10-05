#include "mp3_tag_reader.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int check_id3_header(FILE *fp)
{
    char id[4] = {0};
    unsigned char version[2];

    fseek(fp, 0, SEEK_SET);

    if (fread(id, 1, 3, fp) != 3)
        return 0;

    if (strcmp(id, "ID3") != 0)
        return 0;

    if (fread(version, 1, 2, fp) != 2)
        return 0;

    if (version[0] != 0x03 || version[1] != 0x00)
        return 0;

    return 1;
}

int read_tag_size(FILE *fp)
{
    unsigned char size[4];
    int tag_size;

    fseek(fp, 6, SEEK_SET);

    if (fread(size, 1, 4, fp) != 4)
        return 0;

    tag_size = (size[0] << 21) |
               (size[1] << 14) |
               (size[2] << 7) |
               size[3];

    return tag_size;
}

void view_mp3(char *filename)
{
    FILE *fp;
    int tag_size;
    int bytes_read = 0;

    char frame_id[5];
    unsigned char size[4];
    unsigned char flags[2];
    int frame_size;

    char *data;

    char title[500] = "";
    char artist[500] = "";
    char album[500] = "";
    char year[100] = "";
    char music[500] = "";
    char composer[500] = "";
    char comment[500] = "";

    fp = fopen(filename, "rb");

    if (fp == NULL)
    {
        printf("ERROR: Unable to open file\n");
        return;
    }

    if (!check_id3_header(fp))
    {
        printf("ERROR: ID3v2.3 tag not found\n");
        fclose(fp);
        return;
    }

    printf("ID3v2.3 tag found\n");

    tag_size = read_tag_size(fp);

    fseek(fp, 10, SEEK_SET);

    while (bytes_read < tag_size)
    {
        if (fread(frame_id, 1, 4, fp) != 4)
            break;

        frame_id[4] = '\0';

        if (frame_id[0] == 0)
            break;

        if (fread(size, 1, 4, fp) != 4)
            break;

        fread(flags, 1, 2, fp);

        frame_size = (size[0] << 24) |
                     (size[1] << 16) |
                     (size[2] << 8) |
                     size[3];

        if (frame_size <= 0 || frame_size > 100000)
            break;

        data = malloc(frame_size + 1);

        if (data == NULL)
            break;

        if (fread(data, 1, frame_size, fp) != (size_t)frame_size)
        {
            free(data);
            break;
        }

        data[frame_size] = '\0';

        if (strcmp(frame_id, "TIT2") == 0)
        {
            strcpy(title, data + 1);
        }
        else if (strcmp(frame_id, "TPE1") == 0)
        {
            strcpy(artist, data + 1);
        }
        else if (strcmp(frame_id, "TALB") == 0)
        {
            strcpy(album, data + 1);
        }
        else if (strcmp(frame_id, "TYER") == 0)
        {
            strcpy(year, data + 1);
        }
        else if (strcmp(frame_id, "TCON") == 0)
        {
            strcpy(music, data + 1);
        }
        else if (strcmp(frame_id, "TCOM") == 0)
        {
            strcpy(composer, data + 1);
        }
        else if (strcmp(frame_id, "COMM") == 0)
{
    int pos = 4;

    while (pos < frame_size && data[pos] != '\0')
    {
        pos++;
    }

    if (pos < frame_size)
    {
        pos++;
        strcpy(comment, data + pos);
    }
}

        free(data);

        bytes_read = ftell(fp) - 10;
    }

    printf("\n--------------- MP3 TAG DETAILS ---------------\n");
    printf("Title       : %s\n", title);
    printf("Artist      : %s\n", artist);
    printf("Album       : %s\n", album);
    printf("Year        : %s\n", year);
    printf("Music       : %s\n", music);
    printf("Composer    : %s\n", composer);
    printf("Comment     : %s\n", comment);
    printf("------------------------------------------------\n");

    fclose(fp);
}