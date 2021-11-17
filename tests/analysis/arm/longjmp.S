@
@ This file is distributed under the MIT License. See LICENSE.md for details.
@

    .syntax unified
    .type _start, "function"
    .global _start
_start:
    push {lr}
    bl longjmp
    pop {lr}
    bx lr

    .type longjmp, "function"
longjmp:
    ble ret
    push {r0}
    bx r0
ret:
    bx lr
