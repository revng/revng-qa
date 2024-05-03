#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

.intel_syntax noprefix
.type _start, @function
.global _start
_start:
    call function
    ret

.intel_syntax noprefix
.type function, @function
function:
    # rsp would be an argument, but rsp is never an argument
    mov QWORD PTR [0x1000], rsp
    ret
