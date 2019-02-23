#ifndef _X64_H__
#define _X64_H__

#include <types.h>

typedef struct desc_struct desc_struct;
typedef struct gate_struct gate_struct;

struct desc_struct
{
    uint64 x;
} __attribute__((packed));

struct gate_struct
{
    uint64 x[2];
} __attribute__((packed));

extern desc_struct gdt_tbl[];
extern gate_struct idt_tbl[];
extern uint32 tss64_tbl[26];

#define cli() __asm__ __volatile__("cli")
#define sti() __asm__ __volatile__("sti")

#define set_gate(selector_addr, attr, ist, code_addr)                           \
    do                                                                          \
    {                                                                           \
        \         
        uint64 __d0,                                                            \
            __d1;                                                               \
        __asm__ __volatile__("movw %%dx, %%ax\n\t"                              \
                             "andq $0x7, %%rcx\n\t"                             \
                             "addq %4, %%rcx\n\t"                               \
                             "shlq $32, %%rcx\n\t"                              \
                             "addq %%rcx, %%rax\n\t"                            \
                             "xorq %%rcx, %%rcx\n\t"                            \
                             "movq %%rdx, %%rcx\n\t"                            \
                             "shrq $16, %%rcx\n\t"                              \
                             "shlq $48, %%rcx\n\t"                              \
                             "addq %%rcx, %%rax\n\t"                            \
                             "shrq $32, %%rbx\n\t"                              \
                             "movq %%rax, %0\n\t"                               \
                             "movq %%rbx, %1\n\t"                               \
                             : "=m"(*((uint64 *)(selector_addr))),              \
                               "=m"(*((uint64 *)(selector_addr) + 1)),          \
                               "=&a"(__d0), "=&d"(__d1)                         \
                             : "i"(attr << 8), "3"(code_addr), "2"(0x08 << 16), \
                               "c"(ist)                                         \
                             : "memory");                                       \
    } while (0)

#define load_TR(n)                          \
    do                                      \
    {                                       \
        __asm__ __volatile__("ltr	%%ax\n\t" \
                             :              \
                             : "a"(n << 3)  \
                             : "memory");   \
    } while (0)

void set_int_gate(uint32 number, uint8 ist, void *code);

void set_trap_gate(uint32 number, uint8 ist, void *code);

void set_sys_gate(uint32 number, uint8 ist, void *code);

void set_sys_vector();
#endif