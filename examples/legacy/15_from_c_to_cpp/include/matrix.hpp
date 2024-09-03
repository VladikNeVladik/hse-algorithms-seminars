// Copyright 2024 Vladislav Aleinik
//=======================================
// Representation of matrix
//=======================================
#ifndef HEADER_GUARD_MATRIX_HPP_INCLUDED
#define HEADER_GUARD_MATRIX_HPP_INCLUDED

#include <string>

// Structure to represent matrix:
struct Matrix
{
    double** data;
    size_t size_x;
    size_t size_y;
};

// Existance management:
Matrix matrix_create(size_t size_x, size_t size_y);
void matrix_free(Matrix& mat);

// Element access:
double matrix_get(const Matrix& mat, size_t y, size_t x);
void matrix_set(Matrix& mat, size_t y, size_t x, double element);

// Matrix parsing and printout:
Matrix matrix_scan_from_file(const std::string& filename);
void matrix_dump_to_file(const std::string& filename, const Matrix& mat);

// Matrix comparison:
bool double_equal(double el1, double el2);
bool matrix_equal(const Matrix& mat1, const Matrix& mat2);

#endif // HEADER_GUARD_MATRIX_HPP_INCLUDED
