/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

/* RUN-one: one */
/* RUN-two: one two */
/* RUN-three: one two three */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  printf("%d %f %f\n", argc, (float) argc, (float) argc);
  return EXIT_SUCCESS;
}
