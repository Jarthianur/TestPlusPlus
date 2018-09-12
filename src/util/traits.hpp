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

#ifndef SCTF_SRC_UTIL_TRAITS_HPP_
#define SCTF_SRC_UTIL_TRAITS_HPP_

#include <iterator>
#include <type_traits>

namespace sctf
{
namespace util
{
/**
 * @brief Type trait to check for streaming operator compatibility.
 * @tparam S The stream type
 * @tparam T The type to check for
 */
template<typename S, typename T>
class is_streamable
{
    template<typename SS, typename TT>
    static auto test(int)
        -> decltype(std::declval<SS&>() << std::declval<TT>(), std::true_type());

    template<typename, typename>
    static auto test(...) -> std::false_type;

public:
    static const bool value = decltype(test<S, T>(0))::value;
};

/**
 * @brief Type trait to check for iterator capabilities.
 * @tparam T The type to check for
 */
template<typename T>
class is_iterable
{
    template<typename TT>
    static auto test(int)
        -> decltype(std::begin(std::declval<TT&>()) != std::end(std::declval<TT&>()),
                    void(), ++std::declval<decltype(std::begin(std::declval<TT&>())) &>(),
                    void(*std::begin(std::declval<TT&>())), std::true_type());

    template<typename>
    static auto test(...) -> std::false_type;

public:
    static const bool value = decltype(test<T>(0))::value;
};

/**
 * @brief Type trait to check for ordinal relation (operator<, operator>) capabilities.
 * @tparam T The type to check for
 */
template<typename T>
class is_ordinal
{
    template<typename TT>
    static auto test(int)
        -> decltype(std::declval<TT&>() < std::declval<TT&>(),
                    std::declval<TT&>() > std::declval<TT&>(), std::true_type());

    template<typename>
    static auto test(...) -> std::false_type;

public:
    static const bool value = decltype(test<T>(0))::value;
};

}  // namespace util
}  // namespace sctf

#endif  // SCTF_SRC_UTIL_TRAITS_HPP_
