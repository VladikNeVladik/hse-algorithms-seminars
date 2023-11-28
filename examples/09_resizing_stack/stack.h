// No copyright. Vladislav Aleinik, 2023
#ifndef HEADER_GUARD_STACK_H_INCLUDED
#define HEADER_GUARD_STACK_H_INCLUDED

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

//====================//
// Stack verification //
//====================//

// Verify stack internal structure.
// NOTE: this function is for internal use only.
bool stack_ok(struct Stack* stack)
{
    if (stack->capacity != 0 && stack->array == NULL)
    {
        return false;
    }

    if (stack->size > stack->capacity)
    {
        return false;
    }

    return true;
}

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

    VERIFY_CONTRACT(stack_ok(stack),
        "[stack_free] Unable to free an invalid stack (possible double free)\n");

    free(stack->array);

    // Mark stack array as invalid:
    // So that OK will raise error:
    stack->array    = NULL;
    stack->capacity = 0xAA00AA00U;
    stack->size     = 0xAAAAAAAAU;
}

// Perform a resize operation.
// NOTE: this function is for internal use only.
void stack_resize(struct Stack* stack, size_t new_capacity)
{
    assert(stack != NULL);

    VERIFY_CONTRACT(stack_ok(stack),
        "[stack_resize] Unable to resize an invalid stack\n");

    // Allocate new array:
    data_t* new_array = realloc(stack->array, new_capacity * sizeof(data_t));
    VERIFY_CONTRACT(
        new_capacity == 0U || new_array != NULL,
        "[stack_resize] Unable to reallocate memory for stack of new capacity %zu\n",
        new_capacity);

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

    VERIFY_CONTRACT(stack_ok(stack),
        "[stack_push] Unable to push to an invalid stack\n");

    // Resize to provide space for new element:
    if (stack->size == stack->capacity)
    {
        size_t new_capacity =
            (stack->size == 0U)? 1U : (2U * stack->capacity);

        stack_resize(stack, new_capacity);
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

    VERIFY_CONTRACT(stack_ok(stack),
        "[stack_push] Unable to pop from an invalid stack\n");

    // Do not pop from empty stack:
    if (stack->size == 0U)
    {
        return false;
    }

    // Copy element to specified memory location:
    *element = stack->array[stack->size - 1U];

    stack->size -= 1U;

    // Resize array down:
    if (stack->size <= stack->capacity / 4U)
    {
        size_t new_capacity = stack->capacity / 2U;

        stack_resize(stack, new_capacity);
    }

    return true;
}

#endif // HEADER_GUARD_STACK_H_INCLUDED
