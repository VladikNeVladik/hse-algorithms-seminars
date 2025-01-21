// Copyright 2025 Vladislav Aleinik
#ifndef HEADER_GUARD_MATRIX_HPP_INCLUDED
#define HEADER_GUARD_MATRIX_HPP_INCLUDED

#include <string>
#include <vector.hpp>

namespace MatrixArithmetics
{
    using namespace VectorArithmetics;

    // Класс для представления двумерной матрицы NxM.
    class Matrix
    {
    public:
        // Конструктор и деструктор.
        Matrix(size_t size_x, size_t size_y);
        Matrix(const std::string& filename);
        ~Matrix();

        // Арифметические операции.
        Matrix& add(const Matrix& that);

        // Операции сравнения.
        bool equal(const Matrix& that) const;
        bool not_equal() const;

        // Запись матрицы в файл.
        Matrix& dump_to_file(const std::string& filename);

    private:
        Vector* data_;
        size_t size_x_;
        size_t size_y_;
    };

    Matrix sum(const Matrix& first, const Matrix& second);

}; // namespace MatrixArithmetics

#endif // HEADER_GUARD_MATRIX_HPP_INCLUDED
