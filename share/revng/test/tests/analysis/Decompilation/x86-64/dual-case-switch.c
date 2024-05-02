/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

#include <stdint.h>

#define WEAK __attribute__((weak))

WEAK void report(uint64_t code) {
}

WEAK uint64_t opaque(uint64_t arg) {
  return arg;
}

/// With this function we want to enforce the creation of an exit dispatcher
/// with two `case`s following the `while (arg1)` loop. The `switch` to `if`
/// beautify pass, should then create operate in order to reconstruct an `if`.

WEAK uint64_t f(uint64_t arg1) {

  // It is important that `return_value` is initialized to `10` in order to
  // obtain the exit dispatcher. Using another value enables compiler
  // optimizations that produce a CFG where no exit dispatcher `switch`, making
  // it impossible to promote it to an `if` (which is the goal of this test).
  uint64_t return_value = 10;

  if (arg1 != 50) {
    while (arg1 > 0) {
      if (arg1 == 1) {
        return_value = 10;
        report(1);
        break;
      } else if (arg1 == 2) {
        return_value = 20;
        report(3);

        // This `goto` is used in order to reach the abnormal successor of the
        // loop
        goto LABEL_ABNORMAL_EXIT;
      }
      arg1 = opaque(arg1);
      report(arg1);
    }
  }


  report(1);
  goto LABEL_END;

LABEL_ABNORMAL_EXIT:
  report(2);

LABEL_END:
  return return_value;
}

WEAK void _start() {
  f(1);
}
