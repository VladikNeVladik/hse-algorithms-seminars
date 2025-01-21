// Copyright 2025 Vladislav Aleinik
#include <cassert>
#include <cstring>
#include <cmath>

#include <vector.hpp>

#include <utils.hpp>

using Vector = VectorArithmetics::Vector;

//--------------------------
// Конструктор и деструктор
//--------------------------

Vector::Vector(size_t size) :
    data_ (nullptr),
    size_ (size)
{
    // Оператор new бросает исключение std::bad_alloc при возникновении ошибки аллокации.
    data_ = new double[size];
}

Vector::~Vector()
{
    verify_contract(data_ != nullptr,
        "Invalid vector data (possible double free)\n");

    delete[] data_;

    // Set vector data to NULL to detect double free:
    data_ = nullptr;
}

//----------------------------
// Доступ к элементам вектора
//----------------------------

double& Vector::get(size_t index)
{
    verify_contract(data_ != nullptr,
        "Invalid vector data (use-after-free)\n");
    verify_contract(index < size_,
        "Index %zu out of bounds (size is %zu)\n",
        index, size_);

    return data_[index];
}

//-------------------------
// Арифметические операции
//-------------------------

Vector& Vector::add(const Vector& that)
{
    verify_contract(data_ != nullptr,
        "Invalid vector data (possible double free for left argument)\n");
    verify_contract(that.data_ != nullptr,
        "Invalid vector data (possible double free for right argument)\n");
    verify_contract(size_ == that.size_,
        "Unmatched operand sizes (%zu and %zu)\n",
        size_, that.size_);

    for (size_t i = 0U; i < size_; ++i)
    {
        data_[i] += that.data_[i];
    }

    return *this;
}

Vector Vector::sum(const Vector& that)
{
    Vector copy = *this;

    // Примечание: реализация += производит все необходимые проверки.
    copy.add(that);

    // В C++ существуют оптимизации RVO (Return Value Optimization) и NRVO (Named RVO),
    // оптимизирующие копирование возвращаемого значения функции.
    return copy;
}

//--------------------
// Операции сравнения
//--------------------

// Константа времени компиляции, содержащая точность вычислений.
constexpr double DOUBLE_PRECISION = 10e-9;

// Вспомогательная функция для сравнения двух чисел типа double.
bool VectorArithmetics::double_equal(double el1, double el2)
{
    return std::fabs(el1 - el2) < DOUBLE_PRECISION;
}

bool Vector::equal(const Vector& that) const
{
    verify_contract(data_ != nullptr,
        "Invalid vector data (possible double free for left argument)\n");
    verify_contract(that.data_ != nullptr,
        "Invalid vector data (possible double free for right argument)\n");

    // В случае совпадающих объектов проводить полное сравнение не имеет смысла.
    if (this == &that)
    {
        return true;
    }

    // Равенство определено для векторов из одного векторного пространства.
    if (size_ != that.size_)
    {
        return false;
    }

    for (size_t i = 0U; i < size_; ++i)
    {
        if (!double_equal(data_[i], that.data_[i]))
        {
            return false;
        }
    }

    return true;
}

bool Vector::not_equal(const Vector& that) const
{
    return !equal(that);
}
