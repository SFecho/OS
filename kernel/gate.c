#include <asm/x64.h>

void set_int_gate(uint32 number, uint8 ist, void *code)
{
    set_gate(idt_tbl + number, 0x8e, ist, code);
}

void set_trap_gate(uint32 number, uint8 ist, void *code)
{
    set_gate(idt_tbl + number, 0x8f, ist, code);
}

void set_sys_gate(uint32 number, uint8 ist, void *code)
{
    set_gate(idt_tbl + number, 0xef, ist, code);
}

void set_system_int_gate(uint32 number, uint8 ist, void *code) //int3
{
    set_gate(idt_tbl + number, 0xee, ist, code); //P,DPL=3,TYPE=E
}

void set_tss64(uint64 rsp0, uint64 rsp1, uint64 rsp2, uint64 ist1, uint64 ist2, uint64 ist3,
               uint64 ist4, uint64 ist5, uint64 ist6, uint64 ist7)
{
    *(uint64 *)(tss64_tbl + 1) = rsp0;
    *(uint64 *)(tss64_tbl + 3) = rsp1;
    *(uint64 *)(tss64_tbl + 5) = rsp2;

    *(uint64 *)(tss64_tbl + 9) = ist1;
    *(uint64 *)(tss64_tbl + 11) = ist2;
    *(uint64 *)(tss64_tbl + 13) = ist3;
    *(uint64 *)(tss64_tbl + 15) = ist4;
    *(uint64 *)(tss64_tbl + 17) = ist5;
    *(uint64 *)(tss64_tbl + 19) = ist6;
    *(uint64 *)(tss64_tbl + 21) = ist7;
}