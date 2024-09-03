#include <stdio.h>
#include <stdlib.h>

typedef unsigned long long ull_t;

//================================//
// Straightforward implementation //
//================================//

#ifdef SUM_SIMPLE
ull_t sum(const unsigned* array, unsigned size)
{
    ull_t acc = 0ULL;
    for (unsigned i = 0U; i < size; ++i)
    {
        acc += array[i];
    }

    return acc;
}
#endif

//========================//
// Recursion + generators //
//========================//

#ifdef SUM_GENERATOR
ull_t sum(unsigned (*get_element)(unsigned), unsigned size)
{
    if (size == 0U) { return 0ULL; }

    return sum(get_element, size - 1) + get_element(size);
}
#endif

//==================================================//
//             Tail recursion + generators          //
//==================================================//
// NOTE: it requires -O2 optimization level to work

#ifdef SUM_TAIL_RECURSION
ull_t sum_helper(unsigned (*get_element)(unsigned), unsigned size, ull_t acc);

ull_t sum(unsigned (*get_element)(unsigned), unsigned size)
{
    return sum_helper(get_element, size, 0U);
}

ull_t sum_helper(unsigned (*get_element)(unsigned), unsigned size, ull_t acc)
{
    if (size == 0U)
    {
        return acc;
    }

    return sum_helper(get_element, size - 1U, acc + get_element(size));
}
#endif

//================================//
// Good old cycle (best solution) //
//================================//

#ifdef SUM_CYCLE
ull_t sum(unsigned (*get_element)(unsigned), unsigned size)
{
    ull_t acc = 0ULL;

    for (unsigned i = 0U; i <= size; ++i)
    {
        acc += get_element(i);
    }

    return acc;
}
#endif

//=========//
// Testing //
//=========//

unsigned generate_progression(unsigned index)
{
    return index;
}

int main(void)
{
    // Enter number of sequence elements:
    printf("Enter number of sequence elements:\n");

    unsigned sequence_size;
    if (scanf("%u", &sequence_size) != 1)
    {
        printf("Expected one unsigned integer\n");
        return EXIT_FAILURE;
    }

#ifdef SUM_SIMPLE
    unsigned array[sequence_size];
    for (unsigned i = 0U; i < sequence_size; ++i)
    {
        array[i] = i;
    }

    printf("Progression sum of %u elements is: %llu\n",
        sequence_size, sum(array, sequence_size));
#endif

#if defined(SUM_GENERATOR) || defined(SUM_TAIL_RECURSION) || defined(SUM_CYCLE)
    printf("Progression sum of %u elements is: %llu\n",
        sequence_size, sum(&generate_progression, sequence_size));
#endif

    return EXIT_SUCCESS;
}
