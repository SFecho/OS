#ifndef _STRING_H_
#define _STRING_H_

int strlen(const char * str);

int strcmp(const char * first, const char * second);

void memcpy(void * dest, void * src, int size);

char * strcpy(char * dest, const char * src);

char * strncpy(char * dest, const char * src, int count);

char * strcat(char * dest, const char * src);

char * strstr(char * src, char * pattern);

#endif