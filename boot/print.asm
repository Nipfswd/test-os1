print:
    pusha

print_loop:
    ; while (string[i] != 0) { print string[i]; i++ }
    mov al, [bx]        ; 'bx' is the base address for the string
    cmp al, 0
    je print_done

    mov ah, 0x0e
    int 0x10            ; BIOS teletype output

    add bx, 1
    jmp print_loop

print_done:
    popa
    ret



print_nl:
    pusha
    
    mov ah, 0x0e
    mov al, 0x0a        ; newline
    int 0x10
    mov al, 0x0d        ; carriage return
    int 0x10
    
    popa
    ret
