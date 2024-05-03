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
    mov rbp, QWORD PTR [0x1000]
    cmp rbp, 3
    jg skip

    mov QWORD PTR [0x2000], rdi

skip:
    ret
