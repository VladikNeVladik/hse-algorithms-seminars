#ifndef __i386__
#error Wrong architecture!
#endif

#include <stdio.h>

// I/O functions easily callable from asm
// - Accept under-aligned stack
// - Return value (if any) in EAX
// - Up to three arguments in EAX, EDX, ECX
// - Preserve EBX, EBP, ESP, EDI, ESI
//
// Extends macro.c from SASM (usually /usr/share/sasm/NASM/macro.c)
#define IO_ATTR __attribute__((regparm(3),force_align_arg_pointer))

IO_ATTR int io_get_dec(void);

IO_ATTR void io_print_long_long(long long v);
IO_ATTR void io_print_string(const char *s);
IO_ATTR void io_newline(void);

IO_ATTR
int io_get_dec(void)
{
    int r;
    scanf("%d", &r);
    return r;
}

IO_ATTR
void io_print_long_long(long long v)
{
    printf("%lld", v);
}


IO_ATTR
void io_print_string(const char *s)
{
    fputs(s, stdout);
}

IO_ATTR
void io_newline(void)
{
    putchar('\n');
}
