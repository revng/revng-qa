#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

    .intel_syntax noprefix
    .type myfunc, @function
    .globl myfunc
myfunc:
    mov    rax, QWORD PTR [rax]
    cmp    rax, 0x5
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
f:
    mov rax, 0x7
    ret
q:
    mov rax, 0x8
    ret
h:
    mov rax, 0x9
    ret
z:
    mov rax, 0x10
    ret
e:
    mov rax, 0xFF
    ret
f1:
    mov rax, 0x71
    ret
q1:
    mov rax, 0x83
    ret
h1:
    mov rax, 0x93
    ret
z1:
    mov rax, 0x13
    ret
e1:
    mov rax, 0xFA
    ret
f2:
    mov rax, 0xaa
    ret
q2:
    mov rax, 0x78
    ret
h2:
    mov rax, 0x66
    ret
z2:
    mov rax, 0x24
    ret
e2:
    mov rax, 0xF1
    ret
end:
    mov rax, 0x44
    ret

    .intel_syntax noprefix
    .type _start, @function
    .globl _start
_start:
    mov rax, rdi
    call myfunc
    mov rdi, rax
    ret

    .section        .rodata
jumptable:
    .quad one
    .quad two
    .quad three
    .quad f
    .quad q
    .quad h
    .quad z
    .quad e
    .quad f1
    .quad q1
    .quad h1
    .quad z1
    .quad e1
    .quad f2
    .quad q2
    .quad h2
    .quad z2
    .quad e2
    .quad end
