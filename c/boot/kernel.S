.global _start

.section .rodata
message:
	.asciz "Hello AArch64 Baremetal!\n"


.section .text.boot
_start:
	ldr x0, =message
	bl print_uart0
1:  b 1b

print_uart0:
	mov x1, #0x09000000

.loop:
	ldrb w2, [x0], #1
	cbz  w2, .done

.wait:
	ldr w3, [x1, #0x18]     // UARTFR
	tbnz w3, #5, .wait      // 等待 TXFF=0
	strb w2, [x1]           // 写入 UARTDR
	b .loop

.done:
	ret
