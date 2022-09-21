/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

#include <stdlib.h>

int values[] = { 88, 56, 100, 2, 25 };

int cmpfunc(const void *a, const void *b) {
  return (*(int *) a - *(int *) b);
}

int main() {
  qsort(values, 5, sizeof(int), cmpfunc);

  return 0;
}
