#include <svga.h>
#include <os/kernel.h>
#include <asm/x64.h>

void start_kernal(void)
{
	uint32 i;
	uint32 *addr = (uint32 *)0xffff800000a00000;
	char *tmp = (char *)0;

	vbe_info_ptr = (vbe_mode_info *)0xffff800000008200;

	char_pos_info.x_resolution = vbe_info_ptr->x_resolution;
	char_pos_info.y_resolution = vbe_info_ptr->y_resolution;
	char_pos_info.x_char_size = vbe_info_ptr->x_char_size;
	char_pos_info.y_char_size = vbe_info_ptr->y_char_size;
	char_pos_info.x_cursor = char_pos_info.y_cursor = 0;
	char_pos_info.video_frame_address = (uint32 *)0xffff800000a00000; //对应物理地址0xe0000000
	char_pos_info.video_frame_length = char_pos_info.x_resolution * char_pos_info.y_resolution * 4;

	set_color(COLOR_WHITE, COLOR_BLACK);

	for (i = 0; i < char_pos_info.x_resolution * 20; i++)
	{
		tmp = (char *)addr;

		tmp[0] = 0xff;
		tmp[1] = 0x0;
		tmp[2] = 0x0;
		tmp[3] = 0x0;

		addr++;
	}

	for (i = 0; i < char_pos_info.x_resolution * 20; i++)
	{
		tmp = (char *)addr;

		tmp[0] = 0x0;
		tmp[1] = 0xff;
		tmp[2] = 0x0;
		tmp[3] = 0x0;

		addr++;
	}

	set_sys_vector();

	printk("\nhello world%p\n", addr);

	i = 1 / 0;

	while (1)
		;
}