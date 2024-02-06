// No copyright. Vladislav Aleinik, 2024
//=======================================
// Representation of vector
//=======================================
#ifndef HEADER_GUARD_VECTOR_HPP_INCLUDED
#define HEADER_GUARD_VECTOR_HPP_INCLUDED

#include <cstdlib>

#include <utils.hpp>

namespace VectorArithmetics
{
    // Structure to represent the vector in N-dimensional space:
    class Vector
    {
    public:
        //----------------------
        // Existance management
        //----------------------

        // Default constructor:
        // NOTE: delete it as there is no adequate way to define it.
        Vector() = delete;

        // Constructor (non-default):
        Vector(size_t size);

        // Copy/move constructors:
        Vector(const Vector& vector);
        Vector(Vector&& vector);

        // Copy/move assignment:
        Vector& operator=(const Vector& vector);
        Vector& operator=(Vector&& vector);

        // Destructor:
        ~Vector();

        //------------------
        // Getter && setter
        //------------------

        size_t size() const;
        Vector& setSize(size_t size);

        //----------------
        // Element access
        //----------------

        double& operator[](const size_t index);

        //-----------------------
        // Arithmetic operations
        //-----------------------

        Vector operator+(const Vector& other) const;
        Vector& operator+=(const Vector& other);

        //-------------
        // Comparisons
        //-------------

        bool operator==(const Vector& other) const;
        bool operator!=(const Vector& other) const;

    private:
        // Helper function:
        static bool double_equal(double el1, double el2);

        // Verification of internal invariant:
        bool ok() const;

        // Private class members:
        // NOTE: use underscore to "highlight" class members stylistically in the code.
        double* data_;
        size_t size_;
    };

}; // namespace VectorArithmetic

#endif // HEADER_GUARD_VECTOR_HPP_INCLUDED
