#!/bin/bash

#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

set -euo pipefail

if [ $# -gt 5 ] || [ $# -lt 4 ]; then
  echo "$0 INPUT_DIRECTORY OUTPUT_DIRECTORY TRIPLE CFLAGS [OBJDUMP_FLAGS]" > /dev/stderr
  exit 1
fi

INPUT_DIRECTORY="$1"
OUTPUT_DIRECTORY="$2"
TRIPLE="$3"
CFLAGS="$4"
OBJDUMP_FLAGS="$5"

test -n "${INPUT_DIRECTORY}"
test -n "${OUTPUT_DIRECTORY}"
test -n "${TRIPLE}"
test -n "${CFLAGS}"

mkdir -p "${OUTPUT_DIRECTORY}"

# Build the "functions" binary
"${TRIPLE}gcc" \
  ${CFLAGS} -O3 -ggdb3 \
  "${INPUT_DIRECTORY}/functions.c" \
  -o "${OUTPUT_DIRECTORY}/functions"

# Build the binary for runner to `mmap`
"${TRIPLE}gcc" \
  ${CFLAGS} -O3 -static \
  -Wl,--section-start=.text=0x2000000 \
  -fno-zero-initialized-in-bss \
  "${INPUT_DIRECTORY}/gcc/setup_arguments.S" \
  "${INPUT_DIRECTORY}/setup_return_values.c" \
  -o "${OUTPUT_DIRECTORY}/foreign-executable"

# Run `objdump` on it
"${TRIPLE}objdump" \
  ${OBJDUMP_FLAGS} \
  --wide --no-show-raw-insn -h -t -d \
  "${OUTPUT_DIRECTORY}/foreign-executable" \
  > "${OUTPUT_DIRECTORY}/foreign-executable.S"

# Run `gather_symbols.py`
python3 "${INPUT_DIRECTORY}/gather_symbols.py" \
  "expected_state value_dumps" \
  < "${OUTPUT_DIRECTORY}/foreign-executable.S" \
  > "${OUTPUT_DIRECTORY}/gathered_symbols.h"

# Build the `runner`
"${TRIPLE}gcc" \
  ${CFLAGS} -O3 -D_GNU_SOURCE -static \
  -I"${OUTPUT_DIRECTORY}" \
  "${INPUT_DIRECTORY}/runner.c" \
  "${INPUT_DIRECTORY}/printers.c" \
  -o "${OUTPUT_DIRECTORY}/probe-calls"
