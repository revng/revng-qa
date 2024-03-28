/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

#define WEAK __attribute__((weak))

WEAK void report(int code) {
}

WEAK int f(int arg1) {
  int return_value;
  while (arg1 > 0) {
    if (arg1 == 1) {
      return_value = 10;
      report(1);
      goto LABEL_EXIT_1;
    } else if (arg1 == 2) {
      return_value = 20;
      report(2);
      goto LABEL_EXIT_2;
    }
  }

LABEL_EXIT_1:
  report(1);
  goto LABEL_EXIT_COMMON;

LABEL_EXIT_2:
  report(2);

LABEL_EXIT_COMMON:
  return return_value;
}

WEAK void _start() {
  f(1);
}
