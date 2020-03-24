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

/// @file

#ifndef SCTF_STRINGIFY_HPP
#define SCTF_STRINGIFY_HPP

#include <algorithm>
#include <iomanip>
#include <limits>
#include <sstream>
#include <string>
#include <typeinfo>
#include <utility>

#include "cpp_meta.hpp"
#include "regex.hpp"
#include "traits.hpp"

namespace sctf
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
static std::string const& name_for_type() {
    static thread_local std::string name;
    if (name.length() > 0) { return name; }
#ifdef SCTF_SYS_UNIX
    std::string const sig(__PRETTY_FUNCTION__);
    auto const        b = sig.rfind("T = ") + 4;
    name                = sig.substr(b, sig.find_first_of(";]", b) - b);
    name.erase(std::remove(name.begin(), name.end(), ' '), name.end());
#else
    std::string const sig(typeid(T).name());
    auto              b = sig.find("struct ");
    if (b != std::string::npos) {
        name = sig.substr(b + 7);
        return name;
    }
    b = sig.find("class ");
    if (b != std::string::npos) {
        name = sig.substr(b + 6);
    } else {
        name = std::move(sig);
    }
#endif
    return name;
}

/**
 * Get a character as string, where an escape sequence is escaped to be readable.
 *
 * @param c_ is the char to stringify.
 */
inline std::string escaped_char(char c_) {
    char const* ret;
    switch (c_) {
        case '\r': ret = "\\r"; break;
        case '\n': ret = "\\n"; break;
        case '\t': ret = "\\t"; break;
        case '\f': ret = "\\f"; break;
        case '\v': ret = "\\v"; break;
        case '\"': ret = "\\\""; break;
        default: return std::string(1, c_);
    }
    return ret;
}

/**
 * Get a string with escape sequences replaced in a readable representation.
 *
 * @param str_ is the string to escape.
 */
static std::string escaped_string(std::string const& str_) {
    std::string s = str_;
    std::size_t p = 0;
    while ((p = s.find_first_of("\r\n\t\f\v\"", p)) != std::string::npos) {
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
         SCTF_INTERN_ENABLE_IF(SCTF_INTERN_HAS_STREAM_CAPABILITY(T, std::ostringstream) &&
                               !SCTF_INTERN_IS_FLOAT(T))>
std::string to_string(T const& arg_) {
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
template<typename T, SCTF_INTERN_ENABLE_IF(SCTF_INTERN_IS_FLOAT(T))>
std::string to_string(T const& arg_) {
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
template<typename T,
         SCTF_INTERN_ENABLE_IF(!SCTF_INTERN_HAS_STREAM_CAPABILITY(T, std::ostringstream))>
std::string to_string(T const&) {
    return name_for_type<T>();
}

/**
 * Get a string wrapped in quotes and escaped.
 *
 * @param arg_ is the string to transform.
 */
inline std::string to_string(std::string const& arg_) {
    return std::string("\"") + escaped_string(arg_) + "\"";
}

/**
 * Get a cstring wrapped in quotes and escaped.
 *
 * @param arg_ is the cstring to transform.
 */
inline std::string to_string(char const* const& arg_) {
    return std::string("\"") + escaped_string(arg_) + "\"";
}

/**
 * Get a character wrapped in single quotes and escaped.
 *
 * @param arg_ is the character to transform.
 */
inline std::string to_string(char const& arg_) {
    return std::string("'") + escaped_char(arg_) + "'";
}

/**
 * Get a printable string representation for null pointer.
 */
inline std::string to_string(std::nullptr_t const&) {
    return "0";
}

/**
 * Get a printable string representation for booleans.
 *
 * @param arg_ is the bool value to convert to string.
 */
inline std::string to_string(bool const& arg_) {
    return arg_ ? "true" : "false";
}

/**
 * Get a printable string representation for an internal regex.
 *
 * @param arg_ is the regex to convert to string.
 * @return the pattern of arg_.
 */
inline std::string to_string(regex const& arg_) {
    return to_string(arg_.pattern);
}
}  // namespace intern
}  // namespace sctf

#endif  // SCTF_STRINGIFY_HPP
