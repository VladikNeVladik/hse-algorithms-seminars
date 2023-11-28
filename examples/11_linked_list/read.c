// No copyright. Vladislav Aleinik, 2023

#define NDEBUG
#include "polynomial.h"

const char* FILENAME = "./res/poly.bin";

int main(void)
{
    // Read polynomial from file:
    Polynomial poly = polynomial_scan_from_file(FILENAME);

    // Print result:
    polynomial_print(&poly);

    // Free a polynomial:
    polynomial_free(&poly);
}
