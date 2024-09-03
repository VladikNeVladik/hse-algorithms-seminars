// Copyright 2024 Vladislav Aleinik
//=======================================
// Representation of matrix
//=======================================
#ifndef HEADER_GUARD_MATRIX_HPP_INCLUDED
#define HEADER_GUARD_MATRIX_HPP_INCLUDED

#include <string>
#include <vector.hpp>

namespace MatrixArithmetics
{
    using namespace VectorArithmetics;

    // Structure to represent matrix:
    struct Matrix
    {
        // Element access:
        Vector& operator[](const size_t index_y);

        // Public variables:
        Vector* data;
        size_t size_x;
        size_t size_y;
    };

    // Existance management:
    Matrix matrix_create(size_t size_x, size_t size_y);
    void matrix_free(Matrix& mat);

    // Arithmetic operations:
    Matrix operator+(const Matrix& that, const Matrix& other);
    Matrix& operator+=(Matrix& that, const Matrix& other);

    // Matrix comparison:
    bool operator==(const Matrix& that, const Matrix& other);
    bool operator!=(const Matrix& that, const Matrix& other);

    // Matrix parsing and printout:
    Matrix matrix_scan_from_file(const std::string& filename);
    void matrix_dump_to_file(const std::string& filename, Matrix& mat);

}; // namespace MatrixArithmetics

#endif // HEADER_GUARD_MATRIX_HPP_INCLUDED
