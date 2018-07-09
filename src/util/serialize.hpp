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

#include <cstddef>
#include <sstream>
#include <string>
#include <typeinfo>
#include <utility>
#include "traits.hpp"

namespace sctf
{
namespace util
{
/**
 * @brief Resolve a more or less pretty type name.
 * @tparam The type
 * @return the typename as string
 */
template<typename T>
static const std::string& typeName(const T&)
{
#if defined(__GNUG__) || defined(__clang__)
    static std::string name;
    if(name.length() > 0)
    {
        return name;
    }
    const std::string sig(__PRETTY_FUNCTION__);
    std::size_t b = sig.rfind("T = ") + 4;
#ifdef __clang__
    name = sig.substr(b, sig.rfind(']') - b);
#else
    name = sig.substr(b, sig.find(';', b) - b);
#endif
    return name;
#else
    return typeid(T).name();
#endif
}

/**
 * @brief Serialize streamable types.
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

/**
 * @brief Serialize not streamable types.
 */
template<typename T, typename std::enable_if<not is_streamable<
                         std::ostringstream, T>::value>::type* = nullptr>
inline std::string serialize(const T& arg)
{
    return typeName(arg);
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

/**
 * @brief Specialized serialize for nullptr.
 */
template<>
inline std::string serialize(const std::nullptr_t&)
{
    return "0";
}

/**
 * @brief Specialized serialize for pairs.
 */
template<typename T>
inline std::string serialize(const std::pair<T, T>& arg)
{
    return std::string("[") + serialize(arg.first) + ", " + serialize(arg.second) + "]";
}

}  // namespace util
}  // namespace sctf

#endif  // SRC_UTIL_SERIALIZE_HPP_
