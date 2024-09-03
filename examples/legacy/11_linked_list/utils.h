#ifndef HEADER_GUARD_UTILS_H_INCLUDED
#define HEADER_GUARD_UTILS_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>

//================//
// Utility macros //
//================//

#define MIN(a, b) ((a) < (b)? (a) : (b))

#define VERIFY_CONTRACT(contract, format, ...) \
    do { \
        if (!(contract)) { \
            printf((format), ##__VA_ARGS__); \
            exit(EXIT_FAILURE); \
        } \
    } while (0)


#endif // HEADER_GUARD_UTILS_H_INCLUDED
