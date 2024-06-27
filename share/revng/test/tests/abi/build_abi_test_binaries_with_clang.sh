#!/bin/bash

#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

set -euo pipefail

if [ $# -gt 7 ] || [ $# -lt 6 ]; then
  echo "$0 INPUT_DIRECTORY OUTPUT_DIRECTORY TRIPLE CFLAGS CLANG_CFLAGS ARCHITECTURE [OBJDUMP_FLAGS]" > /dev/stderr
  exit 1
fi

INPUT_DIRECTORY="$1"
OUTPUT_DIRECTORY="$2"
TRIPLE="$3"
CFLAGS="$4"
CLANG_CFLAGS="$5"
ARCHITECTURE="$6"
OBJDUMP_FLAGS="$7"

test -n "${INPUT_DIRECTORY}"
test -n "${OUTPUT_DIRECTORY}"
test -n "${TRIPLE}"
test -n "${CFLAGS}"

mkdir -p "${OUTPUT_DIRECTORY}"

# Build the "functions" binary
# NOTE: Since macho support in revng is currently limited, temporarily build
#       this binary with no regard to the specific architecture this code is
#       testing. We can get away with that since it's only ever used to extract
#       information about the prototypes from (in this case) dwarf.
# TODO: when our macho support improves, switch to the proper binary, so that
#       it serves as an extra test of the importer.
"${TRIPLE}gcc" \
  ${CFLAGS} -O3 -ggdb3 -static \
  "${INPUT_DIRECTORY}/functions.c" \
  -o "${OUTPUT_DIRECTORY}/functions"

# Build the binary for runner to `mmap`
clang -c \
  ${CFLAGS} ${CLANG_CFLAGS} -O3 -static \
  -fno-zero-initialized-in-bss -ffreestanding \
  -fno-optimize-sibling-calls -fno-pic \
  -Wno-unused-command-line-argument \
  "${INPUT_DIRECTORY}/setup.c" \
  -o "${OUTPUT_DIRECTORY}/foreign-executable.o"
clang -c \
  ${CFLAGS} ${CLANG_CFLAGS} -O3 -static \
  -fno-zero-initialized-in-bss -ffreestanding \
  -nostdlib -nodefaultlibs -fno-pic \
  -Wno-pointer-to-int-cast -Wno-unused-command-line-argument \
  "${INPUT_DIRECTORY}/musl_memory_functions.c" \
  -o "${OUTPUT_DIRECTORY}/musl_memory_functions.o"
ld64.lld \
  -arch ${ARCHITECTURE} \
  -platform_version macos 14.5 1 \
  "${OUTPUT_DIRECTORY}/foreign-executable.o" \
  "${OUTPUT_DIRECTORY}/musl_memory_functions.o" \
  -o "${OUTPUT_DIRECTORY}/foreign-executable"

# Run `objdump` on it
llvm-objdump \
  ${OBJDUMP_FLAGS} \
  --wide -p \
  "${OUTPUT_DIRECTORY}/foreign-executable" \
  > "${OUTPUT_DIRECTORY}/foreign-executable-sections.txt"
llvm-objdump \
  ${OBJDUMP_FLAGS} \
  --wide -t \
  "${OUTPUT_DIRECTORY}/foreign-executable" \
  > "${OUTPUT_DIRECTORY}/foreign-executable-symbols.txt"
llvm-objdump \
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
