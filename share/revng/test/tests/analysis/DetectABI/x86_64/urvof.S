#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

.intel_syntax noprefix
.type _start, @function
.global _start
_start:
    call urvof
    ret

.type urvof, @function
urvof:
    # This is a dead assignment to a register, therefore it has to be a return
    # value
    mov rax,42
    ret
