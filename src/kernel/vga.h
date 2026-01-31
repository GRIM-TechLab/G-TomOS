/**
 * @file vga.h
 * @brief VGA Terminal Interface Specification.
 * @version 0.02
 * @project G-TomOS
 * @publisher GRIM-TechLab
 */

#pragma once

#include <stdint.h>

void vga_clear(uint8_t color);
void vga_putchar(char c, uint8_t color);
void print_char(char c, uint8_t color);
void vga_lock_prompt();
void vga_backspace();
void print(const char* str, uint8_t color);