#include <asm/x64.h>
#include <os/kernel.h>

void div_error();

void do_div_error(uint64 rsp, uint64 error_code)
{
    uint64 *p = (uint64 *)(rsp + 0x98);
    set_color(COLOR_RED, COLOR_BLACK);
    printk("div(0) error, error_code:%#018lx, rsp:%#018lx, rip: %#018lx\n", error_code, rsp, *p);
    set_color(COLOR_WHITE, COLOR_BLACK);
    while (1)
        ;
}

void set_sys_vector()
{
    set_trap_gate(0, 1, div_error);
}