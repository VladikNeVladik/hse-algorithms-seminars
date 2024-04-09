bits 32

extern io_print_dec, io_print_hex, io_get_dec, io_print_string, io_newline

section .text
global main
main:
    ;===============================;
    ; Overflow semantics and EFLAGS ;
    ;===============================;

    ; Signed (SF), Carry (CF):
    mov ah, 0ffh
    mov al, 0feh
    add ah, al
    ; CFLAGS: SF=1 ZF=0 OF=0 CF=1

    ; No status flags set
    mov ah, 0ffh
    mov al, 0feh
    sub ah, al
    ; CFLAGS: SF=0 ZF=0 OF=0 CF=0

    mov ax, 0FFFFh
    mov dx,    01h
    add ax, dx
    ; CFLAGS: SF=? ZF=? OF=? CF=?

    mov ax, 07FFFh
    mov dx, 07FFFh
    add ax, dx
    ; CFLAGS: SF=? ZF=? OF=? CF=?

    mov ax, 01000h
    mov dx, 01000h
    add ax, dx
    ; CFLAGS: SF=? ZF=? OF=? CF=?

    ;====================;
    ; 64-bit arithmetics ;
    ;====================;

    ; NOTE: x86 architecture is little endian
    ; u64_a   = 0x0123456789ABCDEF
    ; ADDRESS = [00][01][02][03][04][05][06][07]
    ; DATA    = [EF][CD][AB][89][67][45][23][01]

    ; edx:eax = qword [u64_a]
    mov     eax, dword [u64_a + 0]
    mov     edx, dword [u64_a + 4]

    ; ecx:ebx = qword [var64bit_b]
    mov     ebx, dword [u64_b + 0]
    mov     ecx, dword [u64_b + 4]

    ; eax = eax + ebx
    ; Sets CF (Carry Flag)
    add     eax, ebx

    ; edx = edx + ecx + CF
    ; Uses carry flag
    adc     edx, ecx

    ; qword [var64bit_a] = edx:eax
    mov     dword [u64_a + 0], eax
    mov     dword [u64_a + 4], edx

    ;=========================================;
    ; Signed/unsigned Multiplication/division ;
    ;=========================================;

    ; Signed 32-bit multiplication with 32-bit result:
    mov eax, dword [s32_a]
    mov ebx, dword [s32_b]

    ; eax = eax * ebx
    imul eax, ebx

    mov dword [s32_c], eax

    ; edx:eax = eax * ebx
    ; NOTE: just ignore overflow as the result is 32 bit
    mul ebx

    xor eax, eax
    ret

section .data
u64_a:     dq 0x180000002
u64_b:     dq 0x380000004

s32_a:     dd -2
s32_b:     dd -2
s32_c:     dd 0
s32_d:     dd 0

u32_a:     dd 2
u32_b:     dd 2
u32_c:     dd 0
u32_d:     dd 0
