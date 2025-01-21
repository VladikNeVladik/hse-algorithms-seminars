// Copyright 2025 Vladislav Aleinik
#ifndef HEADER_GUARD_VECTOR_HPP_INCLUDED
#define HEADER_GUARD_VECTOR_HPP_INCLUDED

#include <cstdlib>

namespace VectorArithmetics
{
    // Класс для представления вектора в N-мерном пространстве.
    class Vector
    {
    public:
        // Конструктор и деструктор.
        Vector(size_t size);
        ~Vector();

        // Доступ к элементам вектора.
        double& get(size_t index);

        // Арифметические операции.
        Vector& add(const Vector& that);
        Vector sum(const Vector& first);

        // Операции сравнения.
        bool equal(const Vector& that) const;
        bool not_equal(const Vector& that) const;

    private:
        // Поля класса.
        // Инвариант структуры данных:
        // - Указатель data_ указывает на выделенную область динмамической память размера size_.
        double* data_;
        size_t size_;
    };

    // Вспомогательная функция сравнения чисел с плавающей точкой.
    bool double_equal(double el1, double el2);

}; // namespace VectorArithmetic

#endif // HEADER_GUARD_VECTOR_HPP_INCLUDED
