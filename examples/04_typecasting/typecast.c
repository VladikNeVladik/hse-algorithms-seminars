#include <stdio.h>
#include <stdbool.h>

int main(void)
{
    // Loss of precision due to 32-bit arithmetic
    float operand_a = 100000000.0f;
    float operand_b =         1.0f;

    // Implicit cast on arithmetic operation:
    int result_with_losses = operand_a + operand_b;

    printf("(float)%f + ( float)%f = (int)%d\n",
        operand_a, operand_b, result_with_losses);

    // No loss of precision due to type casting:
    int result_lossless = operand_a + (double) operand_b;

    printf("(float)%f + (double)%f = (int)%d\n",
        operand_a, (double)operand_b, result_lossless);

    return 0;
}
