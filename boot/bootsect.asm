[org 0x7C00]

KERNEL_OFFSET   equ 0x1000        ; Where we load the kernel
MODE_INFO_ADDR  equ 0x8000        ; Where we store VBE mode info (phys 0x8000)

[bits 16]

start:
    ; Save BIOS boot drive
    mov byte [BOOT_DRIVE], dl

    ; Set up stack
    mov bp, 0x9000
    mov sp, bp

    ; -----------------------------
    ; Set VBE mode 800x600x32bpp
    ; -----------------------------
    mov ax, 0x4F02                ; VBE set mode
    mov bx, 0x4118                ; 0x4000 = LFB, 0x118 = 800x600x32
    int 0x10

    ; -----------------------------
    ; Get VBE mode info for 0x118
    ; Store at 0000:8000 (phys 0x8000)
    ; -----------------------------
    mov ax, 0x4F01                ; VBE get mode info
    mov cx, 0x118                 ; mode 800x600x32
    mov di, MODE_INFO_ADDR        ; offset = 0x8000
    mov ax, 0x0000
    mov es, ax                    ; ES = 0x0000
    int 0x10

    mov bx, MSG_REAL_MODE 
    call print
    call print_nl

    call load_kernel              ; read the kernel from disk
    call switch_to_pm             ; disable interrupts, load GDT, etc. Jumps to BEGIN_PM
    jmp $                         ; Never executed

%include "boot/print.asm"
%include "boot/print_hex.asm"
%include "boot/disk.asm"
%include "boot/gdt.asm"
%include "boot/32bit_print.asm"
%include "boot/switch_pm.asm"

[bits 16]
load_kernel:
    mov bx, MSG_LOAD_KERNEL
    call print
    call print_nl

    mov bx, KERNEL_OFFSET         ; Read from disk and store in 0x1000
    mov dh, 31                    ; Number of sectors to load
    mov dl, [BOOT_DRIVE]
    call disk_load
    ret

[bits 32]
BEGIN_PM:
    mov ebx, MSG_PROT_MODE
    call print_string_pm
    call KERNEL_OFFSET            ; Give control to the kernel
    jmp $                         ; Stay here when the kernel returns (if ever)

BOOT_DRIVE          db 0
MSG_REAL_MODE       db "Started in 16-bit Real Mode", 0
MSG_PROT_MODE       db "Landed in 32-bit Protected Mode", 0
MSG_LOAD_KERNEL     db "Loading kernel into memory", 0
MSG_RETURNED_KERNEL db "Returned from kernel. Error?", 0

times 510 - ($-$$) db 0
dw 0xAA55
