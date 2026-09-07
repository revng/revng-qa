#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

.intel_syntax noprefix
.type _start, @function
.global _start
_start:
    call function
    ret

# `and rdx, 1` is here for the flags the branch tests, and the value it leaves
# in rdx is a byproduct nobody reads. It must not join rax as a return value of
# `function`, nor rdi as an argument of the call it reaches.
.type function, @function
function:
    mov rdx, QWORD PTR [0x1008]
    and rdx, 1
    jz skip
    mov rdi, QWORD PTR [0x1000]
    call callee
skip:
    mov rax, 1
    ret

.type callee, @function
callee:
    mov QWORD PTR [0x2000], rdi
    ret
