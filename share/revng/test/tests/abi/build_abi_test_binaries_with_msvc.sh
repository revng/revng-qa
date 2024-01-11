#!/bin/bash

#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

set -euo pipefail

if [ $# -gt 7 ]; then
  echo "$0 INPUT_DIRECTORY OUTPUT_DIRECTORY TRIPLE MSVC_TRIPLE CFLAGS MSVC_CFLAGS LDFLAGS" > /dev/stderr
  exit 1
fi

INPUT_DIRECTORY="$1"
OUTPUT_DIRECTORY="$2"
TRIPLE="$3"
MSVC_TRIPLE="$4"
CFLAGS="$5"
MSVC_CFLAGS="$6"
LDFLAGS="$7"

test -n "${INPUT_DIRECTORY}"
test -n "${OUTPUT_DIRECTORY}"
test -n "${TRIPLE}"
test -n "${MSVC_TRIPLE}"
test -n "${CFLAGS}"
test -n "${MSVC_CFLAGS}"
test -n "${LDFLAGS}"

mkdir -p "${OUTPUT_DIRECTORY}"

# Build the "functions" binary
timeout 30 ${MSVC_TRIPLE}cl \
  ${MSVC_CFLAGS} -O2 -std:c11 -Zi -GS- \
  "z:\\${INPUT_DIRECTORY}/functions.c" \
  -Fe"${OUTPUT_DIRECTORY}/functions.exe" \
  -link ${LDFLAGS} /opt:ref,noicf \
  /pdb:"${OUTPUT_DIRECTORY}/functions.pdb" \
  /map:"${OUTPUT_DIRECTORY}/functions.map"

# Build the binary for runner to `mmap`
timeout 30 ${MSVC_TRIPLE}cl \
  ${MSVC_CFLAGS} -O2 -std:c11 -Zi -GS- \
  "z:\\${INPUT_DIRECTORY}/setup.c" \
  -Fe"${OUTPUT_DIRECTORY}/foreign-executable.exe" \
  -link ${LDFLAGS} /opt:ref,noicf /ignore:4281 \
  /dynamicbase:no /base:0x2000000 /fixed /filealign:4096 \
  /pdb:"${OUTPUT_DIRECTORY}/foreign-executable.pdb" \
  /map:"${OUTPUT_DIRECTORY}/foreign-executable-symbols.txt"

# Run `dumpbin` on it
timeout 30 ${MSVC_TRIPLE}dumpbin \
  -nologo -headers \
  "${OUTPUT_DIRECTORY}/foreign-executable.exe" \
  -out:"${OUTPUT_DIRECTORY}/foreign-executable-sections.txt"
timeout 30 ${MSVC_TRIPLE}dumpbin \
  -nologo -symbols -disasm:nobytes \
  "${OUTPUT_DIRECTORY}/foreign-executable.exe" \
  -out:"${OUTPUT_DIRECTORY}/foreign-executable-disassembly.txt"

# Run `gather_symbols.py`
timeout 30 python3 "${INPUT_DIRECTORY}/gather_symbols.py" \
  "expected_state value_dumps address_dumps size_dumps" \
  "${OUTPUT_DIRECTORY}/foreign-executable-sections.txt" \
  "${OUTPUT_DIRECTORY}/foreign-executable-symbols.txt" \
  "${OUTPUT_DIRECTORY}/foreign-executable-disassembly.txt" \
  > "${OUTPUT_DIRECTORY}/gathered_symbols.h"

# Build the `runner`
${TRIPLE}gcc \
  ${CFLAGS} -O3 -D_GNU_SOURCE -static \
  -I"${OUTPUT_DIRECTORY}" \
  "${INPUT_DIRECTORY}/runner.c" \
  "${INPUT_DIRECTORY}/decoders.c" \
  -o "${OUTPUT_DIRECTORY}/probe-calls"

# For consistency with the gcc toolchain, rename binaries
mv "${OUTPUT_DIRECTORY}/functions.exe" "${OUTPUT_DIRECTORY}/functions"
mv "${OUTPUT_DIRECTORY}/foreign-executable.exe" "${OUTPUT_DIRECTORY}/foreign-executable"
