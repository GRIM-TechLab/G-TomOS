/**
 * @file linker.ld
 * @brief Linker script for the G-TomOS kernel.
 * * This document defines the memory layout of the executable binary.
 * It specifies how the linker should combine object files into the 
 * final kernel image, ensuring compatibility with the bootloader.
 */

/* * Entry Point: Defines the initial symbol to be executed upon kernel load.
 * Reference: kernel_main defined in kernel.c
 */
ENTRY(kernel_main)

SECTIONS {
    /**
     * The kernel will be loaded at the 1 MiB (0x100000) physical address.
     * This is a standard convention to avoid the Real Mode IVT and BIOS data areas.
     */
    . = 1M;

    /* Executable code section: Combined from all input object files */
    .text : { 
        *(.text*) 
    }

    /* Read-only data section: Constants, strings, and static data */
    .rodata : { 
        *(.rodata*) 
    }

    /* Initialized writable data: Global and static variables with initial values */
    .data : { 
        *(.data*) 
    }

    /* Uninitialized data (Block Starting Symbol): Reserved memory for variables set to zero */
    .bss : { 
        *(.bss*) 
    }

    /**
     * Note: Future iterations may require 4K alignment for paging compatibility.
     */
}