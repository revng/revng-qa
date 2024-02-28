#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

    .intel_syntax noprefix
    .type _start, @function
    .globl _start
_start:
    cmp    rax,0x3
    ja     end
    jmp    QWORD PTR [rax*8+jumptable]
one:
    mov rax, 0x3
    ret
two:
    mov rax, 0x5
    ret
three:
    mov rax, 0x6
    ret
end:
    mov rax, 0x1
    ret

    .section        .rodata
jumptable:
    .quad one
    .quad two
    .quad three
    .quad end
