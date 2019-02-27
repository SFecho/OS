#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <types.h>

typedef struct memory_e820 memory_e820;
typedef struct memory_desc memory_desc;

struct memory_e820
{
    uint64 addr;
    uint64 length;
    uint32 type;
} __attribute__((packed));

struct memory_desc
{
    memory_e820 e820[32];
    uint64 e820_length;
};

extern memory_desc memory_manage_struct;

void init_memory();

#endif