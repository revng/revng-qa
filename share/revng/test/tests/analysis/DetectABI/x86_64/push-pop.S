#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

.intel_syntax noprefix
.type _start, @function
.global _start
_start:
    call push_pop
    ret

.type push_pop, @function
push_pop:
    push rax
    pop rdx
    ret
