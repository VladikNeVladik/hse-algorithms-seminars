bits 32
extern _start

; Use address fields in flags:

MULTIBOOT_AOUT_KLUDGE equ 0x00010000

MB_MAGIC            equ 0x1BADB002
MB_FLAGS            equ MULTIBOOT_AOUT_KLUDGE
MB_CHECKSUM         equ -(MB_MAGIC+MB_FLAGS)
LOADER_VADDR        equ 0x80000000
LOADER_PADDR        equ 0x8000
MULTIBOOT_TEXT_MODE equ 0x1

section .rodata.multiboot
align 4
    dd MB_MAGIC      ; Magic number
    dd MB_FLAGS      ; Header flags
    dd MB_CHECKSUM   ; Header checksum
    dd LOADER_PADDR  ; Header paddr (loader starts with multiboot header)
    dd LOADER_PADDR  ; Loader paddr
    dd 0x0           ; Ending paddr of loader, ignored
    dd 0x0           ; Ending paddr of bss section, ignored
    dd (_start - LOADER_VADDR + LOADER_PADDR) ; Entry paddr
    dd MULTIBOOT_TEXT_MODE ; Multiboot video mode
    dd 0x0                 ; VGA width
    dd 0x0                 ; VGA height
    dd 0x0                 ; VGA color2
