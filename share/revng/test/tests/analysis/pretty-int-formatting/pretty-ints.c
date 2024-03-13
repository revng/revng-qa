#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

__attribute__((weak))
uint64_t shift_left(int64_t amount) {
  return 0xABCDEF1234567890ULL << amount;
}

__attribute__((weak))
uint64_t logical_shift_right(int64_t amount) {
  return 0xABCDEF1234567890ULL >> amount;
}

__attribute__((weak))
int64_t arithmetic_shift_right(int64_t amount) {
  return 0xABCDEF1234567890LL >> amount;
}

__attribute__((weak))
uint64_t bitwise_and(int64_t arg) {
  return 0xABCDEF1234567890ULL & arg;
}

__attribute__((weak))
uint64_t bitwise_or(int64_t arg) {
  return 0xABCDEF1234567890ULL | arg;
}

__attribute__((weak))
uint64_t bitwise_xor(int64_t arg) {
  return 0xABCDEF1234567890ULL ^ arg;
}

__attribute__((weak))
int8_t data;

__attribute__((weak))
int8_t write_and_get_char() {
  return data = 'X';
}

int main(int argc, const char **argv) {
  puts(NULL);
  return shift_left(argc)
    + logical_shift_right(argc)
    + arithmetic_shift_right(argc)
    + bitwise_or(argc)
    + bitwise_and(argc)
    + bitwise_xor(argc)
    + write_and_get_char();
}
