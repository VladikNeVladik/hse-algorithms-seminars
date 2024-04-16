bits 32

extern io_print_udec, io_print_hex, io_get_udec, io_print_string, io_newline

; Constants:
MAX_ARR_SIZE  equ     10
EXIT_FAILURE  equ     1

section .text
global main
main:
    ; Input array size:
    mov     eax, str_enter_size
    call    io_print_string

    call    io_get_udec
    mov     dword [size], eax

    ; Verify array size:
    cmp     eax, MAX_ARR_SIZE
    jbe     verify_array_size_ok

    ; Print error message and exit:
    mov     eax, str_array_size_too_big
    call    io_print_string

    mov     eax, EXIT_FAILURE
    ret

verify_array_size_ok:

    ;=================;
    ; Data input loop ;
    ;=================;

    ; Initialize index:
    mov     ecx, 0

data_input_loop:
    cmp     ecx, dword [size]
    jae     data_input_loop_exit

    ; Save caller-preserved register before function call:
    mov     dword [index], ecx

    call    io_get_udec

    ; Save entered data to memory:
    mov     ecx, dword [index]
    mov     dword [arr + 4*ecx], eax

    ; Update array index:
    add     ecx, 1

    jmp     data_input_loop
data_input_loop_exit:

    ;==========================;
    ; Search for maximum value ;
    ;==========================;

    ; Initialize loop:
    mov     ebx, 0
    mov     ecx, dword [size]

max_search_loop:
    mov     eax, dword [arr + 4*ecx - 4]

    cmp     ebx, eax
    cmovb   ebx, eax

    loop max_search_loop

    ; Print max value:
    mov eax, str_max_value_is
    call io_print_string

    mov eax, ebx

    ;===============================;
    ; Convert result to hexadecimal ;
    ;===============================;

    mov eax, ebx ; eax = to print
    mov ecx, 8

loop_conevert_hex2char:
    xor edx, edx
    mov ebx, 16
    idiv ebx
    ; edx := edx:eax % ebx
    ; eax := edx:eax / ebx

    mov dl, byte [hex2char + edx]
    mov byte [resstr + ecx - 1], dl

    loop loop_conevert_hex2char

    ; Print result:
    mov eax, resstr
    call io_print_string
    call io_newline

    xor eax, eax
    ret

section .rodata
str_enter_size:         db `Enter array size:\n`,0
str_array_size_too_big: db `Array size is too big\n`, 0
str_max_value_is:       db `Max value is:\n`, 0

section .bss
size:   resd    1
index:  resd    1
arr:    resd    10
resstr: resb    9

section .data
hex2char:    db `0123456789ABCDEF`
