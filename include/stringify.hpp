/*
    Copyright (C) 2017 Jarthianur

    This file is part of TestPlusPlus (Test++).

    TestPlusPlus (Test++) is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TestPlusPlus (Test++) is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TestPlusPlus (Test++).  If not, see <https://www.gnu.org/licenses/>.
*/

/// @file

#ifndef TPP_STRINGIFY_HPP
#define TPP_STRINGIFY_HPP

#include <algorithm>
#include <iomanip>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <typeinfo>
#include <utility>

#include "cpp_meta.hpp"
#include "regex.hpp"
#include "traits.hpp"

#ifdef TPP_SYS_UNIX
#    include <cxxabi.h>
#endif

namespace tpp
{
namespace intern
{
/**
 * Get a stringified name for any type.
 * To resolve names, RTTI is used in a way, so that the name string is prepared only once.
 *
 * @tparam T is the type whose name is returned.
 */
template<typename T>
static auto
name_for_type(T const& arg_) -> std::string const& {
    static thread_local std::string name;
    if (!name.empty()) {
        return name;
    }
#ifdef TPP_SYS_UNIX
    int                     status = -1;
    std::unique_ptr<char[]> sig(abi::__cxa_demangle(typeid(arg_).name(), nullptr, nullptr, &status));
    name = sig.get();
#else
    name = typeid(arg_).name();
#endif
    return name;
}

/**
 * Get a character as string, where an escape sequence is escaped to be readable.
 *
 * @param c_ is the char to stringify.
 */
inline auto
escaped_char(char c_) -> std::string {
    switch (c_) {
        case '\r': return "\\r";
        case '\n': return "\\n";
        case '\t': return "\\t";
        case '\f': return "\\f";
        case '\v': return "\\v";
        case '\"': return "\\\"";
        case '\b': return "\\b";
        case '\\': return "\\\\";
        default: break;
    }
    return std::string(1, c_);
}

/**
 * Get a string with escape sequences replaced in a readable representation.
 *
 * @param str_ is the string to escape.
 */
static auto
escaped_string(std::string const& str_) -> std::string {
    std::string s = str_;
    std::size_t p = 0;
    while ((p = s.find_first_of("\r\n\t\f\v\"\b\\", p)) != std::string::npos) {
        s.replace(p, 1, escaped_char(s[p]));
        p += 2;
    }
    return s;
}

/**
 * Get a printable string representation for a given value of any type, that is already convertable
 * to string.
 *
 * @tparam T is the type of arg_.
 * @param arg_ is the value to convert to string.
 */
template<typename T,
         TPP_INTERN_ENABLE_IF(TPP_INTERN_HAS_STREAM_CAPABILITY(T, std::ostringstream) && !TPP_INTERN_IS_FLOAT(T))>
auto
to_string(T const& arg_) -> std::string {
    std::ostringstream oss;
    oss << arg_;
    return oss.str();
}

/**
 * Get a printable string representation for a given floating point number.
 *
 * @tparam T is the floating point type.
 * @param arg_ is the number to convert to string.
 */
template<typename T, TPP_INTERN_ENABLE_IF(TPP_INTERN_IS_FLOAT(T))>
auto
to_string(T const& arg_) -> std::string {
    std::ostringstream oss;
    oss << std::setprecision(std::numeric_limits<T>::max_digits10) << arg_;
    return oss.str();
}

/**
 * Get a printable string representation for a given value of any type, that is not convertable to
 * string.
 *
 * @tparam T is the type of arg_.
 * @param arg_ is the value to convert to string.
 * @return the typename for T, as there is no information about the value available.
 */
template<typename T, TPP_INTERN_ENABLE_IF(!TPP_INTERN_HAS_STREAM_CAPABILITY(T, std::ostringstream))>
auto
to_string(T const& arg_) -> std::string {
    return name_for_type<T>(arg_);
}

/**
 * Get a string wrapped in quotes and escaped.
 *
 * @param arg_ is the string to transform.
 */
inline auto
to_string(std::string const& arg_) -> std::string {
    return std::string("\"") + escaped_string(arg_) + "\"";
}

/**
 * Get a cstring wrapped in quotes and escaped.
 *
 * @param arg_ is the cstring to transform.
 */
inline auto
to_string(char const* const& arg_) -> std::string {
    return std::string("\"") + escaped_string(arg_) + "\"";
}

/**
 * Get a character wrapped in single quotes and escaped.
 *
 * @param arg_ is the character to transform.
 */
inline auto
to_string(char const& arg_) -> std::string {
    return std::string("'") + escaped_char(arg_) + "'";
}

/**
 * Get a printable string representation for null pointer.
 */
inline auto
to_string(std::nullptr_t const&) -> std::string {
    return "0";
}

/**
 * Get a printable string representation for booleans.
 *
 * @param arg_ is the bool value to convert to string.
 */
inline auto
to_string(bool const& arg_) -> std::string {
    return arg_ ? "true" : "false";
}

/**
 * Get a printable string representation for an internal regex.
 *
 * @param arg_ is the regex to convert to string.
 * @return the pattern of arg_.
 */
inline auto
to_string(regex const& arg_) -> std::string {
    return to_string(arg_.pattern);
}
}  // namespace intern
}  // namespace tpp

#endif  // TPP_STRINGIFY_HPP
