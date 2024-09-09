//
// Copyright (c) rev.ng Labs Srl. See LICENSE.md for details.
//

typedef unsigned int uint;
typedef unsigned char uchar;

#define DECLARE_OPERATION(op, op_name, type1, type2)                          \
  __attribute__((weak)) type1 op_name##_##type1##_##type2(type1 a, type2 b) { \
    return a op b;                                                            \
  }

DECLARE_OPERATION(+, add, uint, uchar)
DECLARE_OPERATION(+, add, uchar, uint)
DECLARE_OPERATION(-, sub, uint, uchar)
DECLARE_OPERATION(-, sub, uchar, uint)
DECLARE_OPERATION(*, mul, uint, uchar)
DECLARE_OPERATION(*, mul, uchar, uint)
DECLARE_OPERATION(&, and, uint, uchar)
DECLARE_OPERATION(&, and, uchar, uint)
DECLARE_OPERATION(|, or, uint, uchar)
DECLARE_OPERATION(|, or, uchar, uint)
DECLARE_OPERATION(^, xor, uint, uchar)
DECLARE_OPERATION(^, xor, uchar, uint)
DECLARE_OPERATION(<<, lshift, uint, uchar)
DECLARE_OPERATION(<<, lshift, uchar, uint)
DECLARE_OPERATION(>>, rshift, uint, uchar)
DECLARE_OPERATION(>>, rshift, uchar, uint)
DECLARE_OPERATION(+, add, int, char)
DECLARE_OPERATION(+, add, char, int)
DECLARE_OPERATION(-, sub, int, char)
DECLARE_OPERATION(-, sub, char, int)
DECLARE_OPERATION(*, mul, int, char)
DECLARE_OPERATION(*, mul, char, int)
DECLARE_OPERATION(&, and, int, char)
DECLARE_OPERATION(&, and, char, int)
DECLARE_OPERATION(|, or, int, char)
DECLARE_OPERATION(|, or, char, int)
DECLARE_OPERATION(^, xor, int, char)
DECLARE_OPERATION(^, xor, char, int)
DECLARE_OPERATION(<<, lshift, int, char)
DECLARE_OPERATION(<<, lshift, char, int)
DECLARE_OPERATION(>>, rshift, int, char)
DECLARE_OPERATION(>>, rshift, char, int)

__attribute__((weak)) uint return_value() {
  uchar y;
  return y;
}

__attribute__((weak)) uint identity(uchar a) {
  if (a)
    return a;
  return a + 1000;
}

__attribute__((weak)) uint compare(uchar y) {
  uint z;
  if (y >= 4) {
    uint r = identity(z);
    z = r + z;
    z = z + y;
    return z - r;
  } else {
    ++z;
    return y + z;
  }
}

__attribute__((weak)) int main() {
  uint r1 = return_value();
  uchar l;
  uint r2 = compare(l);

  return 0;
}
