#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

function prepare-linux-foreign-executable() {
  if [ $# -ne 5 ]; then
    echo 'prepare-linux-foreign-executable ${TRIPLE} ${CFLAGS} ${OBJDUMP_FLAGS} ${INPUT_DIRECTORY} ${OUTPUT_DIRECTORY}' > /dev/stderr
    exit 1
  fi

  TRIPLE="$1"
  CFLAGS="$2"
  OBJDUMP_FLAGS="$3"
  INPUT_DIRECTORY="$4"
  OUTPUT_DIRECTORY="$5"

  # `-fno-zero-initialized-in-bss` is there for arms, as this leads to us not
  # having to worry about mapping `.bss` too.

  "${TRIPLE}gcc" -c \
    ${CFLAGS} -O3 \
    -fno-zero-initialized-in-bss \
    -ffreestanding \
    "${INPUT_DIRECTORY}/setup.c" \
    -o "${OUTPUT_DIRECTORY}/foreign-executable.o"
  "${TRIPLE}gcc" \
    ${CFLAGS} -O3 \
    -fno-tree-loop-distribute-patterns \
    -Wl,--section-start=.text=0x2000000 \
    -Wl,--section-start=.data=0x3000000 \
    -Wl,--entry=main \
    -nostdlib -nodefaultlibs \
    "${OUTPUT_DIRECTORY}/foreign-executable.o" \
    "${INPUT_DIRECTORY}/musl_memory_functions.c" \
    -o "${OUTPUT_DIRECTORY}/foreign-executable"
    
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
}

function prepare-apple-foreign-executable() {
  if [ $# -ne 6 ]; then
    echo 'prepare-apple-foreign-executable ${TRIPLE} ${CFLAGS} ${OBJDUMP_FLAGS} ${APPLE_ARCHITECTURE_NAME} ${INPUT_DIRECTORY} ${OUTPUT_DIRECTORY}' > /dev/stderr
    exit 1
  fi

  TRIPLE="$1"
  CFLAGS="$2"
  OBJDUMP_FLAGS="$3"
  APPLE_ARCHITECTURE_NAME="$4"
  INPUT_DIRECTORY="$5"
  OUTPUT_DIRECTORY="$6"

  # TODO: add a symlink from `${TRIPLE}clang` to `clang --target=${TRIPLE}`
  #       and use that instead
  clang -c \
    --target=${TRIPLE} \
    ${CFLAGS} -O3 \
    -fno-zero-initialized-in-bss -ffreestanding \
    -fno-optimize-sibling-calls -fno-pic \
    -Wno-unused-command-line-argument \
    "${INPUT_DIRECTORY}/setup.c" \
    -o "${OUTPUT_DIRECTORY}/foreign-executable.o"
  clang -c \
    --target=${TRIPLE} \
    ${CFLAGS} -O3 \
    -fno-zero-initialized-in-bss -ffreestanding \
    -nostdlib -nodefaultlibs -fno-pic \
    -Wno-pointer-to-int-cast -Wno-unused-command-line-argument \
    "${INPUT_DIRECTORY}/musl_memory_functions.c" \
    -o "${OUTPUT_DIRECTORY}/musl_memory_functions.o"

  # TODO: add a symlink for `${TRIPLE}ld64.lld` and use that instead
  ld64.lld \
    -arch ${APPLE_ARCHITECTURE_NAME} \
    -platform_version macos 14.5 1 \
    "${OUTPUT_DIRECTORY}/foreign-executable.o" \
    "${OUTPUT_DIRECTORY}/musl_memory_functions.o" \
    -o "${OUTPUT_DIRECTORY}/foreign-executable"

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
}

function prepare-windows-foreign-executable() {
  if [ $# -ne 5 ]; then
    echo 'prepare-windows-foreign-executable ${TRIPLE} ${CFLAGS} ${LDFLAGS} ${INPUT_DIRECTORY} ${OUTPUT_DIRECTORY}' > /dev/stderr
    exit 1
  fi

  TRIPLE="$1"
  CFLAGS="$2"
  LDFLAGS="$3"
  INPUT_DIRECTORY="$4"
  OUTPUT_DIRECTORY="$5"

  timeout 30 ${TRIPLE}cl -c \
    ${CFLAGS} -O2 -std:c11 -Zi -GS- \
    "z:\\${INPUT_DIRECTORY}/setup.c" \
    -Fo"z:\\${OUTPUT_DIRECTORY}/foreign-executable.obj"
  timeout 30 ${TRIPLE}link \
    ${LDFLAGS} /opt:ref,noicf /filealign:4096 \
    /ignore:4281 /nodefaultlib /entry:main /subsystem:console \
    "z:\\${OUTPUT_DIRECTORY}/foreign-executable.obj" \
    /out:"z:\\${OUTPUT_DIRECTORY}/foreign-executable.exe" \
    /pdb:"z:\\${OUTPUT_DIRECTORY}/foreign-executable.pdb" \
    /map:"z:\\${OUTPUT_DIRECTORY}/foreign-executable-symbols.txt"
    
  timeout 120 ${MSVC_TRIPLE}dumpbin \
    -nologo -headers \
    z:\\"${OUTPUT_DIRECTORY}/foreign-executable.exe" \
    -out:z:\\"${OUTPUT_DIRECTORY}/foreign-executable-sections.txt"
  timeout 120 ${MSVC_TRIPLE}dumpbin \
    -nologo -symbols -disasm:nobytes \
    z:\\"${OUTPUT_DIRECTORY}/foreign-executable.exe" \
    -out:z:\\"${OUTPUT_DIRECTORY}/foreign-executable-disassembly.txt"

  # For consistency with the other toolchains, rename the binary
  mv "${OUTPUT_DIRECTORY}/foreign-executable.exe" "${OUTPUT_DIRECTORY}/foreign-executable"
}
