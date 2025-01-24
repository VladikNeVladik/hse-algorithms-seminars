// Copyright 2025 Vladislav Aleinik
#include <vector.hpp>
#include <matrix.hpp>

#include <iostream>

#include <utils.hpp>

constexpr size_t SIZE = 20U;
const std::string MATRIX_FILENAME = "res/matrix.txt";

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
        vec[i] = i;
    }

    // Производим добавление вектора к самому себе.
    Vector copy = vec + vec;

    // Сравниваем результат сложения с ожидаемым.
    for (size_t i = 0U; i < SIZE; ++i)
    {
        if (!double_equal(2*i, copy[i]))
        {
            std::cout << "REJECT" << std::endl;
            return EXIT_FAILURE;
        }
    }

    //----------------------------
    // Тестовый стенд для матрицы
    //----------------------------

    // Подгружаем область видимости MatrixArithmetics в область видимости функции main.
    using namespace MatrixArithmetics;

    Matrix initial{SIZE, SIZE};

    // Заполняем матрицу данными.
    for (size_t y = 0U; y < SIZE; ++y)
    {
        for (size_t x = 0U; x < SIZE; ++x)
        {
            initial[y][x] = y * SIZE + x;
        }
    }

    // Сохраняем матрицу.
    initial.dump_to_file(MATRIX_FILENAME);

    // Создаём матрицу на основе файла.
    Matrix resulting{MATRIX_FILENAME};

    // Умножаем обе матрицы на два.
    initial += initial;
    resulting += resulting;

    // Сравниваем обе матрицы.
    if (initial != resulting)
    {
        std::cout << "REJECT" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "OK" << std::endl;

    return EXIT_SUCCESS;
}
