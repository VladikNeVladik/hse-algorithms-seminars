// Copyright 2024 Vladislav Aleinik
#include <vector.hpp>
#include <test_system.hpp>

#include <iostream>
#include <type_traits>

//--------------
// Test helpers
//--------------

constexpr size_t SIZE = 100U;

template <typename Data_t>
void fill_vector(VectorArithmetics::Vector<Data_t>& vector)
{
    for (size_t i = 0U; i < SIZE; ++i)
    {
        vector[i] = i;
    }
}

//--------------------------
// Test for Vector template
//--------------------------

using namespace TestSystem;

template <typename Data_t>
bool test_constructor()
{
    using namespace VectorArithmetics;

    try
    {
        Vector<Data_t> vec1{0};
        Vector<Data_t> vec2{SIZE};
    }
    catch (const std::exception& exc)
    {
        return FAIL;
    }

    return OK;
}

template <typename Data_t>
bool test_copy_constructor()
{
    using namespace VectorArithmetics;

    Vector<Data_t> vec1{SIZE};
    fill_vector(vec1);

    Vector<Data_t> vec2{vec1};

    return (vec1 == vec2)? OK : FAIL;
}

template <typename Data_t>
bool test_move_constructor()
{
    using namespace VectorArithmetics;

    Vector<Data_t> vec1{SIZE};
    Vector<Data_t> vec2{SIZE};

    fill_vector(vec1);
    fill_vector(vec2);

    Vector<Data_t> vec3{std::move(vec1)};

    return (vec2 == vec3)? OK : FAIL;
}

template <typename Data_t>
bool test_copy_assignment()
{
    using namespace VectorArithmetics;

    Vector<Data_t> vec1{SIZE};
    fill_vector(vec1);

    Vector<Data_t> vec2{2*SIZE};

    vec2 = vec1;

    return (vec1 == vec2)? OK : FAIL;
}

template <typename Data_t>
bool test_move_assignment()
{
    using namespace VectorArithmetics;

    Vector<Data_t> vec1{SIZE};
    Vector<Data_t> vec2{SIZE};

    fill_vector(vec1);
    fill_vector(vec2);

    Vector<Data_t> vec3{2*SIZE};

    vec3 = std::move(vec1);

    return (vec2 == vec3)? OK : FAIL;
}

template <typename Data_t>
bool test_addition()
{
    using namespace VectorArithmetics;

    // Create vectors and fill them with elements:
    Vector<Data_t> vec1{SIZE};
    Vector<Data_t> vec2{SIZE};

    for (size_t i = 0U; i < SIZE; ++i)
    {
        // NOTE: implicit cast to Data_t is performed.
        vec1[i] = i;
        vec2[i] = 2*i;
    }

    // Add two vectors together:
    vec1 = vec1 + vec1;

    return (vec1 != vec2)? FAIL : OK;
}

template <typename Data_t>
bool test_multiplication()
{
    using namespace VectorArithmetics;

    // Create vectors and fill them with elements:
    Vector<Data_t> vec1{SIZE};
    Vector<Data_t> vec2{SIZE};

    for (size_t i = 0U; i < SIZE; ++i)
    {
        // NOTE: implicit cast to Data_t is performed.
        vec1[i] = i;
        vec2[i] = 2*i;
    }

    // Add two vectors together:
    vec1 = vec1 * 2;

    return (vec1 != vec2)? FAIL : OK;
}

//--------------
// Test palette
//--------------

int main(void)
{
    // "Correct" tests:
    printf("Testing Vector<double>:\n");
    run_test("double-constructor",      test_constructor<double>);
    run_test("double-copy-constructor", test_copy_constructor<double>);
    run_test("double-move-constructor", test_move_constructor<double>);
    run_test("double-copy-assignment",  test_copy_assignment<double>);
    run_test("double-move-assignment",  test_move_assignment<double>);
    run_test("double-addition",         test_addition<double>);
    run_test("double-multiplication",   test_multiplication<double>);
    printf("\n");

    printf("Testing Vector<int>:\n");
    run_test("int-constructor",      test_constructor<int>);
    run_test("int-copy-constructor", test_copy_constructor<int>);
    run_test("int-move-constructor", test_move_constructor<int>);
    run_test("int-copy-assignment",  test_copy_assignment<int>);
    run_test("int-move-assignment",  test_move_assignment<int>);
    run_test("int-addition",         test_addition<int>);
    run_test("int-multiplication",   test_multiplication<int>);
    printf("\n");

    // Wierd and incorrect template instantiations:
    printf("Testing Vector<std::string>:\n");
    run_test("string-constructor",      test_constructor<std::string>);
    run_test("string-copy-constructor", test_copy_constructor<std::string>);
    run_test("string-move-constructor", test_move_constructor<std::string>);
    run_test("string-copy-assignment",  test_copy_assignment<std::string>);
    run_test("string-move-assignment",  test_move_assignment<std::string>);

    // NOTE: why is it compiling?
    // NOTE: why is it failing?
    run_test("string-addition", test_addition<std::string>);

    // NOTE: why is it not compiling?
    // NOTE: is the template writer fault or the template user fault?
    // NOTE: how to improve compiler output message?
    // run_test("string-multiplication", test_multiplication<std::string>);

    return EXIT_SUCCESS;
}
