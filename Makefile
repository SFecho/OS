CC = gcc

ASM = as

OBJS =	boot/_boot.s boot/boot.o		\
		boot/boot.bin boot/_setup32.s	\
		boot/setup32.o boot/setup32.bin	\
		boot/_setup64.s boot/setup64.o 	\
		boot/setup64.bin kernel/main.o	\
		kernel/_head.s	kernel/head.o 	\
		kernel/printk.o kernel/gate.o	\
		kernel/_entry.s kernel/entry.o	\
		kernel/trap.o lib/rb_tree.o		\
		lib/string.o mm/memory.o		\
		kernel.bin
	

LD_OBJS = 	kernel/head.o kernel/main.o 	\
			kernel/printk.o kernel/entry.o	\
			kernel/gate.o kernel/trap.o		\
			mm/memory.o lib/string.o		\
			lib/rb_tree.o

DISK = 40m.img		

INCLUDE = include/		

CFLAGS = -I $(INCLUDE) -mcmodel=large -fno-builtin -nostdinc -fno-stack-protector -m64

all: $(OBJS)

boot/_boot.s: boot/boot.S
	$(CC) -E -I $(INCLUDE) $< -o $@
	
boot/boot.o: boot/_boot.s
	$(ASM) --32 $< -o $@

boot/boot.bin: boot/boot.o
	mkfs.vfat -F 32 $(DISK)
	ld -M --oformat binary -m elf_i386 -o $@ $< -T boot/boot.lds
	dd if=boot/boot.bin of=$(DISK) bs=1 count=11 conv=notrunc
	dd if=boot/boot.bin of=$(DISK) bs=1 seek=71 skip=71 count=375 conv=notrunc
	dd if=boot/boot.bin of=$(DISK) seek=510 skip=510 bs=1 count=2 conv=notrunc
	
boot/_setup32.s: boot/setup32.S
	$(CC) -E -I $(INCLUDE) $< > $@
	
boot/setup32.o: boot/_setup32.s
	$(ASM) --32 $< -o $@

boot/setup32.bin: boot/setup32.o
	ld -M --oformat binary -m elf_i386 -o $@ $< -T boot/boot.lds
	dd if=boot/setup32.bin of=$(DISK) bs=512 count=4 seek=7 conv=notrunc

boot/_setup64.s: boot/setup64.S
	$(CC) -E -I $(INCLUDE) $< > $@
	
boot/setup64.o: boot/_setup64.s
	$(ASM)	--32 $< -o $@

boot/setup64.bin: boot/setup64.o
	ld -M --oformat binary -m elf_i386 -o $@ $< -T boot/boot.lds
	dd if=boot/setup64.bin of=$(DISK) bs=512 count=1 seek=11 conv=notrunc

kernel/_head.s: kernel/head.S
	$(CC) -E -I $(INCLUDE) $< > $@
	
kernel/head.o: kernel/_head.s
	$(ASM) --64 $< -o $@

kernel/main.o: kernel/main.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel/printk.o: kernel/printk.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel/gate.o: kernel/gate.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel/_entry.s: kernel/entry.S
	$(CC) -E -I $(INCLUDE) $< > $@

kernel/entry.o: kernel/_entry.s
	$(ASM) --64 $< -o $@

kernel/trap.o: kernel/trap.c
	$(CC) $(CFLAGS) -c $< -o $@

lib/string.o: lib/string.c
	$(CC) $(CFLAGS) -c $< -o $@

lib/rb_tree.o: lib/rb_tree.c
	$(CC) $(CFLAGS) -c $< -o $@

mm/memory.o: mm/memory.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel.bin: $(LD_OBJS)
	ld  -M --oformat binary -m elf_x86_64 -o kernel.bin $(LD_OBJS) -T kernel/kernel.lds

clean:
	rm -f boot/*.o kernel/*.o mm/*.o lib/*.o
	rm -f *.bin boot/*.bin
	rm -f boot/_*.s kernel/_*.s