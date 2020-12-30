/*
    Copyright (C) 2017  Jarthianur

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/// @file

#ifndef TPP_TRAITS_HPP
#define TPP_TRAITS_HPP

#include <iterator>
#include <type_traits>

#define TPP_INTERN_ENABLE_IF(C) typename std::enable_if<C>::type* = nullptr
#define TPP_INTERN_IS(T, ...) (T<__VA_ARGS__>::value)
#define TPP_INTERN_IS_TYPE(T, R) (TPP_INTERN_IS(std::is_same, T, R))
#define TPP_INTERN_IS_FLOAT(T) (TPP_INTERN_IS(std::is_floating_point, T))
#define TPP_INTERN_IS_VOID(T) (TPP_INTERN_IS(std::is_void, T))
#define TPP_INTERN_HAS_STREAM_CAPABILITY(T, S) (TPP_INTERN_IS(tpp::intern::stream_capability, S, T))
#define TPP_INTERN_HAS_ITERATOR_CAPABILITY(T) (TPP_INTERN_IS(tpp::intern::iterator_capability, T))

namespace tpp
{
namespace intern
{
template<typename S, typename T>
class stream_capability
{
    template<typename SS, typename TT>
    static auto
    test(int) -> decltype(std::declval<SS&>() << std::declval<TT>(), std::true_type());

    template<typename, typename>
    static auto
    test(...) -> std::false_type;

public:
    static const bool value = decltype(test<S, T>(0))::value;
};

template<typename T>
class iterator_capability
{
    template<typename TT>
    static auto
    test(int) -> decltype(std::begin(std::declval<TT&>()) != std::end(std::declval<TT&>()), void(),
                          ++std::declval<decltype(std::begin(std::declval<TT&>()))&>(),
                          void(*std::begin(std::declval<TT&>())), std::true_type());

    template<typename>
    static auto
    test(...) -> std::false_type;

public:
    static const bool value = decltype(test<T>(0))::value;
};
}  // namespace intern
}  // namespace tpp

#endif  // TPP_TRAITS_HPP
