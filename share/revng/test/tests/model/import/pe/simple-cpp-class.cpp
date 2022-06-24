/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

class Foo {
private:
  int x;
  int y;
  static int s;

public:
  Foo(int x, int y) {
    this->x = x;
    this->y = y;
  }

  int retX() { return x; }

  int retY() const { return y; }

  static void dummy(int arg) { (void) arg; }
};

int Foo::s = 0;

int main() {
  Foo f(4, 5);
  (void) f.retX();
  (void) f.retY();
  Foo::dummy(f.retX());

  return 0;
}
