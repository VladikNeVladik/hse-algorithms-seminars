#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

//===============//
// Utility macro //
//===============//

#define VERIFY_CONTRACT(contract, format, ...) \
    do { \
        if (!(contract)) { \
            printf((format), ##__VA_ARGS__); \
            exit(EXIT_FAILURE); \
        } \
    } while (0)

//===================//
// String comparator //
//===================//

static int comp_lexicographic(const void* p1, const void* p2)
{
    const char* str1 = *(const char**)p1;
    const char* str2 = *(const char**)p2;

    return strcmp(str1, str2);
}

//=============================//
// Line sorting implementation //
//=============================//

const size_t MAX_LINES     = 100U;
const size_t MAX_LINE_SIZE = 100U;

// NOTE: "+1" accounts for a null-terminator.
const size_t MAX_LINE_SIZE_WITH_NULL = MAX_LINE_SIZE + 1U;

int main(void)
{
    // Buffer where input string are stored:
    // NOTE: is it optimal in case of memory usage?
    char buffer[MAX_LINES * MAX_LINE_SIZE_WITH_NULL];

    // In line array every item is a null-terminated string:
    // NOTE: memory is not auto-allocated for a 2D VLA.
    char* lines[MAX_LINES];
    for (size_t line_i = 0U; line_i < MAX_LINES; ++line_i)
    {
        lines[line_i] = &buffer[line_i * MAX_LINE_SIZE_WITH_NULL];
    }

    // Input array:
    size_t num_lines = 0U;
    for (size_t line_i = 0U; line_i < MAX_LINES; ++line_i, ++num_lines)
    {
        // Input line in a safe manner:
        char* ret = fgets(lines[line_i], MAX_LINE_SIZE_WITH_NULL, stdin);
        if (ret == NULL)
        {
            break;
        }

        if (!feof(stdin))
        {
            // Compute string length:
            // NOTE: can we get rid of length computation?
            size_t len = strlen(lines[line_i]);

            // Assert that length is correct.
            // NOTE: this is guaranteed by the algorithm and library functions.
            assert(0 < len && len <= MAX_LINE_SIZE);

            // Check that there is a newline:
            VERIFY_CONTRACT(lines[line_i][len - 1U] == '\n',
                "ERROR: Input line %zu exceeds allowed line width (%zu)",
                line_i, MAX_LINE_SIZE);

            // Do not use newline while sorting:
            lines[line_i][len - 1U] = '\0';
        }
    }

    // Sort input lines with specified comparator:
    qsort(lines, num_lines, sizeof(*lines), &comp_lexicographic);

    // Print lines in sorted order:
    for (size_t line_i = 0U; line_i < num_lines; ++line_i)
    {
        printf("%s\n", lines[line_i]);
    }

    return EXIT_SUCCESS;
}
