#include <asm/linkage.h>

#define ERROR_FUNC    (17 * 8)
#define ERROR_CODE  (18 * 8)
#define RIP (19 * 8)
#define CS  (20 * 8)

pop_all:
    popq %r15;
    popq %r14;
    popq %r13;
    popq %r12;
    popq %r11;
    popq %r10;
    popq %r9;
    popq %r8;
    popq %rbx;
    popq %rcx;
    popq %rdx;
    popq %rsi;
    popq %rdi;
    popq %rbp;
    popq %rax;
    movq %rax, %ds
    popq %rax;
    movq %rax, %es;
    popq %rax;
    iretq;

ret_from_exception:
ENTRY(ret_from_int)
   jmp pop_all     

ENTRY(div_error)
   pushq $0            # 错误码
   pushq %rax          # 错误代码地址
   leaq do_div_error(%rip), %rax
   xchgq %rax, (%rsp)
do_error:    
   pushq %rax
   movq %es, %rax
   pushq %rax
   movq %ds, %rax
   pushq %rax
    
   pushq %rbp
   pushq %rdi
   pushq %rsi
   pushq %rdx
   pushq %rcx
   pushq %rbx
   pushq %r8
   pushq %r9
   pushq %r10
   pushq %r11
   pushq %r12
   pushq %r13
   pushq %r14
   pushq %r15

   cld
 # void do_div_error(uint64 rsp, uint64 error_code)  
   movq ERROR_FUNC(%rsp), %rbx
   movq $0x10, %rdi
   movq %rdi, %ds
   movq %rdi, %es
   movq %rsp, %rdi
   movq ERROR_CODE(%rsp), %rsi
   callq *%rbx
   jmp ret_from_exception

# int2
ENTRY(debug)
   push $0
   push %rax
   leaq do_debug(%rip), %rax
   xchg %rax, (%rsp)
   jmp do_error

# int3
ENTRY(nmi)
   pushq $0            # 错误码
   pushq %rax          # 错误代码地址
   leaq do_nmi(%rip), %rax
   xchgq %rax, (%rsp)
   jmp do_error

# int4
ENTRY(breakpoint)
   pushq $0            # 错误码
   pushq %rax          # 错误代码地址
   leaq do_breakpoint(%rip), %rax
   xchgq %rax, (%rsp)
   jmp do_error

# int5
ENTRY(overflow)
   pushq $0            # 错误码
   pushq %rax          # 错误代码地址
   leaq do_overflow(%rip), %rax
   xchgq %rax, (%rsp)
   jmp do_error

# int6
ENTRY(out_of_bound)
   pushq $0            # 错误码
   pushq %rax          # 错误代码地址
   leaq do_out_of_bound(%rip), %rax
   xchgq %rax, (%rsp)
   jmp do_error   

# int7
ENTRY(undefined_opcode)
   pushq $0            # 错误码
   pushq %rax          # 错误代码地址
   leaq do_undefined_opcode(%rip), %rax
   xchgq %rax, (%rsp)
   jmp do_error      

ENTRY(dev_not_available)	/* need rewrite */
	pushq	$0
	pushq	%rax
	leaq	do_dev_not_available(%rip),	%rax
	xchgq	%rax,	(%rsp)
	jmp	do_error


ENTRY(double_fault)
	pushq	%rax
	leaq	do_double_fault(%rip),	%rax
	xchgq	%rax,	(%rsp)
	jmp	do_error

ENTRY(coprocessor_segment_overrun)
	pushq	$0
	pushq	%rax
	leaq	do_coprocessor_segment_overrun(%rip),	%rax
	xchgq	%rax,	(%rsp)
	jmp	do_error

ENTRY(invalid_TSS)
	pushq	%rax
	leaq	do_invalid_TSS(%rip),	%rax
	xchgq	%rax,	(%rsp)
	jmp	do_error

ENTRY(segment_not_present)
	pushq	%rax
	leaq	do_segment_not_present(%rip),	%rax
	xchgq	%rax,	(%rsp)
	jmp	do_error

ENTRY(stack_segment_fault)
	pushq	%rax
	leaq	do_stack_segment_fault(%rip),	%rax
	xchgq	%rax,	(%rsp)
	jmp	do_error

ENTRY(general_protection)
	pushq	%rax
	leaq	do_general_protection(%rip),	%rax
	xchgq	%rax,	(%rsp)
	jmp	do_error

ENTRY(page_fault)
   pushq	%rax
   leaq do_page_fault(%rip), %rax
   xchgq	%rax,	(%rsp)
   jmp do_error

ENTRY(x87_FPU_error)
   pushq	$0
   pushq	%rax
   leaq	do_x87_FPU_error(%rip),	%rax
   xchgq	%rax,	(%rsp)
   jmp	do_error

ENTRY(alignment_check)
   pushq	%rax
   leaq	do_alignment_check(%rip),	%rax
   xchgq	%rax,	(%rsp)
   jmp	do_error

ENTRY(machine_check)
   pushq	$0
   pushq	%rax
   leaq	do_machine_check(%rip),	%rax
   xchgq	%rax,	(%rsp)
   jmp	do_error

ENTRY(SIMD_exception)
   pushq	$0
   pushq	%rax
   leaq	do_SIMD_exception(%rip),	%rax
   xchgq	%rax,	(%rsp)
   jmp do_error

ENTRY(virtualization_exception)
   pushq	$0
   pushq	%rax
   leaq	do_virtualization_exception(%rip),	%rax
   xchgq	%rax,	(%rsp)
   jmp do_error 