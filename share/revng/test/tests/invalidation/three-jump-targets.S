#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

.intel_syntax noprefix

.type _start, @function
.global _start
_start:
    call function
    ret

.type function, @function
function:
    ret

.type other_function, @function
other_function:
    ret
