// Copyright 2024 Vladislav Aleinik
#include <vector.hpp>
#include <matrix.hpp>

#include <iostream>

constexpr size_t SIZE = 100U;
const std::string MATRIX_FILENAME = "res/matrix.txt";

int main(void)
{
    //------------------------
    // Vector basic testbench
    //------------------------

    // Create vector and fill it with elements:
    Vector vec = vector_create(SIZE);

    for (size_t i = 0U; i < SIZE; ++i)
    {
        // Explicit C++-style cast:
        // NOTE: imlicit C-style cast also works.
        double element = static_cast<double>(i);

        vector_set(vec, i, element);
    }

    // Compare elements with those set into vector:
    for (size_t i = 0U; i < SIZE; ++i)
    {
        double expect = static_cast<double>(i);
        double actual = vector_get(vec, i);

        if (!double_equal(expect, actual))
        {
            std::cout << "REJECT" << std::endl;
            return EXIT_FAILURE;
        }
    }

    vector_free(vec);

    //------------------------
    // Matrix basic testbench
    //------------------------

    Matrix initial = matrix_create(SIZE, SIZE);

    // Fill matrix with elements:
    for (size_t y = 0U; y < SIZE; ++y)
    {
        for (size_t x = 0U; x < SIZE; ++x)
        {
            double element = static_cast<double>(y * SIZE + x);

            matrix_set(initial, y, x, element);
        }
    }

    // Save matrix to file:
    matrix_dump_to_file(MATRIX_FILENAME, initial);

    // Read matrix from file:
    Matrix resulting = matrix_scan_from_file(MATRIX_FILENAME);

    // Compare two matricies:
    if (!matrix_equal(initial, resulting))
    {
        std::cout << "REJECT" << std::endl;
        return EXIT_FAILURE;
    }

    matrix_free(initial);
    matrix_free(resulting);

    std::cout << "OK" << std::endl;

    return EXIT_SUCCESS;
}
