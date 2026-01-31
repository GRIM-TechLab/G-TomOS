// keyboard.h
#pragma once
#include <stdint.h>

#define KBD_DATA_PORT 0x60
#define KBD_STATUS_PORT 0x64

void keyboard_install();
char keyboard_get_char();
void keyboard_poll();

