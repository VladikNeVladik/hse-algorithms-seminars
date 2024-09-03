#ifndef __i386__
#error Wrong architecture!
#endif

#include <stdint.h>

// Expect program to be linked with definition of macro functions:
int io_get_dec(void);
unsigned io_get_hex(void);

void io_print_dec(int v);
void io_print_hex(unsigned v);

void io_print_string(const char *s);

void io_newline(void);

// Declare variables:
// NOTE: in general, uninitialezed variables are bad.
static  uint8_t var_08bit;
static uint16_t var_16bit;
static uint32_t var_32bit;

static uint8_t buffer[10U] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

static const uint32_t ushifted = 10U;
static const int32_t sshifted  = 10;

static uint32_t res0, res1;
static  int32_t res2, res3;

// NOTE: disassemble compiled executable of this function
void assembled_example_function()
{
    // Representation of different-sized variables:
    var_08bit = 0x01U;
    var_16bit = 0x0001U;
    var_32bit = 0x00000001U;

    // Integer arithmetics:
    var_08bit += 1;
    var_16bit -= 1;
    var_32bit *= 10;
    var_32bit /= 5;

    // Print arithmetic results:
    io_print_hex(var_08bit);
    io_newline();
    io_print_hex(var_16bit);
    io_newline();
    io_print_hex(var_32bit);
    io_newline();

    // Array accesses:
    buffer[0] = 0xA;
    var_08bit = buffer[1];

    // Bitshift operations:
    res0 = (ushifted << 10U);
    res1 = (ushifted >> 10U);
    res2 = (sshifted << 10U);
    res3 = (sshifted >> 10U);
}
