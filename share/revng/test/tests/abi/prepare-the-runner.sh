#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

function prepare-the-runner() {
  if [ $# -ne 4 ]; then
    echo 'prepare-the-runner ${TRIPLE} ${CFLAGS} ${INPUT_DIRECTORY} ${OUTPUT_DIRECTORY}' > /dev/stderr
    exit 1
  fi

  TRIPLE="$1"
  CFLAGS="$2"
  INPUT_DIRECTORY="$3"
  OUTPUT_DIRECTORY="$4"

  # Run `gather_symbols.py`: it outputs a header that is used when building the runner.
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
}
