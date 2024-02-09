# The end to end example of what happens within the testing suite

Before I start with the example, let's outline the goal in a couple of words. The only thing we can trust, as far as ABI specifics are concerned, is the compiler. So, to check ABI compliance, we compose a special test binary filled with whole lots of calls to different functions, while passing different sets of arguments, filled with randomly generated data. Then we use a special "runner" that injects breakpoints in and dumps the state at a couple of points per call.

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
  uint64_t current_offset = 0;

  address_dumps[0] = (uint64_t) (size_t) &argument_0;
  *(small_struct *) (value_dumps + current_offset) = argument_0;
  current_offset += sizeof(small_struct);
  address_dumps[1] = (uint64_t) (size_t) &argument_1;
  *(big_struct *) (value_dumps + current_offset) = argument_1;
  current_offset += sizeof(big_struct);
}
```
Here, `value_dumps` and `current_offset` are dedicated global variables.
In essence, the only important thing it does is copies the values of the arguments using the default c assignment operator into a dedicated memory location (and keeps track of the offset into the said location - we wouldn't want to override an argument with the next one).

The setup for return value test is really similar, just in the opposite direction:
```cpp
NOINLINE small_struct ABIDEF test_small_rv(void) {
  size_dumps[0] = sizeof(small_struct);
  _Static_assert(sizeof(small_struct) <= 256,
                 "Size of the return value exceeds the allowed maximum.");

  small_struct returned = *(small_struct *) expected_state;

  return returned;
}
NOINLINE big_struct ABIDEF test_bitg_rv(void) {
  size_dumps[0] = sizeof(big_struct);
  _Static_assert(sizeof(big_struct) <= 256,
                 "Size of the return value exceeds the allowed maximum.");

  big_struct returned = *(big_struct *) expected_state;

  return returned;
}
```
Here, `expected_state` is a dedicated global variable filled with random data.
In essence, it copies some known random data into a new variable and returns it, allowing the compiler to freely place wherever said value is expected.

These two files together (with the help of `functions.c` file that provides some variable definitions, basic endianness verification, etc) are compiled into the `"functions binary"` (see the graph [here](./templates/) for how it fits into the big picture).
The debug information from the binary is then extracted to be used as the base of the test - since it is what's used to produce the models (look into the `ImportBinary` analysis for details) used until the very end.

On top of the binary provided to the `ImportBinary` analysis, there are two more distinct ones that are used in a tandem: the `"mmap'ed binary"` and the `"runner binary"`. Let's look into them one at a time (see the same graph [here](./templates/) for the big picture).

`"mmap'ed binary"` boils down to two compiling a pair of templates together: `functions.inc` containing `text_X` bodies (the same ones as in the previous binary), and `setup.inc` containing `setup_X` bodies.

For argument tests, it generates something like this:
```cpp
void NOINLINE setup_args(void) {
  small_struct current_offset = 0;
  _Static_assert(sizeof(small_struct) <= 256,
                 "Size of an argument exceeds the allowed maximum.");
  size_dumps[0] = sizeof(small_struct);
  small_struct argument_0 = *(small_struct *) (expected_state + current_offset);
  current_offset += sizeof(small_struct);
  
  _Static_assert(sizeof(big_struct) <= 256,
                 "Size of an argument exceeds the allowed maximum.");
  size_dumps[1] = sizeof(big_struct);
  big_struct argument_1 = *(big_struct *) (expected_state + current_offset);
  current_offset += sizeof(big_struct);

  test_args(argument_0, argument_1);
}
```

As for the return values (the second part of `"mmap'ed binary"`), the generated file is even simpler for those, it looks something like:
```cpp
void NOINLINE setup_small_rv(void) {
  typedef small_struct return_value_type;

  return_value_type returned = test_small_rv();
  address_dumps[0] = (uint64_t) (size_t) &returned;
  *(return_value_type *) (value_dumps) = returned;
}
void NOINLINE setup_big_rv(void) {
  typedef big_struct return_value_type;

  return_value_type returned = test_big_rv();
  address_dumps[0] = (uint64_t) (size_t) &returned;
  *(return_value_type *) (value_dumps) = returned;
}
```
Which boils down to accepting whatever random data the callee returned and saving both its pointer (so it can later be compared against register values to detect if return value is passed around as a pointer or not) and its raw value into a predefined location.

Linking the results of compilation of these two templates together leads to the `"mmap'ed binary"`.

But we're not done with it yet. Now, that it each of the functions and memory locations we are interested in has found its place (and more importantly, a specific address) within the binary, we need a way to list all those addresses, which is where `gather_symbols.py` script comes in.

It takes in the output of `objdump` command of the `"mmap'ed binary"` as well as a list of variables to carry over. Currently, the list looks like
```
expected_state value_dumps
```

Based on this, `gather_symbols.py` outputs a header, which for this example would look something like
```cpp
struct section {
  char name[6];
  uint64_t size, vma, file_offset;
};
struct memory {
  char name[15];
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
    .name = "expected_state",
    .address = 0x2024000
  }, {
    .name = "value_dumps",
    .address = 0x2024270
  }, {
    .name = "size_dumps",
    .address = 0x20244e0
  }, {
    .name = "address_dumps",
    .address = 0x20245e0
  }, },
  .functions = { {
    .name = "args",
    .address = 0x20078c0,
    .entry_point = 0x20011e0,
    .callsite_count = 1,
    .callsites = { {
      .before = 0x20012d8,
      .after = 0x20012dd
    } }
  }, {
    .name = "small_rv",
    .address = 0x2007920,
    .entry_point = 0x20012f0,
    .callsite_count = 1,
    .callsites = { {
      .before = 0x2001409,
      .after = 0x200140e
    } }
  }, {
    .name = "big_rv",
    .address = 0x2007980,
    .entry_point = 0x2001420,
    .callsite_count = 1,
    .callsites = { {
      .before = 0x2001674,
      .after = 0x2001679
    } }
  }, }, };
```

This header is then used as the _only_ source of truth for compiling the non-template `runner.c` file.
For more details about it, see the comments within the file itself.

The other half of the runner is represented in `decoders.c`. Let's also look at it piece by piece.

First there is a function for each test, so `test_X` would always have a `decode_X` counterpart that decodes the memory location written in `function.inc` above and dumps it to stdout in a predetermined format. To improve reusability, the format itself is encoded into the `DECODE_A_SINGLE_VALUE` macro. It accepts the type, the size, the address in memory and two pointers to data: "expected" input and "found" output.

Notice how similar the functions for decoding arguments are to those for return values:
```cpp
static void decode_args(const struct encoded value) {
  uint64_t current_offset = 0;
  puts("    Arguments:");

  DECODE_A_SINGLE_VALUE(small_struct,
                        value.sizes[0],
                        value.addresses[0],
                        value.input + current_offset,
                        value.output + current_offset);
  current_offset += value.sizes[0];

  DECODE_A_SINGLE_VALUE(big_struct,
                        value.sizes[1],
                        value.addresses[1],
                        value.input + current_offset,
                        value.output + current_offset);
  current_offset += value.sizes[1];
}

static void decode_small_rv(const struct encoded value) {
  puts("    ReturnValues:");
  DECODE_A_SINGLE_VALUE(small_struct,
                        value.sizes[0],
                        value.addresses[0],
                        value.input + 0,
                        value.output + 0);
}
static void decode_big_rv(const struct encoded value) {
  puts("    ReturnValues:");
  DECODE_A_SINGLE_VALUE(big_struct,
                        value.sizes[0],
                        value.addresses[0],
                        value.input + 0,
                        value.output + 0);
}
```

And lastly, there's also a dispatcher. To avoid hard-coding the names of all the functions into the runner, it just passes them as a first argument here and the dispatcher is the one responsible for selecting the right printer in each case.
```cpp
void decode(const char *name, const struct encoded value) {
  if (strcmp("args", name) == 0)
    return decode_args(value);
  if (strcmp("small_rv", name) == 0)
    return decode_small_rv(value);
  if (strcmp("big_rv", name) == 0)
    return decode_big_rv(value);
  assert((0, "unknown test"));
}
```

It might be useful to compare how the "encoder" (`functions.inc`) and the "decoder" (`decoders.inc`) use the same memory location to communicate the values of predefined object from one binary to the other.

Linked together, `runner.c` and `decoders.c` form the last out of three binaries this testing suite uses: `"runner binary"`. Which is all that the `revng-qa` side of the testing pipeline produces.

(for the continuation see [`share/revng/test/tests/abi/end_to_end.md`](https://github.com/revng/revng/blob/develop/share/revng/test/tests/abi/end_to_end.md) in the `revng` repository)
