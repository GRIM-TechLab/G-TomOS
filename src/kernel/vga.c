/**
 * @file vga.c
 * @brief VGA Text Mode Driver implementation for G-TomOS.
 * @version 0.02
 * @author GRIM-TechLab
 * * Provides low-level primitives for screen manipulation, hardware cursor
 * control, and terminal emulation including scrolling and newline handling.
 */

#include "vga.h"
#include "io.h"

/* Video Hardware Constants */
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

/* Internal State Variables */
static uint16_t* const vga_buffer = (uint16_t*) VGA_MEMORY;
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

/**
 * @brief Formats a character and color attribute into a VGA-compatible 16-bit word.
 * @param c Character to be displayed.
 * @param color 8-bit color attribute (background/foreground).
 * @return 16-bit encoded VGA display cell.
 */
static inline uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | (uint16_t)color << 8;
}

/**
 * @brief Updates the hardware cursor position by communicating with the VGA CRT Controller.
 * Utilizes I/O ports 0x3D4 (Address) and 0x3D5 (Data).
 */
static void move_cursor() {
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;

    outb(0x3D4, 0x0F); // Select Cursor Location Low Register
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E); // Select Cursor Location High Register
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

/**
 * @brief Shifts the terminal buffer upwards when the cursor exceeds screen bounds.
 * Implements linear memory copying for row migration and clears the final row.
 */
static void scroll() {
    if (cursor_y < VGA_HEIGHT)
        return;

    /* Shift all rows up by one position */
    for (int y = 1; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            vga_buffer[(y - 1) * VGA_WIDTH + x] = vga_buffer[y * VGA_WIDTH + x];
        }
    }

    /* Clear the emerging bottom row */
    for (int x = 0; x < VGA_WIDTH; x++) {
        vga_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = vga_entry(' ', 0x07);
    }

    cursor_y = VGA_HEIGHT - 1;
}

/**
 * @brief Clears the entire VGA buffer with a specified color attribute.
 * Resets software coordinates and synchronizes hardware cursor.
 */
void vga_clear(uint8_t color) {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
        vga_buffer[i] = vga_entry(' ', color);
}

/**
 * @brief Processes a single character for display, handling control sequences like '\n'.
 * Automatically triggers scrolling and cursor updates as necessary.
 */
void vga_putchar(char c, uint8_t color) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        vga_buffer[cursor_y * VGA_WIDTH + cursor_x] = vga_entry(c, color);
        cursor_x++;
    }

    /* Handle horizontal overflow */
    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }

    scroll();
    move_cursor();
}

/**
 * @brief Iterates through a null-terminated string to output it to the terminal.
 * @param str Pointer to the source character array.
 * @param color Color attribute to be applied to all characters in the string.
 */
void print(const char* str, uint8_t color) {
    while (*str)
        vga_putchar(*str++, color);
}

static uint8_t prompt_limit = 0;

void vga_lock_prompt() {
    prompt_limit = cursor_x;
}

void vga_backspace() {
    if (cursor_x > prompt_limit) {
        cursor_x--;
        vga_buffer[cursor_y * VGA_WIDTH + cursor_x] = vga_entry(' ', 0x0F);
        move_cursor();
    }
}