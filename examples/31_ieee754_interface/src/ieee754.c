// No copyright. Vladislav Aleinik, 2024

// Request specific standard library implementation (glibc):
#define _GNU_SOURCE

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <math.h>
#include <fenv.h>

#include <signal.h>

//=====================//
// IEEE754 arithmetics //
//=====================//

double not_a_number()
{
    // sign     = 0
    // exponent =  111 1111 1111
    // fraction =                1000 0000  ... 0000
    uint64_t NaN = 0x7FF8000000000000LLU;

    return *((double*) &NaN);
}

bool is_nan(double x)
{
    return x != x;
}

bool is_infinity(double x)
{
    double expected_NaN = 0.0 * x;

    return is_nan(expected_NaN);
}

//======================//
// Dump FPU environment //
//======================//

#pragma STDC FENV_ACCESS ON

void show_fe_exceptions(void)
{
    printf("Current exceptions raised: ");

    int bitmask = fetestexcept(FE_ALL_EXCEPT);

    if (bitmask & FE_DIVBYZERO)
        printf(" FE_DIVBYZERO");
    if (bitmask & FE_INEXACT)
        printf(" FE_INEXACT");
    if (bitmask & FE_INVALID)
        printf(" FE_INVALID");
    if (bitmask & FE_OVERFLOW)
        printf(" FE_OVERFLOW");
    if (bitmask & FE_UNDERFLOW)
        printf(" FE_UNDERFLOW");
    if (bitmask == 0)
        printf(" none");
    printf("\n");
}

void show_fe_rounding_method(void)
{
    printf("Current rounding method:    ");
    switch (fegetround()) {
           case FE_TONEAREST:  printf ("FE_TONEAREST");  break;
           case FE_DOWNWARD:   printf ("FE_DOWNWARD");   break;
           case FE_UPWARD:     printf ("FE_UPWARD");     break;
           case FE_TOWARDZERO: printf ("FE_TOWARDZERO"); break;
           default:            printf ("unknown");
    };
    printf("\n");
}

void show_fe_environment(void)
{
    show_fe_exceptions();
    show_fe_rounding_method();
}

//====================//
// Exception handling //
//====================//

// Bring color to one's life:
#define RED     "\033[1;31m"
#define RESET   "\033[0m"

void handler_division_by_zero(int signum)
{
    printf(RED"[FPU EXCEPTION] Division by zero\n"RESET);
    exit(EXIT_FAILURE);
}

void enable_division_by_zero_check()
{
    // Enable FE_DIVBYZERO trap:
    #ifdef FE_DIVBYZERO
        if (feenableexcept(FE_DIVBYZERO) == -1)
        {
            printf("[ERROR] Unable to enable \"division by zero\" trap\n");
            exit(EXIT_FAILURE);
        }
    #else
        printf("[BUG] Not implemented \"division by zero\" error-check on this architecture\n");
        exit(EXIT_FAILURE);
    #endif

    // Enable SIGFPE signal:
    sigset_t block_all_signals;
    if (sigfillset(&block_all_signals) == -1)
    {
        printf("[ERROR] Unable to create filled signal mask\n");
        exit(EXIT_FAILURE);
    }

    struct sigaction act =
    {
        .sa_handler = handler_division_by_zero,
        .sa_mask    = block_all_signals,
        .sa_flags   = 0
    };
    if (sigaction(SIGFPE, &act, NULL) == -1)
    {
        printf("[ERROR] Unable to set \"division by zero\" handler\n");
        exit(EXIT_FAILURE);
    }
}

void disable_division_by_zero_check()
{
    // Disable FE_DIVBYZERO trap:
    #ifdef FE_DIVBYZERO
        if (fedisableexcept(FE_DIVBYZERO) == -1)
        {
            printf("[ERROR] Unable to disable \"division by zero\" trap\n");
            exit(EXIT_FAILURE);
        }
    #else
        printf("[BUG] Not implemented \"division by zero\" error-check on this architecture\n");
        exit(EXIT_FAILURE);
    #endif

    struct sigaction act =
    {
        .sa_handler = SIG_IGN,
        .sa_flags   = 0
    };
    if (sigaction(SIGFPE, &act, NULL) == -1)
    {
        printf("[ERROR] Unable to disable \"division by zero\" signal\n");
        exit(EXIT_FAILURE);
    }
}

//===============//
// Main function //
//===============//

#define SAMPLE_SIZE 100
#define POW_BASE    1.1f
#define POW_STEP    0.1f
#define NUM_STEPS   10

int main(void)
{
    //===============================//
    // Print FP value/representation //
    //===============================//

    float       flt_a = 1.0;
    double      dbl_b = 1.0;
    long double ldb_c = 1.0;

    printf("flt_a = %f, sizeof = %zu\n", flt_a, sizeof(flt_a));
    printf("flt_a = ");

    uint32_t* repr_a = (uint32_t*) &flt_a;
    for (int i = 31; i >= 0; --i)
    {
        printf("%d", (*repr_a >> i) & 1);

        if (i % 8 == 0)
        {
            printf(" ");
        }
    }
    printf("\n");

    printf("dbl_b = %lf, sizeof = %zu\n", dbl_b, sizeof(dbl_b));
    printf("dbl_b = ");

    uint64_t* repr_b = (uint64_t*) &dbl_b;
    for (int i = 63; i >= 0; --i)
    {
        printf("%lld", (*repr_b >> i) & 1);
        if (i % 8 == 0)
        {
            printf(" ");
        }
    }
    printf("\n");

    printf("ldb_c = %Lf, sizeof = %zu\n", ldb_c, sizeof(ldb_c));

    printf("\n");

    //==================================//
    // Showcase NaN and infinity checks //
    //==================================//

    printf("Not a number check: %s\n", is_nan(not_a_number()) ? "OK" : "FAILED");
    printf("Infinity check:     %s\n", is_infinity(1.0/0.0)   ? "OK" : "FAILED");

    //=========================//
    // Show error accumulation //
    //=========================//

    for (int step = 0; step < NUM_STEPS; ++step)
    {
        float base = POW_BASE + step * POW_STEP;

        printf("Sum of power series for base %f\n", base);

        float powers[SAMPLE_SIZE] = {};
        for (size_t i = 0U; i < SAMPLE_SIZE; ++i)
        {
            powers[i] = powf(base, i);
        }

        float sum = 0.0;
        for (size_t i = 0U; i < SAMPLE_SIZE; ++i)
        {
            sum += powers[i];
        }

        printf("    Min-to-max summation: %f\n", sum);

        sum = 0.0;
        for (int i = SAMPLE_SIZE - 1; i >= 0; --i)
        {
            sum += powers[i];
        }

        printf("    Max-to-min summation: %f\n", sum);

        sum = (powf(base, SAMPLE_SIZE) - 1.0) / (base - 1.0);
        printf("    Analythical result:   %f\n", sum);

        printf("\n");
    }

    //===================================//
    // Work with current FPU environment //
    //===================================//

    // Show default environment:
    show_fe_environment();
    printf("\n");

    // Perform some computation under default environment:
    // rint() with FE_DOWNWARD  is floor()
    // rint() with FE_UPWARD    is ceil()
    // rint() with FE_UPWARD    is ceil()
    // rint() with FE_TONEAREST is like round(), but rounded "to even" instead of "away from zero"
    printf("+11.5 -> %+4.1f\n", rint(+11.5));
    printf("-12.5 -> %+4.1f\n", rint(-12.5));
    show_fe_environment();
    printf("\n");

    // Save current environment:
    fenv_t cur_env;
    fegetenv(&cur_env);

    // Perform some computation with new rounding method:
    feclearexcept(FE_ALL_EXCEPT);
    fesetround(FE_DOWNWARD);
    printf("1.0/0.0 = %f\n", 1.0/0.0);
    printf("+11.5 -> %+4.1f\n", rint(+11.5));
    printf("-12.5 -> %+4.1f\n", rint(-12.5));
    show_fe_environment();
    printf("\n");

    // Restore previous environment:
    fesetenv(&cur_env);
    show_fe_environment();
    printf("\n");

    //==========================//
    // Work with FPU exceptions //
    //==========================//

    // Undetected divide by zero:
    printf("1.0/0.0 = %f\n", 1.0/0.0);

    // [Linux Moment Warning]
    // Use OS-specific exception handling:
    enable_division_by_zero_check();

    // Divide by zero raises exception:
    printf("1.0/0.0 = %f\n", 1.0/0.0);

    disable_division_by_zero_check();

    return EXIT_SUCCESS;
}
