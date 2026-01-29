/**
 * @file kernel.c
 * @brief Core kernel entry point and low-level VGA driver implementation.
 * @project G-TomOS
 */

#include <stdint.h>
#include "io.h"

/**
 * @section Multiboot Specification Header
 * @brief This header allows a Multiboot-compliant bootloader (such as GRUB or QEMU)
 * to recognize the file as an executable kernel and load it into memory.
 */
#define MULTIBOOT_MAGIC    0x1BADB002
#define MULTIBOOT_FLAGS    (1 << 0 | 1 << 1)
#define MULTIBOOT_CHECKSUM -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

__attribute__((section(".multiboot"), used))
const struct {
    uint32_t magic;
    uint32_t flags;
    uint32_t checksum;
} multiboot_header = {
    MULTIBOOT_MAGIC,
    MULTIBOOT_FLAGS,
    MULTIBOOT_CHECKSUM
};

/* Standard VGA text mode dimensions */
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
/* Memory-mapped I/O address for color text mode display */
#define VGA_MEMORY 0xB8000

/* Pointer to the video memory buffer */
static uint16_t* vga = (uint16_t*) VGA_MEMORY;
/* Current cursor position coordinates */
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

/**
 * @brief Combines a character and color attribute into a 16-bit VGA entry.
 * @param c Character to display.
 * @param color Color attribute byte.
 * @return 16-bit formatted VGA cell value.
 */
static inline uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | (uint16_t)color << 8;
}

/**
 * @brief Synchronizes the hardware cursor position with software coordinates.
 * Communicates with the VGA Controller via I/O ports 0x3D4 and 0x3D5.
 */
void move_cursor() {
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;
    
    /* Command to set the low byte of the cursor position */
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    
    /* Command to set the high byte of the cursor position */
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

/**
 * @brief Clears the entire display buffer and resets cursor position.
 */
void clear_screen() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
        vga[i] = vga_entry(' ', 0x07); // Fill with space character and light gray color

    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

/**
 * @brief Outputs a null-terminated string to the VGA display.
 * @param str Pointer to the character string.
 */
void print(const char* str) {
    while (*str) {
        vga[cursor_y * VGA_WIDTH + cursor_x] = vga_entry(*str++, 0x07);
        cursor_x++;
        
        /* Note: Add wrapping logic here in future iterations if cursor_x exceeds VGA_WIDTH */
    }
    move_cursor();
}

/**
 * @brief Primary kernel execution loop. 
 * Initializes the display and enters a low-power wait state.
 */
void kernel_main() {
    clear_screen();
    print("G-TomOS:>");
    
    while (1) {
        /* Suspend CPU execution until the next interrupt */
        __asm__ volatile ("hlt");
    }
}