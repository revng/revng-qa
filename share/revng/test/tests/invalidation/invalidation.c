/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

#include <stdio.h>
#include <stdint.h>

uint64_t g_var = 20;

uint64_t called_twice(uint64_t a) {
  return a * g_var;
}

uint64_t caller_1(uint64_t a, uint64_t b) {
  return a + called_twice(b);
}

uint64_t caller_2(uint64_t a, uint64_t b, uint64_t c) {
  return a + b + called_twice(c);
}

void use_puts() {
  puts("Hello World");
}

int main(int argc, const char **argv) {
  uint64_t var = 0;
  var = caller_1(argc, 1);
  var += caller_2(argc, 2, 3);

  use_puts();
  return var;
}
