#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

.intel_syntax noprefix
.type _start, @function
.global _start
_start:
    call func1
    call func2
    ret

# In this situation we have a return value that is used in one call site and is
# ignored in another one. Therefore it's not Dead, in general.

.type func1, @function
func1:
    call callee
    mov rax,42
    ret

.type func2, @function
func2:
    call callee
    mov rbx,rax
    ret

.type callee, @function
callee:
    mov rax,41
    ret
