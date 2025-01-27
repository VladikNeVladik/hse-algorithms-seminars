// Copyright 2025 Vladislav Aleinik
#include <vector.hpp>

#include <utils.hpp>

#include <cstdlib>
#include <algorithm>
#include <cmath>

using Vector = VectorArithmetics::Vector;

//---------------------------
// Конструкторы и деструктор
//---------------------------

Vector::Vector() :
    data_ (nullptr),
    size_ (0)
{}

Vector::Vector(size_t size) :
    data_ (nullptr),
    size_ (size)
{
    // Оператор new бросает исключение std::bad_alloc при возникновении ошибки аллокации.
    data_ = new double[size];
}

Vector::Vector(const Vector& that) :
    data_ (nullptr),
    size_ (that.size_)
{
    // Аллоцируем новый массив нужного размера.
    data_ = new double[size_];

    // Производим глубокое копирование элементов.
    std::copy_n(that.data_, size_, data_);
}

Vector::~Vector()
{
    verify_contract(data_ != nullptr,
        "Invalid vector data (possible double free)\n");

    delete[] data_;

    // Выставляем значение указателя в NULL для детектирования повторного вызова деструктора.
    data_ = nullptr;
}

//-----------------------
// Оператор присваивания
//-----------------------

Vector& Vector::operator=(const Vector& that)
{
    if (this == &that)
    {
        // В случае присваивания самому себе никакие действия не требуются.
        return *this;
    }

    // Производим реаллокацию имеющегося диапозона памяти.
    if (data_ != nullptr)
    {
        delete[] data_;
    }

    data_ = new double[that.size_];

    // Копируем размер.
    size_ = that.size_;

    // Производим глубокое копирование элементов.
    std::copy_n(that.data_, size_, data_);

    return *this;
}

//----------------------------
// Доступ к элементам вектора
//----------------------------

double& Vector::operator[](size_t index)
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

Vector& Vector::operator+=(const Vector& that)
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

Vector VectorArithmetics::operator+(const Vector& first, const Vector& second)
{
    Vector copy{first};

    copy += second;

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

bool Vector::operator==(const Vector& that) const
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

bool Vector::operator!=(const Vector& that) const
{
    return !(*this == that);
}
