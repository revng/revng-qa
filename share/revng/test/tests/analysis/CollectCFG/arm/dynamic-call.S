@
@ This file is distributed under the MIT License. See LICENSE.md for details.
@

    .syntax unified
    .type _start, "function"
    .global _start
_start:
    push {lr}
    bl use_plt
    pop {lr}
    bx lr

    .type use_plt, "function"
use_plt:
    push {lr}
    bl strlen
    pop {lr}
    bx lr
