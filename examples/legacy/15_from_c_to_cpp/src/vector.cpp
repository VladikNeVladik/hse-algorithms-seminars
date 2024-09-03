// No copyright. Vladislav Alenik, 2024

#include <vector.hpp>

#include <cassert>

// Existance management:
Vector vector_create(size_t size)
{
    // NOTE: new throws exception std::bad_alloc on allocation error.
    double* data = new double[size];

    Vector to_return;
    to_return.data = data;
    to_return.size = size;

    return to_return;
}

void vector_free(Vector& vec)
{
    VERIFY_CONTRACT(vec.data != nullptr,
        "Invalid vector data (possible double free)\n");

    delete vec.data;

    // Set vector data to NULL to detect double free:
    vec.data = nullptr;
}

// Element access:
double vector_get(const Vector& vec, size_t index)
{
    VERIFY_CONTRACT(vec.data != nullptr,
        "Invalid vector data (possible double free)\n");
    VERIFY_CONTRACT(index < vec.size,
        "Index %zu out of bounds (size is %zu)\n",
        index, vec.size);

    return vec.data[index];
}

void vector_set(Vector& vec, size_t index, double element)
{
    VERIFY_CONTRACT(vec.data != nullptr,
        "Invalid vector data\n");
    VERIFY_CONTRACT(index < vec.size,
        "Index %zu out of bounds (size is %zu)\n",
        index, vec.size);

    vec.data[index] = element;
}
