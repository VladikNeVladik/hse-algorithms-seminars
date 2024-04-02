bits 32

extern io_print_dec, io_print_hex, io_get_dec, io_print_string, io_newline

section .text
global main
main:
    ; Print "Hello, world!" string
    mov eax, hello ; eax = hello
    call io_print_string

    ; Print 32-bit hex value:
    ; io_print_hex accepts argument as EAX register
    mov eax, [dword var] ; eax = *var
    call io_print_hex
    call io_newline

    ; Print "Enter two numbers to be added:" string
    mov eax, addstr ; eax = addstr
    call io_print_string

    ; Read 32-bit decimal value from stdin
    ; Returns value in EAX register
    call io_get_dec
    ; Store result in EBX
    mov ebx, eax ; ebx = eax

    ; Get another decimal value into EAX
    call io_get_dec
    add ebx, eax ; ebx += eax

    ; Print "The result is:" string
    mov eax, rsltstr ; eax = rsltstr
    call io_print_string

    ; Print addition result
    mov eax, ebx ; eax = ebx
    call io_print_dec
    call io_newline

    ; Return 0
    xor eax, eax ; eax = eax ^ eax
    ret

section .rodata
hello:   db `Hello, world!\n`, 0
addstr:  db `Enter two numbers to be added:\n`, 0
rsltstr: db `The result is:\n`, 0

section .data
var: dd 0xDEADBEEF
