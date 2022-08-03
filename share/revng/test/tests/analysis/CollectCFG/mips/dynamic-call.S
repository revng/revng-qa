#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

    .globl __start
    .type __start, "function"
    .set noreorder
    .set nomacro
__start:
    lui $gp,%hi(__gnu_local_gp)
    addiu $gp,$gp,%lo(__gnu_local_gp)
    addiu $sp,$sp,-32
    lw $t9,%call16(strlen)($gp)
    sw $ra,28($sp)
    jalr $t9
    nop

    lw      $ra,28($sp)
    addiu   $2,$2,2
    addiu   $sp,$sp,32
    jr      $ra
    nop
