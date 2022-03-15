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

int main(void) {

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  assert(!runtime_endianness_check());
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
  assert(runtime_endianness_check());
#endif

  return 0;
};
