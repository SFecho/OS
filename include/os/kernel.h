#ifndef _KERNAL_H_
#define _KERNAL_H_

#include <types.h>
#include <svga.h>
#include <stdarg.h>

void set_color(unsigned int font_color, unsigned int bg_color);

int printk(const char *fmt, ...);

int vsprintf(char *buf, const char *fmt, va_list args);

#endif