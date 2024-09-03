#ifndef __i386__
#error Wrong architecture!
#endif

#include <stdint.h>

// Expect program to be linked with definition of macro functions:
int      io_get_dec(void);
unsigned io_get_hex(void);

void io_print_dec(int v);
void io_print_hex(unsigned v);

void io_print_string(const char *s);

void io_newline(void);

// NOTE: disassemble compiled executable of this function
void assembled_example_function(void)
{
    // Unsigned 32-bit multiplication:
    static uint32_t u32_a;
    static uint32_t u32_b;
    static uint32_t u32_c;
    static uint32_t u32_d;

    u32_c = u32_a * u32_b;
    u32_d = u32_a / u32_b;

    // Signed 32-bit multiplication:
    static int32_t s32_a;
    static int32_t s32_b;
    static int32_t s32_c;
    static int32_t s32_d;

    s32_c = s32_a * s32_b;
    s32_d = s32_a / s32_b;

    // Unsigned 64-bit multiplication:
    static uint64_t u64_a;
    static uint64_t u64_b;
    static uint64_t u64_c;
    static uint64_t u64_d;

    u64_c = u64_a * u64_b;
    u64_d = u64_a / u64_b;

    // Signed 64-bit multiplication:
    static int64_t s64_a;
    static int64_t s64_b;
    static int64_t s64_c;
    static int64_t s64_d;

    s64_c = s64_a * s64_b;
    s64_d = s64_a / s64_b;
}
