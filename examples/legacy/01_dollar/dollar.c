#include <stdio.h>

const unsigned DOLLAR_START  = 0;
const unsigned DOLLAR_FINISH = 1000;
const unsigned DOLLAR_STEP   = 100;

const double RUBLES_IN_DOLLAR = 97.8439;

int main(void)
{
    for (unsigned num_dollars = DOLLAR_START;
        num_dollars <= DOLLAR_FINISH;
        num_dollars += DOLLAR_STEP)
    {
        double num_rubles = RUBLES_IN_DOLLAR * num_dollars;

        printf("%4d USD = %10.3f RUB\n", num_dollars, num_rubles);
    }

    return 0;
}
