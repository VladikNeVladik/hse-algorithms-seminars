#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <stdio.h>
#include <string.h>

#include <time.h>

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

//==========================//
// String search algorithms //
//==========================//

char* naive_strstr(char* haystack, char* needle)
{
    // Compute lengths of both strings:
    size_t haystack_len = strlen(haystack);
    size_t needle_len   = strlen(needle);

    if (needle_len > haystack_len)
    {
        return NULL;
    }

    // Perform a naive search:
    char* cur = haystack;
    while (cur + needle_len <= haystack + haystack_len)
    {
        // Check current string for being a match:
        bool match = true;
        for (size_t index = 0U; index < needle_len; ++index)
        {
            if (cur[index] != needle[index])
            {
                match = false;
                break;
            }
        }

        // Return current match:
        if (match)
        {
            return cur;
        }

        // Move on to next possible match:
        cur += 1;
    }

    return NULL;
}

//===============//
// Main function //
//===============//

// Name of file to search in:
const char* HAYSTACK_FILENAME = "res/book-war-and-peace.txt";

int main(void)
{
    //--------------------//
    // Open haystack file //
    //--------------------//

    // Open haystack file:
    FILE* haystack_file = fopen(HAYSTACK_FILENAME, "r");
    VERIFY_CONTRACT(
        haystack_file != NULL,
        "Unable to open file: %s\n", HAYSTACK_FILENAME
    );

    // Measure file size:
    int ret = fseek(haystack_file, 0L, SEEK_END);
    VERIFY_CONTRACT(
        ret != -1,
        "Unable to find end of file: %s\n", HAYSTACK_FILENAME
    );

    long haystack_len = ftell(haystack_file);
    VERIFY_CONTRACT(
        haystack_len != -1,
        "Unable to measure size for file: %s\n", HAYSTACK_FILENAME
    );

    // Get back to starting position in file:
    ret = fseek(haystack_file, 0L, SEEK_SET);
    VERIFY_CONTRACT(
        ret != -1,
        "Unable to rewind to start of file: %s\n", HAYSTACK_FILENAME
    );

    //-------------------------//
    // Prepare haystack string //
    //-------------------------//

    // Allocate memory for the haystack:
    // NOTE: +1 is for the null terminator
    char* haystack = (char*) malloc(haystack_len + 1);
    VERIFY_CONTRACT(
        haystack != NULL,
        "Unable to allocate %ld bytes of memory\n", haystack_len
    );

    // Read string from file:
    size_t bytes_read = fread(haystack, 1U, haystack_len, haystack_file);
    VERIFY_CONTRACT(
        bytes_read == haystack_len,
        "Unable to read haystack from file (read %lu of %lu bytes)\n",
        bytes_read, haystack_len
    );

    // Place a null-terminator:
    haystack[haystack_len] = '\0';

    //-----------------------//
    // Prepare needle string //
    //-----------------------//

    printf("Enter needle:\n");

    // Allocate buffer for needle:
    // NOTE: +1 is for the null-terminator
    const size_t MAX_NEEDLE_SIZE = 1000;
    char needle[MAX_NEEDLE_SIZE + 1];

    // Read needle from stdin:
    char* ndl = fgets(needle, MAX_NEEDLE_SIZE + 1, stdin);
    VERIFY_CONTRACT(
        ndl != NULL,
        "Unable to read needle string\n"
    );

    // Replace the trailings newline (if present):
    char* newline = strchr(needle, '\n');
    if (newline != NULL)
    {
        *newline = '\0';
    }

    //--------------------------------------//
    // Find all occurences in War and Peace //
    //--------------------------------------//

    // Start time measurement:
    clock_t ticks_start = clock();

    size_t occurences = 0U;
    char* cur = haystack;
    while (true)
    {
        // Find next occurence of needle in haystack:
        cur = strstr(cur, needle);
        if (cur == NULL)
        {
            break;
        }

        // Prepare for serching for next occurence:
        cur += 1;
        occurences += 1U;
    }

    // Finish time measurement:
    clock_t ticks_end = clock();

    double ticks_delta = ticks_end - ticks_start;
    double seconds = ticks_delta / CLOCKS_PER_SEC;

    printf("Number of occurences: %lu\n", occurences);
    printf("Time: %10.6lfs\n", seconds);

    return EXIT_SUCCESS;
}
