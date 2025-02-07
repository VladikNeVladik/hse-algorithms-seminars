// Copyright 2025 Vladislav Aleinik
#ifndef HEADER_GUARD_VECTOR_HPP_INCLUDED
#define HEADER_GUARD_VECTOR_HPP_INCLUDED

#include <cstdlib>

// Производим предварительную декларацию класса Matrix.
namespace MatrixArithmetics
{
    class Matrix;
}; // namespace MatrixArithmetics

namespace VectorArithmetics
{
    // Класс для представления вектора в N-мерном пространстве.
    class Vector
    {
    public:
        // Конструкторы и деструктор.
        Vector(size_t size);
        Vector(const Vector& that);
        Vector(Vector&& that);
        ~Vector();

        // Копирование объектов.
        Vector& operator=(const Vector& that);
        Vector& operator=(Vector&& that);

        // Доступ к элементам вектора.
        double& operator[](size_t index);

        // Арифметические операции.
        Vector& operator+=(const Vector& that);

        // Операции сравнения.
        bool operator==(const Vector& that) const;
        bool operator!=(const Vector& that) const;

    private:
        // Обозначаем конструктор без аргмуентов как приватный.
        Vector();
        // Для доступа к нему деларируем friend-класс.
        friend class MatrixArithmetics::Matrix;

        // Поля класса.
        // Инвариант структуры данных:
        // - Указатель data_ указывает на выделенную область динмамической память размера size_.
        double* data_;
        size_t size_;
    };

    Vector operator+(const Vector& first, const Vector& second);

    // Вспомогательная функция сравнения чисел с плавающей точкой.
    bool double_equal(double el1, double el2);

}; // namespace VectorArithmetic

#endif // HEADER_GUARD_VECTOR_HPP_INCLUDED
