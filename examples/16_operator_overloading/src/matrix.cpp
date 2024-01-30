// No copyright. Vladislav Aleinik, 2024

#include <matrix.hpp>

#include <iostream>
#include <fstream>
#include <cmath>

#include <utils.hpp>

using namespace MatrixArithmetics;

//----------------------
// Existance management
//----------------------

Matrix MatrixArithmetics::matrix_create(size_t size_x, size_t size_y)
{
    // Allocate 2D-array.
    // NOTE: new throws exception std::bad_alloc on allocation error.
    Vector* data = new Vector[size_y];

    for (size_t y = 0U; y < size_y; ++y)
    {
        // NOTE: this may throw exception std::bad_alloc on allocation error.
        data[y] = vector_create(size_x);
    }

    Matrix to_return = {data, size_x, size_y};

    return to_return;
}

void MatrixArithmetics::matrix_free(Matrix& mat)
{
    VERIFY_CONTRACT(mat.data != nullptr,
        "Invalid matrix data (possible double free)\n");

    // Free all the rows:
    for (size_t y = 0U; y < mat.size_y; ++y)
    {
        vector_free(mat.data[y]);
    }

    // Free array of rows:
    delete[] mat.data;

    // Set matrix data to NULL to detect double free:
    mat.data = nullptr;
}

//----------------
// Element access
//----------------

Vector& Matrix::operator[](const size_t index_y)
{
    VERIFY_CONTRACT(this->data != nullptr,
        "Invalid matrix data\n");
    VERIFY_CONTRACT(index_y < this->size_y,
        "Index y=%zu out of bounds (size_y is %zu)\n",
        index_y, this->size_y);

    return this->data[index_y];
}

//-----------------------
// Arithmetic operations
//-----------------------

Matrix MatrixArithmetics::operator+(const Matrix& that, const Matrix& other)
{
    Matrix to_return = matrix_create(that.size_x, that.size_y);

    // NOTE: the implemenation of += performs all correctness checks.
    to_return += that;
    to_return += other;

    // NOTE: there is not actual copying in return statement thanks to:
    //       RVO (Return Value Optimization) and NRVO (Named RVO).
    return to_return;
}

Matrix& MatrixArithmetics::operator+=(Matrix& that, const Matrix& other)
{
    VERIFY_CONTRACT(that.data != nullptr,
        "Invalid matrix data (possible double free for left argument)\n");
    VERIFY_CONTRACT(other.data != nullptr,
        "Invalid matrix data (possible double free for right argument)\n");
    VERIFY_CONTRACT(that.size_x == other.size_x,
        "Unmatched operand X-sizes (%zu and %zu)\n",
        that.size_x, other.size_x);
    VERIFY_CONTRACT(that.size_y == other.size_y,
        "Unmatched operand Y-sizes (%zu and %zu)\n",
        that.size_y, other.size_y);

    for (size_t y = 0U; y < that.size_y; ++y)
    {
        that.data[y] += other.data[y];
    }

    return that;
}

//-------------------
// Matrix comparison
//-------------------

bool MatrixArithmetics::operator==(const Matrix& that, const Matrix& other)
{
    VERIFY_CONTRACT(that.data != nullptr,
        "Invalid matrix data (possible double free for left argument)\n");
    VERIFY_CONTRACT(other.data != nullptr,
        "Invalid matrix data (possible double free for right argument)\n");

    // Case of identical equality:
    if (&that == &other)
    {
        return true;
    }

    if (that.size_y != other.size_y ||
        that.size_x != other.size_x)
    {
        return false;
    }

    for (size_t y = 0U; y < that.size_y; ++y)
    {
        if (that.data[y] != other.data[y])
        {
            return false;
        }
    }

    return true;
}

bool MatrixArithmetics::operator!=(const Matrix& that, const Matrix& other)
{
    return !(that == other);
}

//-----------------------------
// Matrix parsing and printout
//-----------------------------

Matrix MatrixArithmetics::matrix_scan_from_file(const std::string& filename)
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

            to_return[y][x] = element;
        }
    }

    // NOTE: C++ automatically closes opened file on function end.
    //       It is a major feature of C++ and will be discussed later.

    return to_return;
}

void MatrixArithmetics::matrix_dump_to_file(const std::string& filename, Matrix& mat)
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
            file << mat[y][x] << " ";

            VERIFY_CONTRACT(file.good(),
                "Unable to dump matrix element to file \'%s\'\n",
                filename.c_str());
        }

        // Dump last element into file:
        file << mat[y][x] << std::endl;

        VERIFY_CONTRACT(file.good(),
            "Unable to dump matrix element to file \'%s\'\n",
            filename.c_str());
    }

    // NOTE: C++ automatically closes opened file on function end.
    //       It is a major feature of C++ and will be discussed later.
}
