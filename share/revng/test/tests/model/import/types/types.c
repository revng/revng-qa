/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

// Note that the object file generated from this file is bounded to use bfd,
// instead of gold.

#include <stdint.h>

uint32_t g_uint32 __attribute__((section(".g_uint32")));
uint64_t g_uint64 __attribute__((section(".g_uint64")));
char g_char_100[100] __attribute__((section(".g_char_100")));

void _start() {
}
