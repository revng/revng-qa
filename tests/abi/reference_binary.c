/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "functions.h"

ABIDEF void *set_return_value_up(void) {
  return 0;
}

int main() {
  puts("0xdc984ef2647af904ull");
  return 0;
};
