//
// This file is distributed under the MIT License. See LICENSE.md for details.
//

#include <stdint.h>

// An irreducible loop: the initial branch jumps into the middle of the loop
// (`phase2`), so the cycle has two entry points and cannot be structured. The
// decompiler therefore emits a goto, which lets the edit-c-body test exercise
// goto-label renaming. The function also has local variables and plain
// statements, so it doubles as input for the comment and RETYPE cases.
int64_t irreducible(const uint8_t *code, int64_t n, int64_t start) {
  int64_t pc = 0;
  int64_t acc = 0;
  if (start)
    goto phase2;
  while (pc < n) {
    acc += code[pc];
    pc++;
  phase2:
    acc = acc * 3 + code[pc & 7];
    pc++;
  }
  return acc;
}

int main(int argc, char **argv) {
  return (int) irreducible((const uint8_t *) argv[0], argc, argc);
}
