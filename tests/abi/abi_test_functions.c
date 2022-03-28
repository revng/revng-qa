/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "functions.inc"

ABIDEF void *set_return_value_up(void) {
  return 0;
}

int main(void) {

#ifdef ENDIANNESS_LITTLE
  assert(!runtime_endianness_check());
#elif defined(ENDIANNESS_BIG)
  assert(runtime_endianness_check());
#endif

  return 0;
};
