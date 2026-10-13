# Documentation for the configuration options provided by this generator

## `common.yml`

This file is used to declare global options that stay the same for every architecture and every test function other configuration defines.

- `stack_byte_count` - number of stack bytes to generate for each test call.
- `lfsr_seed` - the seed passed to the random number generator.
- `iteration_count` - number of times each test should be repeated. Multiple iterations are a good way to make colliding results caused by limited randomness of generated numbers have no practical impact on the test results.
- `maximum_argument_count` and `maximum_argument_size` - the maximum number of possible arguments, and their maximum size. They are used when choosing the size of the buffers used for intermediate data storage.

## `functions.yml`

This file is used to declare the list of test functions (and structs they use) in an architecture-independent manner.

### - `structs`

Declares a list of structs used by the tests, they are declared in this format:
```yaml
struct_name:
  - "type_of_field_0 name_of_field_0"
  - "type_of_field_1 name_of_field_1"
  - "type_of_field_2 name_of_field_2"
  - ...
```
Types can be arbitrary, both pointers and arrays are also allowed.

### - `packed_structs`

The way structs are specified here is no different from that of `structs`. Functionally, the only difference is that these functions get declared in the output header inside the `#pragma pack(push, 1)` and `#pragma pack(pop)` pair.

### - `argument_tests`

Declares a list of test function declared in this format:
```yaml
function_name:
  - "type_of_argument_0"
  - "type_of_argument_1"
  - "type_of_argument_2"
  - ...
```
Note that only types are provided, no names.
Pointers are allowed, arrays are not (but you can use structs to simulate them).

### - `return_value_tests`

Declares a list of test function declared in this format:
```yaml
function_name: "return_value_type"
```
Note that this is not an array, as only a function can only have a single return value type.
Pointers are allowed, arrays are not (but you can use structs to simulate them).

### Additional notes

- On top of typical C types (signed and unsigned versions of `int8_t`, `int16_t`, `int32_t`, and `int64_t`), special macros are provided for `INT128_T` and `UINT128_T`. They are replaced by the compiler specific version of the type if the compiler provides one for the given architecture or by a similarly sized struct for those that do not.

## `architectures.yml`

NOTE: this template was heavily used in the past, when we generated the test function bodies as assembly. Now it's slimmed down to just two fields:

- `register_size` - the size of a single general purpose register for a given architecture
- `register_list` - the list of registers we are interested in testing.
