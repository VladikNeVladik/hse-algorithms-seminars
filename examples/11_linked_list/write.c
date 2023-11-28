// No copyright. Vladislav Aleinik, 2023

#define NDEBUG
#include "polynomial.h"

const char* FILENAME = "./res/poly.bin";

int main(void)
{
    // Polynomial parsing:
    Polynomial poly = polynomial_parse();

    // Print result:
    polynomial_print(&poly);

    // Write polynomial to file:
    polynomial_dump_to_file(FILENAME, &poly);

    // Free a polynomial:
    polynomial_free(&poly);
}
