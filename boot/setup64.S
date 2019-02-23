#include <asm/boot.h>

.code32
.section .text
	
	mov $0x10, %ax 
	mov %ax, %ds
	mov %ax, %ss
	mov %ax, %es
	mov %ax, %gs
	mov %ax, %fs
	movl $0x9fff0, %esp
		
	call support_long_mode
	
	test %eax, %eax
	jz no_support
	
	movl $0x11007, 0x10000 
	movl $0, 0x10004
	
	movl $0x11007, 0x10800	
	movl $0, 0x10804
	
	movl $0x12007, 0x11000
	movl $0, 0x11004
	
	movl $0x000083, 0x12000   
	movl $0, 0x12004
	
	movl $0x200083, 0x12008
	movl $0, 0x1200c
	
	movl $0x400083, 0x12010
	movl $0, 0x12014
	
	movl $0x600083, 0x12018
	movl $0, 0x1201c
	
	movl $0x800083, 0x12020
	movl $0, 0x12024
	
	movl $0xa00083, 0x12028
	movl $0x0, 0x1202c
	
	movl $gdt64, %eax
	addl $0x91000, %eax
	movl $0x91000, %ebx
	addl $gdt64_ptr, %ebx
	
	movl %eax, %ds:2(%ebx)
	
	lgdt (%ebx)
	
	mov $SELECTOR_DATA64, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	mov $0x9fff0, %esp
	
	
	mov	%cr4, %eax
	bts $5, %eax
	mov %eax, %cr4		
	
	mov $0x10000, %eax
	mov %eax, %cr3
	
	mov $0xc0000080, %ecx
	rdmsr					
	
	bts $8, %eax			
	wrmsr
	
	mov %cr0, %eax
	bts $0, %eax
	bts $31, %eax
	mov %eax, %cr0
	
	ljmp $0x08, $0x100000
		
support_long_mode:
	mov $0x80000000, %eax
	cpuid
	cmp $0x80000001, %eax
	setnb %al
	jb support_long_mode_done
	mov $0x80000001, %eax
	cpuid
	bt $29, %edx
	setc %al
	
support_long_mode_done:
	movzx %al, %eax
	ret
	
no_support:	
	jmp .	
	

	
.section .gdt64
gdt64:
	.quad 0x0000000000000000	
desc_code64:	
	.quad 0x0020980000000000	
desc_data64:
	.quad 0x0000920000000000
	
.equ GDT64_LEN, . - gdt64

.equ SELECTOR_CODE64, desc_code64 - gdt64
.equ SELECTOR_DATA64, desc_data64 - gdt64

gdt64_ptr:
	.word	GDT64_LEN - 1
	.long	0


#end of	