org 0x7C00

start:
    mov ax, 0x0000
    mov es, ax
    mov bx, 0x1000

    mov ah, 0x02
    mov al, 63
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, 0x00
    int 0x13
    jc disk_error

    jmp 0x0000:0x1000
disk_error:
    mov si, disk_error_msg
    mov ah, 0x0E
.print:
    lodsb
    or al, al
    jz start
    int 0x10
    jmp .print

disk_error_msg:   db 'DISK READ ERROR :[',0

times 510 - ($ - $$) db 0
dw 0xAA55
