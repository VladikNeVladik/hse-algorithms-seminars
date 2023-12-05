// No copyright. Vladislav Aleinik, 2023

#include "tree.h"

#define NUM_INSERTED      100U
#define NUM_SEARCHED      10U
#define NUM_FULL_SEARCHES 10U

int main(void)
{
    // Initialize the search database:
    Tree search_db;
    tree_init(&search_db);

    // Create a little array of searched elements:
    key_t saved[NUM_SEARCHED];

    // Fill it with random data:
    for (size_t iteration_i = 0U; iteration_i < NUM_INSERTED; ++iteration_i)
    {
        // Generate random key and value:
        key_t key = rand();
        value_t value = key;

        // Add element to the data structure:
        tree_set(&search_db, key, value);

        // Possibly cache element:
        if (iteration_i + NUM_SEARCHED >= NUM_INSERTED)
        {
            saved[iteration_i + NUM_SEARCHED - NUM_INSERTED] = key;
        }
    }

    // Search for saved elements for a couple of times:
    for (size_t search_i = 0U; search_i < NUM_FULL_SEARCHES; ++search_i)
    {
        for (size_t saved_i = 0U; saved_i < NUM_SEARCHED; ++saved_i)
        {
            key_t search_key = saved[saved_i];
            value_t found_value;

            bool found = tree_search(&search_db, search_key, &found_value);
            TEST_CONDITION(found,
                "[TREE SEARCH] Unable to find an element\n");
            TEST_CONDITION(search_key == found_value,
                "[TREE SEARCH] Found unexpected value\n");
        }
    }

    // Remove saved elements from the tree:
    for (size_t saved_i = 0U; saved_i < NUM_SEARCHED; ++saved_i)
    {
        key_t search_key = saved[saved_i];
        value_t removed_value;

        bool removed = tree_remove(&search_db, search_key, &removed_value);
        TEST_CONDITION(removed,
            "[TREE DELETION] Element not found\n");
        TEST_CONDITION(search_key == removed_value,
            "[TREE DELETION] Found unexpected value\n");
    }

    // Ensure the search fails:
    for (size_t saved_i = 0U; saved_i < NUM_SEARCHED; ++saved_i)
    {
        key_t search_key = saved[saved_i];
        value_t found_value;

        bool found = tree_search(&search_db, search_key, &found_value);
        TEST_CONDITION(!found,
            "[TREE FAIL SEARCH] Found spurious element\n");
    }

    // Print the tree:
    tree_print(&search_db);

    return EXIT_SUCCESS;
}
