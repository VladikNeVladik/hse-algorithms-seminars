bits 32

;==============================;
; Requested external functions ;
;==============================;

; stdio.h
extern printf, scanf
; stdlib.h
extern calloc, free

;================;
; Utility macros ;
;================;

; Friendly reminder - stack frame structure (from System V ABI for i386):
; [ebp + 16]            ...
; [ebp + 12] memory argument fourbyte 1
; [ebp +  8] memory argument fourbyte 0
; <EPS here is 16 bytes aligned>
; [ebp +  4] return address
; [ebp +  0] previous ebp value
; [ebp -  4] varN
;     ...
; [esp +  4] var1
; [esp +  0] var0

; Make stack be 16 bytes aligned after pushing %1 bytes
; enter X, 0 is eqivalent to:
; push  ebp
; mov   ebp, esp
; sub   esp, X
%macro FUNCTION_PROLOGUE 1.nolist
    enter   %1, 0
    and     esp, 0xfffffff0
%endmacro

; leave is eqivalent to:
; mov   esp, ebp
; pop   ebp
%macro FUNCTION_EPILOGUE 0.nolist
    leave
%endmacro

; Make stack be 16 bytes aligned after pushing %1 bytes (without seperate stack frame):
%macro ALIGN_STACK 1.nolist
    sub     esp, %1
    and     esp, 0xfffffff0
    add     esp, %1
%endmacro

; leave is eqivalent to:
; mov   esp, ebp
; pop   ebp
%macro UNALIGN_STACK 1.nolist
    add     esp, %1
%endmacro

;=======================;
; Matrix initialization ;
;=======================;

section .text
global matrix_init
%define     size_y      dword [ebp + 16]
%define     size_x      dword [ebp + 12]
%define     matrix_base dword [ebp +  8]
%define     tmp_ebx     dword [ebp -  4]
%define     tmp_edi     dword [ebp -  8]
%define     tmp_esi     dword [ebp - 12]
matrix_init:
    ; Create an aligned stack frame with 12 bytes of automatic variables:
    FUNCTION_PROLOGUE 12
    ; Save callee-preserved registers to automatic storage:
    mov     tmp_ebx, ebx
    mov     tmp_esi, esi
    mov     tmp_edi, edi
    ; Do not preserve FPU control word as it is unchanged.
    ; Do not preserve FPU status word and ST(i) registers

    ; Index variables:
    ; eax = y
    ; ebx = x
    ; ecx = matrix[y][x]

    ; Prepare floating point values for conditional moves:
    fldz    ; ST(2)
    fld1    ; ST(1)
    fldz    ; ST(0)

    ; Outer loop init:
    mov     eax, 0
    mov     ecx, matrix_base
.out_loop:
    cmp     eax, size_y
    jae     .out_loop_exit

    ; Inner loop init:
    mov     ebx, 0
    .in_loop:
        cmp     ebx, size_x
        jae     .in_loop_exit

        ; matrix[y][x] = (x <= y)? 1 : 0;
        cmp     eax, ebx

        fcmovbe     st0, st1
        fcmovnbe    st0, st2
        fst         dword [ecx]

        ; Update indecies:
        add     ebx, 1
        add     ecx, 4
        jmp     .in_loop
    .in_loop_exit:

    ; Update indecies:
    add     eax, 1
    jmp     .out_loop
.out_loop_exit:

.exit:
    ; Restore callee-preserved registers:
    mov     ebx, tmp_ebx
    mov     esi, tmp_esi
    mov     edi, tmp_edi
    ; Restore stack value after alignment:
    FUNCTION_EPILOGUE
    ; Return to caller procedure:
    ret

; Undef macros for safety reasons:
%undef      size_y
%undef      size_x
%undef      matrix_base
%undef      tmp_ebx
%undef      tmp_edi
%undef      tmp_esi

;=================;
; Matrix printout ;
;=================;

section .text
global matrix_print
%define     size_y      dword [ebp + 16]
%define     size_x      dword [ebp + 12]
%define     matrix_base dword [ebp +  8]
%define     tmp_ebx     dword [ebp -  4]
%define     tmp_edi     dword [ebp -  8]
%define     tmp_esi     dword [ebp - 12]
matrix_print:
    ; Create an aligned stack frame with 12 bytes of automatic variables:
    FUNCTION_PROLOGUE 12
    ; Save callee-preserved registers to automatic storage:
    mov     tmp_ebx, ebx
    mov     tmp_esi, esi
    mov     tmp_edi, edi

    ; Index variables:
    ; esi = x
    ; edi = y
    ; ebx = matrix[y][x]

    ; Outer loop init:
    mov     esi, 0
    mov     ebx, matrix_base
.out_loop:
    cmp     esi, size_y
    jae     .out_loop_exit

    ; Inner loop init:
    mov     edi, 0
    .in_loop:
        cmp     edi, size_x
        jae     .in_loop_exit

        fld     dword [ebx]

        ; Print matrix[y][x];
        ALIGN_STACK 12
        sub     esp, 8
        fstp    qword [esp]
        push    str_format_matrix_elem
        call    printf
        UNALIGN_STACK 12

        ; Update indecies:
        add     edi, 1
        add     ebx, 4
        jmp     .in_loop
    .in_loop_exit:

    ; Print newline:
    ALIGN_STACK 4
    push    str_format_newline
    call    printf
    UNALIGN_STACK 4

    ; Update indecies:
    add     esi, 1
    jmp     .out_loop
.out_loop_exit:

.exit:
    ; Restore callee-preserved registers:
    mov     ebx, tmp_ebx
    mov     esi, tmp_esi
    mov     edi, tmp_edi
    ; Restore stack value after alignment:
    FUNCTION_EPILOGUE
    ; Return to caller procedure:
    ret

; Undef macros for safety reasons:
%undef      size_y
%undef      size_x
%undef      matrix_base
%undef      tmp_ebx
%undef      tmp_edi
%undef      tmp_esi

section .rodata
str_format_matrix_elem:     db "%5.1f ", 0
str_format_newline:         db 0xA, 0

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

section .text
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
%define     tmp_ebx     dword [ebp -  4]
%define     tmp_edi     dword [ebp -  8]
%define     tmp_esi     dword [ebp - 12]
%define     idx_C_y     dword [ebp - 16]
%define     idx_C_x     dword [ebp - 20]
%define     fpu_ctrl    word  [ebp - 22]
matrix_multiply:
    ; Create an aligned stack frame with 22 bytes of automatic variables:
    FUNCTION_PROLOGUE 22
    ; Save callee-preserved registers to automatic storage:
    mov     tmp_ebx, ebx
    mov     tmp_esi, esi
    mov     tmp_edi, edi
    ; Store FPU control word and reset FPU status:
    ; NOTE: this prevents QNaN on FPU stack overflow
    fstcw   fpu_ctrl
    finit

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
.out_loop:
    cmp     eax, size_C_y
    jae     .out_loop_exit

    ; Inner loop init:
    mov     ebx, 0
    .in_loop:
        cmp     ebx, size_A_x
        jae     .in_loop_exit

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
        ; ST(1) = multilplication accumulator
        ; ST(0) = result of multiplication
        ; edx   = index
        fldz

        mov     ebx, 0
        mov     edx, 0

        .mult_loop:
            cmp     edx, size_C_x
            jae     .mult_loop_exit

            ; Update partial sum:
            fld     dword [esi]
            fld     dword [edi]
            fmulp

            faddp

            ; Update indecies:
            add     edx, 1
            add     esi, 4
            imul    eax, size_A_x, 4
            add     edi, eax
            jmp     .mult_loop
        .mult_loop_exit:

        ; Save mat_C[c_y][c_x] to C memory:
        fstp    dword [ecx]

        ; Restore abx, ebx from memory:
        mov     eax, idx_C_y
        mov     ebx, idx_C_x

        ; Update indecies:
        add     ebx, 1
        add     ecx, 4
        jmp     .in_loop
    .in_loop_exit:

    ; Update indecies:
    add     eax, 1
    jmp     .out_loop
.out_loop_exit:

.exit:
    ; Restore FPU control word:
    fldcw   fpu_ctrl

    ; Restore callee-preserved registers:
    mov     ebx, tmp_ebx
    mov     esi, tmp_esi
    mov     edi, tmp_edi
    ; Restore stack value after alignment:
    FUNCTION_EPILOGUE
    ; Return to caller procedure:
    ret

; Undef macros for safety reasons:
%undef      size_C_y
%undef      size_C_x
%undef      base_C
%undef      size_B_y
%undef      size_B_x
%undef      base_B
%undef      size_A_y
%undef      size_A_x
%undef      base_A
%undef      tmp_ebx
%undef      tmp_edi
%undef      tmp_esi
%undef      idx_C_y
%undef      idx_C_x

;===============;
; Main function ;
;===============;

; Hard-code matrix size:
MATRIX_SIZE     equ    8

section .text
global main
%define     tmp_ebx     dword [ebp -  4]
%define     tmp_edi     dword [ebp -  8]
%define     tmp_esi     dword [ebp - 12]
%define     mat_A       dword [ebp - 16]
%define     mat_B       dword [ebp - 20]
%define     mat_C       dword [ebp - 24]
main:
    ; Create an aligned stack frame with 24 bytes of automatic variables:
    FUNCTION_PROLOGUE 24
    ; Save callee-preserved registers to automatic storage:
    mov     tmp_ebx, ebx
    mov     tmp_esi, esi
    mov     tmp_edi, edi

    ;=========================================;
    ; Allocate memory for the three matricies ;
    ;=========================================;

    ; Allocate matrix A:
    ALIGN_STACK 8
    push    4                           ; size
    push    MATRIX_SIZE * MATRIX_SIZE   ; nmemb
    call    calloc
    UNALIGN_STACK 8

    ; ASSUMPTION: mat_A is non-null
    mov     mat_A, eax

    ; Allocate matrix B:
    ALIGN_STACK 8
    push    4                           ; size
    push    MATRIX_SIZE * MATRIX_SIZE   ; nmemb
    call    calloc
    UNALIGN_STACK 8

    ; ASSUMPTION: mat_B is non-null
    mov     mat_B, eax

    ; Allocate matrix C:
    ALIGN_STACK 8
    push    4                           ; size
    push    MATRIX_SIZE * MATRIX_SIZE   ; nmemb
    call    calloc
    UNALIGN_STACK 8

    ; ASSUMPTION: mat_C is non-null
    mov     mat_C, eax

    ;============================;
    ; Fill matricies with values ;
    ;============================;

    ALIGN_STACK 12
    push    MATRIX_SIZE
    push    MATRIX_SIZE
    mov     eax, mat_A
    push    eax
    call    matrix_init
    UNALIGN_STACK 12

    ALIGN_STACK 12
    push    MATRIX_SIZE
    push    MATRIX_SIZE
    mov     eax, mat_B
    push    eax
    call    matrix_init
    UNALIGN_STACK 12

    ;========================;
    ; Perform multiplication ;
    ;========================;

    ALIGN_STACK 24
    push    mat_C
    push    MATRIX_SIZE
    push    mat_B
    push    MATRIX_SIZE
    push    MATRIX_SIZE
    push    mat_A
    call    matrix_multiply
    UNALIGN_STACK 24

    ;========================;
    ; Print resulting matrix ;
    ;========================;

    ALIGN_STACK 12
    push    MATRIX_SIZE
    push    MATRIX_SIZE
    mov     eax, mat_C
    push    eax
    call    matrix_print
    UNALIGN_STACK 12

    ;=======================;
    ; Free allocated memory ;
    ;=======================;

    ALIGN_STACK 4
    mov     eax, mat_A
    push    eax
    call    free
    UNALIGN_STACK 4

    ALIGN_STACK 4
    mov     eax, mat_B
    push    eax
    call    free
    UNALIGN_STACK 4

    ALIGN_STACK 4
    mov     eax, mat_C
    push    eax
    call    free
    UNALIGN_STACK 4

    ; Set return value to 0:
    xor     eax, eax
.exit:
    ; Restore callee-preserved registers:
    mov     ebx, tmp_ebx
    mov     esi, tmp_esi
    mov     edi, tmp_edi
    ; Restore stack value after alignment:
    FUNCTION_EPILOGUE
    ; Return to caller procedure:
    ret

; Undefine macros for safety considerations:
%undef     tmp_ebx
%undef     tmp_edi
%undef     tmp_esi
%undef     mat_A
%undef     mat_B
%undef     mat_C
