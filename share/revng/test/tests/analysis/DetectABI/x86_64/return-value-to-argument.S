#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

# TODO: this test has no ground truth

.intel_syntax noprefix
.type _start, @function
.global _start
_start:
    call return_value_to_argument
    ret

.type return_value_to_argument, @function
return_value_to_argument:
    call return_rax
    call rax_not_my_argument
    ret

.type return_rax, @function
return_rax:
    mov rax, 42
    ret

.type rax_not_my_argument, @function
rax_not_my_argument:
    mov rax, 43
    ret
