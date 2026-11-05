# The test functions

This directory contains a number of YAML files filled with test functions.

Note that this directory is iterated at the generation time and *every* YAML file it contains is merged into a single set of maps, key by key.

For example, the `argument_tests` entries from all the files are pooled together and end up as a single `argument_tests` map inside the generator. Meaning that the grouping is purely logical and the resulting header file (so, the produced binary) does not preserve it - leading to a single, flat list of tests. The same is true for all the maps.

Note that non-map entries (like `stack_argument_test_prefix_count`) must only appear *once.* A hard generation-time error otherwise. Same for multiples of the same key (think, function name) across different files.

The tests are split into the following groups.

### `1-simple-arguments.yml`

Each simple argument test accepts exactly one argument.

The goal is making sure all the different structs and/or unions do not cause any obvious issues when passed in.

NOTE: when adding a new struct/union, it's highly recommended to add a simple test for it here too. To serve as a canary, making sure other tests using it really test what they are designed for and are not tripped by the basic placement logic and/or alignment computation.

NOTE: which also applies the other way around. When there are multiple failing tests, `simple_` functions should be handled first - as they usually indicate a wider issue as opposed to something more specific defined in other files.

This file is divided into sections:
- 1. Integer primitives and pointers.
- 2. Floating point primitives.
- 3. Simple structs and unions.
- 4. Special structs and unions (big, padded, misaligned, etc)
- 5. Simple structs and unions with *only* floating point fields.
- 6. Simple structs and unions with a mix floating point and integer fields.
- 7. Special structs and unions (big, padded, misaligned, etc) with floating point fields.

### `2-return-values.yml`

This file mirrors `1-simple-arguments.yml` pretty closely and it's recommended to keep the two in sync.

The two files are divided into the exact same set of sections and most of the comments in `1-simple-arguments.yml` apply to `2-return-values.yml` just as well.

### `3-multiple-arguments.yml`

These tests are designed to ensure the argument register lists are set correctly as well to verify that mixing differently-sized arguments in registers does not break anything.

These also serve as canaries for more specific tests. Think of them as verifying the baseline logic in the multi-register/stack cases.

### `4-stack-arguments.yml`

Note that this group takes advantage of a specialized generator logic, which allows us to remove the need to spell `gpr_t` 8 times for each test entry.

It is divided into sections:
- 1. Canary tests indicating the stack itself works correctly.
- 2. Simple stack arguments preceded by padded dummy arguments.
- 3. Double-gpr-aligned stack arguments.
- 4. Unnaturally sized stack arguments.
- 5. Unnaturally aligned stack arguments.
- 6. Stack arguments involving floating point.

### `5-expressive-power.yml`

These are dedicated tests ensuring the expressive power we require of `abi::Definition` is not mis-used.

Namely, there is a single test group in here for each boolean flag and strategy `abi::Definition` declares - each highlighting a specific quirk we support.

Note that not all the tests belong exclusively to this section, some entries instead contain comments with names of the tests from the other files that highlight the corresponding logic.

### `types.yaml`

This is a helper file containing the type definitions used by the functions declared in all the other files.

Note that currently the definitions of *all* the structs and unions are contained here. While, that's not a hard requirement (the generator would merge `structs:` from different files just as well as it merges `argument_tests:`), it's a good idea to keep them in one place as they are designed to be shared between the tests: for example, it's expected that each struct/union appears as a simple argument test, a return value test AND (except for the simplest ones) among one of the more complex tests dedicated to reproducing specific behavior.

This file is divided into:
- `structs`: structs in the format of `name: [list-of-fields]`.
- `packed_structs`: structs in the format of `name: [list-of-fields]`, identical to `structs`. The only difference is how the generator handles them.
- `unions`: unions in the format of `name: [list-of-fields]`.
