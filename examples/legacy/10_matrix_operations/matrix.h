// Copyright 2024 Vladislav Aleinik
//=======================================
// Representation of matrix
//=======================================

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

#include "utils.h"

// Structure to represent matrix:
// NOTE: intoduce a typedef for simplified syntax.
typedef struct
{
    double** data;
    size_t size_x;
    size_t size_y;
} Matrix;

//----------------------
// Existance management
//----------------------

Matrix matrix_create(size_t size_x, size_t size_y)
{
    // Allocate array of rows:
    double** data = (double**) calloc(size_y, sizeof(*data));
    VERIFY_CONTRACT(data != NULL,
        "[matrix_create] Unable to allocate memory\n");

    // Allocate rows:
    for (size_t y = 0U; y < size_y; ++y)
    {
        data[y] = (double*) calloc(size_x, sizeof(*data[y]));
        VERIFY_CONTRACT(data[y] != NULL,
            "[matrix_create] Unable to allocate memory\n");
    }

    return (Matrix) {data, size_x, size_y};
}

void matrix_free(Matrix* mat)
{
    assert(mat != NULL);

    VERIFY_CONTRACT(mat->data != NULL,
        "[matrix_free] Invalid matrix data (possible double free)\n");

    // Free all the rows:
    for (size_t y = 0U; y < mat->size_y; ++y)
    {
        free(mat->data[y]);
    }

    // Free array of rows:
    free(mat->data);

    // Set matrix data to NULL to detect double free:
    mat->data = NULL;
}

//----------------
// Element access
//----------------

double matrix_get(const Matrix* mat, size_t y, size_t x)
{
    assert(mat != NULL);

    VERIFY_CONTRACT(mat->data != NULL,
        "[matrix_get] Invalid matrix data\n");
    VERIFY_CONTRACT(y < mat->size_y,
        "[matrix_get] Index y=%zu out of bounds (size_y is %zu)\n",
        y, mat->size_y);
    VERIFY_CONTRACT(x < mat->size_x,
        "[matrix_get] Index x=%zu out of bounds (size_x is %zu)\n",
        x, mat->size_x);

    return mat->data[y][x];
}

void matrix_set(const Matrix* mat, size_t y, size_t x, double element)
{
    assert(mat != NULL);

    VERIFY_CONTRACT(mat->data != NULL,
        "[matrix_set] Invalid matrix data (possible double free)\n");
    VERIFY_CONTRACT(y < mat->size_y,
        "[matrix_set] Index y=%zu out of bounds (size_y is %zu)\n",
        y, mat->size_y);
    VERIFY_CONTRACT(x < mat->size_x,
        "[matrix_set] Index x=%zu out of bounds (size_x is %zu)\n",
        x, mat->size_x);

    mat->data[y][x] = element;
}

//-----------------------------
// Matrix parsing and printout
//-----------------------------

Matrix matrix_scan_from_file(const char* filename)
{
    assert(filename != NULL);

    // Open file:
    FILE* file = fopen(filename, "r");
    VERIFY_CONTRACT(file != NULL,
        "[matrix_scan_from_file] Unable to open file \'%s\'\n",
        filename);

    // Scan matrix sizes:
    size_t size_y = 0U, size_x = 0U;
    int ret = fscanf(file, "%zu %zu\n", &size_y, &size_x);
    VERIFY_CONTRACT(ret == 2U,
        "[matrix_scan_from_file] Unable to parse matrix sizes\n");

    // Allocate zero matrix of specified size:
    Matrix to_return = matrix_create(size_y, size_x);

    // Parse matrix elements:
    for (size_t y = 0U; y < size_y; ++y)
    {
        for (size_t x = 0U; x < size_x; ++x)
        {
            double element = 0.0;
            int ret = fscanf(file, "%lf[ \n]", &element);
                VERIFY_CONTRACT(ret == 1U,
                    "[matrix_scan_from_file] Unable to parse matrix elements\n");

            matrix_set(&to_return, y, x, element);
        }
    }

    // Close opened file:
    ret = fclose(file);
    VERIFY_CONTRACT(ret != EOF,
        "[matrix_scan_from_file] Error on file close \'%s\'\n",
        filename);

    return to_return;
}

//-------------------
// Matrix operations
//-------------------

void matrix_dump_to_file(const char* filename, const Matrix* mat)
{
    assert(filename != NULL);
    assert(mat != NULL);

    // Open file:
    FILE* file = fopen(filename, "w");
    VERIFY_CONTRACT(file != NULL,
        "[matrix_dump_to_file] Unable to open file \'%s\'\n",
        filename);

    // Dump matrix sizes:
    int ret = fprintf(file, "%zu %zu\n", mat->size_y, mat->size_x);
    VERIFY_CONTRACT(ret > 0,
        "[matrix_dump_to_file] Unable to dump matrix sizes\n");

    // Dump matrix elements:
    for (size_t y = 0U; y < mat->size_y; ++y)
    {
        size_t x = 0U;
        for (; x + 1U < mat->size_x; ++x)
        {
            // Dump element (use space as delimiter):
            int ret = fprintf(file, "%lf ", matrix_get(mat, y, x));
            VERIFY_CONTRACT(ret > 0,
                "[matrix_dump_to_file] Unable to dump matrix elements\n");
        }

        int ret = fprintf(file, "%lf\n", matrix_get(mat, y, x));
        VERIFY_CONTRACT(ret > 0,
            "[matrix_dump_to_file] Unable to dump matrix elements\n");
    }

    // Close opened file:
    ret = fclose(file);
    VERIFY_CONTRACT(ret != EOF,
        "[matrix_dump_to_file] Error on file close \'%s\'\n",
        filename);
}

//-------------------
// Matrix comparison
//-------------------

#define DOUBLE_PRECISION 10e-9

bool double_equal(double el1, double el2)
{
    return fabs(el1 - el2) < DOUBLE_PRECISION;
}

bool matrix_equal(const Matrix* mat1, const Matrix* mat2)
{
    assert(mat1 != NULL);
    assert(mat2 != NULL);

    VERIFY_CONTRACT(mat1->data != NULL,
        "[matrix_equal] Invalid matrix data (possible double free)\n");
    VERIFY_CONTRACT(mat2->data != NULL,
        "[matrix_equal] Invalid matrix data (possible double free)\n");

    // Obvious case of reflexivity:
    if (mat1 == mat2)
    {
        return true;
    }

    // Sizes of matricies must be equal:
    if (mat1->size_x != mat2->size_x ||
        mat1->size_y != mat2->size_y)
    {
        return false;
    }

    // All elements of matricies must be equal:
    for (size_t y = 0U; y < mat1->size_y; ++y)
    {
        for (size_t x = 0U; x < mat1->size_x; ++x)
        {
            double el1 = matrix_get(mat1, y, x);
            double el2 = matrix_get(mat2, y, x);

            if (!double_equal(el1, el2))
            {
                return false;
            }
        }
    }

    return true;
}
