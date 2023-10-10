#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

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

//==============//
// String input //
//==============//

#define MAX_ARRAY_SIZE 40U

int main(void)
{
    char input_str[MAX_ARRAY_SIZE];

    // NOTE: if string bigger then MAX_ARRAY_SIZE is entered,
    //       the buffer is overflown (Undefined Bahavior);
    int ret = scanf("%s", input_str);
    VERIFY_CONTRACT(ret == 1, "ERROR: expected string\n");

    printf("Entered \"%s\"\n", input_str);

    return EXIT_SUCCESS;
}
