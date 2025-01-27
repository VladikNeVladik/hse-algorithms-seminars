// Copyright 2024 Vladislav Aleinik
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdexcept>

#include <time.h>

//=================================================//
// Тестовый стенд для измерения производительности //
//=================================================//

// Добавление собственного заголовочного файла
#include <stack.hpp>

const unsigned BENCHMARK_STEP       =  10000U;
const unsigned BENCHMARK_NUM_PUSHES = 200000U;

int main(void)
{
    // Стратегии реаллокации
    StackResizePolicy policies[] =
    {
        RESIZE_SUM,
        RESIZE_MUL
    };

    const char* policy_names[] =
    {
        "Линейная реаллокация",
        "Экспоненциальная реаллокация с гистерезисом"
    };

    for (size_t policy_i = 0; policy_i < sizeof(policies) / sizeof(StackResizePolicy); ++policy_i)
    {
        // Печатаем имя стратегии реаллокации
        printf("=== %s ===\n", policy_names[policy_i]);

        // Инициализируем стек с соответствующей стратегией реаллокации
        Stack stack(policies[policy_i]);

        // Добавляем элементы в стек
        for (Data_t bench_i = 0U; bench_i < BENCHMARK_NUM_PUSHES; bench_i += BENCHMARK_STEP)
        {
            // Начало измеряемого отрезка времени
            clock_t ticks_start = clock();

            for (Data_t push_i = bench_i; push_i < bench_i + BENCHMARK_STEP; ++push_i)
            {
                stack.push(push_i);
            }

            // Конец измеряемого отрезка времени
            clock_t ticks_end = clock();

            double ticks_delta = ticks_end - ticks_start;
            double seconds = ticks_delta / CLOCKS_PER_SEC;

            // Печатаем время на наполнение стека элементов
            printf("Push[%9lu, %9lu]: %10.6lfs\n",
                bench_i, bench_i + BENCHMARK_STEP - 1U, seconds);
        }

        // Извлекаем все элементы из стека
        for (Data_t bench_i = BENCHMARK_NUM_PUSHES; bench_i > 0U; bench_i -= BENCHMARK_STEP)
        {
            // Начало измеряемого отрезка времени
            clock_t ticks_start = clock();

            for (Data_t pop_i = bench_i; pop_i > bench_i - BENCHMARK_STEP; --pop_i)
            {
                try
                {
                    Data_t popped = stack.pop();
                    if (popped != pop_i - 1U)
                    {
                        printf("[ERROR] Popped invalid element (got %lu, expected %lu)!\n", popped, pop_i - 1U);
                        return EXIT_FAILURE;
                    }
                }
                catch (std::runtime_error& exc)
                {
                    printf("[ERROR] Runtime error: %s\n", exc.what());
                    return EXIT_FAILURE;
                }
            }

            // Конец измеряемого отрезка времени
            clock_t ticks_end = clock();

            double ticks_delta = ticks_end - ticks_start;
            double seconds = ticks_delta / CLOCKS_PER_SEC;

            // Печатаем время на извлечение элементов из стека
            printf("Pop [%9lu, %9lu]: %10.6lfs\n",
                bench_i - 1U, bench_i - BENCHMARK_STEP, seconds);
        }
    }

    // Тестирование на безопасность исключений.
    Stack stack = {0, 1, 2};

    // Извлекаем элементы стека.
    stack.pop();
    stack.pop();
    stack.pop();

    try
    {
        stack.pop();
    }
    catch (const std::runtime_error& exc)
    {
        // Функция pop предоставляет строгую гарантию безопасности.
        // Поэтому после выброса исключения объект будет находиться в том же состоянии,
        // что и до выброса исключения.
        printf("Catch exception: \'%s\'\n", exc.what());
    }

    if (!stack.empty())
    {
        printf("[ERROR] Stack state changed\n");
        return EXIT_FAILURE;
    }

    // Производим доступ к объекту.
    stack.push(10);

    if (stack.pop() != 10)
    {
        printf("[ERROR] Invalid stack\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
