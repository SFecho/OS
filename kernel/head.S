.global _start

.section .text
_start:
	movw $0x10, %ax
	movw %ax, %ds
	movw %ax, %ss
	movw %ax, %fs
	movw %ax, %es
	mov $0x9fff0, %esp

#################
	xorq %rax, %rax
	movl %ds:0x8228, %eax
	addq $0x83, %rax
	movq $8, %rcx
	movq $0x103028, %rbx
#填充页表	
write_page:
	movq %rax, %ds:(%rbx)
	addq $0x200000, %rax
	addq $8, %rbx
loop write_page	
#################	
	
	lidt idt_ptr(%rip)
	lgdt gdt_ptr(%rip)

	movw $0x10, %ax
	movw %ax, %ds
	movw %ax, %ss
	movw %ax, %gs
	movw %ax, %fs
	movw %ax, %es
	movq $0x9fff0, %rsp
	
	movq $0x101000, %rax
	movq %rax, %cr3
	
	movq x64_seg(%rip), %rax
	
	pushq $0x08
	pushq %rax
	lretq
	
	
x64_seg:
	.quad entry64
	
entry64:
	mov $0x10, %rax
	mov %rax, %ds
	mov %rax, %ss
	mov %rax, %gs
	mov %rax, %es	      
	movq $0xffff80000009fff0, %rsp
setup_idt:
	leaq ignore_int(%rip), %rdx
	xorq %rax, %rax
	movq $(0x08 << 16), %rax
	movq $0x8e0000000000, %r8 
	addq %r8, %rax
	movw %dx, %ax
	movq %rdx, %r8		#####
	shrq $16, %r8
	shlq $48, %r8
	addq %r8, %rax
	shrq $32, %rdx
	leaq idt_tbl(%rip), %rdi
	movq $256, %rcx

write_idt_item:
	movq %rax, %ds:(%rdi)
	movq %rdx, %ds:8(%rdi)
	addq $16, %rdi
	loop write_idt_item


setup_tss64:
	leaq	tss64_tbl(%rip),	%rdx
	xorq	%rax, %rax
	xorq	%rcx, %rcx
	movq	$0x89, %rax
	shlq	$40, %rax
	movl	%edx, %ecx
	shrl	$24, %ecx
	shlq	$56, %rcx
	addq	%rcx, %rax
	xorq	%rcx, %rcx
	movl	%edx, %ecx
	andl	$0xffffff, %ecx
	shlq	$16, %rcx
	addq	%rcx, %rax
	addq	$103, %rax
	leaq	gdt_tbl(%rip), %rdi
	movq	%rax, 64(%rdi)
	shrq	$32, %rdx
	movq	%rdx, 72(%rdi)

	mov	$0x40,	%ax
	ltr	%ax	
#===========================================

	movq go_to_kernal(%rip), %rax
	pushq $0x08
	pushq %rax
	lretq
	
go_to_kernal:
	.quad start_kernal

# 哑中断代码
ignore_int:
	cld
	pushq	%rax
	pushq	%rbx
	pushq	%rcx
	pushq	%rdx
	pushq	%rbp
	pushq	%rdi
	pushq	%rsi

	pushq	%r8
	pushq	%r9
	pushq	%r10
	pushq	%r11
	pushq	%r12
	pushq	%r13
	pushq	%r14
	pushq	%r15

	movq	%es,	%rax
	pushq	%rax
	movq	%ds,	%rax
	pushq	%rax

	movq	$0x10,	%rax
	movq	%rax,	%ds
	movq	%rax,	%es
				
	# 参数自左向右为rdi rsi rdx rcx r8 r9，其他的通过栈

	leaq	int_msg(%rip),	%rax
	pushq	%rax
	movq	%rax,	%rdi
	movq	$0,	%rax
	callq	printk

	addq	$0x8,	%rsp

	# 陷入死循环
	jmp .	

	popq	%rax
	movq	%rax,	%ds
	popq	%rax
	movq	%rax,	%es

	popq	%r15
	popq	%r14
	popq	%r13
	popq	%r12
	popq	%r11
	popq	%r10
	popq	%r9
	popq	%r8

	popq	%rsi
	popq	%rdi
	popq	%rbp
	popq	%rdx
	popq	%rcx
	popq	%rbx
	popq	%rax
	iretq

int_msg:
	.asciz "unknown interrupt or exception at rip\n"

.align 8
.org 0x1000		
_PML4E:
	.quad 0x102007
	.fill 255, 8, 0
	.quad 0x102007
	.fill 255, 8, 0

.org 0x2000	
_PDPTE:	
	.quad 0x103003	
	.fill 511, 8, 0		
	
.org 0x3000	
_PDE:
	.quad 0x000083	# start linear addr = 0x000000 super mode
	.quad 0x200083	# start linear addr = 0x200000 super mode
	.quad 0x400083	# start linear addr = 0x400000 super mode
	.quad 0x600083	# start linear addr = 0x600000 super mode
	.quad 0x800083	# start linear addr = 0x800000 super mode

	.fill 507, 8, 0	

.section .data
############################
#	page tbl
############################
.global gdt_tbl
############################
#	gdt table
############################
gdt_tbl:
	.quad 0x0000000000000000
	.quad 0x0020980000000000	# kernal code
	.quad 0x0000920000000000	# kernal data
	.quad 0x0020f80000000000	# user code
	.quad 0x0000f20000000000	# user data
	.quad 0x00cf9a000000ffff	# 32bit code
	.quad 0x00cf92000000ffff	# 32bit data
	.fill 10, 8, 0				# 80B,  0x0
gdt_tbl_end:
	
gdt_ptr:
	gdt_limit:	
		.word	gdt_tbl_end - gdt_tbl - 1
	gdt_base:	
		.quad	gdt_tbl

.global idt_tbl
############################
#	idt table
############################		
idt_tbl:
	.fill 512, 8, 0
idt_tbl_end:	

idt_ptr:
	idt_limit:
		.word	idt_tbl_end - idt_tbl - 1
	idt_base:
		.quad	idt_tbl
		
.global tss64_tbl
############################
#	tss
############################		
tss64_tbl:
	.fill 13, 8, 0
	
tss64_end:

tss64_tbl_ptr:

tss64_limit:
	.word	tss64_end - tss64_tbl - 1
tss64_base:
	.quad 	tss64_tbl
		
		
	
	