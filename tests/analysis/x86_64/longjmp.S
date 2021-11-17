#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

.intel_syntax noprefix
.type _start, @function
.global _start
_start:
    call longjmp
    ret

.type longjmp, @function
longjmp:
    jle ret
    push rax
    jmp rax
ret:
    ret
