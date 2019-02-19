#!Makefile

C_SOURCES = $(shell find . -name "*.c")
C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES))
S_SOURCES = $(shell find . -name "*.s")
S_OBJECTS = $(patsubst %.s, %.o, $(S_SOURCES))

CC = clang
LD = ld
ASM = nasm

C_FLAGS = -c -Wall -m32 -ggdb -nostdinc -fno-builtin -fno-stack-protector -I include
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
	if [ ! -d "./build" ]; then mkdir ./build; fi
	@echo Linking kernel...
	$(LD) $(LD_FLAGS) $(S_OBJECTS) $(C_OBJECTS) -o build/boogie_kernel

.PHONY:clean
clean:
	$(RM) $(S_OBJECTS) $(C_OBJECTS) build/*

.PHONY:update_image
update_image:
	cp image/floppy.img build/boogie.img
	if [ ! -d "/mnt/boogie_img_dev" ]; then sudo mkdir /mnt/boogie_img_dev; fi
	sudo mount build/boogie.img /mnt/boogie_img_dev
	sudo cp build/boogie_kernel /mnt/boogie_img_dev/boogie_kernel
	sleep 1
	sudo umount /mnt/boogie_img_dev

.PHONY:mount_image
mount_image:
	sudo mount build/boogie.img /mnt/boogie_img_dev

.PHONY:umount_image
umount_image:
	sudo umount /mnt/boogie_img_dev

.PHONY:qemu
qemu:
	qemu-system-i386 -fda build/boogie.img -boot a

.PHONY:debug
debug:
	qemu-system-i386 -S -s -fda build/boogie.img -boot a &
	sleep 1
	cgdb -x scripts/gdbinit

