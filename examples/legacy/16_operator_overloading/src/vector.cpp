// No copyright. Vladislav Alenik, 2024

#include <vector.hpp>

#include <cassert>
#include <cstring>
#include <cmath>

//----------------------
// Existance management
//----------------------

using namespace VectorArithmetics;

Vector VectorArithmetics::vector_create(size_t size)
{
    // NOTE: new throws exception std::bad_alloc on allocation error.
    double* data = new double[size];

    Vector to_return;
    to_return.data = data;
    to_return.size = size;

    return to_return;
}

void VectorArithmetics::vector_free(Vector& vec)
{
    VERIFY_CONTRACT(vec.data != nullptr,
        "Invalid vector data (possible double free)\n");

    delete[] vec.data;

    // Set vector data to NULL to detect double free:
    vec.data = nullptr;
}

//----------------
// Element access
//----------------

double& VectorArithmetics::Vector::operator[](const size_t index)
{
    VERIFY_CONTRACT(this->data != nullptr,
        "Invalid vector data (possible double free)\n");
    VERIFY_CONTRACT(index < this->size,
        "Index %zu out of bounds (size is %zu)\n",
        index, this->size);

    return this->data[index];
}

//-----------------------
// Arithmetic operations
//-----------------------

Vector VectorArithmetics::operator+(const Vector& that, const Vector& other)
{
    Vector to_return = vector_create(that.size);

    // NOTE: the implemenation of += performs all correctness checks.
    to_return += that;
    to_return += other;

    // NOTE: there is not actual copying in return statement thanks to:
    //       RVO (Return Value Optimization) and NRVO (Named RVO).
    return to_return;
}

Vector& VectorArithmetics::operator+=(Vector& that, const Vector& other)
{
    VERIFY_CONTRACT(that.data != nullptr,
        "Invalid vector data (possible double free for left argument)\n");
    VERIFY_CONTRACT(other.data != nullptr,
        "Invalid vector data (possible double free for right argument)\n");
    VERIFY_CONTRACT(that.size == other.size,
        "Unmatched operand sizes (%zu and %zu)\n",
        that.size, other.size);

    for (size_t i = 0U; i < that.size; ++i)
    {
        that.data[i] += other.data[i];
    }

    return that;
}

//-------------
// Comparisons
//-------------

constexpr double DOUBLE_PRECISION = 10e-9;

// NOTE: never (N.E.V.E.R.) overload operators of arithmetic types.
// PRANK TIP: you can do it somewhere in yor friend's code
//            and observe his/her frustration over it (0__0).
bool VectorArithmetics::double_equal(double el1, double el2)
{
    return std::fabs(el1 - el2) < DOUBLE_PRECISION;
}

bool VectorArithmetics::operator==(const Vector& that, const Vector& other)
{
    VERIFY_CONTRACT(that.data != nullptr,
        "Invalid vector data (possible double free for left argument)\n");
    VERIFY_CONTRACT(other.data != nullptr,
        "Invalid vector data (possible double free for right argument)\n");

    // Case of identical equality:
    if (&that == &other)
    {
        return true;
    }

    if (that.size != other.size)
    {
        return false;
    }

    for (size_t i = 0U; i < that.size; ++i)
    {
        if (!double_equal(that.data[i], other.data[i]))
        {
            return false;
        }
    }

    return true;
}

bool VectorArithmetics::operator!=(const Vector& that, const Vector& other)
{
    return !(that == other);
}
