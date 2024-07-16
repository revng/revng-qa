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
  ${CFLAGS} -O3 -ggdb3 -static \
  "${INPUT_DIRECTORY}/functions.c" \
  -o "${OUTPUT_DIRECTORY}/functions"

# Build the binary for runner to `mmap`
"${TRIPLE}gcc" -c \
  ${CFLAGS} -O3 -static \
  -fno-zero-initialized-in-bss \
  -ffreestanding \
  "${INPUT_DIRECTORY}/setup.c" \
  -o "${OUTPUT_DIRECTORY}/foreign-executable.o"
"${TRIPLE}gcc" \
  ${CFLAGS} -O3 -static \
  -Wl,--section-start=.text=0x2000000 \
  -Wl,--section-start=.data=0x3000000 \
  -Wl,--entry=main \
  -nostdlib -nodefaultlibs \
  "${OUTPUT_DIRECTORY}/foreign-executable.o" \
  "${INPUT_DIRECTORY}/musl_memory_functions.c" \
  -o "${OUTPUT_DIRECTORY}/foreign-executable"

# Run `objdump` on it
"${TRIPLE}objdump" \
  ${OBJDUMP_FLAGS} \
  --wide -h \
  "${OUTPUT_DIRECTORY}/foreign-executable" \
  > "${OUTPUT_DIRECTORY}/foreign-executable-sections.txt"
${TRIPLE}objdump \
  ${OBJDUMP_FLAGS} \
  --wide -t \
  "${OUTPUT_DIRECTORY}/foreign-executable" \
  > "${OUTPUT_DIRECTORY}/foreign-executable-symbols.txt"
${TRIPLE}objdump \
  ${OBJDUMP_FLAGS} \
  --wide --no-show-raw-insn -d \
  "${OUTPUT_DIRECTORY}/foreign-executable" \
  > "${OUTPUT_DIRECTORY}/foreign-executable-disassembly.txt"

# Run `gather_symbols.py`
python3 "${INPUT_DIRECTORY}/gather_symbols.py" \
  "expected_state value_dumps address_dumps size_dumps" \
  "${OUTPUT_DIRECTORY}/foreign-executable-sections.txt" \
  "${OUTPUT_DIRECTORY}/foreign-executable-symbols.txt" \
  "${OUTPUT_DIRECTORY}/foreign-executable-disassembly.txt" \
  > "${OUTPUT_DIRECTORY}/gathered_symbols.h"

# Build the `runner`
"${TRIPLE}gcc" \
  ${CFLAGS} -O3 -D_GNU_SOURCE -static \
  -I"${OUTPUT_DIRECTORY}" \
  "${INPUT_DIRECTORY}/runner.c" \
  "${INPUT_DIRECTORY}/decoders.c" \
  -o "${OUTPUT_DIRECTORY}/probe-calls"
