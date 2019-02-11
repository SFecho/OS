#ifndef _PRINTK_H_
#define _PRINTK_H_

#include <types.h>
#include <svga.h>
#include <stdarg.h>

typedef struct font_struct font_struct;
typedef struct font_color font_color;

struct font_struct
{
    uint32 x_resolution;
    uint32 y_resolution;
    uint32 x_cursor;
    uint32 y_cursor;
    uint32 x_char_size;
    uint32 y_char_size;
    uint32 font_color;
    uint32 bg_color;
    uint32 * video_frame_address;
    uint32   video_frame_length;
};


font_struct char_pos_info;

void color_putchar(uint32 * video_addr, int32 window_width, int32 cursor_x, int32 cursor_y, uint8 ascii_code);

void set_color(unsigned int font_color,unsigned int bg_color);

int printk(const char * fmt, ...);

int vsprintf(char * buf, const char *fmt, va_list args);

#endif