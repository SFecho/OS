#ifndef _DIV_H_
#define _DIV_H_

#define do_div(n, base) ({ \
int __res; \
__asm__("divq %%rcx":"=a" (n),"=d" (__res):"0" (n),"1" (0),"c" (base)); \
__res; })

#endif