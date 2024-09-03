bits 32

;==============================;
; Requested external functions ;
;==============================;

; stdio.h
extern printf, scanf
; stdlib.h
extern calloc, free
; string.h
extern memcpy

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

;================;
; Apply function ;
;================;

;----------------------------------------------------------
; Function: apply
; Description: applies function with list of arguments
;              to each array element
;----------------------------------------------------------
; Parameters (from last to first):
;   int                 ...     (in)        - list of nargs 4-byte arguments
;   int                 nargs   (in)        - number of arguments
;   void (*fn) (...)    fn      (in)        - applied function
;   size_t              length  (in)        - array size
;   int*                array   (in/out)    - array of elements
; Return value: none
;----------------------------------------------------------
section .text
global apply
%define     arglist         dword [ebp + 24]
%define     nargs           dword [ebp + 20]
%define     fn              dword [ebp + 16]
%define     length          dword [ebp + 12]
%define     array           dword [ebp +  8]
%define     tmp_ebx         dword [ebp -  4]
%define     tmp_edi         dword [ebp -  8]
%define     tmp_esi         dword [ebp - 12]
%define     arglist_moved   dword [ebp - 16]
apply:
    ; Create an aligned stack frame with 16 bytes of automatic variables:
    FUNCTION_PROLOGUE 16
    ; Save callee-preserved registers to automatic storage:
    mov     tmp_ebx, ebx
    mov     tmp_esi, esi
    mov     tmp_edi, edi

    ;=================================;
    ; Prepare stack for function call ;
    ;=================================;

    ; Compute argument area size:
    ; EBX := size of n elements
    mov     ebx, nargs
    imul    ebx, ebx, 4

    ; Prepare argument area:
    sub     esp, 4
    sub     esp, ebx
    and     esp, 0xfffffff0

    ; Compute modified arglist address:
    mov     arglist_moved, esp

    ; Iterate over the array:
    ; ESI = index of element in an array
    ; EDI = pointer to array element
    mov     esi, 0
    mov     edi, array
.apply_loop:
    cmp     esi, length
    je      .apply_loop_exit

    ; Copy all first nargs arguments to argument area on stack:
    ALIGN_STACK 12
    push    ebx                 ; number of copied bytes
    lea     eax, arglist        ; <
    push    eax                 ; < src
    push    arglist_moved       ; < dst
    call    memcpy
    UNALIGN_STACK 12

    ; Return stack to state prior to memcpy call:
    mov     esp, arglist_moved

    ; Save current element to element slot:
    mov     eax, arglist_moved
    add     eax, ebx
    mov     ecx, dword [edi]
    mov     dword [eax], ecx

    ; Perform function call:
    ; NOTE: the stack is aligned previouly and already has all the required arguments
    ; NOTE: do not clear the argument area afterwards and reuse it in subsequent call
    mov     eax, fn
    call    eax

    add     esi, 1
    add     edi, 4
    jmp     .apply_loop
.apply_loop_exit:

    ; Restore callee-preserved registers:
    mov     ebx, tmp_ebx
    mov     esi, tmp_esi
    mov     edi, tmp_edi
    ; Restore stack value after alignment:
    FUNCTION_EPILOGUE
    ; Return to caller procedure:
    ret

; Undefine macros for safety considerations:
%undef      arglist
%undef      nargs
%undef      fn
%undef      length
%undef      array
%undef      tmp_ebx
%undef      tmp_edi
%undef      tmp_esi
%undef      elem_ptr

;===============;
; Main function ;
;===============;

section .text
global main
%define     tmp_ebx     dword [ebp -  4]
%define     tmp_edi     dword [ebp -  8]
%define     tmp_esi     dword [ebp - 12]
%define     array_size  dword [ebp - 16]
%define     array_ptr   dword [ebp - 20]
main:
    ; Create an aligned stack frame with 20 bytes of automatic variables:
    FUNCTION_PROLOGUE 20
    ; Save callee-preserved registers to automatic storage:
    mov     tmp_ebx, ebx
    mov     tmp_esi, esi
    mov     tmp_edi, edi

    ;===========================;
    ; Allocate memory for input ;
    ;===========================;

    ; Print prompt:
    ALIGN_STACK 4
    push    str_prompt_array_size   ; format
    call    printf
    UNALIGN_STACK 4

    ; Enter array size:
    ; ASSUMPTION: a valid number is entered
    ALIGN_STACK 8
    lea     ebx, array_size         ; <
    push    ebx                     ; < pointer to input variable
    push    str_format_array_size   ; format
    call    scanf
    UNALIGN_STACK 8

    ; Allocate dynamic memory on heap for array elements:
    ; ASSUMPTION: memory allocation succeeds
    ALIGN_STACK 8
    push    4           ; array element size
    push    array_size  ; number of array elements
    call    calloc
    UNALIGN_STACK 8

    mov     array_ptr, eax

    ;======================;
    ; Input array elements ;
    ;======================;

    ; Print prompt:
    ALIGN_STACK 4
    push    str_prompt_array_elements   ; format
    call    printf
    UNALIGN_STACK 4

    ; Enter array:
    mov     ebx, 0
    mov     edi, array_ptr
.enter_array_loop:
    cmp     ebx, array_size
    je      .enter_array_loop_exit

    ; Enter array element:
    ; ASSUMPTION: a valid number is entered
    ALIGN_STACK 8
    push    edi                         ; pointer to input element
    push    str_format_array_element    ; format
    call    scanf
    UNALIGN_STACK 8

    add     ebx, 1
    add     edi, 4
    jmp     .enter_array_loop
.enter_array_loop_exit:

    ;=========================;
    ; Call the apply function ;
    ;=========================;

    ALIGN_STACK 20
    push    str_format_array_printout ; arglist[0]
    push    1                         ; nargs
    push    printf                    ; fn
    push    array_size                ; length
    push    array_ptr                 ; array
    call    apply
    UNALIGN_STACK 20

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
%undef     array_size
%undef     array_ptr

section .rodata
str_prompt_array_size:      db `Enter array size:\n`, 0
str_format_array_size:      db `%u`, 0
str_prompt_array_elements:  db `Enter array elements:\n`, 0
str_format_array_element:   db `%d`, 0
str_format_array_printout:  db `%d\n`, 0
