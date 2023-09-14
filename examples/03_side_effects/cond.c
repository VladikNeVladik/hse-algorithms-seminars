#include <stdio.h>
#include <stdbool.h>

char print_a(void)
{
    printf("Side effect \"A\"\n");

    return 'A';
}

char print_b(void)
{
    printf("Side effect \"B\"\n");

    return 'B';
}

int main(void)
{
    printf("Conditional operator return value: %c\n", true? print_a() : print_b());

    printf("Comma operator return value: %c\n", (print_a(), print_b()));

    return 0;
}
