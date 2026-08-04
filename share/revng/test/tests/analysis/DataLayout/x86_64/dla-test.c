/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

// DLA end-to-end test: recover, purely from memory-access patterns in a
// stripped binary, the struct types of (1) a segment (global), (2) a function
// stack frame, and (3) a function argument's pointee.
//
// No inline assembly and no libc. A static, non-PIE binary keeps the segment
// layout clean (no GOT / PIE-relocation clutter). revng analyses the binary
// statically (it is never run), so the program need not exit cleanly.

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;

struct Segment {
  u64 s0;
  u32 s1;
  u32 s2;
  u64 s3;
  u16 s4;
  u16 s5;
  u32 s6;
  u64 s7;
  u32 s8;
  u32 s9;
};

struct Stack {
  u64 k0;
  u32 k1;
  u32 k2;
  u64 k3;
  u16 k4;
  u16 k5;
  u32 k6;
  u64 k7;
  u64 k8;
  u32 k9;
};

struct Arg {
  u64 a0;
  u32 a1;
  u32 a2;
  u64 a3;
  u16 a4;
  u16 a5;
  u32 a6;
  u64 a7;
  u32 a8;
  u64 a9;
};

// Global => segment.
struct Segment g_segment = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

// Volatile global pointer: reading it forces a real load of a value neither the
// compiler nor revng can pin down, so the pointee type of use_arg can only come
// from its access pattern (an opaque pointer, without resorting to mmap).
struct Arg *volatile g_argptr;

// Sink for the computed result, so the accesses feeding it are not optimised
// away.
volatile u64 g_result;

// A weak, empty function the optimiser cannot see through: passing a pointer to
// it forces the pointee into memory and blocks the surrounding accesses from
// being optimised away.
__attribute__((weak, noinline)) void escape(void *p) {
  (void) p;
}

__attribute__((noinline)) static u64 use_segment(struct Segment *p) {
  return p->s0 + p->s1 + p->s2 + p->s3 + p->s4 + p->s5 + p->s6 + p->s7 + p->s8
         + p->s9;
}

__attribute__((noinline)) static u64 use_stack(u64 seed) {
  volatile struct Stack s;
  s.k0 = seed;
  s.k1 = seed + 1;
  s.k2 = seed + 2;
  s.k3 = seed + 3;
  s.k4 = seed + 4;
  s.k5 = seed + 5;
  s.k6 = seed + 6;
  s.k7 = seed + 7;
  s.k8 = seed + 8;
  s.k9 = seed + 9;
  escape((void *) &s);
  return s.k0 + s.k1 + s.k2 + s.k3 + s.k4 + s.k5 + s.k6 + s.k7 + s.k8 + s.k9;
}

__attribute__((noinline)) static u64 use_arg(struct Arg *p) {
  return p->a0 + p->a1 + p->a2 + p->a3 + p->a4 + p->a5 + p->a6 + p->a7 + p->a8
         + p->a9;
}

void _start(void) {
  u64 acc = 0;
  acc += use_segment(&g_segment);
  acc += use_stack(acc);
  acc += use_arg((struct Arg *) g_argptr);
  g_result = acc;
}
