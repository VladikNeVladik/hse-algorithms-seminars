// Copyright 2024 Vladislav Aleinik
#include <vector.hpp>
#include <test_system.hpp>

#include <iostream>

#include <memory>

constexpr size_t SIZE = 100U;

//-------------------------
// Вспомогательная функция
//-------------------------

void fill_vector(VectorArithmetics::Vector& vector)
{
    for (size_t i = 0U; i < SIZE; ++i)
    {
        vector[i] = i;
    }
}

//----------------------------
// Тестовый набор для вектора
//----------------------------

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

    // Создаём векторы и заполняем их элементами.
    Vector vec2{SIZE};
    Vector vec1{SIZE};

    for (size_t i = 0U; i < SIZE; ++i)
    {
        vec1[i] = i;
        vec2[i] = 2*i;
    }

    // Складываем два векора вместе.
    vec1 = vec1 + vec1;

    return (vec1 != vec2)? FAIL : OK;
}

//-----------------------------------------------------
// Демонстрационный тестовый набор для std::uniqie_ptr
//-----------------------------------------------------

bool test_unique_ptr_copy()
{
    using namespace VectorArithmetics;

    std::unique_ptr<Vector> first = std::make_unique<Vector>(SIZE);

    // Извлекаем из std::unique_ptr значение по указателю.
    fill_vector(*first);

    // Создаём и заполняем копию вектора.
    Vector vec1{SIZE};
    fill_vector(vec1);

    // Производим перемещение указателя.
    std::unique_ptr<Vector> copy = std::move(first);

    return copy.get() != nullptr && first.get() == nullptr;
}

bool test_shared_ptr_copy()
{
    using namespace VectorArithmetics;

    std::shared_ptr<Vector> first = std::make_shared<Vector>(SIZE);

    // Извлекаем из std::unique_ptr значение по указателю.
    fill_vector(*first);

    // Создаём и заполняем копию вектора.
    Vector vec1{SIZE};
    fill_vector(vec1);

    // Производим перемещение указателя.
    std::shared_ptr<Vector> copy = first;

    return *first == vec1 && *first == *copy;
}

//-----------------
// Тестовые наборы
//-----------------

int main(void)
{
    printf("Test Vector operations\n");
    run_test("constructor",      test_constructor);
    run_test("copy-constructor", test_copy_constructor);
    run_test("move-constructor", test_move_constructor);
    run_test("copy-assignment",  test_copy_assignment);
    run_test("move-assignment",  test_move_assignment);
    run_test("addition",         test_addition);

    printf("Test smart pointer operations\n");
    run_test("unique-ptr-copy", test_unique_ptr_copy);
    run_test("shared-ptr-copy", test_shared_ptr_copy);

    return EXIT_SUCCESS;
}
