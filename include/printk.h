#ifndef _PRINTK_H_
#define _PRINTK_H_

#include <types.h>
#include <svga.h>



typedef struct char_pos char_pos;
typedef struct print_spec print_spec;

struct char_pos
{
    uint32 x_resolution;
    uint32 y_resolution;
    uint32 x_cursor;
    uint32 y_cursor;
    uint32 x_char_size;
    uint32 y_char_size;
    uint32 * video_frame_address;
    uint32   video_frame_length;
};

 struct print_spec
 {
    uint32 flags;
    uint32 width;
    uint32 precision;
    uint32 radix;
    int64  value;
 };

char_pos char_pos_info;

void color_putchar(uint32 * video_addr, uint32 window_width, int cursor_x, int cursor_y, int char_color, char ascii_code);

int printk(const char * format, ...);

int putchar(char ascii_code);

#endif