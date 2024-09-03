// Copyright 2024 Vladislav Aleinik
#include <vector.hpp>
#include <test_system.hpp>

#include <iostream>

constexpr size_t SIZE = 100U;

//--------------
// Test helpers
//--------------

void fill_vector(VectorArithmetics::Vector& vector)
{
    for (size_t i = 0U; i < SIZE; ++i)
    {
        vector[i] = i;
    }
}

//-------
// Tests
//-------

using namespace TestSystem;

bool test_constructor()
{
    using namespace VectorArithmetics;

    try
    {
        Vector vec1{0};
        Vector vec2{SIZE};
    }
    catch (const std::exception& exc)
    {
        return FAIL;
    }

    return OK;
}

bool test_copy_constructor()
{
    using namespace VectorArithmetics;

    Vector vec1{SIZE};
    fill_vector(vec1);

    Vector vec2{vec1};

    return (vec1 == vec2)? OK : FAIL;
}

bool test_move_constructor()
{
    using namespace VectorArithmetics;

    Vector vec1{SIZE};
    Vector vec2{SIZE};

    fill_vector(vec1);
    fill_vector(vec2);

    Vector vec3{std::move(vec1)};

    return (vec2 == vec3)? OK : FAIL;
}

bool test_copy_assignment()
{
    using namespace VectorArithmetics;

    Vector vec1{SIZE};
    fill_vector(vec1);

    Vector vec2{2*SIZE};

    vec2 = vec1;

    return (vec1 == vec2)? OK : FAIL;
}

bool test_move_assignment()
{
    using namespace VectorArithmetics;

    Vector vec1{SIZE};
    Vector vec2{SIZE};

    fill_vector(vec1);
    fill_vector(vec2);

    Vector vec3{2*SIZE};

    vec3 = std::move(vec1);

    return (vec2 == vec3)? OK : FAIL;
}

bool test_addition()
{
    using namespace VectorArithmetics;

    // Create vectors and fill them with elements:
    Vector vec2{SIZE};
    Vector vec1{SIZE};

    for (size_t i = 0U; i < SIZE; ++i)
    {
        // NOTE: implicit cast to double is performed.
        vec1[i] = i;
        vec2[i] = 2*i;
    }

    // Add two vectors together:
    vec1 = vec1 + vec1;

    return (vec1 != vec2)? FAIL : OK;
}

//--------------
// Test palette
//--------------

int main(void)
{
    run_test("constructor",      test_constructor);
    run_test("copy-constructor", test_copy_constructor);
    run_test("move-constructor", test_move_constructor);
    run_test("copy-assignment",  test_copy_assignment);
    run_test("move-assignment",  test_move_assignment);
    run_test("addition",         test_addition);

    return EXIT_SUCCESS;
}
