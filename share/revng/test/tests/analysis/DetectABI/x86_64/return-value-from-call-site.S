#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

.intel_syntax noprefix
.type _start, @function
.global _start
_start:
    call caller
    ret

.type callee, @function
callee:
    mov rax, 42
    ret

.type caller, @function
caller:
    # callee has one return value, we should inherit it
    call callee
    ret
