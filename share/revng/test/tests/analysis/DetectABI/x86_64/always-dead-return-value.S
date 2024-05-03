#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

# WIP: drop

.intel_syntax noprefix
.type _start, @function
.global _start
_start:
    call func
    ret

.type func, @function
func:
    call callee
    mov rax,42
    ret

# In this function rax is a return value, but on all the call sites the return
# value is ignored.
.type callee, @function
callee:
    mov rax,41
    ret
