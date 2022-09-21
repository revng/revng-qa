/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

struct second_struct {
  int a;
  int b;
};

struct my_struct {
  int x;
  int y;
  struct second_struct *z;
};

int main() {
  struct my_struct A;
  A.x = 3;
  A.y = 4;
  A.z = 0;
  return A.x;
}
