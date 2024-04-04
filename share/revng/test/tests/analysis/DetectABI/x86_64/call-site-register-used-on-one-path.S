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
    # The write
    mov rdi, 42

    mov rbp, QWORD PTR [0x1000]
    cmp rbp, 3
    jg skip

    # Use the result of the write on one path out of the two reaching the call
    mov QWORD PTR [0x2000], rdi

skip:
    # The call
    call rbp
    ret
