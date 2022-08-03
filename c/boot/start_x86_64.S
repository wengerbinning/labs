# AT&T

.global _start

.section .rodata
message:
	.asciz "Hello, x86_64 from QEMU!\n"

.section .text
_start:
	mov $1, %rdi
	lea message(%rip), %rsi
	mov $25, %rdx
	mov $1, %rax
	syscall

	xor %rdi, %rdi
	mov $60, %rax
	syscall
