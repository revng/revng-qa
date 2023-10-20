# Documentation for the configuration options provided by this generator

## `common.yml`

This file is used to declare global options that stay the same for every architecture and every test function other configuration defines.

- `stack_byte_count` - number of stack bytes to generate for each test call.
- `lfsr_seed` - the seed passed to the random number generator.
- `iteration_count` - number of times each test should be repeated. Multiple iterations are a good way to make colliding results caused by limited randomness of generated numbers have no practical impact on the test results.

## `functions.yml`

This file is used to declare the list of test functions (and structs they use) in an architecture-independent manner.

### - `helper_structs`

Declares a list of structs used by the tests, they are declared in this format:
```yaml
struct_name:
  - "type_of_field_0 name_of_field_0"
  - "type_of_field_1 name_of_field_1"
  - "type_of_field_2 name_of_field_2"
  - ...
```
Types can be arbitrary, both pointers and arrays are also allowed.

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
Pointers are allowed, arrays are not.

### - `return_value_tests`

Declares a list of test function declared in this format:
```yaml
function_name: "return_value_type"
```
Note that this is not an array, as only a function can only have a single return value type.
Pointers are allowed, arrays are not.

### Additional notes

- On top of typical C types (signed and unsigned versions of `int8_t`, `int16_t`, `int32_t`, and `int64_t`), special macros are provided for `INT128_T` and `UINT128_T`. They are replaced by the compiler specific version of the type if the compiler provides one for the given architecture or by a similarly sized struct for those that do not.

## `architectures.yml`

(OUTDATED)

This file contains a list of separate dictionary entries each of which represents a single supported architecture and MUST provide the value for each of the following fields:

- `register_type` - the C type equivalent for a single generic purpose register (typical values include `uint64_t` for 64-bit architectures and `uint32_t` for 32-bit ones). This value is used as the internal placeholder for potential register values, it's really important for the test integrity that the size of this type matches the actual register size.
- `register_size` - the size of the `register_type` type.
- `register_list` - the list of register names to test provided in the `__asm`-compatible manner for the supported compilers.

as well as a template for each the following:

- `call_a_function` - architecture-specific way to call a function in the default assembly. Has a single argument:
  - `{{ function_name }}` - is replaced by the name of the callee when this template is used.

- `return_from_function` - architecture-specific way to return from the callee. Has no arguments (must be valid assembly).

- `return_from_function_with_cleanup` - a special case of `return_from_function` used exclusively when setting up big return values in ABIs where callee is responsible for cleaning up the stack argument after the call. Has no arguments (must be valid assembly).  
  It's advised to provide invalid instructions (for example, `.word 0xe7f0def0` in ARM) for each architecture where such callee-cleaned function are not supported by any major ABI.

- `fill_a_register` - architecture-specific way to copy a value from a dedicated location in memory into a specified register. Has three arguments:
  - `register` - the register to fill in.
  - `location` (+ `offset`) - the location to read the data from. Note that offset MUST be added to the location by the template provided here.

- `push_to_stack` - there are two kinds of these instructions, the first line MUST specify which kind is used, options include:
  - `{{ fill_a_register }}` - single register push
  - `{{ fill_two_registers }}` - double register push

  All the following lines after the first one can be used freely, as with any other template.

  In the single register case (the one you should use unless it's really inefficient for the current architecture) - it only has a single argument:
  - `register` to indicate the register value of which has to be pushed to the stack.

  In the double register case - there are two:
  - `register` - the top half of the double-register word to be pushed.
  - `second_register` - the bottom half.

- `cleanup_stack_arguments ` - architecture-specific way of removing the specified number of bytes from the top of the stack (popping them). It has a single argument:
  - `stack_size` - the number of bytes to pop.

- `save_return_address` - architecture-specific way of saving the return address of the current function onto the stack. It has no arguments - so it must be valid assembly.

- `restore_return_address` - architecture-specific way of loading a value from the specified stack offset into the place where function return address is normally placed (for example top of the stack for x86 or `lr` register for ARM). It has a single argument:
  - `stack_size` - the offset from the top of the stack until the value that must be loaded.

