/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

class Foo;
void doSomething(Foo *f);

int main() {
  Foo *f = nullptr;
  doSomething(f);
  return 0;
}

class Foo {
private:
  int x;
  int y;

public:
  Foo(int x, int y) {
    this->x = x;
    this->y = y;
  }

  int retX() { return x; }

  int retY() const { return y; }

  static void dummy(int arg) { (void) arg; }
};

void doSomething(Foo *f) {
  (void) f;
}
