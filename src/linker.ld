/**
 * @file linker.ld
 * @brief Linker script for the G-TomOS kernel.
 * Defines the memory layout for all kernel object files.
 */

ENTRY(kernel_main)   /* Entry point of the kernel */

SECTIONS {
    /* Load the kernel at 1 MiB (0x100000) */
    . = 1M;

    /* Code section: includes kernel.c, vga.c, keyboard.c */
    .text : {
        *(.text*)
    }

    /* Read-only data section: constants, strings, etc. */
    .rodata : {
        *(.rodata*)
    }

    /* Initialized data section: global and static variables with initial values */
    .data : {
        *(.data*)
    }

    /* BSS section: uninitialized global/static variables */
    .bss : {
        *(.bss*)
    }

    /* Future: alignment for paging can be added here if needed */
}
