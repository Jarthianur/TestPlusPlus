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

#ifndef SRC_UTIL_SERIALIZE_HPP_
#define SRC_UTIL_SERIALIZE_HPP_

#include <sstream>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <utility>

namespace sctf
{
namespace util
{
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
 * @brief Serialize generic types.
 * @tparam T The type
 * @param arg The element to serialize
 * @return the element as string
 */
template<typename T, typename std::enable_if<
                         is_streamable<std::ostringstream, T>::value>::type* = nullptr>
inline std::string serialize(const T& arg)
{
    std::ostringstream oss;
    oss << arg;
    return oss.str();
}

template<typename T, typename std::enable_if<not is_streamable<
                         std::ostringstream, T>::value>::type* = nullptr>
inline std::string serialize(const T& arg)
{
    return typeid(T).name();
}

/**
 * @brief Specialized serialize for strings (dummy).
 */
template<>
inline std::string serialize(const std::string& arg)
{
    return arg;
}

/**
 * @brief Specialized serialize for C-strings.
 */
template<>
inline std::string serialize(const char* const& arg)
{
    return std::string(arg);
}

template<>
inline std::string serialize(const std::nullptr_t&)
{
    return "0";
}

}  // namespace util
}  // namespace sctf

#endif  // SRC_UTIL_SERIALIZE_HPP_
