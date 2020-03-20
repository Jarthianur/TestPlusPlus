/*
 Copyright_License {

 Copyright (C) 2017 Julian P. Becht
 Author: Julian P. Becht

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License version 3
 as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 }
 */

/// @file

#ifndef SCTF_TRAITS_HPP
#define SCTF_TRAITS_HPP

#include <iterator>
#include <type_traits>

/**
 * Make this template function available if the condition resolves to be true.
 *
 * EXAMPLE:
 * @code
 * template<typename T, SCTF_PRIVATE_ENABLE_IF(SCTF_PRIVATE_IS_FLOAT(T))>
 * @endcode
 *
 * @param C is the condition.
 */
#define SCTF_PRIVATE_ENABLE_IF(C) typename std::enable_if<C>::type* = nullptr

/// Logical conjunction for use in template meta programming.
#define SCTF_PRIVATE_AND &&

/// Logical disjunction for use in template meta programming.
#define SCTF_PRIVATE_OR ||

/// Logical negation for use in template meta programming.
#define SCTF_PRIVATE_NOT !

/**
 * Generically check for a trait to be fulfilled.
 *
 * @param T is the trait.
 * @param ... is a variadic list of parameters, passed to the trait.
 */
#define SCTF_PRIVATE_IS(T, ...) (T<__VA_ARGS__>::value)

/**
 * Check for a type to be of a specific type.
 *
 * @param T is the type to check.
 * @param R is the type that T should be.
 */
#define SCTF_PRIVATE_IS_TYPE(T, R) (SCTF_PRIVATE_IS(std::is_same, T, R))

/**
 * Check for a type to be a floating point number.
 *
 * @param T is the type to check.
 */
#define SCTF_PRIVATE_IS_FLOAT(T) (SCTF_PRIVATE_IS(std::is_floating_point, T))

/**
 * Check for a type to have streaming capabilities for a given stream.
 *
 * @param T is the type to check.
 * @param S is the stream.
 */
#define SCTF_PRIVATE_HAS_STREAM_CAPABILITY(T, S) \
    (SCTF_PRIVATE_IS(sctf::private_::stream_capability, S, T))

/**
 * Check for a type to have iterator capabilities.
 *
 * @param T is the type to check.
 */
#define SCTF_PRIVATE_HAS_ITERATOR_CAPABILITY(T) \
    (SCTF_PRIVATE_IS(sctf::private_::iterator_capability, T))

/**
 * Check for a type to have equality comparison capabilities for another given type.
 *
 * @param L is the left hand type to check.
 * @param R is the right hand type.
 */
#define SCTF_PRIVATE_HAS_EQUALITY_CAPABILITY(L, R) \
    (SCTF_PRIVATE_IS(sctf::private_::equality_capability, L, R))

/**
 * Check for a type to have unequality comparison capabilities for another given type.
 *
 * @param L is the left hand type to check.
 * @param R is the right hand type.
 */
#define SCTF_PRIVATE_HAS_UNEQUALITY_CAPABILITY(T, R) \
    (SCTF_PRIVATE_IS(sctf::private_::unequality_capability, T, R))

namespace sctf
{
namespace private_
{
/**
 * Type trait to check for streaming operator capability in template meta programming.
 * S must implement operator<< for T.
 *
 * @tparam S is the stream type.
 * @tparam T is the type that needs to be streamable into S.
 */
template<typename S, typename T>
class stream_capability
{
    template<typename SS, typename TT>
    static auto test(int) -> decltype(std::declval<SS&>() << std::declval<TT>(), std::true_type());

    template<typename, typename>
    static auto test(...) -> std::false_type;

public:
    /// Resolves to true, if T is streamable to S.
    static const bool value = decltype(test<S, T>(0))::value;
};

/**
 * Type trait to check for iterator capabilities in template meta programming.
 * T must implement begin and end, while the resulting iterator must implement operator++ and
 * operator*.
 *
 * @tparam T is the type that needs to be iterable.
 */
template<typename T>
class iterator_capability
{
    template<typename TT>
    static auto test(int)
        -> decltype(std::begin(std::declval<TT&>()) != std::end(std::declval<TT&>()), void(),
                    ++std::declval<decltype(std::begin(std::declval<TT&>()))&>(),
                    void(*std::begin(std::declval<TT&>())), std::true_type());

    template<typename>
    static auto test(...) -> std::false_type;

public:
    /// Resolves to true, if T is iterable.
    static const bool value = decltype(test<T>(0))::value;
};

/**
 * Type trait to check for equality comparison capability in template meta programming.
 * L must implement operator== for R.
 *
 * @tparam L is the left hand type.
 * @tparam R is the right hand type.
 */
template<typename L, typename R>
class equality_capability
{
    template<typename LL, typename RR>
    static auto test(int) -> decltype(std::declval<LL>() == std::declval<RR>(), std::true_type());

    template<typename, typename>
    static auto test(...) -> std::false_type;

public:
    /// Resolves to true, if L can be compared for equality to R.
    static const bool value = decltype(test<L, R>(0))::value;
};

/**
 * Type trait to check for unequality comparison capability in template meta programming.
 * L must implement operator!= for R.
 *
 * @tparam L is the left hand type.
 * @tparam R is the right hand type.
 */
template<typename L, typename R>
class unequality_capability
{
    template<typename LL, typename RR>
    static auto test(int) -> decltype(std::declval<LL>() != std::declval<RR>(), std::true_type());

    template<typename, typename>
    static auto test(...) -> std::false_type;

public:
    /// Resolves to true, if L can be compared for unequality to R.
    static const bool value = decltype(test<L, R>(0))::value;
};
}  // namespace private_
}  // namespace sctf

#endif  // SCTF_TRAITS_HPP
