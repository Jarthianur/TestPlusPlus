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
#include <limits>
#include <sstream>
#include <string>
#include <typeinfo>
#include <utility>

#include "common/traits.hpp"

namespace sctf
{
namespace private_
{
/**
 * Get a stringified name for every possible type.
 */
template<typename T>
static std::string const& name_for_type()
{
    static thread_local std::string name;
    if (name.length() > 0)
    {
        return name;
    }
#if defined(__GNUG__) || defined(__clang__)
    std::string const sig(__PRETTY_FUNCTION__);
    auto const        b = sig.rfind("T = ") + 4;
    name                = sig.substr(b, sig.find_first_of(";]", b) - b);
    name.erase(std::remove(name.begin(), name.end(), ' '), name.end());
#else
    std::string const sig(typeid(T).name());
    auto              b = sig.find("struct ");
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
        name = std::move(sig);
    }
#endif
    return name;
}

static char const* strip_namespace(std::string const& class_)
{
    std::size_t c;
    if ((c = class_.find_last_of(':')) != std::string::npos)
    {
        return class_.c_str() + c + 1;
    }
    return class_.c_str();
}

static std::string& escape_string(std::string& str_)
{
    std::size_t p = 0;
    while ((p = str_.find_first_of("\r\n\t\f\v\"", p)) != std::string::npos)
    {
        switch (str_[p])
        {
            case '\r': str_.replace(p, 1, "\\r"); break;
            case '\n': str_.replace(p, 1, "\\n"); break;
            case '\t': str_.replace(p, 1, "\\t"); break;
            case '\f': str_.replace(p, 1, "\\f"); break;
            case '\v': str_.replace(p, 1, "\\v"); break;
            case '"': str_.replace(p, 1, "\\\""); break;
        }
    }
    return str_;
}

/**
 * Get a printable string representation for any type, that is already convertable to string, but
 * not a floatingpoint type.
 */
template<typename T, ENABLE_IF(IS_STREAMABLE(T, std::ostringstream) AND NOT IS_FLOAT(T))>
std::string to_string(T const& arg_)
{
    std::ostringstream oss;
    oss << arg_;
    return oss.str();
}

/**
 * Get a printable string representation for any floatingpoint type.
 */
template<typename T, ENABLE_IF(IS_STREAMABLE(T, std::ostringstream) AND IS_FLOAT(T))>
std::string to_string(T const& arg_)
{
    std::ostringstream oss;
    oss << std::setprecision(std::numeric_limits<T>::max_digits10) << arg_;
    return oss.str();
}

/**
 * Get a printable string representation for any type, that is not convertable to string.
 * That is basically the typename.
 */
template<typename T, ENABLE_IF(NOT IS_STREAMABLE(T, std::ostringstream))>
std::string to_string(T const&)
{
    return name_for_type<T>();
}

inline std::string to_string(std::string const& arg_)
{
    return std::string("\"") + escape_string(arg_) + "\"";
}

inline std::string to_string(char const* arg_)
{
    return std::string("\"") + escape_string(arg_) + "\"";
}

inline std::string to_string(char const& arg_)
{
    return std::string("'") + arg_ + "'";
}

/**
 * Get a printable string representation for a null pointer.
 */
inline std::string to_string(std::nullptr_t const&)
{
    return "0";
}

/**
 * Get a printable string representation for boolean type.
 */
inline std::string to_string(bool const& arg_)
{
    return arg_ ? "true" : "false";
}
}  // namespace private_
}  // namespace sctf

#endif  // SCTF_COMMON_STRINGIFY_HPP
