/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

#include <stdlib.h>

// Reaches `malloc` and `free` through their PLT entries. `detect-abi` marks
// the entries as `AlwaysInline`, so the calls to them have to disappear.
void *use_plt(unsigned long Size) {
  void *Buffer = malloc(Size);
  free(Buffer);
  return Buffer;
}

int main(int Argc, char **Argv) {
  return use_plt(Argc) != 0;
}
