/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

#define PREVENT_TESTS_FROM_BEING_REMOVED_BY_THE_LINKER
#include "functions.inc"

/* RUN-default: nope */

uint8_t expected_state[/* doesn't matter */ 1];
uint8_t value_dumps[/* doesn't matter */ 1];
uint64_t size_dumps[/* doesn't matter */ 1];
uint64_t address_dumps[/* doesn't matter */ 1];

static size_t runtime_endianness_check() {
  size_t i = 1;
  return !*((uint8_t *) &i);
}

int main(void) {

#ifdef ENDIANNESS_LITTLE
  assert(!runtime_endianness_check() && "little endianness expected!");
#elif defined(ENDIANNESS_BIG)
  assert(runtime_endianness_check() && "big endianness expected!");
#endif

  return 0;
};
