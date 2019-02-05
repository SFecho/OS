# 1 "boot/boot.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "boot/boot.S"
# 1 "include/asm/boot.h" 1
# 2 "boot/boot.S" 2
.code16

.global _start, disk_addr_pkg

.section .text
#程序开始
_start:
 #FAT32文件系统头
 jmp entry
 nop

 .ascii "TIAN.FAT"
 #FAT32 BPB 53字节
 .word 0 #每扇区字节数

 .byte 0 #一个簇所占的扇区数
 .word 0 #保留扇区数，即在第一个FAT之前的扇区个数
 .byte 2 #FAT数
 .word 0 #根目录项数，FAT32文件系统置0
 .word 0 #小扇区数，FAT32置0
 .byte 0 #媒体描述符，硬盘为0xf8
 .word 0 #每FAT扇区数，FAT32不可用
 .word 0 #Int 0x13中断的磁盘磁头数
 .word 0 #磁头数(Number of Head) 本字段包含使用INT 13h的磁盘的“磁头数”
 .long 0 #隐藏扇区数(Hidden Sector) 该分区上引导扇区之前的扇区数
 .long 0 #80MB硬盘的总扇区数
 .long 0 #每FAT扇区数(Sectors Per FAT)(只被FAT32使用)
 .word 0 #扩展标志(Extended Flag)(只被FAT32使用)
 .word 0 #FS版本号
 .long 0 #根目录簇号(Root Cluster Number)(只供FAT32使用)
 .word 0 #文件系统信息扇区号
 .word 0 #备份引导扇区(只供FAT32使用) 为一个非零值，
     #这个非零值表示该分区保存引导扇区的副本的保留区中的扇区号
 .long 0,0,0 #保留字段，12字节

 #FAT32分区的扩展BPB字段
 .byte 0 #驱动器号，0x80为硬盘
 .byte 0 #保留
 .byte 0 #扩展引导标签,本字段必须要有能被Windows 2000所识别的值0x28或0x29
 .long 0 # 分区序号(Volume Serial Number) 在格式化磁盘时所产生的一个随机序号
 .ascii "Master Disk" #卷标，11字节
 .ascii "FAT32   " #文件系统ID(FS-System ID) FAT32文件系统中一般取为"FAT32"

entry:
 movw $0x7c0, %ax
 movw %ax, %ds
    movw $0x9000, %ax
 movw %ax, %es
 movw %ax, %ss
 movw $0xff00, %sp
 xor %si, %si
 xor %di, %di
 movw $256, %cx
 rep movsw

 ljmp $0x9000, $go


go:
 #clear scr
 movw $0x0600, %ax
 movb $0x7, %bh
    xor %cx, %cx
 movw $0x184f, %dx
    int $0x10

    #显示booting system...
    movw $boot_msg, %bp
    movw $BOOT_MSG_LEN, %cx
    movw $0x1301, %ax
    movw $0x000c, %bx
    movw $0, %dx
    int $0x10

 #读取Loader，Loader在磁盘的第二个扇区，FAT32预留扇区为32个
 movw %cs, %ax
 movw %ax, %ds
 movw $disk_addr_pkg, %bx
 movb $4, 2(%bx) #传输4块
 movw $0, 4(%bx) #offset = 0
 movw $0x9020, 6(%bx) #seg = 0x9020
 movl $7, 8(%bx)
 movl $0, 12(%bx)
 call read_disk

 #读取setup64.S，Loader在磁盘的第二个扇区，FAT32预留扇区为32个
 movw %cs, %ax
 movw %ax, %ds
 movw $disk_addr_pkg, %bx
 movb $1, 2(%bx) #传输1块
 movw $0, 4(%bx) #offset = 0
 movw $0x9100, 6(%bx) #seg = 0x9020
 movl $11, 8(%bx)
 movl $0, 12(%bx)
 call read_disk

 ljmp $0x9020, $0



#磁盘读取
read_disk:
 push %ax
 push %dx
 push %si
 push %bx

 movb $0x42, %ah
 movb $0x80, %dl
 movw $disk_addr_pkg, %si
 movw %cs, %bx
 movw %bx, %ds
 int $0x13

 pop %bx
 pop %si
 pop %dx
 pop %ax
 ret

boot_msg:
 .ascii "booting..."
.equ BOOT_MSG_LEN, .- boot_msg


#磁盘地址包，支持LBA模式
disk_addr_pkg:
 .byte 0x10 #包大小
 .byte 0 #保留
 .byte 0 #传输块数
 .byte 0 #保留
 .word 0 #偏移
 .word 0 #段
 .long 0 #LBA低32Bit
 .long 0 #LBA高32Bit


fill_space:
 .fill 510-(fill_space - _start)
bios_flag:
    .word 0xaa55
