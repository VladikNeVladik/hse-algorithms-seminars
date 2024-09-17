#include <stdio.h>
#include <string.h>

const char SAY_UPPER_CHAR = '_';
const char SAY_LOWER_CHAR = '-';

#define NUM_COW_ROWS 5U
const char* COW[NUM_COW_ROWS] = {
    "        \\   ^__^             ",
    "         \\  (oo)\\_______     ",
    "            (__)\\       )\\/\\ ",
    "                ||----w |    ",
    "                ||     ||    ",
};

const char* WHAT_DOES_THE_COW_SAY = "E=mc^2, oh, MOOOOO";

int main(void)
{
    unsigned cowsay_length = strlen(WHAT_DOES_THE_COW_SAY);

    // Print upper box line:
    putc(' ', stdout);
    for (int i = 0; i < cowsay_length; ++i)
    {
        putc(SAY_UPPER_CHAR, stdout);
    }
    putc(' ',  stdout);
    putc('\n', stdout);

    // Print cow saying:
    printf("<%s>\n", WHAT_DOES_THE_COW_SAY);

    // Print lower box line:
    putc(' ', stdout);
    for (int i = 0; i < cowsay_length; ++i)
    {
        putc(SAY_LOWER_CHAR, stdout);
    }
    putc(' ',  stdout);
    putc('\n', stdout);

    // Print cow:
    for (int cow_i = 0; cow_i < NUM_COW_ROWS; ++cow_i)
    {
        puts(COW[cow_i]);
    }

    return 0;
}
