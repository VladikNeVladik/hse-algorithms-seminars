// Copyright 2024 Vladislav Aleinik
//=======================================
// Representation of vector
//=======================================
#ifndef HEADER_GUARD_VECTOR_HPP_INCLUDED
#define HEADER_GUARD_VECTOR_HPP_INCLUDED

#include <cstdlib>
#include <cassert>
#include <cstring>
#include <cmath>
#include <algorithm>

#include <utils.hpp>

namespace VectorArithmetics
{
    // Structure to represent the vector in N-dimensional space:
    template <typename Data_t>
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

        Data_t& operator[](const size_t index);

        //-----------------------
        // Arithmetic operations
        //-----------------------

        Vector operator+(const Vector& other) const;
        Vector& operator+=(const Vector& other);

        Vector operator*(const Data_t& koeff) const;
        Vector& operator*=(const Data_t& koeff);

        //-------------
        // Comparisons
        //-------------

        bool operator==(const Vector& other) const;
        bool operator!=(const Vector& other) const;

    private:
        // Verification of internal invariant:
        bool ok() const;

        // Private class members:
        // NOTE: use underscore to "highlight" class members stylistically in the code.
        Data_t* data_;
        size_t size_;
    };

    //=============================
    // Vector template definitions
    //=============================

    //----------------------
    // Existance management
    //----------------------

    // Constructor (non-default):
    template <typename Data_t>
    Vector<Data_t>::Vector(size_t size) :
        data_ (new Data_t[size]),
        size_ (size)
    {
        // NOTE: new throws exception std::bad_alloc on allocation error.

        // NOTE: be paranoid, ensure the invariant is established.
        VERIFY_CONTRACT(this->ok(),
            "Unable to construct Vector instance");
    }

    // Copy constructor:
    template <typename Data_t>
    Vector<Data_t>::Vector(const Vector<Data_t>& vector)
    {
        VERIFY_CONTRACT(vector.ok(),
            "Unable to create Vector from invalid origin");

        // Allocate new array and perform deep copy:
        data_ = new Data_t[vector.size_];
        size_ = vector.size_;

        std::copy_n(vector.data_, size_, data_);

        // NOTE: be paranoid, ensure the invariant is established.
        VERIFY_CONTRACT(this->ok(),
            "Unable to construct Vector instance");
    }

    // Move constructor:
    template <typename Data_t>
    Vector<Data_t>::Vector(Vector<Data_t>&& vector)
    {
        VERIFY_CONTRACT(vector.ok(),
            "Unable to create Vector from invalid origin");

        // Perform the shallow assignment:
        data_ = vector.data_;
        size_ = vector.size_;

        // Make move origin invalid:
        vector.data_ = nullptr;
        vector.size_ = 0U;

        // NOTE: be paranoid, ensure the invariant is established.
        VERIFY_CONTRACT(this->ok(),
            "Unable to construct Vector instance");
        // NOTE: be paranoid, ensure the invariant no longer holds for move origin.
        VERIFY_CONTRACT(!vector.ok(),
            "Move operation is non-destructive for move origin");
    }

    // Copy assignment:
    template <typename Data_t>
    Vector<Data_t>& Vector<Data_t>::operator=(const Vector<Data_t>& vector)
    {
        VERIFY_CONTRACT(this->ok(),
            "Left operand of copy assignment is invalid");
        VERIFY_CONTRACT(vector.ok(),
            "Right operand of copy assignment is invalid");

        // Check the case of self-assignment:
        if (this == &vector)
        {
            return *this;
        }

        // Deallocate previously allocated memory:
        delete[] data_;

        // Allocate new array and perform deep copy:
        data_ = new Data_t[vector.size_];
        size_ = vector.size_;

        std::copy_n(vector.data_, size_, data_);

        // NOTE: be paranoid, ensure the invariant still holds.
        VERIFY_CONTRACT(this->ok(),
            "Unable to perform copy assignment");

        return *this;
    }

    // Move assignment:
    template <typename Data_t>
    Vector<Data_t>& Vector<Data_t>::operator=(Vector<Data_t>&& vector)
    {
        VERIFY_CONTRACT(this->ok(),
            "Left operand of move assignment is invalid");
        VERIFY_CONTRACT(vector.ok(),
            "Right operand of move assignment is invalid");
        VERIFY_CONTRACT(this != &vector,
            "Move assignment from itself must not be generated by compiler");

        // Deallocate previously allocated memory:
        delete[] data_;

        // Perform shallow copy:
        data_ = vector.data_;
        size_ = vector.size_;

        // Make move origin invalid:
        vector.data_ = nullptr;
        vector.size_ = 0U;

        // NOTE: be paranoid, ensure the invariant still holds.
        VERIFY_CONTRACT(this->ok(),
            "Unable to perform move assignment");
        // NOTE: be paranoid, ensure the invariant no longer holds for move origin.
        VERIFY_CONTRACT(!vector.ok(),
            "Move operation is non-destructive for move origin");

        return *this;
    }

    // Destructor:
    template <typename Data_t>
    Vector<Data_t>::~Vector()
    {
        if (data_ != nullptr)
        {
            delete[] data_;
        }

        // Set vector data to NULL to detect double free:
        data_ = nullptr;
    }

    //------------------
    // Getter && setter
    //------------------

    template <typename Data_t>
    size_t Vector<Data_t>::size() const
    {
        VERIFY_CONTRACT(this->ok(),
            "Invalid vector data (possible double free)");

        return size_;
    }

    template <typename Data_t>
    Vector<Data_t>& Vector<Data_t>::setSize(size_t new_size)
    {
        VERIFY_CONTRACT(this->ok(),
            "Invalid vector data (possible double free)");

        if (new_size == size_)
        {
            return *this;
        }

        // NOTE: new throws exception std::bad_alloc on allocation error.
        Data_t* new_data = new Data_t[new_size];

        std::copy_n(new_data, std::min(new_size, size_), data_);

        // Unallocate old allocated data:
        delete[] data_;

        // Update data internal variables:
        data_ = new_data;
        size_ = new_size;

        // NOTE: be paranoid, ensure the invariant still holds.
        VERIFY_CONTRACT(this->ok(),
            "Unable to set size");

        return *this;
    }


    //----------------
    // Element access
    //----------------

    template <typename Data_t>
    Data_t& Vector<Data_t>::operator[](const size_t index)
    {
        VERIFY_CONTRACT(this->ok(),
            "Invalid vector data (possible double free)");
        VERIFY_CONTRACT(index < size_,
            "Index %zu out of bounds (size is %zu)",
            index, size_);

        return data_[index];
    }

    //-----------------------
    // Arithmetic operations
    //-----------------------

    template <typename Data_t>
    Vector<Data_t> Vector<Data_t>::operator+(const Vector<Data_t>& other) const
    {
        // Copy-construct the vector as a copy of *this:
        Vector<Data_t> to_return{*this};

        // NOTE: the implemenation of += performs all correctness checks.
        to_return += other;

        // NOTE: there is no actual copying in return statement thanks to:
        //       RVO (Return Value Optimization) and NRVO (Named RVO).
        return to_return;
    }

    template <typename Data_t>
    Vector<Data_t>& Vector<Data_t>::operator+=(const Vector<Data_t>& other)
    {
        VERIFY_CONTRACT(this->ok(),
            "Invalid vector data (possible double free for left argument)");
        VERIFY_CONTRACT(other.ok(),
            "Invalid vector data (possible double free for right argument)");
        VERIFY_CONTRACT(size_ == other.size_,
            "Unmatched operand sizes (%zu and %zu)",
            size_, other.size_);

        for (size_t i = 0U; i < size_; ++i)
        {
            data_[i] += other.data_[i];
        }

        return *this;
    }

    template <typename Data_t>
    Vector<Data_t> Vector<Data_t>::operator*(const Data_t& koeff) const
    {
        // Copy-construct the vector as a copy of *this:
        Vector<Data_t> to_return{*this};

        to_return *= koeff;

        // NOTE: there is no actual copying in return statement thanks to:
        //       RVO (Return Value Optimization) and NRVO (Named RVO).
        return to_return;
    }

    template <typename Data_t>
    Vector<Data_t>& Vector<Data_t>::operator*=(const Data_t& koeff)
    {
        VERIFY_CONTRACT(this->ok(),
            "Invalid vector data (possible double free for left argument)");

        for (size_t i = 0U; i < size_; ++i)
        {
            data_[i] *= koeff;
        }

        return *this;
    }

    //-------------
    // Comparisons
    //-------------

    template <typename Data_t>
    bool Vector<Data_t>::operator==(const Vector<Data_t>& other) const
    {
        VERIFY_CONTRACT(this->ok(),
            "Invalid vector data (possible double free for left argument)");
        VERIFY_CONTRACT(other.ok(),
            "Invalid vector data (possible double free for right argument)");

        // Case of identical equality:
        if (this == &other)
        {
            return true;
        }

        if (size_ != other.size_)
        {
            return false;
        }

        for (size_t i = 0U; i < size_; ++i)
        {
            if (data_[i] != other.data_[i])
            {
                return false;
            }
        }

        return true;
    }

    template <typename Data_t>
    bool Vector<Data_t>::operator!=(const Vector<Data_t>& other) const
    {
        return !(*this == other);
    }

    //------------------------------------
    // Verification of internal invariant
    //------------------------------------

    template <typename Data_t>
    bool Vector<Data_t>::ok() const
    {
        // The only internal invariant -- data_ is non-null.
        return data_ != nullptr;
    }

}; // namespace VectorArithmetic

#endif // HEADER_GUARD_VECTOR_HPP_INCLUDED
