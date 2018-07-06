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

#include <iostream>
#include <sstream>
#include <string>

namespace sctf
{
namespace util
{
/**
 * @brief Serialize generic types.
 * @tparam T The type
 * @param arg The element to serialize
 * @return the element as string
 */
template<typename T>
inline std::string serialize(const T& arg)
{
    std::ostringstream oss;
    oss << arg;
    return oss.str();
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
