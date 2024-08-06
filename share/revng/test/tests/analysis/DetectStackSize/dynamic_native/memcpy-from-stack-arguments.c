#include <stdint.h>

typedef struct {
  uint8_t data[512];
} data_512_t;

data_512_t global;

void memcpy_from_stack_arguments(data_512_t argument) __attribute__((weak));
void memcpy_from_stack_arguments(data_512_t argument) {
  global = argument;
}

void main() {
  data_512_t argument = {};
  memcpy_from_stack_arguments(argument);
}
