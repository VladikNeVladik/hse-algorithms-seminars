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
    //------------------------
    // Vector basic testbench
    //------------------------

    // Create vector and fill it with elements:
    struct Vector vec = vector_create(SIZE);

    for (size_t i = 0U; i < SIZE; ++i)
    {
        double element = i;

        vector_set(&vec, i, element);
    }

    // Compare elements with those set into vector:
    for (size_t i = 0U; i < SIZE; ++i)
    {
        double expect = i;
        double actual = vector_get(&vec, i);

        if (!double_equal(expect, actual))
        {
            printf("REJECT\n");
            return EXIT_FAILURE;
        }
    }

    vector_free(&vec);

    //------------------------
    // Matrix basic testbench
    //------------------------

    Matrix initial = matrix_create(SIZE, SIZE);

    // Fill matrix with elements:
    for (size_t y = 0U; y < SIZE; ++y)
    {
        for (size_t x = 0U; x < SIZE; ++x)
        {
            double element = y * SIZE + x;

            matrix_set(&initial, y, x, element);
        }
    }

    // Save matrix to file:
    matrix_dump_to_file(MATRIX_FILENAME, &initial);

    // Read matrix from file:
    Matrix resulting = matrix_scan_from_file(MATRIX_FILENAME);

    // Compare two matricies:
    if (!matrix_equal(&initial, &resulting))
    {
        printf("REJECT\n");
        return EXIT_FAILURE;
    }

    matrix_free(&initial);
    matrix_free(&resulting);

    printf("OK\n");
    return EXIT_SUCCESS;
}
