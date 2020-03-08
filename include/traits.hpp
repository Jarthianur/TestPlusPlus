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

#ifndef SCTF_TRAITS_HPP
#define SCTF_TRAITS_HPP

#include <iterator>
#include <type_traits>

/// enable_if convenience wrappers for template declarations.
#define ENABLE_IF(C) typename std::enable_if<C>::type* = nullptr
#define AND &&
#define OR ||
#define NOT !
#define IS(T, ...) (T<__VA_ARGS__>::value)
#define IS_TYPE(T, R) (IS(std::is_same, T, R))
#define IS_FLOAT(T) (IS(std::is_floating_point, T))
#define HAS_STREAM_CAPABILITY(T, S) (IS(sctf::private_::stream_capability, S, T))
#define HAS_ITERATOR_CAPABILITY(T) (IS(sctf::private_::iterator_capability, T))
#define HAS_EQUALITY_CAPABILITY(T, R) (IS(sctf::private_::equality_capability, T, R))
#define HAS_UNEQUALITY_CAPABILITY(T, R) (IS(sctf::private_::unequality_capability, T, R))

namespace sctf
{
namespace private_
{
/**
 * Type trait to check for streaming operator capability.
 * @tparam S The stream type
 * @tparam T The type to check for
 */
template<typename S, typename T>
class stream_capability
{
    template<typename SS, typename TT>
    static auto test(int) -> decltype(std::declval<SS&>() << std::declval<TT>(), std::true_type());

    template<typename, typename>
    static auto test(...) -> std::false_type;

public:
    static const bool value = decltype(test<S, T>(0))::value;
};

/**
 * Type trait to check for iterator capabilities.
 * @tparam T The type to check for
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
    static const bool value = decltype(test<T>(0))::value;
};

/**
 * Type trait to check for equality operator capability.
 * @tparam S The left hand type
 * @tparam T The right hand type
 */
template<typename S, typename T>
class equality_capability
{
    template<typename SS, typename TT>
    static auto test(int) -> decltype(std::declval<SS>() == std::declval<TT>(), std::true_type());

    template<typename, typename>
    static auto test(...) -> std::false_type;

public:
    static const bool value = decltype(test<S, T>(0))::value;
};

/**
 * Type trait to check for unequality operator capability.
 * @tparam S The left hand type
 * @tparam T The right hand type
 */
template<typename S, typename T>
class unequality_capability
{
    template<typename SS, typename TT>
    static auto test(int) -> decltype(std::declval<SS>() != std::declval<TT>(), std::true_type());

    template<typename, typename>
    static auto test(...) -> std::false_type;

public:
    static const bool value = decltype(test<S, T>(0))::value;
};

}  // namespace private_
}  // namespace sctf

#endif  // SCTF_TRAITS_HPP
