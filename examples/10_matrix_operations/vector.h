// No copyright. Vladislav Aleinik, 2023
//=================================================
// Representation of vector in N-dimensional space
//=================================================

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "utils.h"

// Structure to represent the vector in N-dimensional space:
struct Vector
{
    double* data;
    size_t size;
};

//----------------------
// Existance management
//----------------------

struct Vector vector_create(size_t size)
{
    double* data = (double*) calloc(size, sizeof(double));
    VERIFY_CONTRACT(data != NULL,
        "[vector_create] Unable to allocate memory\n");

    struct Vector to_return;
    to_return.data = data;
    to_return.size = size;

    return to_return;
}

void vector_free(struct Vector* vec)
{
    VERIFY_CONTRACT(vec->data != NULL,
        "[vector_free] Invalid vector data (possible double free)\n");

    free(vec->data);

    // Set vector data to NULL to detect double free:
    vec->data = NULL;
}

//----------------
// Element access
//----------------

double vector_get(const struct Vector* vec, size_t index)
{
    assert(vec != NULL);

    VERIFY_CONTRACT(vec->data != NULL,
        "[vector_get] Invalid vector data (possible double free)\n");
    VERIFY_CONTRACT(index < vec->size,
        "[vector_get] Index %zu out of bounds (size is %zu)\n",
        index, vec->size);

    return vec->data[index];
}

void vector_set(struct Vector* vec, size_t index, double element)
{
    assert(vec != NULL);

    VERIFY_CONTRACT(vec->data != NULL,
        "[vector_get] Invalid vector data\n");
    VERIFY_CONTRACT(index < vec->size,
        "[vector_get] Index %zu out of bounds (size is %zu)\n",
        index, vec->size);

    vec->data[index] = element;
}
