/**
 * @file io.h
 * @brief Hardware Input/Output port communication abstractions.
 */

#pragma once
#include <stdint.h>

/**
 * @brief Transmits a byte of data to a specified I/O port.
 * Uses inline assembly to execute the 'outb' instruction.
 * @param port The target hardware port address.
 * @param value The 8-bit data value to transmit.
 */
static inline void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}