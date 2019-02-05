#ifndef _X86_H__
#define _X86_H__

#define DA_32		0x4000	
#define DA_DPL0		0x00	

#define DA_DPL1		0x20
#define DA_DPL2		0x40
#define DA_DPL3		0x60

#define DA_386TSS	0x89
	
#define DA_LDT		0x82

#define DA_DR		0x90
#define DA_DRW		0x92
#define DA_DRWA		0x93

#define DA_C		0x98
#define DA_CR		0x9a
#define DA_CCO		0x9c
#define DA_CCOR		0x9e

#define DA_TASKGATE		85h	
#define DA_386CGATE		0x8c
#define DA_386IGATE		0x8e
#define DA_386TGATE		0x8f


#define SA_TIL			0x4			
#define SA_TIG			0x0			

#define SA_RPL0			0x0
#define SA_RPL1			0x1
#define SA_RPL2			0x2
#define SA_RPL3			0x3

#define cli()   __asm__ __volatile__ ("cli")
#define sti()   __asm__ __volatile__ ("sti")
#endif