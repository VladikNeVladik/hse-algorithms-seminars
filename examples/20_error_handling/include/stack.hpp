// Copyright 2025 Vladislav Aleinik
#ifndef HEADER_GUARD_STACK_H_INCLUDED
#define HEADER_GUARD_STACK_H_INCLUDED

#include <cstdlib>
#include <cstdint>
#include <cstdbool>
#include <initializer_list>

#include <utils.hpp>

// Стратегия реаллокации памяти при её нехватке
enum StackResizePolicy
{
    // При нехватке памяти выделяется память под один дополнительный элемент
    RESIZE_SUM = 0,
    // При нехватке памяти полный объём памяти удваивается
    RESIZE_MUL = 1
};

// Data_t - тип, параметризующий структуру данных.
typedef uint64_t Data_t;

class Stack
{
    public:
        // Конструкторы и деструктор.
        Stack(StackResizePolicy policy /* = RESIZE_SUM */);
        Stack(std::initializer_list<Data_t> list);
        Stack(const Stack& that);
        Stack(Stack&& that) noexcept;

        ~Stack() noexcept;

        // Копирование объектов.
        Stack& operator=(const Stack& that);
        Stack& operator=(Stack&& that) noexcept;

        // Добавление и удаление элементов.
        Stack& push(const Data_t& data);

        Data_t pop();

        // Признак пустоты стека.
        bool empty() const noexcept;

    private:
    // Проверка инварианта стека.
    void check() const;

    // Операция изменения размера выделенной памяти стека.
    Stack& resize(size_t new_capacity);

    // Массив с элементами стека
    Data_t* array_;
    // Количество элементов в стеке
    size_t size_;
    // Размер выделенной памяти (кол-во элементов типа Data_t)
    size_t capacity_;
    // Стратегия перевыделения памяти при её нехватке
    StackResizePolicy policy_;

    // Инвариант структуры данных:
    // - Указатель array_ указывает на валидную область памяти размера capacity_.
    // - size_ <= capacity_.
    // - 0 < capacity_.
    // - Элементы array_[0] .. array_[size_ - 1] - валидные элементы типа Data_t.
};

#endif // HEADER_GUARD_STACK_H_INCLUDED
