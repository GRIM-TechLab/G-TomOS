#include "keyboard.h"
#include "vga.h"
#include "io.h"
#include <stdint.h>

#define BUFFER_SIZE 256

static char buffer[BUFFER_SIZE];
static uint8_t head = 0;
static uint8_t tail = 0;

static const char scancode_ascii[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',0,
    'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\',
    'z','x','c','v','b','n','m',',','.','/',0,
    '*',0,' ','0',0,0,0,0,0,0,0,0,0,0,0,0
};

void keyboard_handler() {
    uint8_t scancode = inb(KBD_DATA_PORT);
    if (scancode & 0x80) return; // клавішу відпущено

    char c = scancode_ascii[scancode];
    if (c) {
        buffer[head] = c;
        head = (head + 1) % BUFFER_SIZE;
    }
}

char keyboard_get_char() {
    if (head == tail) return 0;
    char c = buffer[tail];
    tail = (tail + 1) % BUFFER_SIZE;
    return c;
}

void keyboard_install() {
    print("Keyboard driver installed\n", 0x0A);
}

void keyboard_poll() {
    uint8_t status = inb(KBD_STATUS_PORT);
    if (status & 0x01) {
        uint8_t scancode = inb(KBD_DATA_PORT);
        if (!(scancode & 0x80)) { // тільки натискання клавіші
            char c = scancode_ascii[scancode];
            if (c) {
                buffer[head] = c;
                head = (head + 1) % BUFFER_SIZE;
            }
        }
    }
}
