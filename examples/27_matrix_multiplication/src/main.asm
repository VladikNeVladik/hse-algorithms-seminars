bits 32

extern io_print_hex, io_print_char, io_print_string, io_newline

; Constants:
MATRIX_SIZE     equ    32 ;   4 KB
; MATRIX_SIZE     equ    64 ;  16 KB
; MATRIX_SIZE     equ   128 ;  64 KB
; MATRIX_SIZE     equ   256 ; 256 KB  0,03 s
; MATRIX_SIZE     equ   512 ;   1 MB  0,22 s
; MATRIX_SIZE     equ  1024 ;   4 MB  2,44 s
; MATRIX_SIZE     equ  2048 ;  16 MB 64,07 s
; MATRIX_SIZE     equ  4096 ;  64 MB ????? s

; Matrix data:
section .bss
; Position in matrix: matrix[y][x] = matrix + 4 * (MATRIX_SIZE * y + x)
mat_A:          resd MATRIX_SIZE * MATRIX_SIZE
mat_B:          resd MATRIX_SIZE * MATRIX_SIZE
mat_C:          resd MATRIX_SIZE * MATRIX_SIZE
eax_tmp:        resd 1
ebx_tmp:        resd 1

section .text
global main
main:
    ;=====================;
    ; Initialize matrix A ;
    ;=====================;

    ; Index variables:
    ; eax = y
    ; ebx = x
    ; ecx = matrix[y][x]

    ; Outer loop init:
    mov     eax, 0
    mov     ecx, mat_A
mat_A_init_out_loop:
    cmp     eax, MATRIX_SIZE
    jae     mat_A_init_out_loop_exit

    ; Inner loop init:
    mov     ebx, 0
    mat_A_init_in_loop:
        cmp     ebx, MATRIX_SIZE
        jae     mat_A_init_in_loop_exit

        ; matrix[y][x] = (x <= y)? 1 : 0;
        mov     esi, 0x1
        mov     edi, 0x0

        cmp     eax, ebx
        cmovbe  edx, esi
        cmova   edx, edi

        mov     dword [ecx], edx

        ; Update indecies:
        add     ebx, 1
        add     ecx, 4
        jmp     mat_A_init_in_loop
    mat_A_init_in_loop_exit:

    ; Update indecies:
    add     eax, 1
    jmp     mat_A_init_out_loop
mat_A_init_out_loop_exit:

    ;=====================;
    ; Initialize matrix B ;
    ;=====================;

    ; Index variables:
    ; eax = x
    ; ebx = y
    ; ecx = matrix[y][x]

    ; Outer loop init:
    mov     eax, 0
    mov     ecx, mat_B
mat_B_init_out_loop:
    cmp     eax, MATRIX_SIZE
    jae     mat_B_init_out_loop_exit

    ; Inner loop init:
    mov     ebx, 0
    mat_B_init_in_loop:
        cmp     ebx, MATRIX_SIZE
        jae     mat_B_init_in_loop_exit

        ; matrix[y][x] = (x <= y)? 1 : 0;
        mov     esi, 0x1
        mov     edi, 0x0

        cmp     eax, ebx
        cmovbe  edx, esi
        cmova   edx, edi

        mov     dword [ecx], edx

        ; Update indecies:
        add     ebx, 1
        add     ecx, 4
        jmp     mat_B_init_in_loop
    mat_B_init_in_loop_exit:

    ; Update indecies:
    add     eax, 1
    jmp     mat_B_init_out_loop
mat_B_init_out_loop_exit:

    ;=====================================;
    ; Trivial multiplication of matricies ;
    ;=====================================;

    ; Index variables:
    ; eax = c_y = a_y
    ; ebx = c_x = b_x = mul accum
    ; ecx = &mat_C[c_y][c_x]
    ; edx = a_x = b_y
    ; esi = &mat_A[a_y][a_x]
    ; edi = &mat_B[b_y][b_x]

    ; Outer loop init:
    mov     eax, 0
    mov     ecx, mat_C
mat_C_calc_out_loop:
    cmp     eax, MATRIX_SIZE
    jae     mat_C_calc_out_loop_exit

    ; Inner loop init:
    mov     ebx, 0
    mat_C_calc_in_loop:
        cmp     ebx, MATRIX_SIZE
        jae     mat_C_calc_in_loop_exit

        ; Save eax, ebx to temporary memory:
        mov     dword [eax_tmp], eax
        mov     dword [ebx_tmp], ebx

        ; Initialize loop indexes:
        lea     edi, dword [mat_B + 4 * ebx]

        ; Initialize esi as lea command is picky:
        ; lea   esi, dword [mat_A + 4 * MATRIX_SIZE * ebx]
        imul    esi, eax, 4 * MATRIX_SIZE
        add     esi, mat_A

        ; Initialize loop accumulator:
        ; eax = result of multiplication
        ; ebx = multilplication accumulator
        ; edx = index
        mov     ebx, 0
        mov     edx, 0

        mat_C_mult_loop:
            cmp     edx, MATRIX_SIZE
            jae     mat_C_mult_loop_exit

            ; Update partial sum:
            mov     eax, dword [esi]
            imul    eax, dword [edi]
            add     ebx, eax

            ; Update indecies:
            add     edx, 1
            add     esi, 4
            add     edi, 4 * MATRIX_SIZE ; Jump out of cache line
            jmp     mat_C_mult_loop
        mat_C_mult_loop_exit:

        ; Save mat_C[c_y][c_x] to C memory:
        mov     dword [ecx], ebx

        ; Restore abx, ebx from memory:
        mov     eax, dword [eax_tmp]
        mov     ebx, dword [ebx_tmp]

        ; Update indecies:
        add     ebx, 1
        add     ecx, 4
        jmp     mat_C_calc_in_loop
    mat_C_calc_in_loop_exit:

    ; Update indecies:
    add     eax, 1
    jmp     mat_C_calc_out_loop
mat_C_calc_out_loop_exit:

    ;==============;
    ; Early return ;
    ;==============;

    ; Use it to measure time:
    ; xor     eax, eax
    ; ret

    ;==============;
    ; Print result ;
    ;==============;

    ; Index variables:
    ; esi = x
    ; edi = y
    ; ebx = matrix[y][x]

    ; Outer loop init:
    mov     esi, 0
    mov     ebx, mat_C
mat_C_print_out_loop:
    cmp     esi, MATRIX_SIZE
    jae     mat_C_print_out_loop_exit

    ; Inner loop init:
    mov     edi, 0
    mat_C_print_in_loop:
        cmp     edi, MATRIX_SIZE
        jae     mat_C_print_in_loop_exit

        ; matrix[y][x] = (x <= y)? 1 : 0;
        mov     eax, dword [ebx]
        call    io_print_hex

        ; Print whitespace:
        mov     eax, ' '
        call    io_print_char

        ; Update indecies:
        add     edi, 1
        add     ebx, 4
        jmp     mat_C_print_in_loop
    mat_C_print_in_loop_exit:

    ; Print newline:
    call    io_newline

    ; Update indecies:
    add     esi, 1
    jmp     mat_C_print_out_loop
mat_C_print_out_loop_exit:

    xor     eax, eax
    ret

section .bss
