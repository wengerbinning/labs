[BITS 16]
;[ORG 0x7C00]

extern main

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    call main

hang:
    jmp hang

times 510-($-$$) db 0
dw 0xAA55
