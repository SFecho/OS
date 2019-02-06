#ifndef _BOOT_LOADER_H_
#define _BOOT_LOADER_H_

#define BOOT_SEG		0x7c0
#define MEM_INFO_SEG	0x7e0
#define LOADER_SEG		0x9020
#define INIT_SEG		0x9000
#define TMP_KERNEL_SEG		0x1000
#define VBE_INFO_SEG		0x800

#define BOCHS_SVGA_MODE		0x143
#define PHYSICS_SVGA_MODE	0x118

#define KERNEL_ADDR		0x100000


/*
#define TOTAL_SECTOR	0x27DE0
#define FAT_SECTOR		0x4e8
#define CLUS_PER_SECTOR	1
#define START_CLUS_NUM	2
#define RESERVE_SECTOR	0x20
#define START_FAT_SECTOR RESERVE_SECTOR
#define ROOT_DIR_CLUS	((((FAT_SECTOR) * 2) + 32) / (CLUS_PER_SECTOR))
#define DATA_SECTOR	(((FAT_SECTOR) * 2) + 32)
#define get_sector(clus_num)	((((clus_num)-(START_CLUS_NUM)) * (CLUS_PER_SECTOR)) + ((FAT_SECTOR) * 2) + (RESERVE_SECTOR))
*/
	
#endif