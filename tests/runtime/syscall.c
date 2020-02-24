/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

int root(char *buffer, size_t size) {
  int result;
  result = syscall(SYS_dup3, 100, 0);
  return result;
}

int main(int argc, char *argv[]) {
  printf("%d\n", root(argv[1], strlen(argv[1])));
  return EXIT_SUCCESS;
}
