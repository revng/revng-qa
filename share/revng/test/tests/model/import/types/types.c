/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

#include <stdint.h>

char char_80_global[80] __attribute__((section(".custom_data")));
uint64_t uint64_global __attribute__((section(".custom_data")));
uint32_t uint32_global __attribute__((section(".custom_data")));

void _start() {
}
