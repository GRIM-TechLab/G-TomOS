/**
 * @file kernel.c
 * @brief Core kernel entry point and low-level VGA driver implementation.
 * @project G-TomOS
 */

#include <stdint.h>
#include "io.h"
#include "vga.h"
#include "keyboard.h"

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

void kernel_main() {
    vga_clear(0x0F);

    print("G-TomOS kernel started\n", 0x0A);
    keyboard_install();
    print("G-TomOS:> ", 0x0E);
    vga_lock_prompt();

    while (1) {
        keyboard_poll();
        char c = keyboard_get_char();

        if (c != 0) {
            if (c == '\b') {
                vga_backspace();
            } else if (c == '\n') {
                vga_putchar('\n', 0x0F);
                print("G-TomOS:> ", 0x0E);
                vga_lock_prompt();
            } else {
                vga_putchar(c, 0x0F);
            }
        }
    }
}