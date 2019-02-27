#ifndef _SVGA_H_
#define _SVGA_H_

#include <types.h>

#define COLOR_RED 0x00ff0000
#define COLOR_GREEN 0x0000ff00
#define COLOR_BLUE 0x000000ff
#define COLOR_WHITE 0x00ffffff
#define COLOR_ORANGE 0x00ff8000 //橙
#define COLOR_YELLOW 0x00ffff00 //黄
#define COLOR_BLACK 0x00000000  //黑
#define COLOR_INDIGO 0x0000ffff //靛
#define COLOR_PURPLE 0x008000ff //紫

typedef struct vbe_mode_info vbe_mode_info;
typedef struct font_struct font_struct;
typedef struct font_color font_color;

struct vbe_mode_info
{
	uint16 mode_attr;	//模式属性
	uint8 wina_attr;	 //窗口A属性
	uint8 winb_attr;	 //窗口B属性
	uint16 win_gran;	 //窗口颗粒度
	uint16 win_size;	 //窗口大小
	uint16 wina_seg;	 //实模式下窗口A段地址
	uint16 winb_seg;	 //实模式下窗口B段地址
	uint32 win_func_ptr; //实模式下窗口功能入口地址
	uint16 byte_per_scanline;

	/*vbe 1.2以上强制提供的信息*/
	uint16 x_resolution;  //水平分辨率
	uint16 y_resolution;  //垂直分辨率
	uint8 x_char_size;	//字符宽度
	uint8 y_char_size;	//字符高度
	uint8 numof_plane;	//内存平面数量
	uint8 bit_per_px;	 //每像素占用位宽
	uint8 numof_bank;	 //块数量
	uint8 mem_model;	  //内存模式类型
	uint8 bank_size;	  //块容量
	uint8 numof_img_page; //图像页数量
	uint8 page_reserve;   //为图像分页时保留使用

	/*直接颜色描绘区域*/
	uint8 red_mask_size;		  //direct color的红色屏蔽位宽
	uint8 red_field_pos;		  //红色屏蔽位起始位置
	uint8 green_mask_size;		  //direct color的绿色屏蔽位宽
	uint8 green_field_pos;		  //绿色屏蔽位起始位置
	uint8 blue_mask_size;		  //direct color的蓝色屏蔽位宽
	uint8 blue_field_pos;		  //蓝色屏蔽位起始位置
	uint8 reserve_mask_size;	  //direct color的保留色屏蔽位宽
	uint8 reserve_mask_pos;		  //保留色屏蔽位起始位置
	uint8 direct_color_mode_info; //direct color模式属性

	/*vbe 2.0以上强制提供的信息*/
	uint32 physics_addr; //平坦帧物理地址
	uint32 page_reserve_a;
	uint16 page_reserve_b;

} __attribute__((packed));

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
	uint32 *video_frame_address;
	uint32 video_frame_length;
};

/*光标字符信息模块*/
extern font_struct char_pos_info;

/*vbe_mode_info指针*/
extern vbe_mode_info *vbe_info_ptr;

#endif