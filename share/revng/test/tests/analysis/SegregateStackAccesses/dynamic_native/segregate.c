/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

#include <stdint.h>

intptr_t Unknown;

typedef struct {
  intptr_t A;
  intptr_t B;
} TwoIntegers;

typedef struct {
  intptr_t A;
  intptr_t B;
  intptr_t D;
  intptr_t E;
  intptr_t F;
  intptr_t G;
  intptr_t H;
} ManyIntegers;

// RawFunctionType with register arguments, all primitives
intptr_t raw_primitives_on_registers(intptr_t A, intptr_t B) {
  return Unknown + A + B;
}

intptr_t call_raw_primitives_on_registers() {
  return Unknown + raw_primitives_on_registers(1, 2);
}

// RawFunctionType with register arguments, with pointers
intptr_t raw_pointers_on_registers(intptr_t *A, intptr_t *B) {
  return Unknown + *A + *B;
}

intptr_t call_raw_pointers_on_registers() {
  return Unknown + raw_pointers_on_registers(&Unknown, &Unknown);
}

// RawFunctionType with stack arguments, all primitives
intptr_t raw_primitives_on_stack(intptr_t A,
                                 intptr_t B,
                                 intptr_t C,
                                 intptr_t D,
                                 intptr_t E,
                                 intptr_t F,
                                 intptr_t G,
                                 intptr_t H) {
  return Unknown + G + H;
}

intptr_t call_raw_primitives_on_stack() {
  return Unknown + raw_primitives_on_stack(1, 2, 3, 4, 5, 6, 7, 8);
}

// CABIFunctionType with scalar arguments, only in registers
intptr_t cabi_primitives_on_registers(intptr_t A, intptr_t B) {
  return Unknown + A + B;
}

intptr_t call_cabi_primitives_on_registers() {
  return Unknown + cabi_primitives_on_registers(1, 2);
}

// CABIFunctionType with scalar arguments, both in registers and on the stack
intptr_t cabi_primitives_on_stack(intptr_t A,
                                  intptr_t B,
                                  intptr_t C,
                                  intptr_t D,
                                  intptr_t E,
                                  intptr_t F,
                                  intptr_t G,
                                  intptr_t H) {
  return Unknown + G + H;
}

intptr_t call_cabi_primitives_on_stack() {
  return Unknown + cabi_primitives_on_stack(1, 2, 3, 4, 5, 6, 7, 8);
}

// CABIFunctionType with aggregate on registers
intptr_t cabi_aggregate_on_registers(TwoIntegers Arg) {
  return Unknown + Arg.A + Arg.B;
}

intptr_t call_cabi_aggregate_on_registers() {
  return Unknown + cabi_aggregate_on_registers((TwoIntegers){ 1, 2 });
}

// CABIFunctionType with aggregate on the stack
intptr_t cabi_aggregate_on_stack(intptr_t A,
                                 intptr_t B,
                                 intptr_t C,
                                 intptr_t D,
                                 intptr_t E,
                                 intptr_t F,
                                 TwoIntegers Arg) {
  return Unknown + Arg.A + Arg.B;
}

intptr_t call_cabi_aggregate_on_stack() {
  return Unknown + cabi_aggregate_on_stack(1, 2, 3, 4, 5, 6, (TwoIntegers){ 1, 2 });
}

// CABIFunctionType with aggregate partly in the registers and partly on the
// stack
intptr_t cabi_aggregate_on_stack_and_registers(intptr_t A,
                                               intptr_t B,
                                               intptr_t C,
                                               intptr_t D,
                                               intptr_t E,
                                               TwoIntegers Arg) {
  return Unknown + Arg.A + Arg.B;
}

intptr_t call_cabi_aggregate_on_stack_and_registers() {
  return Unknown + cabi_aggregate_on_stack_and_registers(1,
                                                         2,
                                                         3,
                                                         4,
                                                         5,
                                                         (TwoIntegers){ 1, 2 });
}

TwoIntegers raw_return_small_aggregate(void) {
  TwoIntegers result;
  result.A = 124;
  result.B = 123;
  return result;
}

intptr_t call_raw_return_small_aggregate() {
  return Unknown + raw_return_small_aggregate().B;
}

TwoIntegers cabi_return_small_aggregate(void) {
  TwoIntegers result;
  result.A = 124;
  result.B = 123;
  return result;
}

intptr_t call_cabi_return_small_aggregate() {
  return Unknown + cabi_return_small_aggregate().B;
}

ManyIntegers cabi_return_big_aggregate(void) {
  ManyIntegers result;
  result.D = 123;
  return result;
}

intptr_t call_cabi_return_big_aggregate() {
  return Unknown + cabi_return_big_aggregate().D;
}

int main(int argc, char *argv[]) {
  return (call_raw_primitives_on_registers()
          + call_raw_pointers_on_registers()
          + call_raw_primitives_on_stack()
          + call_cabi_primitives_on_registers()
          + call_cabi_primitives_on_stack()
          + call_cabi_aggregate_on_registers()
          + call_cabi_aggregate_on_stack()
          + call_cabi_aggregate_on_stack_and_registers()
          + call_raw_return_small_aggregate()
          + call_cabi_return_small_aggregate()
          + call_cabi_return_big_aggregate());
}
