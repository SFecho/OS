#include <asm/linkage.h>

#define DO_DIV_ERROR    (17 * 8)
#define DIV_ERROR_CODE  (18 * 8)
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
   movq DO_DIV_ERROR(%rsp), %rbx
   movq $0x10, %rdi
   movq %rdi, %ds
   movq %rdi, %es
   movq %rsp, %rdi
   movq DIV_ERROR_CODE(%rsp), %rsi
   callq *%rbx
   jmp ret_from_exception