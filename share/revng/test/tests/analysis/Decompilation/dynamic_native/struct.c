/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

#include <stdint.h>

typedef struct __attribute__((packed)) {
  uint32_t first_uint32;
  uint32_t second_uint32;
} TestStruct;

static TestStruct global_struct = { .first_uint32 = 0, .second_uint32 = 0 };

static TestStruct *getGlobalStruct(void) {
  return &global_struct;
}

static uint32_t getFirstField(TestStruct *struct_ptr) {
  return struct_ptr->first_uint32;
}

static uint32_t getSecondField(TestStruct *struct_ptr) {
  return struct_ptr->second_uint32;
}

int main() {
  TestStruct *struct_ptr = getGlobalStruct();
  uint32_t first_field = getFirstField(struct_ptr);
  uint32_t second_field = getSecondField(struct_ptr);

  return first_field + second_field;
}
