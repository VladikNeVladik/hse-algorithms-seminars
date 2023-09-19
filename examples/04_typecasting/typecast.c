#include <stdio.h>
#include <stdbool.h>

int main(void)
{
    // Loss of precision due to 32-bit arithmetic
    float operand_a = 100000000.0f;
    float operand_b = 1.0f;
    // Implicit cast on arithmetic operation:
    // Cast operand_a: float -> float
    // Cast operand_b: float -> float
    // +: float, float -> float
    // Cast result of +: float -> int
    int result_with_losses = operand_a + operand_b;

    printf("(float)%f + ( float)%f = (int)%d\n",
        operand_a, operand_b, result_with_losses);

    // No loss of precision due to type casting:
    // Cast operand_a: float -> float
    // Cast operand_b: float -> double
    // +: double, double -> double
    // Cast result of +: double -> int
    int result_lossless = operand_a + (double)operand_b;

    printf("(float)%f + (double)%f = (int)%d\n",
        operand_a, (double)operand_b, result_lossless);

    return 0;
}
