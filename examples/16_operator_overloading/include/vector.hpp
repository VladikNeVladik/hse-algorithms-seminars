// No copyright. Vladislav Aleinik, 2024
//=======================================
// Representation of vector
//=======================================
#ifndef HEADER_GUARD_VECTOR_HPP_INCLUDED
#define HEADER_GUARD_VECTOR_HPP_INCLUDED

#include <cstdlib>

#include <utils.hpp>

namespace VectorArithmetics
{
    // Structure to represent the vector in N-dimensional space:
    struct Vector
    {
        // Element access:
        double& operator[](const size_t index);

        // Public variables:
        double* data;
        size_t size;
    };

    // Existance management:
    Vector vector_create(size_t size);
    void vector_free(Vector& vec);

    // Arithmetic operations:
    Vector operator+(const Vector& that, const Vector& other);
    Vector& operator+=(Vector& that, const Vector& other);

    // Comparisons:
    bool double_equal(double el1, double el2);
    bool operator==(const Vector& that, const Vector& other);
    bool operator!=(const Vector& that, const Vector& other);

}; // namespace VectorArithmetic

#endif // HEADER_GUARD_VECTOR_HPP_INCLUDED
