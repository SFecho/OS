#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <types.h>

#define PAGE_GDT_NR 39
#define PAGE_2M_NR  21
#define PAGE_1G_NR  30
#define PAGE_4K_NR  12

#define PAGE_SIZE_4KB   (1UL << PAGE_SIZE_4KB)
#define PAGE_SIZE_2MB   (1UL << PAGE_2M_NR)
#define PAGE_SIZE_1GB   (1UL << PAGE_1G_NR)

#define PAGE_ADDR_OFFSET    ((uint64)0xffff800000000000)

#define PAGE_2M_MASK (~(PAGE_SIZE_2MB - 1))
#define PAGE_1G_MASK (~(PAGE_SIZE_1GB - 1))
#define PAGE_4K_MASK (~(PAGE_SIZE_4KB - 1))

#define PAGE_2M_ALIGN(addr) (((uint64)(addr) + PAGE_SIZE_2MB - 1) & PAGE_2M_MASK)
#define PAGE_4K_ALIGN(addr) (((uint64)(addr) + PAGE_SIZE_4KB - 1) & PAGE_4K_MASK)
#define PAGE_1G_ALIGN(addr) (((uint64)(addr) + PAGE_SIZE_1GB - 1) & PAGE_1G_MASK)

#define virtual_to_physics(addr)    (uint64(addr) - PAGE_ADDR_OFFSET)
#define virtual_to_physics(addr)    ((uint64*)((uint64)(addr) + PAGE_ADDR_OFFSET))

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