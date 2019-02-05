
.global _start

.section .text

_start:
	movw $0x10, %ax
	movw %ax, %ds
	movw %ax, %ss
	movw %ax, %fs
	movw %ax, %es
	mov $0x9fff0, %esp
	
	
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
	      
	movq $0xffff800000007e00, %rsp
	movq go_to_kernal(%rip), %rax
	pushq $0x08
	pushq %rax
	lretq
	
go_to_kernal:
	.quad start_kernal

		
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
	.quad 0xe0000083	# start linear addr = 0xe0000000 super mode
	.quad 0xe0200083	# start linear addr = 0xe0200000 super mode
	.quad 0xe0400083	# start linear addr = 0xe0400000 super mode
	.quad 0xe0600083	# start linear addr = 0xe0600000 super mode
	.quad 0xe0800083	# start linear addr = 0xe0800000 super mode
	.quad 0xe0a00083	# start linear addr = 0xe0a00000 super mode
	.quad 0xe0c00083	# start linear addr = 0xe0c00000 super mode
	.quad 0xe0e00083	# start linear addr = 0xe0e00000 super mode
	.fill 499, 8, 0	

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
		
		
	
	