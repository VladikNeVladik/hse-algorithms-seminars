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
void example_conditionals(void)
{
    static uint32_t u32_a, u32_b;
    static  int32_t s32_c, s32_d;

    if (u32_a > u32_b)
    {
        return;
    }

    if (u32_a != u32_b)
    {
        return;
    }

    if (u32_a <= u32_b && u32_a >= u32_b)
    {
        return;
    }

    if (s32_c == s32_d)
    {
        return;
    }

    if (s32_c > s32_d)
    {
        return;
    }

    if (s32_c >= s32_d || s32_c > s32_d)
    {
        return;
    }
}

typedef enum
{
    CMD_NOP,
    CMD_BEGIN,
    CMD_END,
    CMD_PUSH,
    CMD_PUSHR,
    CMD_POP,
    CMD_POPR
} CommandType;

// NOTE: disassemble compiled executable of this function
void example_ifelse_chain(void)
{
    static CommandType type;
    static int result;

    if      (type == CMD_NOP  ) { result = 0; }
    else if (type == CMD_BEGIN) { result = 1; }
    else if (type == CMD_END  ) { result = 2; }
    else if (type == CMD_PUSH ) { result = 3; }
    else if (type == CMD_PUSHR) { result = 4; }
    else if (type == CMD_POP  ) { result = 5; }
    else if (type == CMD_POPR ) { result = 6; }
}

// NOTE: disassemble compiled executable of this function
void example_switch(void)
{
    static CommandType type;
    static int result;

    switch (type)
    {
    case CMD_NOP:
        result = 0;
    case CMD_BEGIN:
        result = 1;
        break;
    case CMD_END:
        result = 2;
        break;
    case CMD_PUSH:
        result = 3;
        break;
    case CMD_PUSHR:
        result = 4;
        break;
    case CMD_POP:
        result = 5;
        break;
    case CMD_POPR:
        result = 6;
        break;
    }
}
