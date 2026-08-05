//
// This file is distributed under the MIT License. See LICENSE.md for details.
//

#include <stdint.h>

// Two globals read and written by the functions below.
uint64_t first_global = 0x1122334455667788;
uint64_t second_global = 0x99aabbccddeeff00;

// A global remains part of the model even when no function references it.
uint64_t unused_global = 0x123456789abcdef0;

// A string, so that a global variable holding one is listed with what it says.
// It is read from the middle as well as from the start, which is how suffix
// sharing looks: both are references to this one string.
const char greeting[] = "hello glorious world";

uint64_t consume(uint64_t first, uint64_t second);
uint64_t consume_string(const char *string);

// Reads both globals.
uint64_t reads_globals(void) {
  return consume(first_global, second_global);
}

// Writes both globals, so that a reference is recorded no matter the direction
// the data travels in.
uint64_t writes_globals(uint64_t value) {
  first_global = value;
  second_global = value + 1;
  return 0;
}

// Reads the string, from its start and from the middle of it.
uint64_t reads_string(void) {
  return consume_string(greeting) + consume_string(greeting + 14);
}

// No global variable should record this function as a user.
uint64_t touches_nothing(uint64_t first, uint64_t second) {
  return first * 3 + second * 5;
}

uint64_t consume(uint64_t first, uint64_t second) {
  return first ^ second;
}

uint64_t consume_string(const char *string) {
  return (uint64_t) string[0];
}

int main(int argc, char **argv) {
  return (int) (reads_globals() + writes_globals(argc) + reads_string()
                + touches_nothing(argc, argc + 1));
}
