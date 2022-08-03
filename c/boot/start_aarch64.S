# NASM

.global _start

.section .rodata
message:
	.asciz "Hello, AArch64 for QEMU!\n"

.section .text
_start:
	mov x0, #1
	ldr x1, =message
	mov x2, #25
	mov x8, #64
	svc #0

	mov x0, #0
	mov x8, #93
	svc #0
