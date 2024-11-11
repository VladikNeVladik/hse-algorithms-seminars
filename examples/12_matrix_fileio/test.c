// Copyright 2024 Vladislav Aleinik
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "vector.h"
#include "matrix.h"

#define SIZE 100U

const char* MATRIX_FILENAME = "res/matrix.txt";

int main(void)
{
    //----------------------------
    // Тестовый стенд для вектора
    //----------------------------

    // Вектор в SIZE-мерном пространстве
    struct Vector vec;

    // Аллоцируем неинициализированный вектор
    RetCode ret = vector_alloc(&vec, SIZE);
    verify_contract(ret == RET_OK,
        "Unable to allocate vector: %s\n",
        retcode_str(ret));

    // Задаём значения для элементов вектора
    for (size_t i = 0U; i < SIZE; ++i)
    {
        double element = i;

        ret = vector_set(&vec, i, element);
        verify_contract(ret == RET_OK,
            "Unable to set vector[%u]: %s\n", i, retcode_str(ret));
    }

    // Сравниваем значения с ранее записанными в вектор
    for (size_t i = 0U; i < SIZE; ++i)
    {
        // Извлекаем элемент вектора
        double actual;
        ret = vector_get(&vec, i, &actual);
        verify_contract(ret == RET_OK,
            "Unable to get vector[%u]: %s\n", i, retcode_str(ret));

        // Сравниваем значение с ожидаемым
        double expected = i;
        verify_contract(
            double_equal(expected, actual),
            "vector[%u] is invalid: (got %lf, expected %lf)\n",
            i, actual, expected);
    }

    // Освобождаем ресурсы вектора
    ret = vector_free(&vec);
    verify_contract(ret == RET_OK,
        "Unable to free vector: %s\n", retcode_str(ret));

    //----------------------------
    // Тестовый стенд для матрицы
    //----------------------------

    // Аллоцируем матрицу
    struct Matrix initial;
    ret = matrix_alloc(&initial, SIZE, SIZE);
    verify_contract(ret == RET_OK,
        "Unable to allocate matrix: %s\n",
        retcode_str(ret));

    // Инициализируем матрицу
    for (size_t y = 0U; y < SIZE; ++y)
    {
        for (size_t x = 0U; x < SIZE; ++x)
        {
            double element = y * SIZE + x;

            ret = matrix_set(&initial, y, x, element);
            verify_contract(ret == RET_OK,
                "Unable to set matrix[%u][%u]: %s\n", y, x, retcode_str(ret));
        }
    }

    // Сохраняем матрицу в файл
    ret = matrix_dump_to_file(MATRIX_FILENAME, &initial);
    verify_contract(ret == RET_OK,
        "Unable to save matrix to file \'%s\': %s\n",
        MATRIX_FILENAME, retcode_str(ret));

    // Считываем матрицу из файла
    struct Matrix resulting;
    ret = matrix_scan_from_file(MATRIX_FILENAME, &resulting);
    verify_contract(ret == RET_OK,
        "Unable to read matrix from file \'%s\': %s\n",
        MATRIX_FILENAME, retcode_str(ret));

    // Сравниваем две матрицы
    bool equal;
    ret = matrix_equal(&initial, &resulting, &equal);
    verify_contract(ret == RET_OK && equal,
        "Matrix comparison failed: %s\n", retcode_str(ret));

    int ret1;
    ret  = matrix_free(&initial);
    ret1 = matrix_free(&resulting);
    verify_contract(ret == RET_OK && ret1 == RET_OK,
        "Unable to free matrix: %s\n", retcode_str(ret));

    printf("OK\n");

    return EXIT_SUCCESS;
}
