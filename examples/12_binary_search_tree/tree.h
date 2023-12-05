// No copyright. Vladislav Aleinik, 2023
//=======================================
// Implementation of a binary tree
//=======================================
#ifndef HEADER_GUARD_TREE_H_INCLUDED
#define HEADER_GUARD_TREE_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include "utils.h"

//=====================
// Tree data structure
//=====================

typedef int key_t;
typedef int value_t;
typedef uint32_t node_t;

#define NULL_NODE ((node_t) 0xFFFFFFFFU)

typedef struct {
    node_t parent_i;
    node_t lchild_i;
    node_t rchild_i;
    key_t key;
    value_t value;
} TreeNode;

typedef struct {
    // Dynamic array of nodes:
    TreeNode* nodes;
    size_t size;
    size_t capacity;

    // Tree root:
    node_t root_i;
} Tree;

//===========================
// Tree low-level operations
//===========================

// Initialize the tree.
void tree_init(Tree* tree)
{
    assert(tree);

    // Initialize the tree:
    tree->size     = 0U;
    tree->capacity = 1U;

    tree->nodes = calloc(tree->capacity, sizeof(TreeNode));
    VERIFY_CONTRACT(tree->nodes != NULL,
        "[tree_init] Not enough memory to initialize a tree\n");

    tree->root_i = NULL_NODE;
}

// Free allocated dynamic memory.
void tree_free(Tree* tree)
{
    assert(tree);

    free(tree->nodes);
    tree->nodes = NULL;
}

// Get tree node by it's id:
TreeNode* tree_get(Tree* tree, node_t node_i)
{
    assert(tree);
    assert(node_i != NULL_NODE);

    return &tree->nodes[node_i];
}

// Replace subtree "transplanted" with subtree "new".
//
// NOTE: for description of the algorithm check CLRS 12.3.
void tree_transplant(Tree* tree, node_t transplanted_i, node_t new_i)
{
    TreeNode* transplanted = tree_get(tree, transplanted_i);

    if (transplanted->parent_i == NULL_NODE)
    {
        tree->root_i = new_i;
    }
    else
    {
        TreeNode* parent = tree_get(tree, transplanted->parent_i);
        if (transplanted_i == parent->lchild_i)
        {
            parent->lchild_i = new_i;
        }
        else
        {
            parent->rchild_i = new_i;
        }
    }

    if (new_i != NULL_NODE)
    {
        TreeNode* new = tree_get(tree, new_i);
        new->parent_i = transplanted->parent_i;
    }
}

node_t tree_node_allocate(Tree* tree)
{
    assert(tree);

    // Possibly resize the storage size:
    if (tree->size == tree->capacity)
    {
        // Allocate new storage:
        size_t new_capacity =
            (tree->size == 0U)? 1U : (2U * tree->capacity);

        tree->nodes = realloc(tree->nodes, new_capacity * sizeof(TreeNode));
        VERIFY_CONTRACT(tree->nodes != NULL,
            "[tree_node_allocate] Unable to allocate tree node\n");

        // Update tree storage capacity:
        tree->capacity = new_capacity;
    }

    // Use first free element as newly allocated:
    node_t allocated_i = tree->size;
    tree->size += 1U;

    // Zero intialize links to parent/childer:
    TreeNode* allocated = tree_get(tree, allocated_i);

    allocated->parent_i = NULL_NODE;
    allocated->lchild_i = NULL_NODE;
    allocated->rchild_i = NULL_NODE;

    return allocated_i;
}

void tree_node_free(Tree* tree, node_t freed_i)
{
    assert(tree);

    // Push the node to the free list:
    TreeNode* freed = tree_get(tree, freed_i);

    // Unlink the node from everywhere:
    freed->parent_i = NULL_NODE;
    freed->lchild_i = NULL_NODE;
    freed->rchild_i = NULL_NODE;

    // Ensure the array has no holes:
    node_t last_i = tree->size - 1U;
    if (freed_i < tree->size - 1U)
    {
        // Copy rightmost element to the hole:
        TreeNode* last = tree_get(tree, last_i);

        *freed = *last;

        tree_transplant(tree, last_i, freed_i);
    }

    // Decrease tree size:
    tree->size -= 1U;

}

// Search for an element in the tree.
//
// Return the node with matching key.
// Also "return" it's parent node.
node_t tree_search_node(Tree* tree, key_t search_key, node_t* parent_i)
{
    assert(tree);
    assert(parent_i != NULL);

    // Mark root's parent as NULL:
    *parent_i = NULL_NODE;

    // Perform tree walk:
    node_t cur_i = tree->root_i;
    while (cur_i != NULL_NODE)
    {
        TreeNode* node = tree_get(tree, cur_i);

        // Returen the element if it's key matches:
        if (search_key == node->key)
        {
            return cur_i;
        }

        // Search in either left or right subtree:
        *parent_i = cur_i;
        if (search_key < node->key)
        {
            cur_i = node->lchild_i;
        }
        else
        {
            cur_i = node->rchild_i;
        }
    }

    // Return NULL pointer to mark
    return NULL_NODE;
}

// Find minimum element of a subtree.
//
// Assume that
// NOTE: for description of the algorithm check CLRS 12.3.
node_t tree_minimum(Tree* tree, node_t subtree_i)
{
    assert(subtree_i != NULL_NODE);

    TreeNode* subtree = tree_get(tree, subtree_i);
    while (subtree->lchild_i != NULL_NODE)
    {
        subtree_i = subtree->lchild_i;
        subtree = tree_get(tree, subtree_i);
    }

    return subtree_i;
}

#define MAX_PRINT_DEPTH 40U

// Pretty print the tree.
void tree_print_recursive(Tree* tree, node_t node_i, size_t level, int state[MAX_PRINT_DEPTH])
{
    VERIFY_CONTRACT(level < MAX_PRINT_DEPTH,
        "[tree_print_recursive] Exceeded maximum print depth\n");

    for (size_t lvl = 0U; lvl + 1U < level; lvl++)
    {
        if (state[lvl] == 1)
        {
            printf("│  ");
        }
        else
        {
            printf("   ");
        }
    }
    if (level != 0U)
    {
        if (state[level - 1U] == 2)
        {
            printf("└──");
        }
        else
        {
            printf("├──");
        }
    }

    if (node_i == NULL_NODE)
    {
        printf("[NULL]\n");
    }
    else
    {
        TreeNode* node = tree_get(tree, node_i);

        printf("[%08x]\n", node->key);

        state[level] = 1;
        tree_print_recursive(tree, node->lchild_i, level + 1U, state);
        state[level] = 2;
        tree_print_recursive(tree, node->rchild_i, level + 1U, state);
        state[level] = 0;
    }
}

//=====================
// Tree user interface
//=====================

// Get value by key.
bool tree_search(Tree* tree, key_t key, value_t* res)
{
    assert(tree);
    assert(res);

    node_t parent_i = NULL_NODE;
    node_t found_i = tree_search_node(tree, key, &parent_i);
    if (found_i == NULL_NODE)
    {
        return false;
    }

    TreeNode* found = tree_get(tree, found_i);
    *res = found->value;

    return true;
}

// Set value by key.
void tree_set(Tree* tree, key_t key, value_t value)
{
    // Search by the key:
    node_t parent_i = NULL_NODE;
    node_t found_i = tree_search_node(tree, key, &parent_i);

    // Update value of already existing node:
    if (found_i != NULL_NODE)
    {
        TreeNode* found = tree_get(tree, found_i);
        found->value = value;

        return;
    }

    // Allocate a new node for non-existent key:
    node_t allocated_i = tree_node_allocate(tree);

    // Set parentship for newly allocated node:
    // Handle case of root node:
    if (tree->root_i == NULL_NODE)
    {
        tree->root_i = allocated_i;
    }
    // Handle internal or leaf node:
    else
    {
        TreeNode* parent = tree_get(tree, parent_i);
        if (key < parent->key)
        {
            parent->lchild_i = allocated_i;
        }
        else if (parent->key < key)
        {
            parent->rchild_i = allocated_i;
        }
    }

    // Initialize allocated node:
    TreeNode* allocated = tree_get(tree, allocated_i);

    allocated->parent_i = parent_i;
    allocated->key      = key;
    allocated->value    = value;
}

// Remove element by it's key.
//
// NOTE: for description of the algorithm check CLRS 12.3.
bool tree_remove(Tree* tree, key_t key, value_t* ret)
{
    // Search by the key:
    node_t selected_parent_i = NULL_NODE;
    node_t selected_i = tree_search_node(tree, key, &selected_parent_i);

    // Return if no key is found:
    if (selected_i == NULL_NODE)
    {
        return false;
    }

    TreeNode* selected = tree_get(tree, selected_i);

    // Handle the simple case of single child:
    if (selected->lchild_i == NULL_NODE)
    {
        tree_transplant(tree, selected_i, selected->rchild_i);
    }

    // Handle the symetric simple case:
    else if (selected->rchild_i == NULL_NODE)
    {
        tree_transplant(tree, selected_i, selected->rchild_i);
    }

    // Handle the most non-trivial case:
    else
    {
        node_t minimum_i  = tree_minimum(tree, selected->rchild_i);
        TreeNode* minimum = tree_get(tree, minimum_i);

        if (minimum->parent_i != selected_i)
        {
            tree_transplant(tree, minimum_i, minimum->rchild_i);

            minimum->rchild_i = selected->rchild_i;
            TreeNode* right = tree_get(tree, minimum->rchild_i);
            right->parent_i = minimum_i;
        }

        tree_transplant(tree, selected_i, minimum_i);

        minimum->lchild_i = selected->lchild_i;
        TreeNode* left = tree_get(tree, minimum->lchild_i);
        left->parent_i = minimum_i;
    }

    // Free the selected node and return it's value:
    *ret = selected->value;

    tree_node_free(tree, selected_i);

    return true;
}

void tree_print(Tree* tree)
{
    int state[MAX_PRINT_DEPTH] = {};

    node_t root_i = tree->root_i;
    tree_print_recursive(tree, root_i, 0U, state);
}

#endif // HEADER_GUARD_TREE_H_INCLUDED
