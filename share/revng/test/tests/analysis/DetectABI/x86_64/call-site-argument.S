#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

.intel_syntax noprefix
.type _start, @function
.global _start
_start:
    call function
    ret

.type function, @function
function:
    # rdi is an argument of the call, since it would otherwise be an unused assignment
    mov rdi, 42

    mov rbp, QWORD PTR [0x1000]
    call rbp

    ret
