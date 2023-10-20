# The end to end example of what happens within the testing suite

Before I start with the example, let's outline the goal in a couple of words. We can only trust the compiler. So, to check how it emits the ABIs to be used, we provide a callee with some data from `randomized_state` - and make it outputs what it thinks the arguments / return values are into a separate `printable_location`. Then, by comparing the two, it's possible to piece together all the information about the ABI we need. Now, let's look at the whole process one step at a time.

The suite starts with the generator (see [here](./README.md)), which, in its essence, is just a python scripts that consumes templates (see [here](./templates/)) and configuration files (see [here](./config/)) into the sources.

Let's look into a fictional example with only a couple of test functions:
```yaml
# functions.yml
structs:
  small_struct: ["uint16_t a", "uint16_t b"]
  big_struct: ["uint64_t a[8]"]

argument_tests:
  args: ["small_struct", big_struct]

return_value_tests:
  small_rv: "small_struct"
  big_rv: "big_struct"
```

Based on this configuration file, the generated `functions.h` will look something like this:
```cpp
typedef struct {
  uint16_t a;
  uint16_t b;
} small_struct;
typedef struct {
  uint64_t a[8];
} big_struct;

void setup_args(void);
NOINLINE void ABIDEF test_args(small_struct argument_0, big_struct argument_1);

void setup_small_rv(void);
NOINLINE small_struct ABIDEF test_small_rv(void);

void setup_big_rv(void);
NOINLINE big_struct ABIDEF test_big_rv(void);
```
There's not much to say about it: these are the same structs and function prototypes seen in the yaml about, but converted to C.

You might also notice that each test also has a `setup_X` counterpart: it serves the purpose of the entry point for the specific test, in other words, the _caller_, while `test_X` is the analyzed _callee_.

`functions.inc` is quite a bit more interesting, as it contains the bodies of all the `test_X` functions. For arguments it looks like this:
```cpp
NOINLINE void ABIDEF test_args(small_struct argument_0, big_struct argument_1) {
  current_offset = 0;

  /* heavy omittions here, see original template for specifics, but this is enough to convey the basic idea */
  *(small_struct *) (printable_location + current_offset) = argument_0;
  current_offset += sizeof(small_struct)
  *(big_struct *) (printable_location + current_offset) = argument_1;
  current_offset += sizeof(big_struct)
}
```
Here, `printable_location` and `current_offset` are dedicated global variables.
In essence, the only important thing it does is copies the values of the arguments using the default c assignment operator into a dedicated memory location (and keeps track of the offset into the said location - we wouldn't want to override an argument with the next one).

The setup for return value test is really similar, just in the opposite direction:
```cpp
NOINLINE small_struct ABIDEF test_small_rv(void) {
  small_struct returned = *(small_struct *) randomized_state;
  return returned;
}
NOINLINE big_struct ABIDEF test_bitg_rv(void) {
  big_struct returned = *(big_struct *) randomized_state;
  return returned;
}
```
Here, `randomized_state` is a dedicated global variable filled with random data.
In essence, it copies some known random data into a new variable and returns it, allowing the compiler to freely place wherever said value is expected.

These two files together (with the help of `functions.c` file that provides some variable definitions, basic endianness verification, etc) are compiled into the `"functions binary"` (see the graph [here](./templates/) for how it fits into the big picture).
The debug information from the binary is then extracted to be used as the base of the test - since it is what's used to produce the models (look into the `ImportBinary` analysis for details) used until the very end.

On top of the binary provided to the `ImportBinary` analysis, there are two more distinct ones that are used in a tandem: the `"mmap'ed binary"` and the `"runner binary"`. Let's look into them one at a time (see the same graph [here](./templates/) for the big picture).

`"mmap'ed binary"` boils down to two distinct source files: one generated in assembly containing `setup_{{ test_name }}` function definitions for the argument tests, and the other one containing everything else (it's generated in C, and on top of containing bodies of `setup_{{ test_name }}`s for return value tests it also includes `functions.inc`).

`setup_arguments.S` (the first one) is heavily dependent on the architecture, but for the sake of simplicity, let's only look into a single one (`x86_64`) for the current example. Let's look into the example function bit by bit:

It starts by saving the return value address (it's architecture-dependent where it's location, for `x86_64` it's the top of the stack) into a dedicated `saved_return_address` global memory location.
```asm
.global setup_args
setup_args:
  pop rax
  mov qword ptr [saved_return_address], rax
```

Then it pushes randomly generated data from `randomized_state` onto the stack one word at a time, until the expected stack space (defined in the `common.yml`) is filled.
```asm
  mov rax, qword ptr [randomized_state + 616]
  push rax
  ;; (more pushes omitted)
  mov rax, qword ptr [randomized_state + 112]
  push rax
```

After which it replaces values of all easily accessible the registers with random values from the same `randomized_state` location.
```asm
  mov r15, qword ptr [randomized_state + 104]
  mov r14, qword ptr [randomized_state + 96]
  mov r13, qword ptr [randomized_state + 88]
  mov r12, qword ptr [randomized_state + 80]
  mov r11, qword ptr [randomized_state + 72]
  mov r10, qword ptr [randomized_state + 64]
  mov r9, qword ptr [randomized_state + 56]
  mov r8, qword ptr [randomized_state + 48]
  mov rdi, qword ptr [randomized_state + 40]
  mov rsi, qword ptr [randomized_state + 32]
  mov rdx, qword ptr [randomized_state + 24]
  mov rcx, qword ptr [randomized_state + 16]
  mov rbx, qword ptr [randomized_state + 8]
  mov rax, qword ptr [randomized_state + 0]
```

Then it contains a call instruction to the test body itself.
```asm
  call test_args
```

In some cases `longjmp` crashes unless the stack is restored beforehand, so do that:
```asm
#ifndef CalleeIsResponsibleForStackCleanup
  add rsp, 512
#endif
```


And, finally, it restores the saved at the very beginning return address and jumps to it.
```asm
  mov rax, qword ptr [saved_return_address]
  push rax
  ret
```

It's important to note, that the callee is not aware in which conditions it has been called, so it just treats the pre-placed random data as its expected inputs. But thanks to knowing exactly what bytes were put where, we can later trace it all back to determine where exactly did the callee look for each of the arguments.

As for the return values (the second part of `"mmap'ed binary"`), the generated file is even simpler for those, it looks something like:
```cpp
void setup_small_rv(void) {
  small_struct returned = test_small_rv();
  *(small_struct **) printable_location = &returned;
  *(small_struct *) (printable_location + 8) = returned;
}
void setup_big_rv(void) {
  big_struct returned = test_big_rv();
  *(big_struct **) printable_location = &returned;
  *(big_struct *) (printable_location + 8) = returned;
}
```
Which boils down to accepting whatever random data the callee returned and saving both its pointer (so it can later be compared against register values to detect if return value is passed around as a pointer or not) and its raw value into a predefined location.

Linking the results of compilation of these two objects together leads to the `"mmap'ed binary"`.

But we're not done with it yet. Now, that it each of the functions and memory locations we are interested in has found its place (and more importantly, a specific address) within the binary, we need a way to list all those addresses, which is where `gather_symbols.py` script comes in.

It takes in the output of `objdump` command of the `"mmap'ed binary"` as well as a list of variables to carry over. Currently, the list looks like
```
randomized_state printable_location
```

Based on this, `gather_symbols.py` outputs a header, which for this example would look something like
```cpp
struct section {
  char name[6];
  uint64_t size, vma, file_offset;
};
struct memory {
  char name[19];
  uint64_t address;
};
struct callsite {
  uint64_t before, after;
};
struct function {
  char name[9];
  uint64_t address;
  uint64_t entry_point;
  uint64_t callsite_count;
  struct callsite callsites[1];
};
const struct {
  struct section sections[1];
  struct memory variables[2];
  struct function functions[3];
} input = {
  .sections = { {
    .name = ".text",
    .size = 0x00009bb1,
    .vma = 0x0000000002000000,
    .file_offset = 0x00002000
  }, },
  .variables = { {
    .name = "randomized_state",
    .address = 0x200c2e0
  }, {
    .name = "printable_location",
    .address = 0x200c060
  }, },
  .functions = { {
    .name = "args",
    .address = 0x2000175,
    .entry_point = 0x2001656,
    .callsite_count = 1,
    .callsites = { {
      .before = 0x200190f,
      .after = 0x2001914
    } }
  }, {
    .name = "small_rv",
    .address = 0x2000188,
    .entry_point = 0x2001925,
    .callsite_count = 1,
    .callsites = { {
      .before = 0x2001bde,
      .after = 0x2001be3
    } }
  }, {
    .name = "big_rv",
    .address = 0x200019b,
    .entry_point = 0x2001bf4,
    .callsite_count = 1,
    .callsites = { {
      .before = 0x2001ead,
      .after = 0x2001eb2
    } }
  }, }, };
```

This header is then used as the _only_ source of truth for compiling the non-template `runner.c` file.
For more details about it, see the comments within the file itself.

The other half of the runner is represented in `printers.c`. Let's also look at it piece by piece.

First there is a function for each test, so `test_X` would always have a `print_X` counterpart that decodes the memory location written in `function.inc` above and dumps it to stdout in a predetermined format. To improve reusability, the format itself is encoded into the `ARGUMENT` macro. It accepts the type of the argument, a unique name to use internally, and the pointer to the location to read the data from, and an offset to add to the said pointer. Note that it updates the offset as well, so the continuous instances of it keep reading data further and further away from the `output` pointer.
```cpp
ptrdiff_t current_offset = 0;
static void ABIDEF print_args(const uint8_t *output) {
  current_offset = 0;
  puts("    Arguments:");
  ARGUMENT(small_struct, argument_0, output, current_offset);
  ARGUMENT(big_struct, argument_1, output, current_offset);
}
```

Return value printers work similarly, except instead of a single "blob" of data per argument, they only have a single return value, BUT they also print the bytes put into the return value initially (`input`) the call as well the address of the returned value as a pointer.

Similarly to return value, the specific formatting is locked to the `PRINT`. But unlike the raw `PRINTER` there's also a `print_a_register` helper, it differs in that it also provides a `Value` field, which leads to the ability to not only compare this pointer to a register byte-wise but also to compute the difference between the two values.
```cpp
static void print_small_rv(const uint8_t *input, const uint8_t *output) {
  puts("    ReturnValueAddress:");
  print_a_register("small_struct *", output);
  puts("    ReturnValue:");
  PRINT(small_struct, (output + 8));
  puts("    ExpectedReturnValue:");
  PRINT(small_struct, input);
}
static void print_big_rv(const uint8_t *input, const uint8_t *output) {
  puts("    ReturnValueAddress:");
  print_a_register("big_struct *", output);
  puts("    ReturnValue:");
  PRINT(big_struct, (output + 8));
  puts("    ExpectedReturnValue:");
  PRINT(big_struct, input);
}
```

And lastly, there's also a dispatcher. To avoid hard-coding the names of all the functions into the runner, it just passes them as a first argument here and the dispatcher is the one responsible for selecting the right printer in each case.
```cpp
void print(const char *name, const uint8_t *input, const uint8_t *output) {
  if (strcmp("args", name) == 0)
    return print_args(output);
  if (strcmp("small_rv", name) == 0)
    return print_small_rv(input, output);
  if (strcmp("big_rv", name) == 0)
    return print_big_rv(input, output);
  assert((0, "unknown test"));
}
```

It might be useful to compare how the "encoder" (`functions.inc`) and the "decoder" (`printers.inc`) use the same memory location to communicate the values of predefined object from one binary to the other.

Linked together, `runner.c` and `printers.c` form the last out of three binaries this testing suite uses: `"runner binary"`. Which is all that the `revng-qa` side of the testing pipeline produces.

(for the continuation see [`share/revng/test/tests/abi/end_to_end.md`](https://github.com/revng/revng/blob/develop/share/revng/test/tests/abi/end_to_end.md) in the `revng` repository)
