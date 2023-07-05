#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

.intel_syntax noprefix

.section .text

# Referenced by aligned_reference_to_function_0
# 0xAB00000000 (must match)
0:
    ret

# Referenced by mialigned_reference_to_function_1
# 0xAB00000001 (must not match)
1:
    ret

# Called by _start
# 0xAB00000002 (must match)
2:
    ret

# Address written in memory (see _start)
# 0xAB00000003 (must match)
3:
    ret

# Regular function with a symbol
# 0xAB00000004 (must match)
.type regular_function_1, @function
regular_function_1:
    nop
    # The following instruction is referenced by global data, but the
    # instruction is already part of a more robust function
    # 0xAB00000005 (must not match)
    ret

# Regular function with a symbol

# 0xAB00000006 (must match)
.type regular_function_2, @function
regular_function_2:
    # 10 bytes
    movabs rax, OFFSET FLAT:jump_table
    # 0xAB00000010 (must not match)
    # 2 bytes
    jmp QWORD PTR [rax]
    # Following instruction is the destination of a jump table
    # 0xAB00000012 (must not match)
4:
    ret

# Function read whose body is read in _start
# 0xAB00000013 (must not match)
5:
    ret

# Function read whose body is written in _start
# 0xAB00000014 (must not match)
6:
    ret

.type _start, @function
.global _start
_start:
    # Store function #3 address in memory
    movabs rax, OFFSET FLAT:3b
    mov QWORD PTR [rbx], rax

    # Call to function #2
    call 2b

    # Read function #5
    movabs rax, OFFSET FLAT:5b
    mov rax, QWORD PTR [rax]

    # Write function #6
    movabs rax, 5b
    mov QWORD PTR [rax], rax

    ret

.section .rodata

jump_table:
    .quad 4b

# Reference to part of a function
reference_to_part_of_regular_function_1:
    .quad regular_function_1 + 1

# Aligned reference to function 0
aligned_reference_to_function_0:
    .quad 0b

# Misaligned reference to function 1
mialigned_reference_to_function_1:
    .byte 0x42
    .quad 1b
