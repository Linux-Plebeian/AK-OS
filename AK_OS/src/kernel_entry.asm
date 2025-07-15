[BITS 16]

extern main

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00   ; Set up a basic stack
    sti

    call main

.halt:
    cli
    hlt
    jmp .halt
