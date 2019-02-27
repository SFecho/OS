#ifndef _KERNAL_H_
#define _KERNAL_H_

#include <types.h>
#include <svga.h>
#include <stdarg.h>

#define offsetof(type, member) \
    ((size_t) & (((type *)0)->member));

#define container_of(ptr, type, member) ({            \
    const typeof(((type *)0)->member) *_tmp_ = (ptr); \
    (type *)((char *)_tmp_ - offsetof(type, member)); \
})

inline void set_color(unsigned int font_color, unsigned int bg_color);

int printk(const char *fmt, ...);

int vsprintf(char *buf, const char *fmt, va_list args);

#endif