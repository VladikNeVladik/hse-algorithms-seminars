// Copyright 2024 Vladislav Aleinik
//=======================================
// Representation of vector
//=======================================
#ifndef HEADER_GUARD_VECTOR_HPP_INCLUDED
#define HEADER_GUARD_VECTOR_HPP_INCLUDED

#include <cstdlib>

#include <utils.hpp>

// Structure to represent the vector in N-dimensional space:
struct Vector
{
    double* data;
    size_t size;
};

// Existance management:
Vector vector_create(size_t size);
void vector_free(Vector& vec);

// Element access:
double vector_get(const Vector& vec, size_t index);
void vector_set(Vector& vec, size_t index, double element);

#endif // HEADER_GUARD_VECTOR_HPP_INCLUDED
