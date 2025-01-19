// Copyright 2025 Vladislav Aleinik
#include <vector.hpp>

#include <iostream>

constexpr size_t SIZE = 100U;

int main(void)
{
    //----------------------------------------
    // Тривиальный тестовый стенд для вектора
    //----------------------------------------

    // Подгружаем область видимости VectorArithmetics в область видимости функции main.
    using namespace VectorArithmetics;

    // Вызываем конструктор класса Vector.
    Vector vec{SIZE};

    for (size_t i = 0U; i < SIZE; ++i)
    {
        // Производим запись по ссылке на элемент.
        vec.get(i) = i;
    }

    // Производим добавление вектора к самому себе.
    vec.add(vec);

    // Сравниваем результат сложения с ожидаемым.
    for (size_t i = 0U; i < SIZE; ++i)
    {
        if (!double_equal(2*i, vec.get(i)))
        {
            std::cout << "REJECT" << std::endl;
            return EXIT_FAILURE;
        }
    }

    std::cout << "OK" << std::endl;

    return EXIT_SUCCESS;
}
