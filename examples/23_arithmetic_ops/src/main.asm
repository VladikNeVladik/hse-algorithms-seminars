bits 32

extern io_print_dec, io_print_hex, io_get_dec, io_print_string, io_newline

; All sorts of constants and variables:
section .bss
var_08bit:  resb 1 ; Reserve 1 byte
var_16bit:  resw 1 ; Reserve 1 word
var_32bit:  resd 1 ; Reserve 1 double word

res0:       resd 1
res1:       resd 1
res2:       resd 1
res3:       resd 1

section .data
buffer:     db 0, 1, 2, 3, 4, 5, 6, 7, 8, 9

section .rodata
ushifted:   dd 10d
sshifted:   dd 10d

section .text
global main
main:
    ; Representation of different-sized variables:
    mov     al, 01h
    mov     byte [var_08bit], al

    mov     bx, 0001h
    mov     word [var_16bit], bx

    mov     ecx, 00000001h
    mov     dword [var_32bit], ecx

    ; Integer arithmetics:
    mov     al, byte [var_08bit]
    add     al, 1h
    mov     byte [var_08bit], al

    mov     ax, word [var_16bit]
    sub     ax, 1h
    mov     word [var_16bit], ax

    mov     eax, dword [var_32bit]
    mov     ebx, 10d
    mul     ebx ; edx:eax = eax * ebx
    mov     dword [var_32bit], eax

    mov     eax, dword [var_32bit]
    mov     ebx, 5h
    div     ebx ; edx = eax % ebx, eax = eax / ebx
    mov     dword [var_32bit], eax

    ; Print arithmetic results:
    movzx   eax, byte [var_08bit]
    call    io_print_hex
    call    io_newline

    movzx   eax, word [var_16bit]
    call    io_print_hex
    call    io_newline

    mov     eax, dword [var_32bit]
    call    io_print_hex
    call    io_newline

    ; Array accesses:
    mov     byte [buffer], Ah

    mov     al, byte [buffer + 1]
    mov     byte [var_08bit], al

    ; Bitshift operations:
    mov     eax, dword [ushifted]
    shl     eax, 10d
    mov     dword [res0], eax

    mov     eax, dword [ushifted]
    shr     eax, 10d
    mov     dword [res0], eax

    mov     eax, dword [sshifted]
    sal     eax, 10d
    mov     dword [res0], eax

    mov     eax, dword [sshifted]
    sar     eax, 10d
    mov     dword [res0], eax

    ; Return 0
    xor     eax, eax ; eax = eax ^ eax
    ret

; Self-check tasks
section .data
a:  db 0x40
    dw 0x81
    db 0xFE
b:  dd 0x0

section .text
global tasks
tasks:
    ; Task #1
    mov     eax, dword [a]
    mov     dword [b], eax
    ; b = ?

    ; Task #2
    movsx   eax, byte [a + 3]
    mov     dword [b], eax
    ; b = ?

    ; Task #3
    movsx   eax, word [a + 1]
    mov     dword [b], eax
    ; b = ?

    ; Task #4
    movsx   eax, byte [a + 1]
    mov     dword [b], eax
    ; b = ?

    ; Task #5
    movzx   eax, byte [a + 1]
    mov     dword [b], eax
    ; b = ?

    ; Task #6
    xor     eax, eax
    movsx   ax, byte [a + 1]
    mov     dword [b], eax
    ; b = ?

    ; Return 0
    xor     eax, eax ; eax = eax ^ eax
    ret
