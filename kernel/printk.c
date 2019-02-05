#include <printk.h>
#include <stdarg.h>
#include <stddef.h>
#include <asm/linkage.h>
#include <asm/div.h>
#include <types.h>
#include <font.h>

#define is_digit(a) \
    ((a) >= '0' && (a) <= '9')

#define ZERO_PAD    0           /*填充0*/
#define SIGNED      1           /*有符号*/
#define SMALL       2           /*小写*/
#define PLUS        4           /*显示+*/
#define LEFT        8           /*左对齐*/
#define SPECIAL     16          /*0x*/
#define SPACE       32          /*空格填充*/


void color_putchar(uint32 * video_addr, uint32 window_width, int cursor_x, int cursor_y, int char_color, char ascii_code)
{
    int32 i, j;
    char * char_bitmap = font_ascii[ascii_code];
    uint32 bits;

    uint32 * tmp_video_ptr =  video_addr;

    for(i = 0; i < 16; i++)
    {
        video_addr = tmp_video_ptr + (cursor_y + i) * window_width + cursor_x;
        bits = 0x100;
        for(j = 0; j < 8; j++)
        {
            bits >>= 1;
            if(char_bitmap[i] & bits)
                video_addr[j] = char_color;
            else
                video_addr[j] = COLOR_BLACK;
        }
    }
}

static void new_line()
{
    char_pos_info.y_cursor += 16;
    char_pos_info.x_cursor = 0;    
}

int putchar(char ascii_code){
    uint32 tmp;
    if(ascii_code == '\n')
        new_line();
    else if(ascii_code == '\t')
    {
        tmp = ((char_pos_info.x_cursor >> 6) + 1) << 6;
        if(tmp >= char_pos_info.x_resolution)
            new_line();
        else
            char_pos_info.x_cursor = tmp;
    }  
    else
    {
        color_putchar(
            (uint32 *)0xffff800000a00000, 
            char_pos_info.x_resolution, 
            char_pos_info.x_cursor, 
            char_pos_info.y_cursor, 
            COLOR_WHITE, 
            ascii_code
            );
        
        if(char_pos_info.x_cursor + 8 >= char_pos_info.x_resolution)
           new_line();
        else
            char_pos_info.x_cursor += 8; 
    }
    return ascii_code;
}

static int skip_atoi(char ** str)
{
    int i = 0;
    while(is_digit(**str))
    {
        i = i * 10 + *(*str++) - '0';
    }
    return i;
}

static char * itoa(uint64 value, char * buff, int32 size, int32 precision, int32 radix, int32 flags)
{
    char * num_tbl = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char num_list[64];
    int last = 0;
    char ch;
    char sign = 0;



    if(SMALL & flags)
        num_tbl = "0123456789abcdefghijklmnopqrstuvwxyz";

    if(flags & LEFT)
        flags &= ~ZERO_PAD;
    
    if(radix < 2 || radix > 36)
        return NULL;

    ch = (flags & ZERO_PAD) ? '0' : ' ';

    if(flags & SIGNED)
        if(value < 0)
        {
            value = -value; 
            sign = '-';
        }      
        else
             sign = (flags & PLUS) ?  '+' : ((flags & SPACE) ? ' ' : 0);

    if(sign != 0)
        size--;

    if(flags & SPECIAL)
    {
        if(radix == 8)
            size--;          
        else if(radix == 16)
        {
            size -= 2;
        }       
    }

    if(sign)
        *buff++ = sign;

    if(value == 0)
        num_list[last++] = '0';
    else
     {
        while(value)
            num_list[last++] = num_tbl[do_div(value, radix)];
     }

     if(precision < last)
            precision = last;
      size -=  precision;

    if(~(flags &(LEFT | ZERO_PAD)))
        while(size--)
            *buff++ = ' ';
    //%#         
    if(SPECIAL & flags)
     {
        if(radix == 8)
            *buff++ = '0'; 

        if(radix == 16)
            *buff++ = '0';
            *buff++ = num_tbl[33];
     }
    //% 
     if(!(flags & LEFT))
        while(size > 0)     //宽度
        {
            *buff++ = ch;   
            size--;
        }     
    while(last < precision)
    {
        *buff++ = '0';
        precision--;
    }

    while(last--){
        *buff++ = num_list[last];
    }

    while(size--)                       
        *buff++ = ' ';
         
    return buff;
}