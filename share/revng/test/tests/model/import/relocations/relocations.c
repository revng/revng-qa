//
// This file is distributed under the MIT License. See LICENSE.md for details.
//

// A pointer to each kind of target, which position-independent binaries
// relocate. The code refers to no global, so that these pointers are the only
// relocations, and the pointers are grouped in structs, so that their order is
// fixed.

#if defined(_WIN32)
#define IMPORTED __declspec(dllimport)
#else
#define IMPORTED
#endif

// Defined in `imported.c`, which is built as a library
IMPORTED extern int imported_function(int);

// Targets are static: only the pointers below reach them
#define TARGET __attribute__((noinline)) static int

TARGET target_read_only(int X) {
  return X + 1;
}

TARGET target_writable(int X) {
  return X + 2;
}

TARGET target_even(int X) {
  return X + 3;
}

TARGET target_odd(int X) {
  return X + 4;
}

static int object[4] = { 1, 2, 3, 4 };

typedef int (*Function)(int);

const struct {
  Function Code;
} read_only_pointers = { .Code = target_read_only };

struct {
  Function Code;
  int *Object;
  int *ObjectInterior;
  // Consecutive pointers, which packed encodings represent together
  Function Table[4];
  Function Imported;
} writable_pointers = {
  .Code = target_writable,
  .Object = &object[0],
  .ObjectInterior = &object[3],
  .Table = { target_even, target_odd, target_even, target_odd },
  .Imported = imported_function,
};

int entry(void) {
  return 0;
}
