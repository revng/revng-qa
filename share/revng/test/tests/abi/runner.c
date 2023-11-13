/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

/* RUN-default: nope */

/**
 * This file ties together the whole suite by providing a single entry point
 * for the execution.
 *
 * It accepts a path to another binary as the first argument. The file is then
 * mmap'ed and the addresses from within the `gathered_symbols.h` header are
 * used to temporarily replace some of the instructions with interrupts at which
 * the installed interrupt handler saves the state of all the registers and some
 * subset of the stack adjacent to the stack pointer into a predefined location.
 *
 * After saving the state in a specific location, the underlying instruction is
 * restored to its original value, and the execution continues from the same PC
 * (executes the instruction at the same address again without any changes
 * to the state).
 *
 * All the gathered state data (`stored_state` array) is then dumped thanks
 * to external `decode_a_register` and `decode_stack` helpers.
 *
 * Similarly, all the output data from the tests themselves is printed using
 * the `decode` dispatcher (see the docs related to `decoders.c` template).
 */

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "common.h"
#include "gathered_symbols.h"
#include "ucontext_wrapper.h"

uint64_t lfsr = 0;
static uint64_t get_next_lfsr(uint32_t iteration_count) {
  if (lfsr == 0)
    lfsr = constants.seed;
  for (uint32_t iteration = 0; iteration < iteration_count; ++iteration)
    lfsr = (lfsr >> 1) | ((((lfsr >> 1) ^ (lfsr >> 2)) & 1) << 63);
  return lfsr;
}

_Static_assert(sizeof(uint8_t) == 1, "A type with size == 1 is required.");
static void regenerate_the_expected_state(uint8_t *location) {
  for (uint32_t offset = 0; offset < constants.generated_byte_count; ++offset)
    location[offset] = (uint8_t) get_next_lfsr(8);
}

static const struct section *select_a_section(const char *name) {
  const uint64_t count = sizeof(input.sections) / sizeof(struct section);
  for (uint64_t i = 0; i < count; ++i)
    if (strcmp(input.sections[i].name, name) == 0)
      return &input.sections[i];

  return NULL;
}

static const struct memory *select_a_variable(const char *name) {
  const uint64_t count = sizeof(input.variables) / sizeof(struct memory);
  for (uint64_t i = 0; i < count; ++i)
    if (strcmp(input.variables[i].name, name) == 0)
      return &input.variables[i];

  return NULL;
}

struct saved_bytes {
  const struct function *function;
  uint32_t after_the_call, before_the_call, after_the_return;
} saved;

#define ACCESS_A_WORD(address) *((uint32_t *) (size_t) (address))

jmp_buf buffer;
static void test_function(const struct function *function) {
  // Only single callsite tests are supported for now.
  assert(sizeof(function->callsites) == sizeof(struct callsite));

  // Replace interesting instructions with an architecture-specific illegal word
  saved.function = function;
  saved.after_the_call = ACCESS_A_WORD(function->address);
  ACCESS_A_WORD(function->address) = ILLEGAL_INSTRUCTION_WORD;
  saved.before_the_call = ACCESS_A_WORD(function->callsites[0].before);
  ACCESS_A_WORD(function->callsites[0].before) = ILLEGAL_INSTRUCTION_WORD;
  saved.after_the_return = ACCESS_A_WORD(function->callsites[0].after);
  ACCESS_A_WORD(function->callsites[0].after) = ILLEGAL_INSTRUCTION_WORD;

  void (*entry_point)(void) = (void (*)(void))(size_t) function->entry_point;
  if (setjmp(buffer) == 0) {
    entry_point();
    longjmp(buffer, 1);
  }
}

/* Counts the number `SIGILL` was raised shifting the behaviour of the handler
 * accordingly.
 * 0 (first time) corresponds to the signal before the call.
 * 1 (second time) corresponds to the signal after the call.
 * 2 (third time) corresponds to the signal after the return.
 * After which the state loops back to 0.
 */
int current_state = 0;

/* Since it's not safe to call functions like `printf` inside the handler (what
 * if an interrupt happens while inside?), these memory locations are used as
 * an intermediate place to store the state which can be printed later down
 * the line
 */
uint8_t *stored_state[3];
static void malloc_the_stored_state(void) {
  uint64_t allocated_word_count = REGISTER_COUNT + constants.stack_byte_count;
  for (uint32_t i = 0; i < 3; ++i)
    stored_state[i] = (uint8_t *) malloc(sizeof(size_t) * allocated_word_count);
}
static void free_the_stored_state(void) {
  for (uint32_t i = 0; i < 3; ++i)
    free(stored_state[i]);
}

void handler(int signal, siginfo_t *info, void *context) {
  // Abort when an unexpected signal is raised just in case.
  if (signal != SIGILL)
    exit(4);

  // Save registers.
  uint8_t *current_storage = stored_state[current_state];
  for (uint32_t i = 0; i < REGISTER_COUNT; ++i)
    ((size_t *) current_storage)[i] = REGISTER(context, i);

  // Save the stack.
  memcpy(current_storage + REGISTER_COUNT * sizeof(size_t),
         (void *) (size_t) SP(context),
         constants.stack_byte_count);

  // Restore the instruction that caused current interrupt to its original value
  if (current_state == 0)
    ACCESS_A_WORD(saved.function->callsites[0].before) = saved.before_the_call;
  else if (current_state == 1)
    ACCESS_A_WORD(saved.function->address) = saved.after_the_call;
  else if (current_state == 2)
    ACCESS_A_WORD(saved.function->callsites[0].after) = saved.after_the_return;

  if (++current_state > 2)
    current_state = 0;
}

void decode_a_register(const char *name, uint8_t *value);
void decode_stack(uint8_t *data);
static void decode_single_state(uint8_t *data) {
  puts("      Registers:");
  for (uint32_t i = 0; i < REGISTER_COUNT; ++i)
    decode_a_register(register_names[i], data + sizeof(size_t) * i);

  printf("      Stack: ");
  decode_stack(data + sizeof(size_t) * REGISTER_COUNT);
}

static void decode_saved_state(void) {
  puts("    StateBeforeTheCall:");
  decode_single_state(stored_state[0]);
  puts("    StateAfterTheCall:");
  decode_single_state(stored_state[1]);
  puts("    StateAfterTheReturn:");
  decode_single_state(stored_state[2]);
}

void print_header(void);
void decode(const char *name, const struct encoded value);

int main(int argc, char **argv) {
  /* Setup signal handling */
  struct sigaction action = { .sa_sigaction = &handler,
                              .sa_flags = SA_SIGINFO };
  sigemptyset(&action.sa_mask);
  sigaction(SIGILL, &action, NULL);

  if (argc != 2) {
    puts("This tool expects only one argument: a binary to `mmap`.");
    return 1;
  }

  /* load the "inner" binary */
  int binary_file = open(argv[1], O_RDONLY);
  if (binary_file == -1) {
    puts("Unable to open the binary.");
    return 2;
  }
  struct stat binary_file_stats;
  int success = fstat(binary_file, &binary_file_stats);
  if (success == -1) {
    puts("`fstat` failed.");
    return 3;
  }

  const struct section *text_section = select_a_section(".text");
  assert(text_section != NULL);

  void *mapped = mmap((void *) (size_t) text_section->vma,
                      binary_file_stats.st_size,
                      PROT_READ | PROT_WRITE | PROT_EXEC,
                      MAP_PRIVATE | MAP_FIXED,
                      binary_file,
                      text_section->file_offset);
  if (mapped == MAP_FAILED) {
    puts("`mmap` failed.");
    return 4;
  }
  if (mapped != (void *) (size_t) text_section->vma) {
    puts("`mmap` couldn't provide the expected virtual address. Should 'inner' "
         "binary be compiled with a different `section-start` value?");
    return 5;
  }

  puts("---");
  print_header();
  puts("Iterations:");

  const struct memory *expected = select_a_variable("expected_state");
  const struct memory *values = select_a_variable("value_dumps");
  const struct memory *addresses = select_a_variable("address_dumps");
  const struct memory *sizes = select_a_variable("size_dumps");
  const struct encoded encoded = {
    .input = (uint8_t *) (size_t) expected->address,
    .output = (uint8_t *) (size_t) values->address,
    .addresses = (uint64_t *) (size_t) addresses->address,
    .sizes = (uint64_t *) (size_t) sizes->address
  };

  malloc_the_stored_state();

  /* run the tests */
  const uint64_t function_count = sizeof(input.functions)
                                  / sizeof(struct function);
  assert((function_count == constants.function_count,
          "Some functions where lost, is linker trying to be overly smart?"));
  for (uint32_t function = 0; function < function_count; ++function) {
    for (uint32_t i = 0; i < constants.iteration_count; ++i) {
      regenerate_the_expected_state(encoded.input);

      printf("  - Function: \"%s\"\n", input.functions[function].name);
      printf("    Iteration: %d\n", i);
      test_function(&input.functions[function]);
      decode(input.functions[function].name, encoded);
      decode_saved_state();
      puts("");
    }
  }

  puts("...");

  free_the_stored_state();

  success = munmap(mapped, binary_file_stats.st_size);
  if (success == -1) {
    puts("`munmap` failed.");
    return 6;
  }
  success = close(binary_file);
  if (success == -1) {
    puts("`close` failed.");
    return 7;
  }
  return 0;
}
