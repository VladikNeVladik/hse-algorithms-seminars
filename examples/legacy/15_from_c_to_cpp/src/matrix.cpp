// Copyright 2024 Vladislav Aleinik

#include <matrix.hpp>

#include <iostream>
#include <fstream>
#include <cmath>

#include <utils.hpp>

// Existance management:
Matrix matrix_create(size_t size_x, size_t size_y)
{
    // Allocate 2D-array:
    // NOTE: new throws exception std::bad_alloc on allocation error.
    double** data = new double*[size_y];

    for (size_t y = 0U; y < size_y; ++y)
    {
        data[y] = new double[size_x];
        // NOTE: new throws exception std::bad_alloc on allocation error.
    }

    Matrix to_return = {data, size_x, size_y};

    return to_return;
}

void matrix_free(Matrix& mat)
{
    VERIFY_CONTRACT(mat.data != nullptr,
        "Invalid matrix data (possible double free)\n");

    // Free all the rows:
    for (size_t y = 0U; y < mat.size_y; ++y)
    {
        delete[] mat.data[y];
    }

    // Free array of rows:
    delete[] mat.data;

    // Set matrix data to NULL to detect double free:
    mat.data = NULL;
}

// Element access:
double matrix_get(const Matrix& mat, size_t y, size_t x)
{
    VERIFY_CONTRACT(mat.data != nullptr,
        "Invalid matrix data\n");
    VERIFY_CONTRACT(y < mat.size_y,
        "Index y=%zu out of bounds (size_y is %zu)\n",
        y, mat.size_y);
    VERIFY_CONTRACT(x < mat.size_x,
        "Index x=%zu out of bounds (size_x is %zu)\n",
        x, mat.size_x);

    return mat.data[y][x];
}

void matrix_set(Matrix& mat, size_t y, size_t x, double element)
{
    VERIFY_CONTRACT(mat.data != nullptr,
        "Invalid matrix data (possible double free)\n");
    VERIFY_CONTRACT(y < mat.size_y,
        "[matrix_set] Index y=%zu out of bounds (size_y is %zu)\n",
        y, mat.size_y);
    VERIFY_CONTRACT(x < mat.size_x,
        "[matrix_set] Index x=%zu out of bounds (size_x is %zu)\n",
        x, mat.size_x);

    mat.data[y][x] = element;
}

// Matrix parsing and printout:
Matrix matrix_scan_from_file(const std::string& filename)
{
    // Open file:
    std::ifstream file = std::ifstream(filename, std::ios::in);

    VERIFY_CONTRACT(file.good(),
        "Unable to open file \'%s\'\n",
        filename.c_str());

    // Scan matrix sizes:
    size_t size_y = 0U, size_x = 0U;

    // Read sizes from file:
    file >> size_y;
    file >> size_x;

    VERIFY_CONTRACT(file.good(),
        "Unable to parse matrix sizes \'%s\'\n",
        filename.c_str());

    // Allocate zero matrix of specified size:
    Matrix to_return = matrix_create(size_y, size_x);

    // Parse matrix elements:
    for (size_t y = 0U; y < size_y; ++y)
    {
        for (size_t x = 0U; x < size_x; ++x)
        {
            double element;
            file >> element;

            VERIFY_CONTRACT(file.good(),
                "Unable to parse matrix elements\n");

            matrix_set(to_return, y, x, element);
        }
    }

    // NOTE: C++ automatically closes opened file on function end.
    //       It is a major feature of C++ and will be discussed later.

    return to_return;
}

void matrix_dump_to_file(const std::string& filename, const Matrix& mat)
{
    // Open file:
    std::ofstream file = std::ofstream(filename, std::ios::out);

    VERIFY_CONTRACT(file.good(),
        "Unable to open file \'%s\'\n",
        filename.c_str());

    // Dump matrix sizes:
    file << mat.size_y << " " << mat.size_y << std::endl;

    VERIFY_CONTRACT(file.good(),
        "Unable to dump matrix sizes to file \'%s\'\n",
        filename.c_str());

    // Dump matrix elements:
    for (size_t y = 0U; y < mat.size_y; ++y)
    {
        size_t x = 0U;
        for (; x + 1U < mat.size_x; ++x)
        {
            // Dump element (use space as delimiter):
            file << matrix_get(mat, y, x) << " ";

            VERIFY_CONTRACT(file.good(),
                "Unable to dump matrix element to file \'%s\'\n",
                filename.c_str());
        }

        // Dump last element into file:
        file << matrix_get(mat, y, x) << std::endl;

        VERIFY_CONTRACT(file.good(),
            "Unable to dump matrix element to file \'%s\'\n",
            filename.c_str());
    }

    // NOTE: C++ automatically closes opened file on function end.
    //       It is a major feature of C++ and will be discussed later.
}

// Matrix comparison:
constexpr double DOUBLE_PRECISION = 10e-9;

bool double_equal(double el1, double el2)
{
    return std::fabs(el1 - el2) < DOUBLE_PRECISION;
}

bool matrix_equal(const Matrix& mat1, const Matrix& mat2)
{
    VERIFY_CONTRACT(mat1.data != nullptr,
        "Invalid matrix data (possible double free)\n");
    VERIFY_CONTRACT(mat2.data != nullptr,
        "Invalid matrix data (possible double free)\n");

    // Compare pointers for obvious case of reflexivity:
    if (&mat1 == &mat2)
    {
        return true;
    }

    // Sizes of matricies must be equal:
    if (mat1.size_x != mat2.size_x ||
        mat1.size_y != mat2.size_y)
    {
        return false;
    }

    // All elements of matricies must be equal:
    for (size_t y = 0U; y < mat1.size_y; ++y)
    {
        for (size_t x = 0U; x < mat1.size_x; ++x)
        {
            double el1 = matrix_get(mat1, y, x);
            double el2 = matrix_get(mat2, y, x);

            if (!double_equal(el1, el2))
            {
                return false;
            }
        }
    }

    return true;
}
