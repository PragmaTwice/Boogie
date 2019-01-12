#!Makefile

C_SOURCES = $(shell find . -name "*.c")
C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES))
S_SOURCES = $(shell find . -name "*.s")
S_OBJECTS = $(patsubst %.s, %.o, $(S_SOURCES))

CC = gcc
LD = ld
ASM = nasm

C_FLAGS = -c -Wall -m32 -ggdb -gstabs+ -nostdinc -fno-builtin -fno-stack-protector -I include
LD_FLAGS = -T scripts/kernel.ld -m elf_i386 -nostdlib
ASM_FLAGS = -f elf -g -F stabs

all: $(S_OBJECTS) $(C_OBJECTS) link update_image

# The automatic variable `$<' is just the first prerequisite
.c.o:
	@echo Compiling C codes $< ...
	$(CC) $(C_FLAGS) $< -o $@

.s.o:
	@echo Compiling ASM codes $< ...
	$(ASM) $(ASM_FLAGS) $<

link:
	@echo Linking kernel...
	$(LD) $(LD_FLAGS) $(S_OBJECTS) $(C_OBJECTS) -o boogie_kernel

.PHONY:clean
clean:
	$(RM) $(S_OBJECTS) $(C_OBJECTS) boogie_kernel

.PHONY:update_image
update_image:
	sudo mount floppy.img /mnt/boogie_img_dev
	sudo cp boogie_kernel /mnt/boogie_img_dev/boogie_kernel
	sleep 1
	sudo umount /mnt/boogie_img_dev

.PHONY:mount_image
mount_image:
	sudo mount floppy.img /mnt/boogie_img_dev

.PHONY:umount_image
umount_image:
	sudo umount /mnt/boogie_img_dev

.PHONY:qemu
qemu:
	qemu-system-x86_64 -fda floppy.img -boot a	
	#add '-nographic' option if using server of linux distro, such as fedora-server,or "gtk initialization failed" error will occur.

.PHONY:bochs
bochs:
	bochs -f scripts/bochsrc.txt

.PHONY:debug
debug:
	qemu-system-x86_64 -S -s -fda floppy.img -boot a &
	sleep 1
	cgdb -x scripts/gdbinit

