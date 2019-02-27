#include <arch/x64.h>
#include <os/kernel.h>

#define OFFSET_ERR_CODE 0x98

void div_error();
void debug();
void nmi();
void breakpoint();
void overflow();
void out_of_bound();
void undefined_opcode();
void dev_not_available();
void double_fault();
void coprocessor_segment_overrun();
void invalid_TSS();
void segment_not_present();
void stack_segment_fault();
void general_protection();
void page_fault();
void x87_FPU_error();
void alignment_check();
void machine_check();
void SIMD_exception();
void virtualization_exception();

//int 0
void do_div_error(uint64 rsp, uint64 error_code)
{
    uint64 *p = (uint64 *)(rsp + OFFSET_ERR_CODE);
    set_color(COLOR_RED, COLOR_BLACK);
    printk("div(0) error, error_code:%#018lx, rsp:%#018lx, rip:%#018lx\n", error_code, rsp, *p);
    set_color(COLOR_WHITE, COLOR_BLACK);
    while (1)
        ;
}
//int 1
void do_debug(uint64 rsp, uint64 error_code)
{
    uint64 *p = (uint64 *)(rsp + OFFSET_ERR_CODE);
    set_color(COLOR_RED, COLOR_BLACK);
    printk("do_debug(1), error_code:%#018lx, rsp:%#018lx, rip:%#018lx\n", error_code, rsp, *p);
    set_color(COLOR_WHITE, COLOR_BLACK);
    while (1)
        ;
}
//int 2
void do_nmi(uint64 rsp, uint64 error_code)
{
    uint64 *p = (uint64 *)(rsp + OFFSET_ERR_CODE);
    set_color(COLOR_RED, COLOR_BLACK);
    printk("do_nmi(2), error_code:%#018lx, rsp:%#018lx, rip:%#018lx\n", error_code, rsp, *p);
    set_color(COLOR_WHITE, COLOR_BLACK);
    while (1)
        ;
}

//int 3
void do_breakpoint(uint64 rsp, uint64 error_code)
{
    uint64 *p = (uint64 *)(rsp + OFFSET_ERR_CODE);
    set_color(COLOR_RED, COLOR_BLACK);
    printk("do_breakpoint(3), error_code:%#018lx, rsp:%#018lx, rip:%#018lx\n", error_code, rsp, *p);
    set_color(COLOR_WHITE, COLOR_BLACK);
    while (1)
        ;
}

//int 4
void do_overflow(uint64 rsp, uint64 error_code)
{
    uint64 *p = (uint64 *)(rsp + OFFSET_ERR_CODE);
    set_color(COLOR_RED, COLOR_BLACK);
    printk("do_overflow(4), error_code:%#018lx, rsp:%#018lx, rip:%#018lx\n", error_code, rsp, *p);
    set_color(COLOR_WHITE, COLOR_BLACK);
    while (1)
        ;
}

//int 5
void do_out_of_bound(uint64 rsp, uint64 error_code)
{
    uint64 *p = (uint64 *)(rsp + OFFSET_ERR_CODE);
    set_color(COLOR_RED, COLOR_BLACK);
    printk("do_out_of_bound(5), error_code:%#018lx, rsp:%#018lx, rip:%#018lx\n", error_code, rsp, *p);
    set_color(COLOR_WHITE, COLOR_BLACK);
    while (1)
        ;
}
//int 6
void do_undefined_opcode(uint64 rsp, uint64 error_code)
{
    uint64 *p = (uint64 *)(rsp + OFFSET_ERR_CODE);
    set_color(COLOR_RED, COLOR_BLACK);
    printk("do_undefined_opcode(6), error_code:%#018lx, rsp:%#018lx, rip:%#018lx\n", error_code, rsp, *p);
    set_color(COLOR_WHITE, COLOR_BLACK);
    while (1)
        ;
}
//int 7
void do_dev_not_available(uint64 rsp, uint64 error_code)
{
    uint64 *p = (uint64 *)(rsp + OFFSET_ERR_CODE);
    set_color(COLOR_RED, COLOR_BLACK);
    printk("do_dev_not_available(7), error_code:%#018lx, rsp:%#018lx, rip:%#018lx\n", error_code, rsp, *p);
    set_color(COLOR_WHITE, COLOR_BLACK);
    while (1)
        ;
}

//int 8
void do_double_fault(uint64 rsp, uint64 error_code)
{
    uint64 *p = (uint64 *)(rsp + OFFSET_ERR_CODE);
    set_color(COLOR_RED, COLOR_BLACK);
    printk("do_double_fault(8), error_code:%#018lx, rsp:%#018lx, rip:%#018lx\n", error_code, rsp, *p);
    set_color(COLOR_WHITE, COLOR_BLACK);
    while (1)
        ;
}

//int 9
void do_coprocessor_segment_overrun(uint64 rsp, uint64 error_code)
{
    uint64 *p = (uint64 *)(rsp + OFFSET_ERR_CODE);
    set_color(COLOR_RED, COLOR_BLACK);
    printk("do_coprocessor_segment_overrun(9), error_code:%#018lx, rsp:%#018lx, rip:%#018lx\n", error_code, rsp, *p);
    set_color(COLOR_WHITE, COLOR_BLACK);
    while (1)
        ;
}

//int 10
void do_invalid_TSS(uint64 rsp, uint64 error_code)
{
    uint64 *p = (uint64 *)(rsp + OFFSET_ERR_CODE);
    set_color(COLOR_RED, COLOR_BLACK);
    printk("do_invalid_TSS(10), error_code:%#018lx, rsp:%#018lx, rip:%#018lx\n", error_code, rsp, *p);

    if (error_code & 0x01)
        printk("The exception occurred during delivery of an event external to the program,such as an interrupt or an earlier exception.\n");

    if (error_code & 0x02)
        printk("Refers to a gate descriptor in the IDT;\n");
    else
        printk("Refers to a descriptor in the GDT or the current LDT;\n");

    if ((error_code & 0x02) == 0)
        if (error_code & 0x04)
            printk("Refers to a segment or gate descriptor in the LDT;\n");
        else
            printk("Refers to a descriptor in the current GDT;\n");

    printk("Segment Selector Index:%#010x\n", error_code & 0xfff8);
    set_color(COLOR_WHITE, COLOR_BLACK);
    while (1)
        ;
}

//int 11
void do_segment_not_present(uint64 rsp, uint64 error_code)
{
    uint64 *p = (uint64 *)(rsp + OFFSET_ERR_CODE);
    set_color(COLOR_RED, COLOR_BLACK);
    printk("do_segment_not_present(11), error_code:%#018lx, rsp:%#018lx, rip:%#018lx\n", error_code, rsp, *p);

    if (error_code & 0x01)
        printk("The exception occurred during delivery of an event external to the program,such as an interrupt or an earlier exception.\n");

    if (error_code & 0x02)
        printk("Refers to a gate descriptor in the IDT;\n");
    else
        printk("Refers to a descriptor in the GDT or the current LDT;\n");

    if ((error_code & 0x02) == 0)
        if (error_code & 0x04)
            printk("Refers to a segment or gate descriptor in the LDT;\n");
        else
            printk("Refers to a descriptor in the current GDT;\n");

    printk("Segment Selector Index:%#010x\n", error_code & 0xfff8);
    set_color(COLOR_WHITE, COLOR_BLACK);
    while (1)
        ;
}

//int 12
void do_stack_segment_fault(uint64 rsp, uint64 error_code)
{
    uint64 *p = (uint64 *)(rsp + OFFSET_ERR_CODE);
    set_color(COLOR_RED, COLOR_BLACK);
    printk("do_stack_segment_fault(12), error_code:%#018lx, rsp:%#018lx, rip:%#018lx\n", error_code, rsp, *p);

    if (error_code & 0x01)
        printk("The exception occurred during delivery of an event external to the program,such as an interrupt or an earlier exception.\n");

    if (error_code & 0x02)
        printk("Refers to a gate descriptor in the IDT;\n");
    else
        printk("Refers to a descriptor in the GDT or the current LDT;\n");

    if ((error_code & 0x02) == 0)
        if (error_code & 0x04)
            printk("Refers to a segment or gate descriptor in the LDT;\n");
        else
            printk("Refers to a descriptor in the current GDT;\n");

    printk("Segment Selector Index:%#010x\n", error_code & 0xfff8);
    set_color(COLOR_WHITE, COLOR_BLACK);
    while (1)
        ;
}

//int 13
void do_general_protection(uint64 rsp, uint64 error_code)
{
    uint64 *p = (uint64 *)(rsp + OFFSET_ERR_CODE);

    set_color(COLOR_RED, COLOR_BLACK);

    printk("do_general_protection(13), error_code:%#018lx, rsp:%#018lx, rip:%#018lx\n", error_code, rsp, *p);

    if (error_code & 0x01)
        printk("The exception occurred during delivery of an event external to the program,such as an interrupt or an earlier exception.\n");

    if (error_code & 0x02)
        printk("Refers to a gate descriptor in the IDT;\n");
    else
        printk("Refers to a descriptor in the GDT or the current LDT;\n");

    if ((error_code & 0x02) == 0)
        if (error_code & 0x04)
            printk("Refers to a segment or gate descriptor in the LDT;\n");
        else
            printk("Refers to a descriptor in the current GDT;\n");

    printk("Segment Selector Index:%#010x\n", error_code & 0xfff8);

    set_color(COLOR_WHITE, COLOR_BLACK);

    while (1)
        ;
}

//int 14
void do_page_fault(uint64 rsp, uint64 error_code)
{
    uint64 *p = (uint64 *)(rsp + OFFSET_ERR_CODE);
    uint64 cr2 = 0;

    __asm__ __volatile__("movq	%%cr2,	%0"
                         : "=r"(cr2)
                         :
                         : "memory");
    set_color(COLOR_RED, COLOR_BLACK);

    printk("do_page_fault(14), error_code:%#018lx, rsp:%#018lx, rip:%#018lx\n", error_code, rsp, *p);

    if (!(error_code & 0x01))
        printk("page not-present,\t");

    if (error_code & 0x02)
        printk("write cause Fault,\t");
    else
        printk("read cause Fault,\t");

    if (error_code & 0x04)
        printk("fault in user(3)\t");
    else
        printk("fault in supervisor(0,1,2)\t");

    if (error_code & 0x08)
        printk(", reserved bit cause fault\t");

    if (error_code & 0x10)
        printk(",instruction fetch cause fault");

    printk("\n");

    printk("cr2:%#018lx\n", cr2);

    set_color(COLOR_WHITE, COLOR_BLACK);

    while (1)
        ;
}

//int 16
void do_x87_FPU_error(uint64 rsp, uint64 error_code)
{
    uint64 *p = (uint64 *)(rsp + OFFSET_ERR_CODE);
    set_color(COLOR_RED, COLOR_BLACK);
    printk("do_x87_FPU_error(16), error_code:%#018lx, rsp:%#018lx, rip:%#018lx\n", error_code, rsp, *p);
    set_color(COLOR_WHITE, COLOR_BLACK);
    while (1)
        ;
}

//int 17
void do_alignment_check(uint64 rsp, uint64 error_code)
{
    uint64 *p = (uint64 *)(rsp + OFFSET_ERR_CODE);
    set_color(COLOR_RED, COLOR_BLACK);
    printk("do_alignment_check(17), error_code:%#018lx, rsp:%#018lx, rip:%#018lx\n", error_code, rsp, *p);
    set_color(COLOR_WHITE, COLOR_BLACK);
    while (1)
        ;
}

//int 18
void do_machine_check(uint64 rsp, uint64 error_code)
{
    uint64 *p = (uint64 *)(rsp + OFFSET_ERR_CODE);
    set_color(COLOR_RED, COLOR_BLACK);
    printk("do_machine_check(18), error_code:%#018lx, rsp:%#018lx, rip:%#018lx\n", error_code, rsp, *p);
    set_color(COLOR_WHITE, COLOR_BLACK);
    while (1)
        ;
}

//int 19
void do_SIMD_exception(uint64 rsp, uint64 error_code)
{
    uint64 *p = (uint64 *)(rsp + OFFSET_ERR_CODE);
    set_color(COLOR_RED, COLOR_BLACK);
    printk("do_SIMD_exception(19), error_code:%#018lx, rsp:%#018lx, rip:%#018lx\n", error_code, rsp, *p);
    set_color(COLOR_WHITE, COLOR_BLACK);
    while (1)
        ;
}

//int 10
void do_virtualization_exception(uint64 rsp, uint64 error_code)
{
    uint64 *p = (uint64 *)(rsp + OFFSET_ERR_CODE);
    set_color(COLOR_RED, COLOR_BLACK);
    printk("do_virtualization_exception(20), error_code:%#018lx, rsp:%#018lx, rip:%#018lx\n", error_code, rsp, *p);
    set_color(COLOR_WHITE, COLOR_BLACK);
    while (1)
        ;
}

void set_sys_vector()
{
    set_trap_gate(0, 1, div_error);
    set_trap_gate(1, 1, debug);
    set_sys_gate(2, 1, nmi);
    set_sys_gate(3, 1, breakpoint);
    set_sys_gate(4, 1, overflow);
    set_trap_gate(5, 1, out_of_bound);
    set_trap_gate(6, 1, undefined_opcode);
    set_trap_gate(7, 1, dev_not_available);
    set_trap_gate(8, 1, double_fault);
    set_trap_gate(9, 1, coprocessor_segment_overrun);
    set_trap_gate(10, 1, invalid_TSS);
    set_trap_gate(11, 1, segment_not_present);
    set_trap_gate(12, 1, stack_segment_fault);
    set_trap_gate(13, 1, general_protection);
    set_trap_gate(14, 1, page_fault);
    //15 Intel reserved. Do not use.
    set_trap_gate(16, 1, x87_FPU_error);
    set_trap_gate(17, 1, alignment_check);
    set_trap_gate(18, 1, machine_check);
    set_trap_gate(19, 1, SIMD_exception);
    set_trap_gate(20, 1, virtualization_exception);
}