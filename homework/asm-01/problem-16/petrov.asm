bits 32

extern io_print_udec, io_get_udec, io_print_char, io_newline, io_print_string

section .bss

var_coeffs: resd 6
inter_res:  resd 1

section .rodata

space: db ' '


section .text
global main
main:
    ;
    call io_get_udec
    mov dword [var_coeffs], eax ; A11
    call io_get_udec
    mov dword [var_coeffs + 4], eax ; A12
    call io_get_udec
    mov dword [var_coeffs + 8], eax ; A21
    call io_get_udec
    mov dword [var_coeffs + 12], eax ; A22
    call io_get_udec
    mov dword [var_coeffs + 16], eax ; B1
    call io_get_udec
    mov dword [var_coeffs + 20], eax ; B2

    ; 1) Making table of solutions
    ; 2) Restore functions from tables
    ; 3) Simplify functions

    ; Calculating X

    mov eax, dword [var_coeffs]
    mov ebx, dword [var_coeffs + 8]
    or eax, ebx
    mov dword [inter_res], eax

    mov eax, dword [var_coeffs + 16]
    mov ebx, dword [var_coeffs + 20]
    or eax, ebx
    mov ebx, dword [inter_res]
    and ebx, eax
    mov dword [inter_res], ebx

    mov eax, dword [var_coeffs]
    not eax
    mov ebx, dword [var_coeffs + 4]
    mov ecx, dword [var_coeffs + 16]
    or eax, ebx
    or eax, ecx
    mov ebx, dword [inter_res]
    and ebx, eax
    mov dword [inter_res], ebx

    mov eax, dword [var_coeffs + 8]
    not eax
    mov ebx, dword [var_coeffs + 12]
    mov ecx, dword [var_coeffs + 20]
    or eax, ebx
    or eax, ecx
    mov ebx, dword [inter_res]
    and ebx, eax
    mov dword [inter_res], ebx

    mov eax, dword [var_coeffs + 16]
    not eax
    mov ebx, dword [var_coeffs]
    mov ecx, dword [var_coeffs + 4]
    or eax, ebx
    or eax, ecx
    mov ebx, dword [inter_res]
    and ebx, eax
    mov dword [inter_res], ebx

    mov eax, dword [var_coeffs + 20]
    not eax
    mov ebx, dword [var_coeffs + 8]
    mov ecx, dword [var_coeffs + 12]
    or eax, ebx
    or eax, ecx
    mov ebx, dword [inter_res]
    and ebx, eax
    mov dword [inter_res], ebx

    mov eax, dword [var_coeffs + 4]
    not eax
    mov ebx, dword [var_coeffs + 8]
    mov ecx, dword [var_coeffs + 12]
    or eax, ebx
    or eax, ecx
    mov ebx, dword [inter_res]
    and ebx, eax
    mov dword [inter_res], ebx

    mov eax, dword [var_coeffs + 12]
    not eax
    mov ebx, dword [var_coeffs]
    mov ecx, dword [var_coeffs + 4]
    or eax, ebx
    or eax, ecx
    mov ebx, dword [inter_res]
    and ebx, eax
    mov dword [inter_res], ebx

    mov eax, dword [var_coeffs]
    not eax
    mov ebx, dword [var_coeffs + 4]
    not ebx
    mov ecx, dword [var_coeffs + 8]
    not ecx
    mov edx, dword [var_coeffs + 12]
    not edx
    or eax, ebx
    or eax, ecx
    or eax, edx
    mov ebx, dword [inter_res]
    and ebx, eax
    mov dword [inter_res], ebx

    mov eax, dword [var_coeffs + 4]
    not eax
    mov ebx, dword [var_coeffs + 12]
    not ebx
    mov ecx, dword [var_coeffs + 16]
    not ecx
    mov edx, dword [var_coeffs + 20]
    not edx
    or eax, ebx
    or eax, ecx
    or eax, edx
    mov ebx, dword [inter_res]
    and ebx, eax
    mov dword [inter_res], ebx


    mov eax, dword [inter_res]
    call io_print_udec



    ; Calculating Y

    mov dword [inter_res], 0


    mov eax, dword [var_coeffs + 4]
    mov ebx, dword [var_coeffs + 12]
    or eax, ebx
    mov dword [inter_res], eax

    mov eax, dword [var_coeffs + 16]
    mov ebx, dword [var_coeffs + 20]
    or eax, ebx
    mov ebx, dword [inter_res]
    and ebx, eax
    mov dword [inter_res], ebx

    mov eax, dword [var_coeffs + 4]
    not eax
    mov ebx, dword [var_coeffs]
    mov ecx, dword [var_coeffs + 16]
    or eax, ebx
    or eax, ecx
    mov ebx, dword [inter_res]
    and ebx, eax
    mov dword [inter_res], ebx

    mov eax, dword [var_coeffs + 12]
    not eax
    mov ebx, dword [var_coeffs + 8]
    mov ecx, dword [var_coeffs + 20]
    or eax, ebx
    or eax, ecx
    mov ebx, dword [inter_res]
    and ebx, eax
    mov dword [inter_res], ebx

    mov eax, dword [var_coeffs + 16]
    not eax
    mov ebx, dword [var_coeffs]
    mov ecx, dword [var_coeffs + 4]
    or eax, ebx
    or eax, ecx
    mov ebx, dword [inter_res]
    and ebx, eax
    mov dword [inter_res], ebx

    mov eax, dword [var_coeffs + 20]
    not eax
    mov ebx, dword [var_coeffs + 8]
    mov ecx, dword [var_coeffs + 12]
    or eax, ebx
    or eax, ecx
    mov ebx, dword [inter_res]
    and ebx, eax
    mov dword [inter_res], ebx

    mov eax, dword [var_coeffs]
    not eax
    mov ebx, dword [var_coeffs + 4]
    not ebx
    mov ecx, dword [var_coeffs + 12]
    not ecx
    mov edx, dword [var_coeffs + 20]
    or eax, ebx
    or eax, ecx
    or eax, edx
    mov ebx, dword [inter_res]
    and ebx, eax
    mov dword [inter_res], ebx

    mov eax, dword [var_coeffs]
    not eax
    mov ebx, dword [var_coeffs + 12]
    mov ecx, dword [var_coeffs + 16]
    not ecx
    mov edx, dword [var_coeffs + 20]
    not edx
    or eax, ebx
    or eax, ecx
    or eax, edx
    mov ebx, dword [inter_res]
    and ebx, eax
    mov dword [inter_res], ebx

    mov eax, dword [var_coeffs + 4]
    not eax
    mov ebx, dword [var_coeffs + 8]
    not ebx
    mov ecx, dword [var_coeffs + 12]
    not ecx
    mov edx, dword [var_coeffs + 16]
    or eax, ebx
    or eax, ecx
    or eax, edx
    mov ebx, dword [inter_res]
    and ebx, eax
    mov dword [inter_res], ebx

    mov eax, dword [var_coeffs + 4]
    mov ebx, dword [var_coeffs + 8]
    not ebx
    mov ecx, dword [var_coeffs + 16]
    not ecx
    mov edx, dword [var_coeffs + 20]
    not edx
    or eax, ebx
    or eax, ecx
    or eax, edx
    mov ebx, dword [inter_res]
    and ebx, eax
    mov dword [inter_res], ebx

    call io_newline

    mov eax, dword [inter_res]
    call io_print_udec

    xor eax, eax ; Return 0
    ret
