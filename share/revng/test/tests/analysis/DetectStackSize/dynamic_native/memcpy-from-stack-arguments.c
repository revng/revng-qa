#include <stddef.h>
#include <stdint.h>

typedef struct {
  uint64_t data[512 / 8];
} data_512_t;

data_512_t global;

void memcpy_from_stack_arguments(data_512_t argument) __attribute__((weak));
void memcpy_from_stack_arguments(data_512_t argument) {
  global = argument;
}

void backward_memcpy_from_stack_arguments(data_512_t argument)
  __attribute__((weak));
void backward_memcpy_from_stack_arguments(data_512_t argument) {
  for (size_t I = 0; I < 512 / 8; ++I) {
    size_t inverse_index = 512 / 8 - I - 1;
    global.data[inverse_index] = argument.data[inverse_index];
  }
}

void main() {
  data_512_t argument = {};
  memcpy_from_stack_arguments(argument);
  backward_memcpy_from_stack_arguments(argument);
}
