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
#include "types.hpp"

namespace sctf
{
namespace private_
{
/**
 * Get a stringified name for any type.
 */
template<typename T>
static std::string const& name_for_type() {
    static thread_local std::string name;
    if (name.length() > 0) {
        return name;
    }
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
 * Get a readable string representation of some escape sequences.
 */
inline std::string escaped_char(char c_) {
    switch (c_) {
        case '\r': return "\\r";
        case '\n': return "\\n";
        case '\t': return "\\t";
        case '\f': return "\\f";
        case '\v': return "\\v";
        case '\"': return "\\\"";
        default: return std::string(1, c_);
    }
}

/**
 * Get a copy of a string with some escape sequences replaced in a readable representation.
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
 * Get a printable string representation for any type, that is already convertable to string, but
 * not a floatingpoint type.
 */
template<typename T, ENABLE_IF(HAS_STREAM_CAPABILITY(T, std::ostringstream) AND NOT IS_FLOAT(T))>
std::string to_string(T const& arg_) {
    std::ostringstream oss;
    oss << arg_;
    return oss.str();
}

/**
 * Get a printable string representation for any floatingpoint type.
 */
template<typename T, ENABLE_IF(HAS_STREAM_CAPABILITY(T, std::ostringstream) AND IS_FLOAT(T))>
std::string to_string(T const& arg_) {
    std::ostringstream oss;
    oss << std::setprecision(std::numeric_limits<T>::max_digits10) << arg_;
    return oss.str();
}

/**
 * Get a printable string representation for any type, that is not convertable to string.
 * The result is basically the typename.
 */
template<typename T, ENABLE_IF(NOT HAS_STREAM_CAPABILITY(T, std::ostringstream))>
std::string to_string(T const&) {
    return name_for_type<T>();
}

/**
 * Get a string wrapped in quotes and transformed escape sequences.
 */
inline std::string to_string(std::string const& arg_) {
    return std::string("\"") + escaped_string(arg_) + "\"";
}

/**
 * Get a string wrapped in quotes and transformed escape sequences.
 */
inline std::string to_string(char const* const& arg_) {
    return std::string("\"") + escaped_string(arg_) + "\"";
}

/**
 * Get a character wrapped in quotes and transformed escape sequences.
 */
inline std::string to_string(char const& arg_) {
    return std::string("'") + escaped_char(arg_) + "'";
}

/**
 * Get a printable string representation for a null pointer.
 */
inline std::string to_string(std::nullptr_t const&) {
    return "0";
}

/**
 * Get a printable string representation for boolean type.
 */
inline std::string to_string(bool const& arg_) {
    return arg_ ? "true" : "false";
}

inline std::string to_string(regex const& arg_) {
    return to_string(arg_.pattern);
}

}  // namespace private_
}  // namespace sctf

#endif  // SCTF_STRINGIFY_HPP
