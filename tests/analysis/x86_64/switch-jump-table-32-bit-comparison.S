#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

	.text
	.intel_syntax noprefix
	.globl	_start
	.p2align	4, 0x90
	.type	_start,@function
_start:
	add	edi, -65
	cmp	edi, 35
	ja	.LBB0_5
	mov	eax, 1
	jmp	qword ptr [8*rdi + .LJTI0_0]
.LBB0_2:
	mov	eax, 2
	ret
.LBB0_3:
	mov	eax, 4
	ret
.LBB0_4:
	mov	eax, 8
	ret
.LBB0_5:
	xor	eax, eax
.LBB0_6:
	ret
	.section	.rodata,"a",@progbits
	.p2align	3
.LJTI0_0:
	.quad	.LBB0_6
	.quad	.LBB0_2
	.quad	.LBB0_3
	.quad	.LBB0_4
	.quad	.LBB0_5
	.quad	.LBB0_5
	.quad	.LBB0_5
	.quad	.LBB0_5
	.quad	.LBB0_5
	.quad	.LBB0_5
	.quad	.LBB0_5
	.quad	.LBB0_5
	.quad	.LBB0_5
	.quad	.LBB0_5
	.quad	.LBB0_5
	.quad	.LBB0_5
	.quad	.LBB0_5
	.quad	.LBB0_5
	.quad	.LBB0_5
	.quad	.LBB0_5
	.quad	.LBB0_5
	.quad	.LBB0_5
	.quad	.LBB0_5
	.quad	.LBB0_5
	.quad	.LBB0_5
	.quad	.LBB0_5
	.quad	.LBB0_5
	.quad	.LBB0_5
	.quad	.LBB0_5
	.quad	.LBB0_5
	.quad	.LBB0_5
	.quad	.LBB0_5
	.quad	.LBB0_6
	.quad	.LBB0_2
	.quad	.LBB0_3
	.quad	.LBB0_4
