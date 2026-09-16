#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

.intel_syntax noprefix
.type _start, @function
.global _start
_start:
    call caller
    # Read the result: `rax` being live after this call site is the only
    # evidence that `caller` returns it, rather than leaving garbage behind.
    test rax, rax
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
