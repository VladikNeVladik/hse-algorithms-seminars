// Copyright 2024 Vladislav Aleinik
#include <stdio.h>
#include <stdlib.h>

//==================================================================================================
// Функция: fibs
// Назначение: вычисляет число Фибоначчи
//--------------------------------------------------------------------------------------------------
// Параметры:
// n (in) - номер запрашиваемого числа Фибоначчи.
//
// Возвращаемое значение: число Фибоначчи
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// отсутствуют
//==================================================================================================

// Конструкция typedef позволяет создавать псевдонимы типов
typedef unsigned long long ull_t;

// Объявление функции
ull_t fibs(unsigned n);

int main(void)
{
    // Печать 50 чисел Фибоначчи:
    for (unsigned i = 0U; i < 50U; ++i)
    {
        // Вызов функции:
        printf("fibs[%03u] = %llu\n", i, fibs(i));
    }

    return EXIT_SUCCESS;
}

//=====================//
// Определение функции //
//=====================//

#ifdef FIBS_CYCLE
ull_t fibs(unsigned n)
{
    if (n == 0U)
    {
        return 0ULL;
    }
    if (n == 1U)
    {
        return 1ULL;
    }

    ull_t prev = 0ULL;
    ull_t cur  = 1ULL;
    for (unsigned i = 1U; i < n; ++i)
    {
        ull_t tmp = prev;
        prev = cur;
        cur = tmp + cur;
    }

    return cur;
}
#endif

#ifdef FIBS_BAD_RECURSION
ull_t fibs(unsigned n)
{
    if (n == 0U)
    {
        return 0ULL;
    }
    if (n == 1U)
    {
        return 1ULL;
    }

    return fibs(n - 1ULL) + fibs(n - 2ULL);
}
#endif

#ifdef FIBS_GOOD_RECURSION
ull_t fibs_recursion_helper(ull_t acc_prev, ull_t acc_cur, unsigned n)
{
    if (n == 1U)
    {
        return acc_cur;
    }

    return fibs_recursion_helper(acc_cur, acc_prev + acc_cur, n - 1ULL);
}

ull_t fibs(unsigned n)
{
    if (n == 0U)
    {
        return 0U;
    }

    return fibs_recursion_helper(0ULL, 1ULL, n);
}
#endif
