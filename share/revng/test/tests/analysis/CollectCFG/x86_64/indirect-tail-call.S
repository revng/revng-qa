#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

.intel_syntax noprefix
.type _start, @function
.global _start
_start:
    call tailcall
    ret

.type tailcall, @function
tailcall:
    push rax
    pop rax
    jmp rax
