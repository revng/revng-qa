//
// This file is distributed under the MIT License. See LICENSE.md for details.
//

#include <stddef.h>
#include <stdint.h>

#define WEAK __attribute__((weak))

WEAK uint64_t shift_left(int64_t amount) {
  return 0xABCDEF1234567890ULL << amount;
}

WEAK uint64_t logical_shift_right(int64_t amount) {
  return 0xABCDEF1234567890ULL >> amount;
}

WEAK int64_t arithmetic_shift_right(int64_t amount) {
  return 0xABCDEF1234567890LL >> amount;
}

WEAK uint64_t bitwise_and(int64_t arg) {
  return 0xABCDEF1234567890ULL & arg;
}

WEAK uint64_t bitwise_or(int64_t arg) {
  return 0xABCDEF1234567890ULL | arg;
}

WEAK uint64_t bitwise_xor(int64_t arg) {
  return 0xABCDEF1234567890ULL ^ arg;
}

WEAK int8_t data;

WEAK int8_t write_and_get_char() {
  return data = 'X';
}

WEAK int puts(const char *ptr) {
  data = *ptr;
}

WEAK int arg;

int do_stuff() {
  puts(NULL);
  return shift_left(arg) + logical_shift_right(arg)
         + arithmetic_shift_right(arg) + bitwise_or(arg) + bitwise_and(arg)
         + bitwise_xor(arg) + write_and_get_char();
}

WEAK void _start() {
  do_stuff();
}
