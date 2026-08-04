/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

long Unknown;

// The model this test overrides in marks this function as `AlwaysInline`.
// `caller` calls it twice, so its body has to be emitted next to `caller` and
// inlined at both call sites.
long __attribute__((noinline)) inlined_callee(long Value) {
  return Unknown + Value * 3;
}

long __attribute__((noinline)) caller(long Value) {
  return inlined_callee(Value) + inlined_callee(Value + 1);
}

// Not marked: the call to `caller` has to survive as a call.
long __attribute__((noinline)) other_caller(long Value) {
  return caller(Value) + 1;
}

int main(int Argc, char **Argv) {
  return (int) other_caller(Argc);
}
