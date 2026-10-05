#ifndef MP3_TAG_READER_H
#define MP3_TAG_READER_H

#include<stdio.h>

int check_id3_header(FILE *fp);
int read_tag_size(FILE *fp);
void view_mp3(char *filename);

#endif