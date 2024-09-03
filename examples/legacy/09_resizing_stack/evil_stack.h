// Copyright 2024 Vladislav Aleinik
#ifndef HEADER_GUARD_EVIL_STACK_H_INCLUDED
#define HEADER_GUARD_EVIL_STACK_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "utils.h"

//================//
// Data structure //
//================//

struct Stack
{
    // Array for stack elements:
    data_t* array;

    // Number of elements stored in an array:
    size_t size;

    // Maximum possible size for allocated chunk of memory (measured in elements):
    size_t capacity;
};

//===================//
// Memory management //
//===================//

// Initialize stack to initial known state.
void stack_init(struct Stack* stack)
{
    assert(stack != NULL);

    // Initialize size and capacity:
    stack->size = 0U;
    stack->capacity = 1U;

    // Allocate stack of size one:
    stack->array = malloc(1U * sizeof(data_t));
    VERIFY_CONTRACT(
        stack->array != NULL,
        "[stack_init] Unable to allocate memory for stack of capacity %zu\n",
        stack->capacity);
}

// Deallocate stack memory:
void stack_free(struct Stack* stack)
{
    assert(stack != NULL);

    free(stack->array);
}


// Perform a resize operation.
// NOTE: this function is for internal use only.
void stack_resize(struct Stack* stack, size_t new_capacity)
{
    assert(stack != NULL);

    // Allocate new array:
    data_t* new_array = realloc(stack->array, new_capacity * sizeof(data_t));
    VERIFY_CONTRACT(
        new_array != NULL,
        "[stack_resize] Unable to allocate memory for stack of capacity %zu\n",
        stack->capacity);

    // Calculate resulting stack size:
    size_t new_size = MIN(stack->size, new_capacity);

    // Update stack data:
    stack->array    = new_array;
    stack->size     = new_size;
    stack->capacity = new_capacity;
}

//==================//
// Stack operations //
//==================//

// Push an element on top of the stack.
void stack_push(struct Stack* stack, data_t element)
{
    assert(stack != NULL);

    // Resize to provide space for new element:
    if (stack->size == stack->capacity)
    {
        stack_resize(stack, stack->capacity + 1U);
    }

    // Push element:
    stack->array[stack->size] = element;

    stack->size += 1U;
}

// Pop an element from the stack.
//
// Return true on success.
// Return false if there are no elements in the stack.
bool stack_pop(struct Stack* stack, data_t* element)
{
    assert(stack   != NULL);
    assert(element != NULL);

    // Do not pop from empty stack:
    if (stack->size == 0U)
    {
        return false;
    }

    // Copy element to specified memory location:
    *element = stack->array[stack->size - 1U];

    stack->size -= 1U;

    return true;
}

#endif // HEADER_GUARD_EVIL_STACK_H_INCLUDED
