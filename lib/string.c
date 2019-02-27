#include <string.h>

int strlen(const char * str)
{
    register int len = 0;
    while(*str++)
        len++;
    return len;
}

int strcmp(const char * str1, const char * str2)
{
    for( ;*str1 == *str2; str1++, str2++)
        if(*str1 == '\0')
            return 0;
    return *str1 > *str2 ? 1 : -1;
}

void memcpy(void * dest, void * src, int data_size)
{
    while(data_size--)
    {
        *(char *)dest = *(char *)src;
        (char *)dest++;
        (char *)src++;
    }
}

char * strcpy(char * dest, const char * src)
{
    register char * tmp = dest;
    while(*src)
    {
        *tmp = *src;
        src++;
        tmp++;
    }
    *tmp = '\0';
    return dest;
}

char * strcat(char * dest, const char * src)
{
    register char * tmp = dest;
     while(*tmp)
        tmp++;
    strcpy(tmp, src);
	return dest;
}

char * strncpy(char * dest, const char * src, int count)
{
    __asm__ __volatile__ ("cld;         \
        1: decl %2;                     \
        js 2f;                          \
        lodsb;                          \
        stosb;                          \
        testb %%al, %%al;               \
        jne 1b;                         \
        rep;                            \
        stosb;                          \
        2:;"
        :
        :"S"(src), "D"(dest), "c"(count)
        :
    );
    return dest;
} 