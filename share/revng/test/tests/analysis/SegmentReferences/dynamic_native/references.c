/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define WEAK __attribute__((weak))

WEAK uint64_t use_pointer(void *arg) {
  return (uint64_t) arg;
}

WEAK char *some_function() {
  return (char *) 42;
}

uint32_t global_variable;

int main(int argc, char *argv[]) {
  // Ensure we detect `some_function` as a function
  some_function();

  // Reference to global variable
  use_pointer(&global_variable);

  // Reference to an existing function
  use_pointer(&some_function);

  // Reference to an ASCII string
  use_pointer("ascii-string");

  // Reference to an UTF8 string
  use_pointer("utf8-strìng");

  // Reference to an UTF16 string
  use_pointer(u"utf16-string");
}
