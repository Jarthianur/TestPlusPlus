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

#ifndef SCTF_SRC_UTIL_SERIALIZE_HPP_
#define SCTF_SRC_UTIL_SERIALIZE_HPP_

#include <algorithm>
#include <cstddef>
#include <iomanip>
#include <limits>
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
static std::string name_for_type()
{
    static thread_local std::string name;
    if (name.length() > 0)
    {
        return name;
    }
#if defined(__GNUG__) || defined(__clang__)
    const std::string sig(__PRETTY_FUNCTION__);
    std::size_t       b = sig.rfind("T = ") + 4;
#    ifdef __clang__
    name = sig.substr(b, sig.rfind(']') - b);
#    else
    name = sig.substr(b, sig.find(';', b) - b);
#    endif
    name.erase(std::remove(name.begin(), name.end(), ' '), name.end());
    name.shrink_to_fit();
#else
    const std::string sig(typeid(T).name());
    std::size_t       b = sig.find("struct ");
    if (b != std::string::npos)
    {
        name = sig.substr(b + 7);
        return name;
    }
    b = sig.find("class ");
    if (b != std::string::npos)
    {
        name = sig.substr(b + 6);
    }
    else
    {
        name = sig;
    }
#endif
    return name;
}

/**
 * @brief Serialize streamable types.
 * @tparam T The type
 * @param arg The element to serialize
 * @return the element as string
 */
template<typename T, typename std::enable_if<is_streamable<std::ostringstream, T>::value &&
                                             !std::is_floating_point<T>::value>::type* = nullptr>
std::string serialize(const T& arg)
{
    std::ostringstream oss;
    oss << arg;
    return oss.str();
}

/**
 * @brief Serialize floating-point types.
 * @tparam T The type
 * @param arg The element to serialize
 * @return the element as string
 */
template<typename T, typename std::enable_if<is_streamable<std::ostringstream, T>::value &&
                                             std::is_floating_point<T>::value>::type* = nullptr>
std::string serialize(const T& arg)
{
    std::ostringstream oss;
    oss << std::setprecision(std::numeric_limits<T>::max_digits10) << arg;
    return oss.str();
}

/**
 * @brief Serialize not streamable types.
 * @tparam T The type
 * @param unused
 * @return the element as string
 */
template<typename T,
         typename std::enable_if<!is_streamable<std::ostringstream, T>::value>::type* = nullptr>
std::string serialize(const T&)
{
    return name_for_type<T>();
}

/**
 * @brief Specialized serialize for nullptr.
 * @param unused
 * @return "0"
 */
inline std::string serialize(const std::nullptr_t&)
{
    return "0";
}

/**
 * @brief Specialized serialize for bool.
 * @param arg The bool to serialize
 * @return the bool as string
 */
inline std::string serialize(const bool& arg)
{
    return arg ? "true" : "false";
}

}  // namespace util
}  // namespace sctf

#endif  // SCTF_SRC_UTIL_SERIALIZE_HPP_
