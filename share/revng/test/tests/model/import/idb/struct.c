/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

struct my_struct {
  int x;
  int y;
};

int main() {
  struct my_struct A;
  A.x = 3;
  A.y = 4;
  return A.x;
}
