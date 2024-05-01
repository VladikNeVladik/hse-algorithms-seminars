bits 32

extern io_get_udec, io_get_char
extern io_print_udec, io_print_char, io_print_dec
extern io_print_string, io_newline

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

;===============;
; Sort function ;
;===============;

;----------------------------------------------------------
; Function: swap
; Description: swaps two objects of specified size
;----------------------------------------------------------
; Parameters:
;   ptr1 (in/out)   - pointer to first object
;   ptr2 (in/out)   - pointer to second object
;   size (in)       - object size
; Return value: none
;----------------------------------------------------------
global swap
%define     size        dword [ebp + 16]
%define     ptr2        dword [ebp + 12]
%define     ptr1        dword [ebp +  8]
swap:
    ; Create stack frame:
    push    ebp
    mov     ebp, esp
    ; Save used callee-preserved registers:
    push    ebx
    push    esi
    push    edi
    ; Align stack by 16-byte boundary:
    sub     esp, 12

    ; Initialize indecies:
    mov     ebx, ptr1
    mov     edx, ptr2
    mov     ecx, size

.loop_copy:
    mov    al, byte [ebx]
    mov    ah, byte [edx]

    mov    byte [ebx], ah
    mov    byte [edx], al

    add    ebx, 1
    add    edx, 1
    loop  .loop_copy

.exit:
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
    ret

%undef     size
%undef     ptr2
%undef     ptr1

;----------------------------------------------------------
; Function: sort
; Description: sorts an array in-place
;----------------------------------------------------------
; Parameters:
;   base (in/out)   - pointer to array base
;   num (in)        - number of elements
;   size (in)       - size of array element
;   compar (in)     - comparator
; Return value: none
;----------------------------------------------------------
global sort
%define     compar      dword [ebp + 20]
%define     size        dword [ebp + 16]
%define     num         dword [ebp + 12]
%define     base        dword [ebp +  8]
%define     tmp_ecx     dword [ebp -  4]
%define     tmp_edx     dword [ebp -  8]
sort:
    ; Create stack frame:
    push    ebp
    mov     ebp, esp
    ; Allocate automatic variables:
    sub     esp, 8
    ; Save used callee-preserved registers:
    push    ebx
    push    esi
    push    edi
    ; Align stack by 16-byte boundary:
    sub     esp, 4

    ; Leave if array has 0 or 1 elements:
    cmp     num, 1
    jbe     .exit

    ; Choose the last element as the pivot:
    mov     ecx, num
    sub     ecx, 1

    ; Seperate values:
    ; EDX = current index
    ; EBX = &array[EDX]
    ; ECX = pivot index
    ; ESI = temporary pivot index
    ; EDI = &array[ESI]
    mov     edx, 0
    mov     ebx, base
    mov     esi, 0
    mov     edi, base

    ; NOTE: this loop is better traceable with some help from above
    ;       (https://en.wikipedia.org/wiki/Quicksort#Lomuto_partition_scheme)
.loop_partition:
    cmp     edx, ecx
    je      .loop_partition_exit

    ; Compute pointer to pivot:
    ; EAX = &array[ECX]
    mov     eax, ecx
    imul    eax, size
    add     eax, base

    ; Save caller-preserved registers:
    mov     tmp_ecx, ecx
    mov     tmp_edx, edx

    ; Call comparator:
    ; compar(current element, pivot)
    push    eax
    push    ebx
    call    compar
    add     esp, 8

    ; If the current element is less than or equal to the pivot:
    cmp     eax, 0
    jl      .do_not_swap

    ; Swap the current element with the element at the temporary pivot index:
    push    size
    push    ebx
    push    edi
    call    swap
    add     esp, 12

    ; Move the temporary pivot index forward:
    add     esi, 1
    add     edi, size
    .do_not_swap:

    ; Restore caller-preserved registers:
    mov     ecx, tmp_ecx
    mov     edx, tmp_edx

    ; Update cycle indexes:
    add     edx, 1
    add     ebx, size
    jmp     .loop_partition
.loop_partition_exit:

    ; Get last element pointer:
    mov     eax, ecx
    mul     size
    add     eax, base

    ; Swap the pivot with the last element:
    push    size
    push    eax
    push    edi
    call    swap
    add     esp, 12

    ; Sort subarrays recursively:
.sort_first_subarray:
    cmp     esi, 1
    jl      .sort_second_subarray

    ; sort(base, esi - 1, size, compar)
    mov     eax, esi
    sub     eax, 1

    push    compar
    push    size
    push    eax
    push    base
    call    sort
    add     esp, 16

.sort_second_subarray:
    mov     ebx, esi
    add     ebx, 1

    cmp     ebx, num
    jge     .exit

    ; sort(base + (esi+1)*size, num - (esi+1), size, compar):
    mov     eax, ebx
    imul    eax, size
    add     eax, base

    mov     ecx, num
    sub     ecx, ebx

    push    compar
    push    size
    push    ecx
    push    eax
    call    sort
    add     esp, 16

    ; Array is sorted!

.exit:
    ; Restore stack value after alignment:
    add     esp, 4
    ; Restore used callee-preserved registers:
    pop     edi
    pop     esi
    pop     ebx
    ; Reclaim automatic variable stack:
    add     esp, 8
    ; Restore stack frame:
    mov     esp, ebp
    pop     ebp
    ; Return:
    ret
%undef      compar
%undef      size
%undef      num
%undef      base
%undef      tmp_ecx
%undef      tmp_edx

;==================;
; Sorted data type ;
;==================;

struc rational
    .numerator      resd 1
    .denominator    resd 1
endstruc

INPUT_FINISHED      equ 0
INPUT_CONTINUED     equ 1
INPUT_ERROR         equ 2

;----------------------------------------------------------
; Function: rational_input
; Description: parses a rational number from stdin
;----------------------------------------------------------
; Parameters:
;   rslt_ptr (out) - pointer to the resulting ratinal value
; Return value:
;   EAX            - flag that indicates end of input
;                    Possible values:
;                      INPUT_FINISHED
;                      INPUT_CONTINUED
;                      INPUT_ERROR
;----------------------------------------------------------
global rational_input
%define     rslt_ptr    dword [ebp +  8]
rational_input:
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

    ; Save rational pointer to EBX:
    mov     ebx, rslt_ptr

    ; Input numerator:
    call    io_get_udec
    mov     dword [ebx + rational.numerator], eax

    ; Set denominator if there is none:
    mov     dword [ebx + rational.denominator], 1

    ; Input seperator:
    ; EAX = '/' or ',' or '.'
    call    io_get_char

    ; Determine separator:
    ; switch (EAX)
    ; {
    ; case '/':
.case_slash:
    cmp     eax, '/'
    jne     .case_comma

    ; Input denominator:
    call    io_get_udec
    mov     dword [ebx + rational.denominator], eax

    ; Input seperator:
    ; EAX = ',' or '.'
    call    io_get_char

    ; Fallthrough
    ; NOTE: this happens in C if you omit "break;"

    ; case ',':
.case_comma:
    cmp     eax, ','
    jne     .case_dot

    mov     eax, INPUT_CONTINUED
    jmp     .exit

    ; case '.':
.case_dot:
    cmp     eax, '.'
    jne     .case_default

    mov     eax, INPUT_FINISHED
    jmp     .exit

    ; default:
.case_default:
    mov     eax, INPUT_ERROR

    ; Fallthrough:
    ; NOTE: this happens in C if you omit "break;"
    ; }

.exit:
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
    ret

; Undef macros for safety reasons:
%undef      rslt_ptr

;----------------------------------------------------------
; Function: rational_print
; Description: prints a seperate rational number
;----------------------------------------------------------
; Parameters:
;   rat_ptr (in)    - pointer to the printed rational
; Return value: none
;----------------------------------------------------------
global rational_print
%define     rat_ptr     dword [ebp +  8]
rational_print:
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

    ; Save rational pointer to EBX:
    mov     ebx, rat_ptr

.print_numerator:
    mov     eax, dword [ebx + rational.numerator]
    call    io_print_udec

    cmp     dword [ebx + rational.denominator], 1
    je     .do_not_print_denominator

.print_denominator:
    mov     eax, '/'
    call    io_print_char

    mov     eax, dword [ebx + rational.denominator]
    call    io_print_udec

.do_not_print_denominator:
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
    ret

; Undef macros for safety reasons:
%undef      rat_ptr

; Comparison results:
CMP_ABOVE       equ 1
CMP_EQUAL       equ 0
CMP_BELOW       equ -1

;----------------------------------------------------------
; Function: rational_compare
; Description: compares two rational numbers
;----------------------------------------------------------
; Parameters:
;   rat1_ptr (in)   - pointer to the 1st rational
;   rat2_ptr (in)   - pointer to the 2nd rational
; Return value:
;   EAX             - comparison result
;                     Possible values:
;                     - CMP_ABOVE (rat1  > rat2)
;                     - CMP_EQUAL (rat1 == rat2)
;                     - CMP_BELOW (rat1  < rat2)
;----------------------------------------------------------
global rational_compare
%define     rat2_ptr        dword [ebp + 12]
%define     rat1_ptr        dword [ebp +  8]
%define     rat2_val_u      dword [ebp -  4]
%define     rat2_val_l      dword [ebp -  8]
%define     rat1_val_u      dword [ebp - 12]
%define     rat1_val_l      dword [ebp - 16]
rational_compare:
    ; Create stack frame:
    push    ebp
    mov     ebp, esp
    ; Allocate 4 variables on stack:
    sub     esp, 16
    ; Save used callee-preserved registers:
    push    ebx
    push    esi
    push    edi
    ; Align stack by 16-byte boundary:
    sub     esp, 12

    ; Math:
    ; rat1 = n1 / d1
    ; rat2 = n2 / d2
    ; rat1_val = n1*d2
    ; rat2_val = n2*d1
    ; result = sign(n1*d2 - n2*d2) = sign(rat1_val - rat2_val)

    ; Preload rat1 and rat2 base values:
    mov     esi, rat1_ptr
    mov     edi, rat2_ptr

    ; Compute rat1_val:
    mov     eax, dword [esi + rational.numerator]
    mul     dword [edi + rational.denominator]

    mov     rat1_val_u, edx
    mov     rat1_val_l, eax

    ; Compute rat1_val:
    mov     eax, dword [edi + rational.numerator]
    mul     dword [esi + rational.denominator]

    mov     rat2_val_u, edx
    mov     rat2_val_l, eax

    ; Perform comparison:
.cmp_upper:
    ; Preload CMOV operands:
    mov     ebx, CMP_EQUAL
    mov     esi, CMP_BELOW
    mov     edi, CMP_ABOVE

    ; Compare upper parts:
    mov     ecx, rat1_val_u
    cmp     ecx, rat2_val_u
    je      .cmp_lower

    ; Set return value:
    cmovb   eax, esi ; CMP_BELOW
    cmova   eax, edi ; CMP_ABOVE

    jmp     .exit

.cmp_lower:
    ; Compare lower parts:
    mov     ecx, rat1_val_l
    cmp     ecx, rat2_val_l

    ; Set return value:
    cmovb   eax, esi ; CMP_BELOW
    cmove   eax, ebx ; CMP_EQUAL
    cmova   eax, edi ; CMP_ABOVE

.exit:
    ; Restore stack value after alignment:
    add     esp, 12
    ; Restore used callee-preserved registers:
    pop     edi
    pop     esi
    pop     ebx
    ; Reclaim memory for automatic variables:
    add     esp, 16
    ; Restore stack frame:
    mov     esp, ebp
    pop     ebp
    ; Return:
    ret

; Undef macros for safety reasons:
%undef      rat2_ptr
%undef      rat1_ptr
%undef      rat2_val_u
%undef      rat2_val_l
%undef      rat1_val_u
%undef      rat1_val_l


;===============;
; Main function ;
;===============;

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

    ;==========================;
    ; Input array to be sorted ;
    ;==========================;

    ; Use wierd stack memory allocation scheme:
    ;   Allocate values one by one.
    ;   This scheme is essentially impossible in C language.

    ; ESI = array size
    ; NOTE: do not handle zero-sized input arrays correctly.
    mov     esi, 0

.loop_input_array_element:
    ; Allocate one more structure on stack:
    sub     esp, 8
    add     esi, 1

    ; Save current array base:
    ; EBX = array base
    mov     ebx, esp

    ; Align stack by 16-byte boundary:
    and     esp, -16

    push    ebx
    call    rational_input
    pop     ecx ; Clear stack to it's aligned state

    ; Restore stack value:
    mov     esp, ebx

    ; Determine input result:
    ; switch (EAX)
    ; {
    .case_input_error:
        cmp     eax, INPUT_ERROR
        jne     .case_input_continued

        mov     eax, str_input_error
        call    io_print_string

        ; exit(EXIT_FAILURE);
        mov     eax, 1
        jmp     .exit

    .case_input_continued:
        cmp     eax, INPUT_CONTINUED
        jne     .case_input_finished

        ;
        jmp     .loop_input_array_element

    .case_input_finished:
        ; Falthrough.
    ; }

    ; Falthrough.
.loop_input_array_element_exit:

    ; At this point:
    ; EBX = array base
    ; ESI = number of element
    ; [EBX + 8n] array[n]
    ; [EBX +  8] array[1]
    ; [EBX +  0] array[0] (last entered element)

    ; Align stack by 16-byte boundary:
    and     esp, -16

    ;============;
    ; Sort array ;
    ;============;

    push    rational_compare
    push    8
    push    esi
    push    ebx
    call    sort

    ;==============;
    ; Print result ;
    ;==============;

    ; Do not print empty array:
    cmp     esi, 0
    je      .loop_print_result_exit

    ; EDI = current index
    mov     edi, esi
.loop_print_result:
    sub     edi, 1

    ; Form input argument:
    imul    eax, edi, 8
    add     eax, ebx

    ; Print rational number:
    push    eax
    call    rational_print
    pop     ecx ; Clear stack to it's aligned state

    ; Print separator:
    mov     ecx, '.'
    mov     edx, ','

    cmp     edi, 0
    cmove   eax, ecx
    cmovne  eax, edx
    call    io_print_char

    ; Update cycle index:
    cmp     edi, 0
    jne     .loop_print_result
.loop_print_result_exit:

    ; Clear array memory on stack:
    ; stack head = array-base + (stack size)*sizeof(rational)
    mov     esp, ebx
    imul    esi, esi, 8
    add     esp, esi

    ; Set return value to 0:
    xor     eax, eax
.exit:
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
    ret

section .rodata
str_input_error:         db `Input error\n`, 0
