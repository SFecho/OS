#include <string.h>

int strlen(const char * str)
{
    register int __res __asm__("cx");
    __asm__ __volatile__ ("cld;  \
            repne;              \
            scasb;              \
            notl %0;            \
            decl %0"
            :"=c"(__res) /*output list*/
            :"D"(str),"a"(0), "0"(0xffffffff)/*Input list*/
            :
    );
    return __res;
}

int strcmp(const char * str1, const char * str2)
{
    register int __res __asm__("ax");
    __asm__ __volatile__ ("cld; \
            1: lodsb;           \
            scasb;              \
            jne 2f;             \
            testb %%al, %%al;   \
            jne 1b;             \
            xorl %%eax, %%eax;  \
            jmp 3f;             \
            2: movl $1, %%eax;  \
            jl 3f;              \
            negl %%eax;         \
            3:"
            :"=a"(__res)
            :"S"(str1), "D"(str2)
            :
    );
    return __res;
}

void memcpy(void * dest, void * src, int data_size)
{
    __asm__ __volatile__ ("cld; \
            rep;                \
            movsb;"
            : /*Output list*/
            :"S"(src), "D"(dest),  "c"(data_size) /*Input list*/
            :"ax"
    );
}

char * strcpy(char * dest, const char * src)
{
    __asm__ __volatile__ ("cld; \
            1: lodsb;           \
            stosb;              \
            testb %%al, %%al;   \
            jnz 1b"
            :
            :"S"(src), "D"(dest)
            :"ax"
    );

    return dest;
}

char * strcat(char * dest, const char * src)
{
    __asm__ __volatile__ ("cld;         \
                    repnz;              \
                    scasb;              \
                    decq %1;            \
                    1: lodsb;           \
                    stosb;              \
                    testb %%al, %%al;   \
                    jnz 1b"
    :
    :"S"(src), "D"(dest), "a"(0), "c"(0xffffffff)
    :
    );

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