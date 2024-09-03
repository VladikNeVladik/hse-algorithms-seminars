// Copyright 2024 Vladislav Aleinik
//=================================================
// Implementation of sparce polinomial arithmetics
//=================================================

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// Type of element to be used with the list:
typedef struct {
    int64_t coefficient;
    uint32_t exponent;
} Monomial;

typedef Monomial Data_t;

// Use list data structure:
#include "list.h"

typedef struct {
    List monomials;
} Polynomial;

//=================================
// Polynomial existance management
//=================================

// Initialize a polynomial.
void polynomial_init(Polynomial* poly)
{
    assert(poly != NULL);

    list_init(&poly->monomials);
}

// Free a polynomial.
void polynomial_free(Polynomial* poly)
{
    assert(poly != NULL);

    list_free(&poly->monomials);
}

//===========================
// Polynomial element access
//===========================

// Insert monomial into sorted polynomial.
//
// Monomials must be sorted by ascending exponent.
// If monomial is already present in polynomial,
// it's coefficient is added to found monomial's
// coefficient.
void polynomial_insert_sorted(Polynomial* poly, Monomial inserted)
{
    assert(poly != NULL);

    // Do not insert zero monomial:
    if (inserted.coefficient == 0)
    {
        return;
    }

    // Iterate over monomials:
    Node* prev = list_dummy(&poly->monomials);
    Node* mono = list_head(&poly->monomials);

    while (mono != NULL)
    {
        // Get associated list element:
        Monomial* data = list_get_ptr(mono);

        // Insert element before the selected monomial:
        if (inserted.exponent < data->exponent)
        {
            list_insert_after(prev, inserted);
            return;
        }

        // Modify existing monomial:
        if (inserted.exponent == data->exponent)
        {
            data->coefficient += inserted.coefficient;

            // Remove the monomial if resulting coefficient is zero:
            if (data->coefficient == 0)
            {
                list_remove_next(prev, NULL);
            }

            // Do not search for any other occurences:
            return;
        }

        // Move to next list element:
        prev = mono;
        mono = list_next(mono);
    }

    // Insert element as last if it's exponent is the biggest:
    list_insert_after(prev, inserted);
}

//=================================
// Polynomial printing and parsing
//=================================

void polynomial_print(Polynomial* poly)
{
    assert(poly != NULL);

    Node* mono = list_head(&poly->monomials);
    while (mono != NULL)
    {
        // Get next monomial in advance:
        Node* next = list_next(mono);

        // Get associated list element:
        Monomial data = list_get(mono);

        // Print current monomial:
        printf("%ld*x^%u", data.coefficient, data.exponent);

        // Also print possible link to next monomial:
        if (next != NULL)
        {
            printf(" + ");
        }

        // Step forward to next monomial;
        mono = next;
    }

    // Print newline for prettiness:
    printf("\n");
}

// Polynomial grammar:
// Space      ::= "[ \t]*"
// Exponent   ::= "x" | "x^" & Unsigned
// Monomial   ::= Signed | Signed & "*" & Exponent | Exponent
// Polynomial ::= Monomial & [Space & "+" & Space & Monimial]* & "\n"

bool parse_char(char expect)
{
    char ch = getc(stdin);
    if (ch == expect)
    {
        return true;
    }

    ungetc(ch, stdin);
    return false;
}

void parse_spaces(void)
{
    // Accept whitespaces (without writing them anywhere):
    scanf("%*[ \t]");
}

void parse_exponent(uint32_t* exponent)
{
    bool parsed_x = parse_char('x');
    VERIFY_CONTRACT(parsed_x,
        "[parse_exponent] Unable to scan 'x'\n");

    bool parsed_cap = parse_char('^');
    if (!parsed_cap)
    {
        *exponent = 1U;
        return;
    }

    int parsed_unsigned = scanf("%u", exponent);
    VERIFY_CONTRACT(parsed_unsigned == 1,
        "[parse_exponent] Unable to scan exponent\n");
}

void parse_monomial(Monomial* scanned)
{
    int parsed_coeff = scanf("%ld", &scanned->coefficient);
    if (parsed_coeff == 1)
    {
        // Parse multiplication:
        bool parsed_mul = parse_char('*');
        if (!parsed_mul)
        {
            scanned->exponent = 0U;
            return;
        }

        // Try to parse exponent:
        parse_exponent(&scanned->exponent);

        return;
    }

    // Use the default coefficient otherwise:
    scanned->coefficient = 1;

    parse_exponent(&scanned->exponent);
}

Polynomial polynomial_parse(void)
{
    Polynomial result;
    polynomial_init(&result);

    // Scan monomial:
    Monomial scanned;
    parse_monomial(&scanned);

    polynomial_insert_sorted(&result, scanned);

    while (true)
    {
        // Exit condition:
        bool parsed_newline = parse_char('\n');
        if (parsed_newline)
        {
            break;
        }

        parse_spaces();

        bool parsed_plus = parse_char('+');
        VERIFY_CONTRACT(parsed_plus,
            "[polynomial_parse] Unable to parse '+'\n");

        parse_spaces();

        // Scan monomial:
        Monomial scanned;
        parse_monomial(&scanned);

        polynomial_insert_sorted(&result, scanned);
    }

    return result;
}

//=================================
// Polynomial dumping and scanning
//=================================

typedef struct {
    int64_t coefficient;
    uint32_t exponent;
} __attribute__((packed)) PackedMonomial;

uint64_t POLY_MAGIC = 0xDEADBEEFB01DFACE;

// Dunp polynomial to file:
void polynomial_dump_to_file(const char* filename, Polynomial* poly)
{
    assert(filename != NULL);
    assert(poly != NULL);

    // Open file:
    FILE* file = fopen(filename, "wb");
    VERIFY_CONTRACT(file != NULL,
        "[polynomial_dump_to_file] Unable to open file \'%s\'\n",
        filename);

    // Dump special magic number:
    uint64_t magic = htobe64(POLY_MAGIC);

    size_t ret = fwrite(&magic, sizeof(POLY_MAGIC), 1U, file);
    VERIFY_CONTRACT(ret == 1U,
        "[polynomial_dump_to_file] Unable to dump magic number to file \'%s\'\n",
        filename);

    // Iterate over monomials:
    Node* mono = list_head(&poly->monomials);
    while (mono != NULL)
    {
        // Get current monomial:
        Monomial data = list_get(mono);

        // Switch data to big endian:
        PackedMonomial packed;
        packed.coefficient = htobe64(data.coefficient);
        packed.exponent    = htobe32(data.exponent);

        // Write monomial to file:
        // NOTE: beware of endianness and alignment issues
        size_t ret = fwrite(&packed, sizeof(PackedMonomial), 1U, file);
        VERIFY_CONTRACT(ret == 1U,
            "[polynomial_dump_to_file] Unable to dump monomial to file \'%s\'\n",
            filename);

        // Step forward to next monomial;
        mono = list_next(mono);
    }

    // Close opened file:
    ret = fclose(file);
    VERIFY_CONTRACT(ret != EOF,
        "[matrix_dump_to_file] Error on file close \'%s\'\n",
        filename);
}

// Scan polynomial from file:
Polynomial polynomial_scan_from_file(const char* filename)
{
    assert(filename != NULL);

    // Open file:
    FILE* file = fopen(filename, "rb");
    VERIFY_CONTRACT(file != NULL,
        "[polynomial_dump_to_file] Unable to open file \'%s\'\n",
        filename);

    uint64_t magic = 0U;

    // Read special magic number:
    size_t ret = fread(&magic, sizeof(magic), 1U, file);
    VERIFY_CONTRACT(ret == 1U,
        "[polynomial_scan_from_file] Unable to read magic number to file \'%s\'\n",
        filename);
    VERIFY_CONTRACT(be64toh(magic) == POLY_MAGIC,
        "[polynomial_scan_from_file] Invalid magic: %016lx\n",
        magic);

    // Initialize the resulting polynomial:
    Polynomial result;
    polynomial_init(&result);

    Node* insert_after = list_dummy(&result.monomials);
    while (true)
    {
        // Try to read data from file:
        PackedMonomial packed;

        size_t ret = fread(&packed, sizeof(PackedMonomial), 1U, file);
        if (ret == 0)
        {
            // Return if no data is left:
            break;
        }

        VERIFY_CONTRACT(ret == 1U,
            "[polynomial_scan_from_file] Unable to scan monomial from file \'%s\'\n",
            filename);

        // Unpack data:
        Monomial data;
        data.coefficient = be64toh(packed.coefficient);
        data.exponent    = be32toh(packed.exponent);

        // Push data to polynomial:
        list_insert_after(insert_after, data);
    }

    return result;
}
