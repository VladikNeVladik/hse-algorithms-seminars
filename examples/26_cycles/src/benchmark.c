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
void example_loop(void)
{
    static uint32_t arr[10U];

    uint32_t max = 0U;
    for (uint32_t i = 0; i < 10U; i++)
    {
        if (max < arr[i])
        {
            max = arr[i];
        }
    }
}
