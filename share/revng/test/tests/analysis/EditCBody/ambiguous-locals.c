//
// This file is distributed under the MIT License. See LICENSE.md for details.
//

#include <stdint.h>

int64_t consume(int64_t first, int64_t second);

// Two locals read only by the `consume` call, so rev.ng identifies both by the
// same address and cannot tell them apart. The store keeps the loads from being
// folded into the call arguments, which is what makes them locals at all.
int64_t ambiguous(const int64_t *left, const int64_t *right, int64_t *out) {
  int64_t first = *left;
  int64_t second = *right;
  *out = 0;
  return consume(first, second);
}

int64_t consume(int64_t first, int64_t second) {
  return first * 3 + second * 5;
}

int main(int argc, char **argv) {
  int64_t buffer[3] = { argc, (int64_t) (intptr_t) argv[0], 0 };
  return (int) ambiguous(buffer, buffer + 1, buffer + 2);
}
