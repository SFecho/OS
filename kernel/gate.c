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
