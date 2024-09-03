bits 32

extern io_print_dec, io_print_hex, io_get_dec, io_print_string, io_newline

section .text
global main
main:
    ;=======================;
    ; Comparison operations ;
    ;=======================;

    ; eax = *u32_a
    ; ebx = *u32_b
    mov eax, dword [u32_a]
    mov ebx, dword [u32_b]

    ; Comparison instruction (which is simply a sub):
    cmp eax, ebx
    ; CFLAGS: SF=? ZF=? OF=? CF=?

    ; Is there any difference? Why use two instructions?
    sub eax, ebx
    ; CFLAGS: SF=? ZF=? OF=? CF=?

    ; Try again for a different pair of values:
    mov eax, dword [s32_c]
    mov ebx, dword [s32_d]

    cmp eax, ebx
    ; CFLAGS: SF=? ZF=? OF=? CF=?

    ;=======;
    ; Jumps ;
    ;=======;

    ; Uncoditional jump to a better place:
    jmp after_evil_instructions

    ; These instruction is skipped (and is evil):
    call 0

after_evil_instructions:

    ;==================;
    ; Conditional jump ;
    ;==================;

    ; Print prompt "Enter two equal numbers, please:"
print_prompt:
    mov eax, inputstr
    call io_print_string
    call io_newline

    ; Read two decimal numbers from terminal:
    call io_get_dec
    mov ebx, eax

    call io_get_dec
    mov ecx, eax

    ; Perform comparison:
    cmp ebx, ecx

    ; Perform conditional jump
    jne print_prompt

    ; Print "success!" line:
    mov eax, successstr
    call io_print_string
    call io_newline

    ;==================;
    ; Conditional move ;
    ;==================;

    ; Read two numbers from terminal:
    call io_get_dec
    mov ebx, eax

    call io_get_dec
    mov ecx, eax

    ; Perform comparison operation:
    cmp ebx, ecx

    ; Peroform a couple of conditional operations:
    cmovl eax, dword [cmp_neg]
    cmove eax, dword [cmp_zer]
    cmovg eax, dword [cmp_pos]

    ; Print result:
    call io_print_dec

    ;====================;
    ; Module computation ;
    ;====================;

    ; Task #1: compute |eax| using branches
    mov eax, dword [s32_c]

    cmp eax, 0
    jge task1_unchanged

    neg eax

task1_unchanged:

    ; Task #2: compute |eax| using conditional moves
    mov eax, dword [s32_c]

    ; ebx = -eax
    mov ebx, 0
    sub ebx, eax

    cmp eax, 0
    cmovl eax, ebx

    ; Task #3: compute |eax| using SAR instruction

    mov ebx, eax
    sar ebx, 31 ; 11111111 (-1) or 00000000 (0)

    imul ebx, 2   ; -2 or 0
    imul ebx, eax ; -2*eax or 0
    add ebx, eax  ; -eax or +eax

    xor eax, eax
    ret

section .rodata
inputstr:   db `Enter two equal numbers, please:`, 0
successstr: db `Success!`, 0

section .data
u32_a:      dd  10
u32_b:      dd  15
s32_c:      dd  -3
s32_d:      dd  -3

cmp_pos:    dd +1
cmp_zer:    dd 0
cmp_neg:    dd -1
