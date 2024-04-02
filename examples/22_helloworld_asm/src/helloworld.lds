OUTPUT_FORMAT("elf32-i386");
OUTPUT_ARCH("i386");
ENTRY(_start);

LOADER_VADDR = 0x80000000;
LOADER_PADDR = 0x8000;
LOADER_SHIFT = LOADER_VADDR - LOADER_PADDR;

SECTIONS
{
    . = LOADER_VADDR;

    .text : AT(ADDR(.text) - LOADER_SHIFT)
    {
        KEEP(*(.rodata.multiboot))
        *(.rodata)
        *(.text)
    }

    .data : AT(ADDR(.data) - LOADER_SHIFT)
    {
        *(.data)
        *(.bss)
    }
}
