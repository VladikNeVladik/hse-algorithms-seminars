bits 32

extern io_print_hex, io_print_char, io_print_string, io_newline

; Constants:                     SIMPLE  TRANSPOSED
MATRIX_SIZE     equ    16
; MATRIX_SIZE     equ    1024 ;  3,04s   0,85s
; MATRIX_SIZE     equ    2048 ;  65,04s  5,59s

; Friendly reminder - stack frame structure (from System V ABI for i386):
; [ebp + 16]            ...
; [ebp + 12] memory argument fourbyte 1
; [ebp +  8] memory argument fourbyte 0
; [ebp +  4] return address
; [ebp +  0] previous ebp value
; [ebp -  4] varN
;     ...
; [esp +  4] var1
; [esp +  0] var0

;=======================;
; Matrix initialization ;
;=======================;

global matrix_init
%define size_y          dword [ebp + 16]
%define size_x          dword [ebp + 12]
%define matrix_base     dword [ebp +  8]
matrix_init:
    ; Create stack frame:
    push    ebp
    mov     ebp, esp
    ; Save used callee-preserved registers:
    push    ebx
    push    esi
    push    edi
    ; Do not allocate any automatic variables.
    ; Ignore stack alignment as no library function calls are made.

    ; Index variables:
    ; eax = y
    ; ebx = x
    ; ecx = matrix[y][x]

    ; Outer loop init:
    mov     eax, 0
    mov     ecx, matrix_base
.matrix_init_out_loop:
    cmp     eax, size_y
    jae     .matrix_init_out_loop_exit

    ; Inner loop init:
    mov     ebx, 0
    .matrix_init_in_loop:
        cmp     ebx, size_x
        jae     .matrix_init_in_loop_exit

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
        jmp     .matrix_init_in_loop
    .matrix_init_in_loop_exit:

    ; Update indecies:
    add     eax, 1
    jmp     .matrix_init_out_loop
.matrix_init_out_loop_exit:

    ; Restore used callee-preserved registers:
    pop     esi
    pop     edi
    pop     ebx

    ; Restore stack frame:
    mov     esp, ebp
    pop     ebp
    ; Return:
    ret

; Undef macros for safety reasons:
%undef size_y
%undef size_x
%undef matrix_base

;=================;
; Matrix printout ;
;=================;

global matrix_print
%define size_y          dword [ebp + 16]
%define size_x          dword [ebp + 12]
%define matrix_base     dword [ebp +  8]
matrix_print:
    ; Create stack frame:
    push    ebp
    mov     ebp, esp
    ; Save used callee-preserved registers:
    push    ebx
    push    esi
    push    edi
    ; Align stack by 16-byte boundary:
    sub     esp, 12
    ; Do not allocate any automatic variables.

    ; Index variables:
    ; esi = x
    ; edi = y
    ; ebx = matrix[y][x]

    ; Outer loop init:
    mov     esi, 0
    mov     ebx, matrix_base
.matrix_print_out_loop:
    cmp     esi, size_y
    jae     .matrix_print_out_loop_exit

    ; Inner loop init:
    mov     edi, 0
    .matrix_print_in_loop:
        cmp     edi, size_x
        jae     .matrix_print_in_loop_exit

        ; matrix[y][x] = (x <= y)? 1 : 0;
        mov     eax, dword [ebx]
        call    io_print_hex

        ; Print whitespace:
        mov     eax, ' '
        call    io_print_char

        ; Update indecies:
        add     edi, 1
        add     ebx, 4
        jmp     .matrix_print_in_loop
    .matrix_print_in_loop_exit:

    ; Print newline:
    call    io_newline

    ; Update indecies:
    add     esi, 1
    jmp     .matrix_print_out_loop
.matrix_print_out_loop_exit:

    ; Restore stack value after alignment:
    add     esp, 12
    ; Restore callee-preserved registers:
    pop     edi
    pop     esi
    pop     ebx
    ; Restore stack frame:
    mov     esp, ebp
    pop     ebp
    ; Return:
    xor     eax, eax
    ret

; Undef macros for safety reasons:
%undef size_y
%undef size_x
%undef matrix_base

;==============================;
; Simple matrix multiplication ;
;==============================;

; Actual list of arguments:
; base_C
; size_B_x = size_C_x
; base_B
; size_A_y = size_C_y
; size_A_x = size_B_y
; base_A

global matrix_multiply
; Function arguments:
%define size_C_y        dword [ebp + 16]
%define size_C_x        dword [ebp + 24]
%define base_C          dword [ebp + 28]
%define size_B_y        dword [ebp + 12]
%define size_B_x        dword [ebp + 24]
%define base_B          dword [ebp + 20]
%define size_A_y        dword [ebp + 16]
%define size_A_x        dword [ebp + 12]
%define base_A          dword [ebp +  8]
; Function automatic variables:
%define idx_C_y         dword [ebp -  4]
%define idx_C_x         dword [ebp -  8]
matrix_multiply:
    ; Create stack frame:
    push    ebp
    mov     ebp, esp
    ; Allocate space for automatic variables:
    ; Also align stack by 16-byte boundary.
    sub     esp, 12
    ; Save used callee-preserved registers:
    push    ebx
    push    esi
    push    edi

    ; Variable mapping:
    ; eax = c_y = a_y
    ; ebx = c_x = b_x = mul accum
    ; ecx = &mat_C[c_y][c_x]
    ; edx = a_x = b_y
    ; esi = &mat_A[a_y][a_x]
    ; edi = &mat_B[b_y][b_x]

    ; Outer loop init:
    mov     eax, 0
    mov     ecx, base_C
.matrix_multiply_out_loop:
    cmp     eax, size_C_y
    jae     .matrix_multiply_out_loop_exit

    ; Inner loop init:
    mov     ebx, 0
    .matrix_multiply_in_loop:
        cmp     ebx, size_A_x
        jae     .matrix_multiply_in_loop_exit

        ; Save eax, ebx to temporary memory:
        mov     idx_C_y, eax
        mov     idx_C_x, ebx

        ; Initialize loop indexes:
        ; edi = base_B + 4 * ebx
        imul    edi, ebx, 4
        add     edi, base_B

        ; esi = base_A + 4 * size_A_x * eax:
        imul    esi, eax, 4
        imul    esi, size_A_x
        add     esi, base_A

        ; Initialize loop accumulator:
        ; eax = result of multiplication
        ; ebx = multilplication accumulator
        ; edx = index
        mov     ebx, 0
        mov     edx, 0

        .matrix_multiply_mult_loop:
            cmp     edx, size_C_x
            jae     .matrix_multiply_mult_loop_exit

            ; Update partial sum:
            mov     eax, dword [esi]
            imul    eax, dword [edi]
            add     ebx, eax

            ; Update indecies:
            add     edx, 1
            add     esi, 4
            imul    eax, size_A_x, 4
            add     edi, eax
            jmp     .matrix_multiply_mult_loop
        .matrix_multiply_mult_loop_exit:

        ; Save mat_C[c_y][c_x] to C memory:
        mov     dword [ecx], ebx

        ; Restore abx, ebx from memory:
        mov     eax, idx_C_y
        mov     ebx, idx_C_x

        ; Update indecies:
        add     ebx, 1
        add     ecx, 4
        jmp     .matrix_multiply_in_loop
    .matrix_multiply_in_loop_exit:

    ; Update indecies:
    add     eax, 1
    jmp     .matrix_multiply_out_loop
.matrix_multiply_out_loop_exit:

    ; Restore used callee-preserved registers:
    pop     edi
    pop     esi
    pop     ebx
    ; Reclaim stack memory for automatic variables:
    add     esp, 12
    ; Restore stack frame:
    mov     esp, ebp
    pop     ebp
    ; Return:
    xor     eax, eax
    ret

; Undef macros for safety reasons:
%undef size_C_y
%undef size_C_x
%undef base_C
%undef size_B_y
%undef size_B_x
%undef base_B
%undef size_A_y
%undef size_A_x
%undef base_A
; Function automatic variables:
%undef idx_C_y
%undef idx_C_x

;==================================;
; Transposed matrix multiplication ;
;==================================;

; Actual list of arguments:
; base_C
; size_B_x = size_C_x
; base_B
; size_A_y = size_C_y
; size_A_x = size_B_y
; base_A

section .text
global matrix_multiply_tr
; Function arguments:
%define size_C_y        dword [ebp + 16]
%define size_C_x        dword [ebp + 24]
%define base_C          dword [ebp + 28]
%define size_B_y        dword [ebp + 12]
%define size_B_x        dword [ebp + 24]
%define base_B          dword [ebp + 20]
%define size_A_y        dword [ebp + 16]
%define size_A_x        dword [ebp + 12]
%define base_A          dword [ebp +  8]
; Function automatic variables:
%define idx_C_y         dword [ebp -  4]
%define idx_C_x         dword [ebp -  8]
matrix_multiply_tr:
    ; Create stack frame:
    push    ebp
    mov     ebp, esp
    ; Allocate space for automatic variables:
    ; Also align stack by 16-byte boundary.
    sub     esp, 12
    ; Save used callee-preserved registers:
    push    ebx
    push    esi
    push    edi

    ;--------------------------------------;
    ; Transpose matrix B elements in-place ;
    ;--------------------------------------;

    ; Index variables:
    ; eax = y
    ; ebx = x
    ; ecx = mat_B[y][x]
    ; edx = mat_B[x][y]

    ; Outer loop init:
    mov     eax, 0
.matrix_transpose_out_loop:
    cmp     eax, size_B_y
    jae     .matrix_transpose_out_loop_exit

    ; Inner loop init:
    mov     ebx, 0

    mov     ecx, base_B
    imul    esi, eax, 4
    imul    esi, size_B_x
    add     ecx, esi

    mov     edx, base_B
    imul    esi, eax, 4
    add     edx, esi
    .matrix_transpose_in_loop:
        cmp     ebx, eax
        jae     .matrix_transpose_in_loop_exit

        mov     esi, dword [ecx]
        mov     edi, dword [edx]

        mov     dword [ecx], edi
        mov     dword [edx], esi

        ; Update indecies:
        add     ebx, 1
        add     ecx, 4
        imul    esi, size_B_y, 4
        add     edx, esi
        jmp     .matrix_transpose_in_loop
    .matrix_transpose_in_loop_exit:

    ; Update indecies:
    add     eax, 1
    jmp     .matrix_transpose_out_loop
.matrix_transpose_out_loop_exit:

    ; Swap matrix B sizes:
    mov esi, size_B_x
    mov edi, size_B_y

    mov size_B_x, edi
    mov size_B_y, esi

    ;-----------------------------------------;
    ; Perform optimized matrix multiplication ;
    ;-----------------------------------------;

    ; Variable mapping:
    ; eax = c_y = a_y
    ; ebx = c_x = b_x = mul accum
    ; ecx = &mat_C[c_y][c_x]
    ; edx = a_x = b_y
    ; esi = &mat_A[a_y][a_x]
    ; edi = &mat_B[b_x][b_y]

    ; Outer loop init:
    mov     eax, 0
    mov     ecx, base_C
.matrix_multiply_tr_out_loop:
    cmp     eax, size_C_y
    jae     .matrix_multiply_tr_out_loop_exit

    ; Inner loop init:
    mov     ebx, 0
    .matrix_multiply_tr_in_loop:
        cmp     ebx, size_C_x
        jae     .matrix_multiply_tr_in_loop_exit

        ; Save eax, ebx to temporary memory:
        mov     idx_C_y, eax
        mov     idx_C_x, ebx

        ; Initialize loop indexes:
        ; edi = base_B + 4 * size_B_x * ebx
        imul    edi, ebx, 4
        imul    edi, size_B_x
        add     edi, base_B

        ; esi = base_A + 4 * size_A_x * eax:
        imul    esi, eax, 4
        imul    esi, size_A_x
        add     esi, base_A

        ; Initialize loop accumulator:
        ; eax = result of multiplication
        ; ebx = multilplication accumulator
        ; edx = index
        mov     ebx, 0
        mov     edx, 0

        .matrix_multiply_tr_mult_loop:
            cmp     edx, size_A_x
            jae     .matrix_multiply_tr_mult_loop_exit

            ; Update partial sum:
            mov     eax, dword [esi]
            imul    eax, dword [edi]
            add     ebx, eax

            ; Update indecies:
            add     edx, 1
            add     esi, 4
            add     edi, 4
            jmp     .matrix_multiply_tr_mult_loop
        .matrix_multiply_tr_mult_loop_exit:

        ; Save mat_C[c_y][c_x] to C memory:
        mov     dword [ecx], ebx

        ; Restore abx, ebx from memory:
        mov     eax, idx_C_y
        mov     ebx, idx_C_x

        ; Update indecies:
        add     ebx, 1
        add     ecx, 4
        jmp     .matrix_multiply_tr_in_loop
    .matrix_multiply_tr_in_loop_exit:

    ; Update indecies:
    add     eax, 1
    jmp     .matrix_multiply_tr_out_loop
.matrix_multiply_tr_out_loop_exit:

    ; Restore used callee-preserved registers:
    pop     edi
    pop     esi
    pop     ebx
    ; Reclaim stack memory for automatic variables:
    add     esp, 12
    ; Restore stack frame:
    mov     esp, ebp
    pop     ebp
    ; Return:
    xor     eax, eax
    ret

; Undef macros for safety reasons:
%undef size_C_y
%undef size_C_x
%undef base_C
%undef size_B_y
%undef size_B_x
%undef base_B
%undef size_A_y
%undef size_A_x
%undef base_A
; Function automatic variables:
%undef idx_C_y
%undef idx_C_x

;===============;
; Main function ;
;===============;

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
    ; Create stack frame:
    push    ebp
    mov     ebp, esp
    ; Save used callee-preserved registers:
    push    ebx
    push    esi
    push    edi
    ; Align stack by 16-byte boundary:
    sub     esp, 12
    ; Do not allocate any automatic variables.

    ;=====================;
    ; Initialize matrix A ;
    ;=====================;

    push MATRIX_SIZE ; size_y
    push MATRIX_SIZE ; size_x
    push mat_A       ; matrix_base
    call matrix_init

    ;=====================;
    ; Initialize matrix B ;
    ;=====================;

    push MATRIX_SIZE ; size_y
    push MATRIX_SIZE ; size_x
    push mat_B       ; matrix_base
    call matrix_init

    ;=====================================;
    ; Trivial multiplication of matricies ;
    ;=====================================;

    push mat_C
    push MATRIX_SIZE
    push mat_B
    push MATRIX_SIZE
    push MATRIX_SIZE
    push mat_A
    call matrix_multiply

    ;==============;
    ; Print result ;
    ;==============;

    push MATRIX_SIZE ; size_y
    push MATRIX_SIZE ; size_x
    push mat_C       ; matrix_base
    call matrix_print

    ; Restore stack value after alignment:
    add     esp, 12
    ; Restore used callee-preserved registers:
    pop     edi
    pop     esi
    pop     ebx
    ; Restore stack frame:
    mov     esp, ebp
    pop     ebp
    ; Return:
    xor     eax, eax
    ret
