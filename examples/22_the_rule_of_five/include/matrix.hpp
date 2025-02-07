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
        // Конструкторы и деструктор.
        Matrix() = delete;
        Matrix(size_t size_x, size_t size_y);
        Matrix(const std::string& filename);

        Matrix(const Matrix& that);
        Matrix(Matrix&& that);

        ~Matrix();

        // Копирование объектов.
        Matrix& operator=(const Matrix& that);
        Matrix& operator=(Matrix&& that);

        // Доступ к элементам матрицы.
        Vector& operator[](size_t index);

        // Арифметические операции.
        Matrix& operator+=(const Matrix& that);

        // Операции сравнения.
        bool operator==(const Matrix& that) const;
        bool operator!=(const Matrix& that) const;

        // Запись матрицы в файл.
        Matrix& dump_to_file(const std::string& filename);

    private:
        Vector* data_;
        size_t size_x_;
        size_t size_y_;
    };

    Matrix operator+(const Matrix& first, const Matrix& second);

}; // namespace MatrixArithmetics

#endif // HEADER_GUARD_MATRIX_HPP_INCLUDED
