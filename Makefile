# Makefile for G-TomOS v0.02 Pre-Alpha

# Compiler and linker
CC      = gcc
LD      = ld
CFLAGS  = -m32 -ffreestanding -fno-stack-protector -fno-pic -nostdlib -Wall -Wextra
LDFLAGS = -m elf_i386 -T src/linker.ld

# Source and object files
SRCS    = src/kernel/kernel.c src/kernel/vga.c src/kernel/keyboard.c
OBJS    = $(SRCS:.c=.o)

# Output
KERNEL  = bin/gtomos.elf
ISO_DIR = iso
ISO     = gtomos.iso

# Default target
all: $(KERNEL)

# Compile all .c files into .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link all objects into the kernel ELF
$(KERNEL): $(OBJS)
	$(LD) $(LDFLAGS) -o $(KERNEL) $(OBJS)

# Create ISO with GRUB
iso: $(KERNEL)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL) $(ISO_DIR)/boot/
	cp grub.cfg $(ISO_DIR)/boot/grub/
	grub-mkrescue -o $(ISO) $(ISO_DIR)

# Clean all build files
clean:
	rm -f $(OBJS) $(KERNEL) $(ISO)
	rm -rf $(ISO_DIR)
