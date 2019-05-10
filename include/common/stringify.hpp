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

#ifndef SCTF_COMMON_STRINGIFY_HPP
#define SCTF_COMMON_STRINGIFY_HPP

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <typeinfo>
#include <utility>

#include "common/traits.hpp"
//#undef __clang__
namespace sctf
{
namespace _
{
/**
 * @brief Resolve a more or less pretty type name.
 * @tparam The type
 * @return the typename as string
 */
template<typename T>
static const char* name_for_type()
{
    static thread_local std::string name;
    if (name.length() > 0)
    {
        return name.c_str();
    }
#if defined(__GNUG__) || defined(__clang__)
    const std::string sig(__PRETTY_FUNCTION__);
    std::cout << __LINE__ << ": " << sig << std::endl;
    std::size_t b = sig.rfind("T = ") + 4;
#    ifdef __clang__
    name = sig.substr(b, sig.rfind(']') - b);
    std::cout << __LINE__ << ": " << name << std::endl;
#    else
    name = sig.substr(b, sig.find(';', b) - b);
    std::cout << __LINE__ << ": " << name << std::endl;
#    endif
    name.erase(std::remove(name.begin(), name.end(), ' '), name.end());
    std::cout << __LINE__ << ": " << name << std::endl;
#else
    const std::string sig(typeid(T).name());
    std::size_t       b = sig.find("struct ");
    if (b != std::string::npos)
    {
        name = sig.substr(b + 7);
        return name.c_str();
    }
    b = sig.find("class ");
    if (b != std::string::npos)
    {
        name = sig.substr(b + 6);
    }
    else
    {
        name = std::move(sig);
    }
#endif
    return name.c_str();
}

/**
 * @brief Serialize streamable types.
 * @tparam T The type
 * @param arg The element to serialize
 * @return the element as string
 */
template<typename T, ENABLE_IF(IS_STREAMABLE(T, std::ostringstream) AND NOT IS_FLOAT(T))>
std::string to_string(const T& arg)
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
template<typename T, ENABLE_IF(IS_STREAMABLE(T, std::ostringstream) AND IS_FLOAT(T))>
std::string to_string(const T& arg)
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
template<typename T, ENABLE_IF(NOT IS_STREAMABLE(T, std::ostringstream))>
std::string to_string(const T&)
{
    return name_for_type<T>();
}

/**
 * @brief Specialized serialize for nullptr.
 * @param unused
 * @return "0"
 */
inline std::string to_string(const std::nullptr_t&)
{
    return "0";
}

/**
 * @brief Specialized serialize for bool.
 * @param arg The bool to serialize
 * @return the bool as string
 */
inline std::string to_string(const bool& arg)
{
    return arg ? "true" : "false";
}

}  // namespace _
}  // namespace sctf

#endif  // SCTF_COMMON_STRINGIFY_HPP
