/*
    Copyright (C) 2017 Jarthianur

    This file is part of simple-cpp-test-framework.

    simple-cpp-test-framework is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    simple-cpp-test-framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with simple-cpp-test-framework.  If not, see <https://www.gnu.org/licenses/>.
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
#define SCTF_INTERN_ENABLE_IF(C) typename std::enable_if<C>::type* = nullptr

/**
 * Generically check for a trait to be fulfilled.
 *
 * @param T is the trait.
 * @param ... is a variadic list of parameters, passed to the trait.
 */
#define SCTF_INTERN_IS(T, ...) (T<__VA_ARGS__>::value)

/**
 * Check for a type to be of a specific type.
 *
 * @param T is the type to check.
 * @param R is the type that T should be.
 */
#define SCTF_INTERN_IS_TYPE(T, R) (SCTF_INTERN_IS(std::is_same, T, R))

/**
 * Check for a type to be a floating point number.
 *
 * @param T is the type to check.
 */
#define SCTF_INTERN_IS_FLOAT(T) (SCTF_INTERN_IS(std::is_floating_point, T))

/**
 * Check for a type to have streaming capabilities for a given stream.
 *
 * @param T is the type to check.
 * @param S is the stream.
 */
#define SCTF_INTERN_HAS_STREAM_CAPABILITY(T, S) \
    (SCTF_INTERN_IS(sctf::intern::stream_capability, S, T))

/**
 * Check for a type to have iterator capabilities.
 *
 * @param T is the type to check.
 */
#define SCTF_INTERN_HAS_ITERATOR_CAPABILITY(T) \
    (SCTF_INTERN_IS(sctf::intern::iterator_capability, T))

namespace sctf
{
namespace intern
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
}  // namespace intern
}  // namespace sctf

#endif  // SCTF_TRAITS_HPP
