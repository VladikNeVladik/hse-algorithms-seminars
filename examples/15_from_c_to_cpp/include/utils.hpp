// No copyright. Vladislav Aleinik, 2024
//=======================================
// Representation of matrix
//=======================================
#ifndef HEADER_GUARD_UTILS_HPP_INCLUDED
#define HEADER_GUARD_UTILS_HPP_INCLUDED

#include <cstdlib>
#include <cstdio>
#include <stdexcept>

//================//
// Utility macros //
//================//

constexpr size_t EXCEPTION_STR_SIZE = 100U;
extern char explanation_str[EXCEPTION_STR_SIZE];

#define VERIFY_CONTRACT(contract, format, ...)         \
    do {                                               \
        if (!(contract)) {                             \
            snprintf(                                  \
                explanation_str, EXCEPTION_STR_SIZE,   \
                "[%s:%d:%s] " format,                  \
                __FILE__, __LINE__, __func__,          \
                ##__VA_ARGS__);                        \
                                                       \
            throw std::runtime_error(explanation_str); \
        }                                              \
    } while (0)

#endif // HEADER_GUARD_UTILS_HPP_INCLUDED
