#include <stdbool.h>
#include <stdio.h>
#include <limits.h>

int main(void)
{
    short num1, num2 = 0;

    // Parse program input:
    int num_inputs = scanf("%hd\n%hd", &num1, &num2);
    if (num_inputs < 2)
    {
        printf("adder: expected input \"<addentum1>\\n<addentum2>\\n\"\n");
        return 1;
    }

    // Cast numbers to long and check if they overflow on addition:
    int num1_ext = num1;
    int num2_ext = num2;
    int sum_ext = num1_ext + num2_ext;
    if (sum_ext < SHRT_MIN || sum_ext > SHRT_MAX)
    {
        printf("adder: %hd+%hd is too big to be represented in short\n", num1, num2);
        return 1;
    }

    // Perform short integer addition:
    // NOTE: if num1 + num2 is not representable by short, a problem might arise.
    short sum = num1 + num2;

    // Print result:
    printf("%hd\n", sum);

    // Another approach to the problem of overflow - use compiler builtins:
    short rslt;
    bool error = __builtin_add_overflow(num1, num2, &rslt);
    if (error)
    {
        printf("adder: %hd+%hd is too big to be represented in short\n", num1, num2);
        return 1;
    }

    printf("%hd\n", rslt);

    return 0;
}
