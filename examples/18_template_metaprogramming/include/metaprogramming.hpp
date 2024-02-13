// No copyright. Vladislav Aleinik, 2024
//===============================================
// Hand-written template metaprogramming library
// Most of it is implemented in:
// - <type_traits>
// - <experimental/type_traits>
//===============================================

// NOTE:
//  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄         ▄       ▄▄▄▄▄▄▄▄▄▄▄  ▄         ▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄         ▄  ▄
// ▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░▌       ▐░▌     ▐░░░░░░░░░░░▌▐░▌       ▐░▌▐░░░░░░░░░░░▌▐░▌       ▐░▌▐░▌
// ▐░█▀▀▀▀▀▀▀▀▀  ▀▀▀▀█░█▀▀▀▀ ▐░█▀▀▀▀▀▀▀█░▌▐░▌       ▐░▌     ▐░█▀▀▀▀▀▀▀█░▌▐░▌       ▐░▌▐░█▀▀▀▀▀▀▀█░▌▐░▌       ▐░▌▐░▌
// ▐░▌               ▐░▌     ▐░▌       ▐░▌▐░▌       ▐░▌     ▐░▌       ▐░▌▐░▌       ▐░▌▐░▌       ▐░▌▐░▌       ▐░▌▐░▌
// ▐░█▄▄▄▄▄▄▄▄▄      ▐░▌     ▐░█▄▄▄▄▄▄▄█░▌▐░█▄▄▄▄▄▄▄█░▌     ▐░█▄▄▄▄▄▄▄█░▌▐░▌   ▄   ▐░▌▐░█▄▄▄▄▄▄▄█░▌▐░█▄▄▄▄▄▄▄█░▌▐░▌
// ▐░░░░░░░░░░░▌     ▐░▌     ▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌     ▐░░░░░░░░░░░▌▐░▌  ▐░▌  ▐░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░▌
//  ▀▀▀▀▀▀▀▀▀█░▌     ▐░▌     ▐░█▀▀▀▀▀▀▀█░▌ ▀▀▀▀█░█▀▀▀▀      ▐░█▀▀▀▀▀▀▀█░▌▐░▌ ▐░▌░▌ ▐░▌▐░█▀▀▀▀▀▀▀█░▌ ▀▀▀▀█░█▀▀▀▀ ▐░▌
//           ▐░▌     ▐░▌     ▐░▌       ▐░▌     ▐░▌          ▐░▌       ▐░▌▐░▌▐░▌ ▐░▌▐░▌▐░▌       ▐░▌     ▐░▌      ▀
//  ▄▄▄▄▄▄▄▄▄█░▌     ▐░▌     ▐░▌       ▐░▌     ▐░▌          ▐░▌       ▐░▌▐░▌░▌   ▐░▐░▌▐░▌       ▐░▌     ▐░▌      ▄
// ▐░░░░░░░░░░░▌     ▐░▌     ▐░▌       ▐░▌     ▐░▌          ▐░▌       ▐░▌▐░░▌     ▐░░▌▐░▌       ▐░▌     ▐░▌     ▐░▌
//  ▀▀▀▀▀▀▀▀▀▀▀       ▀       ▀         ▀       ▀            ▀         ▀  ▀▀       ▀▀  ▀         ▀       ▀       ▀
//
// NOTE:
//  ▄         ▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄   ▄
// ▐░▌       ▐░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░▌ ▐░▌
// ▐░▌       ▐░▌▐░█▀▀▀▀▀▀▀█░▌ ▀▀▀▀▀▀▀▀▀█░▌▐░█▀▀▀▀▀▀▀█░▌▐░█▀▀▀▀▀▀▀█░▌▐░█▀▀▀▀▀▀▀█░▌▐░▌
// ▐░▌       ▐░▌▐░▌       ▐░▌          ▐░▌▐░▌       ▐░▌▐░▌       ▐░▌▐░▌       ▐░▌▐░▌
// ▐░█▄▄▄▄▄▄▄█░▌▐░█▄▄▄▄▄▄▄█░▌ ▄▄▄▄▄▄▄▄▄█░▌▐░█▄▄▄▄▄▄▄█░▌▐░█▄▄▄▄▄▄▄█░▌▐░▌       ▐░▌▐░▌
// ▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░▌       ▐░▌▐░▌
// ▐░█▀▀▀▀▀▀▀█░▌▐░█▀▀▀▀▀▀▀█░▌▐░█▀▀▀▀▀▀▀▀▀ ▐░█▀▀▀▀▀▀▀█░▌▐░█▀▀▀▀█░█▀▀ ▐░▌       ▐░▌▐░▌
// ▐░▌       ▐░▌▐░▌       ▐░▌▐░▌          ▐░▌       ▐░▌▐░▌     ▐░▌  ▐░▌       ▐░▌ ▀
// ▐░▌       ▐░▌▐░▌       ▐░▌▐░█▄▄▄▄▄▄▄▄▄ ▐░▌       ▐░▌▐░▌      ▐░▌ ▐░█▄▄▄▄▄▄▄█░▌ ▄
// ▐░▌       ▐░▌▐░▌       ▐░▌▐░░░░░░░░░░░▌▐░▌       ▐░▌▐░▌       ▐░▌▐░░░░░░░░░░▌ ▐░▌
//  ▀         ▀  ▀         ▀  ▀▀▀▀▀▀▀▀▀▀▀  ▀         ▀  ▀         ▀  ▀▀▀▀▀▀▀▀▀▀   ▀
// NOTE: This code is hard to understand and serves self-teaching purposes only.
// HINT: Use the standard library instead.

// Use namespace to prevent namespace collision with namespace std:
namespace Meta
{
    //---------------------------------
    // Basic metaprogramming utilities
    //---------------------------------

    // The equivalent of declaring a typed constant:
    template <typename Type, Type Value>
    struct integral_constant
    {
        static constexpr Type value = Value;
    };

    // Special case of bool constant:
    template <bool Value>
    using bool_constant = integral_constant<bool, Value>;

    // The metaprogramming equivalent of true:
    using true_type = bool_constant<true>;

    // The metaprogramming equivalent of false:
    using false_type = bool_constant<false>;

    //----------------------------
    // Implimenation of enable_if
    //----------------------------

    // Description of enable_if:
    // - If the first argument is true, define const static member variable "type".
    // - If the first argument is false, do not define "type".
    //
    // This metafunction uses SFINAE principle:
    // - Substitution Failure Is Not An Error.

    // Default specialization of template:
    template<bool, typename Type = void>
    struct enable_if {};

    // In case of "true" define "type":
    template<typename Type>
    struct enable_if<true, Type>
    {
        using type = Type;
    };

    // Define a shorthand for "enable_if<..., ...>::type":
    template <bool Cond, typename Type>
    using enable_if_t = enable_if<Cond, Type>::type;

    //---------------------------
    // All sorts of conditionals
    //---------------------------

    // ToDo

    //-------------------------
    // Implimenation of void_t
    //-------------------------

    // ToDo

    //------------------------------
    // Implimenation of is_detected
    //------------------------------

    // ToDo

    //-------------------------------------------
    // Implimenation of is_default_constructible
    //-------------------------------------------

    // ToDo

    //----------------------------------------
    // Implimenation of is_copy_constructible
    //----------------------------------------

    // ToDo

    //----------------------------------------
    // Implimenation of is_copy_assignable
    //----------------------------------------

    // ToDo

    //-----------------------------
    // Implimenation of is_addable
    //-----------------------------

    // ToDo

    //----------------------------------
    // Implimenation of is_multiplyable
    //----------------------------------

    // ToDo

    //--------------------------------
    // Implimenation of is_comparable
    //--------------------------------

    // ToDo

}; // namespace Meta
