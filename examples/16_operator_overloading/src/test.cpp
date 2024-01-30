// No copyright. Vladislav Aleinik, 2023
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

    // Make all vector operations directly visible from main function scope:
    using namespace VectorArithmetics;

    // Create vector and fill it with elements:
    Vector vec = vector_create(SIZE);

    for (size_t i = 0U; i < SIZE; ++i)
    {
        // NOTE: implicit cast to double is performed.
        vec[i] = i;
    }

    // Add two vectors together:
    vec += vec;

    // Compare elements with those set into vector:
    for (size_t i = 0U; i < SIZE; ++i)
    {
        if (!double_equal(2*i, vec[i]))
        {
            std::cout << "REJECT" << std::endl;
            return EXIT_FAILURE;
        }
    }

    vector_free(vec);

    //------------------------
    // Matrix basic testbench
    //------------------------

    // Make all matrix operations directly visible from main function scope:
    using namespace MatrixArithmetics;

    Matrix initial = matrix_create(SIZE, SIZE);

    // Fill matrix with elements:
    for (size_t y = 0U; y < SIZE; ++y)
    {
        for (size_t x = 0U; x < SIZE; ++x)
        {
            initial[y][x] = y * SIZE + x;
        }
    }

    // Save matrix to file:
    matrix_dump_to_file(MATRIX_FILENAME, initial);

    // Read matrix from file:
    Matrix resulting = matrix_scan_from_file(MATRIX_FILENAME);

    // Multiply both matricies by two:
    initial += initial;
    resulting += resulting;

    // Compare two matricies:
    if (initial != resulting)
    {
        std::cout << "REJECT" << std::endl;
        return EXIT_FAILURE;
    }

    matrix_free(initial);
    matrix_free(resulting);

    std::cout << "OK" << std::endl;

    return EXIT_SUCCESS;
}
