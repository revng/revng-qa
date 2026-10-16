//
// This file is distributed under the MIT License. See LICENSE.md for details.
//

// An IFUNC: only the loader calls the resolver, through an `R_*_IRELATIVE`
// relocation, and only the resolver refers to the implementation

__attribute__((noinline)) static int implementation(int X) {
  return X + 1;
}

static void *resolver(void) {
  return (void *) implementation;
}

int indirect(int X) __attribute__((ifunc("resolver")));

int entry(int X) {
  return indirect(X);
}
