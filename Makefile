# --- G-TomOS Automated Build System Specification (ELF Edition) ---
# @file Makefile
# @description Manages the compilation of C sources and linkage into a 
# high-level ELF executable. Directs output to a dedicated binary directory.

# Toolchain configuration
CC = gcc
LD = ld

# Compilation Flags:
# -m32: Target 32-bit x86 architecture.
# -ffreestanding: Required for OS development (no standard C library).
# -O2: Optimization level 2.
# -Wall -Wextra: Enable rigorous warning diagnostics.
# -Isrc/kernel: Header search path for kernel components.
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -Isrc/kernel

# Linker Flags:
# -m elf_i386: Explicitly set the output format to 32-bit ELF.
# -T src/linker.ld: Use the custom linker script for memory mapping.
LDFLAGS = -m elf_i386 -T src/linker.ld

# Source and Object Discovery:
SOURCES = $(shell find src -name "*.c")
OBJECTS = $(SOURCES:.c=.o)

# Output Management:
# The binary is placed in the 'bin/' directory for structural clarity.
BIN_DIR = bin
KERNEL_ELF = $(BIN_DIR)/gtomos.elf

.PHONY: all clean setup

# Default Target: Ensure the environment is ready and build the ELF image.
all: setup $(KERNEL_ELF)

# Setup Phase: Creates the output directory if it does not exist.
setup:
	@mkdir -p $(BIN_DIR)

# Linkage Phase: Aggregates objects into the final Executable and Linkable Format (ELF).
$(KERNEL_ELF): $(OBJECTS)
	@echo "STATUS: Initiating linkage of G-TomOS ELF kernel..."
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS)
	@echo "SUCCESS: ELF image generated at $(KERNEL_ELF)."

# Compilation Phase:
%.o: %.c
	@echo "COMPILE: Processing source file $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Cleanup Procedure:
clean:
	@echo "CLEAN: Removing intermediate objects and the binary directory..."
	rm -f $(shell find src -name "*.o")
	rm -rf $(BIN_DIR)