// No copyright. Vladislav Aleinik, 2023
//=======================================
// Implementation of single-linked list
//=======================================
#ifndef HEADER_GUARD_LIST_H_INCLUDED
#define HEADER_GUARD_LIST_H_INCLUDED

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "utils.h"

//=====================
// List data structure
//=====================

struct NodeImpl {
    struct NodeImpl* next;
    Data_t data;
};

typedef struct NodeImpl Node;

typedef struct {
    // NOTE: dummy node does not store an element.
    Node dummy;
} List;

//======================
// Basic list utilities
//======================

// Initialize linked list.
void list_init(List* list)
{
    assert(list != NULL);

    // An empty list is represented as NULL node:
    list->dummy.next = NULL;
}

// Get list dummy node:
Node* list_dummy(List* list)
{
    assert(list != NULL);

    return &list->dummy;
}

// Get list head.
Node* list_head(List* list)
{
    assert(list != NULL);

    return list->dummy.next;
}

// Get next node.
Node* list_next(Node* node)
{
    return (node == NULL)? NULL : node->next;
}

// Free linked list:
void list_free(List* list)
{
    assert(list != NULL);

    // Iterate over a list:
    Node* node = list_head(list);
    while (node != NULL)
    {
        Node* next = list_next(node);

        // Free allocated list node:
        free(node);

        node = next;
    }
}

//==========================
// Element access utilities
//==========================

// Copy element stored in a linked list.
Data_t list_get(Node* node)
{
    assert(node != NULL);

    return node->data;
}

// Get pointer to element stored in a linked list.
Data_t* list_get_ptr(Node* node)
{
    assert(node != NULL);

    return &node->data;
}

// Insert data element right after selected (pivot) node.
// Return the newly created node.
Node* list_insert_after(Node* pivot, Data_t to_insert)
{
    assert(pivot != NULL);

    // Allocate new node in dynamic memory:
    Node* new = malloc(sizeof(Node));
    VERIFY_CONTRACT(new != NULL,
        "[list_insert_after] Not enough memory to allocate new list node\n");

    // Initialize new node:
    new->next = pivot->next;
    new->data = to_insert;

    // Update next node:
    pivot->next = new;

    // Return newly created node:
    return new;
}

bool list_remove_next(Node* pivot, Data_t* result)
{
    assert(pivot != NULL);

    // Get next node:
    Node* next = pivot->next;
    if (next == NULL)
    {
        return false;
    }

    // Unlink next node from the list:
    pivot->next = next->next;

    // Return result if requested:
    if (result != NULL)
    {
        *result = next->data;
    }

    // Free unused node:
    free(next);

    return true;
}


#endif // HEADER_GUARD_LIST_H_INCLUDED
