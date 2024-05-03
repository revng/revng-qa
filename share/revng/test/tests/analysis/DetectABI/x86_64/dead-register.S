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

# In this function call rax is an argument, but in the callee is NoOrDead,
# therefore, rax is Dead in callee.
.type func, @function
func:
    mov rax,42
    call callee
    ret

.type callee, @function
callee:
    mov rax,43
    ret
