#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <time.h>

//=========================//
// Stack testing benchmark //
//=========================//

// Type of element to be used with the stack:
typedef uint64_t data_t;

// Version of stack to be used:
#include "evil_stack.h"

#define BENCHMARK_STEP       10000U
#define BENCHMARK_NUM_PUSHS 100000U

int main(void)
{
    // Initialize stack:
    struct Stack lab_rat;
    stack_init(&lab_rat);

    // Push a bunch of element to the stack:
    for (data_t bench_i = 0U; bench_i < BENCHMARK_NUM_PUSHS; bench_i += BENCHMARK_STEP)
    {
        // Start time measurement:
        clock_t ticks_start = clock();

        for (data_t push_i = bench_i; push_i < bench_i + BENCHMARK_STEP; ++push_i)
        {
            stack_push(&lab_rat, push_i);
        }

        // Finish time measurement:
        clock_t ticks_end = clock();

        double ticks_delta = ticks_end - ticks_start;
        double seconds = ticks_delta / CLOCKS_PER_SEC;

        // Printout time:
        printf("Push[%8lu, %8lu]: %10.6lfs\n",
            bench_i, bench_i + BENCHMARK_STEP - 1U, seconds);
    }

    // Pop all the elements from the stack:
    for (data_t bench_i = BENCHMARK_NUM_PUSHS; bench_i > 0U; bench_i -= BENCHMARK_STEP)
    {
        // Start time measurement:
        clock_t ticks_start = clock();

        for (data_t pop_i = bench_i; pop_i > bench_i - BENCHMARK_STEP; --pop_i)
        {
            data_t popped = 0U;
            bool ret = stack_pop(&lab_rat, &popped);
            if (ret == false)
            {
                printf("[ERROR] Detected spurious failed pop!\n");
                return EXIT_FAILURE;
            }

            if (popped != pop_i - 1U)
            {
                printf("[ERROR] Popped invalid element (got %lu, expected %lu)!\n", popped, pop_i - 1U);
                return EXIT_FAILURE;
            }
        }

        // Finish time measurement:
        clock_t ticks_end = clock();

        double ticks_delta = ticks_end - ticks_start;
        double seconds = ticks_delta / CLOCKS_PER_SEC;

        // Printout time:
        printf("Pop [%8lu, %8lu]: %10.6lfs\n",
            bench_i - 1U, bench_i - BENCHMARK_STEP, seconds);
    }

    // Free stack:
    stack_free(&lab_rat);

    return EXIT_SUCCESS;
}
