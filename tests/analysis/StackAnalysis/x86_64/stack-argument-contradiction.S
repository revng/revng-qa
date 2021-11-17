#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

.intel_syntax noprefix
.type _start, @function
.global _start
_start:
    call func
    ret

# Here we have func, the caller, which has a stack slot has a No (see saofc.S),
# but the callee which is a Yes, therefore this should lead to contradiction

.type func, @function
func:
    lea rsp,[rsp-8]
    call callee
    mov rax,[rsp]
    lea rsp,[rsp+8]
    ret

.type callee, @function
callee:
    mov rax,[rsp+8]
    ret
