#include <asm/linkage.h>
#include <os/kernel.h>
#include <stddef.h>
#include <asm/div.h>
#include <string.h>
#include <os/font.h>

char buff[4096];

#define is_digit(a) \
	((a) >= '0' && (a) <= '9')

#define ZERO_PAD 1 /*填充0*/
#define SIGNED 2   /*有符号*/
#define SMALL 4	/*小写*/
#define PLUS 8	 /*显示+*/
#define LEFT 16	/*左对齐*/
#define SPECIAL 32 /*0x*/
#define SPACE 64   /*空格填充*/

static void putchar(uint32 *video_addr, int32 window_width, int32 cursor_x, int32 cursor_y, uint8 ascii_code)
{
	int32 i, j;
	char *char_bitmap = font_ascii[ascii_code];
	uint32 bits;

	uint32 *tmp_video_ptr = video_addr;

	for (i = 0; i < 16; i++)
	{
		video_addr = tmp_video_ptr + (cursor_y + i) * window_width + cursor_x;
		bits = 0x100;
		for (j = 0; j < 8; j++)
		{
			bits >>= 1;
			if (char_bitmap[i] & bits)
				video_addr[j] = char_pos_info.font_color;
			else
				video_addr[j] = char_pos_info.bg_color;
		}
	}
}

static int skip_atoi(const char **str)
{
	int i = 0;
	while (is_digit(**str))
	{
		i = i * 10 + *((*str)++) - '0';
	}
	return i;
}

static char *itoa(char *str, int64 value, int32 radix, int32 size, int32 precision, int32 flags)
{
	char *num_tbl = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char num_list[64];
	int last = 0;
	char ch;
	char sign = 0;

	if (SMALL & flags)
		num_tbl = "0123456789abcdefghijklmnopqrstuvwxyz";

	if (flags & LEFT)
		flags &= ~ZERO_PAD;

	if (radix < 2 || radix > 36)
		return NULL;

	ch = (flags & ZERO_PAD) ? '0' : ' ';

	if (flags & SIGNED && value < 0)
	{
		value = -value;
		sign = '-';
	}
	else
		sign = (flags & PLUS) ? '+' : ((flags & SPACE) ? ' ' : 0);

	if (sign)
		size--;

	if (flags & SPECIAL)
	{
		if (radix == 8)
			size--;
		else if (radix == 16)
		{
			size -= 2;
		}
	}

	if (value == 0)
		num_list[last++] = '0';
	else
	{
		while (value)
			num_list[last++] = num_tbl[do_div(value, radix)];
	}

	if (precision < last)
		precision = last;
	size -= precision;

	if (!(flags & (LEFT + ZERO_PAD)))
		while (size-- > 0)
			*str++ = ' ';
	if (sign)
		*str++ = sign;
	//%#
	if (SPECIAL & flags)
	{
		if (radix == 8)
			*str++ = '0';

		else if (radix == 16)
		{
			*str++ = '0';
			*str++ = num_tbl[33];
		}
	}
	//%
	if (!(flags & LEFT))
		while (size-- > 0) //宽度
			*str++ = ch;

	while (last < precision--)
		*str++ = '0';
	while (last-- > 0)
		*str++ = num_list[last];

	while (size-- > 0)
		*str++ = ' ';

	return str;
}

static inline void console_writeline(char *buff, int size)
{
	int count = 0;
	int line = 0;
	for (count = 0; count < size || line; count++)
	{
		if (line > 0)
		{
			count--;
			goto Label_tab;
		}
		if ((unsigned char)*(buff + count) == '\n')
		{
			char_pos_info.y_cursor++;
			char_pos_info.x_cursor = 0;
		}
		else if ((unsigned char)*(buff + count) == '\b')
		{
			char_pos_info.x_cursor--;
			if (char_pos_info.x_cursor < 0)
			{
				char_pos_info.x_cursor = (char_pos_info.x_resolution / char_pos_info.x_char_size - 1) * char_pos_info.x_char_size;
				char_pos_info.y_cursor--;
				if (char_pos_info.y_cursor < 0)
					char_pos_info.y_cursor = (char_pos_info.y_resolution / char_pos_info.y_char_size - 1) * char_pos_info.y_char_size;
			}
			putchar(
				char_pos_info.video_frame_address,
				char_pos_info.x_resolution,
				char_pos_info.x_cursor * char_pos_info.x_char_size,
				char_pos_info.y_cursor * char_pos_info.y_char_size,
				' ');
		}
		else if ((unsigned char)*(buff + count) == '\t')
		{
			line = ((char_pos_info.x_cursor + 8) & ~(8 - 1)) - char_pos_info.x_cursor;

		Label_tab:
			line--;
			putchar(
				char_pos_info.video_frame_address,
				char_pos_info.x_resolution,
				char_pos_info.x_cursor * char_pos_info.x_char_size,
				char_pos_info.y_cursor * char_pos_info.y_char_size,
				' ');
			//	putchar(Pos.FB_addr , Pos.XResolution , Pos.XPosition * Pos.XCharSize , Pos.YPosition * Pos.YCharSize , FRcolor , BKcolor , ' ');
			char_pos_info.x_cursor++;
		}
		else
		{
			putchar(
				char_pos_info.video_frame_address,
				char_pos_info.x_resolution,
				char_pos_info.x_cursor * char_pos_info.x_char_size,
				char_pos_info.y_cursor * char_pos_info.y_char_size,
				(unsigned char)*(buff + count));
			char_pos_info.x_cursor++;
		}

		if (char_pos_info.x_cursor >= (char_pos_info.x_resolution / char_pos_info.x_char_size))
		{
			char_pos_info.y_cursor++;
			char_pos_info.x_cursor = 0;
		}
		if (char_pos_info.y_cursor >= (char_pos_info.y_resolution / char_pos_info.y_char_size))
		{
			char_pos_info.y_cursor = 0;
		}
	}
}

int vsprintf(char *buf, const char *fmt, va_list args)
{
	char *str, *s;
	int flags;
	int field_width;
	int precision;
	int len, i;

	int qualifier; /* 'h', 'l', 'L' or 'Z' for integer fields */

	for (str = buf; *fmt; fmt++)
	{

		if (*fmt != '%')
		{
			*str++ = *fmt;
			continue;
		}
		flags = 0;
	repeat:
		fmt++;
		switch (*fmt)
		{
		case '-':
			flags |= LEFT;
			goto repeat;
		case '+':
			flags |= PLUS;
			goto repeat;
		case ' ':
			flags |= SPACE;
			goto repeat;
		case '#':
			flags |= SPECIAL;
			goto repeat;
		case '0':
			flags |= ZERO_PAD;
			goto repeat;
		}

		/* get field width */

		field_width = -1;
		if (is_digit(*fmt))
			field_width = skip_atoi(&fmt);
		else if (*fmt == '*')
		{
			fmt++;
			field_width = va_arg(args, int);
			if (field_width < 0)
			{
				field_width = -field_width;
				flags |= LEFT;
			}
		}

		/* get the precision */

		precision = -1;
		if (*fmt == '.')
		{
			fmt++;
			if (is_digit(*fmt))
				precision = skip_atoi(&fmt);
			else if (*fmt == '*')
			{
				fmt++;
				precision = va_arg(args, int);
			}
			if (precision < 0)
				precision = 0;
		}

		qualifier = -1;
		if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L' || *fmt == 'Z')
		{
			qualifier = *fmt;
			fmt++;
		}

		switch (*fmt)
		{
		case 'c':

			if (!(flags & LEFT))
				while (--field_width > 0)
					*str++ = ' ';
			*str++ = (unsigned char)va_arg(args, int);
			while (--field_width > 0)
				*str++ = ' ';
			break;

		case 's':

			s = va_arg(args, char *);
			if (!s)
				s = '\0';
			len = strlen(s);
			if (precision < 0)
				precision = len;
			else if (len > precision)
				len = precision;

			if (!(flags & LEFT))
				while (len < field_width--)
					*str++ = ' ';
			for (i = 0; i < len; i++)
				*str++ = *s++;
			while (len < field_width--)
				*str++ = ' ';
			break;

		case 'o':

			if (qualifier == 'l')
				str = itoa(str, va_arg(args, unsigned long), 8, field_width, precision, flags);
			else
				str = itoa(str, va_arg(args, unsigned int), 8, field_width, precision, flags);
			break;

		case 'p':

			if (field_width == -1)
			{
				field_width = 2 * sizeof(void *);
				flags |= ZERO_PAD;
			}
			flags |= (SPECIAL | SMALL);
			str = itoa(str, (int64)va_arg(args, void *), 16, field_width, precision, flags);
			break;

		case 'x':

			flags |= SMALL;

		case 'X':

			if (qualifier == 'l')
				str = itoa(str, va_arg(args, unsigned long), 16, field_width, precision, flags);
			else
				str = itoa(str, va_arg(args, unsigned int), 16, field_width, precision, flags);
			break;

		case 'd':
		case 'i':

			flags |= SIGNED;
		case 'u':

			if (qualifier == 'l')
				str = itoa(str, va_arg(args, unsigned long), 10, field_width, precision, flags);
			else
				str = itoa(str, va_arg(args, unsigned int), 10, field_width, precision, flags);
			break;

		case 'n':

			if (qualifier == 'l')
			{
				long *ip = va_arg(args, long *);
				*ip = (str - buf);
			}
			else
			{
				int *ip = va_arg(args, int *);
				*ip = (str - buf);
			}
			break;

		case '%':

			*str++ = '%';
			break;

		default:

			*str++ = '%';
			if (*fmt)
				*str++ = *fmt;
			else
				fmt--;
			break;
		}
	}
	*str = '\0';
	return str - buf;
}

void set_color(unsigned int font_color, unsigned int bg_color)
{
	char_pos_info.bg_color = bg_color;
	char_pos_info.font_color = font_color;
}

int printk(const char *fmt, ...)
{
	int i = 0;
	va_list args;
	va_start(args, fmt);
	i = vsprintf(buff, fmt, args);
	va_end(args);
	console_writeline(buff, i);
	return i;
}