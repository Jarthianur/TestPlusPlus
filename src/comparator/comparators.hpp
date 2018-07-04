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

#ifndef COMPARATOR_COMPARATORS_HPP_
#define COMPARATOR_COMPARATORS_HPP_

#include <memory>
#include <string>

#include "../util/serialize.hpp"

#if __cplusplus >= 201703L

#include <optional>
#define STD_OPTIONAL std::optional
#define STD_NULLOPT std::nullopt

#elif __cplusplus == 201402L

#include <experimental/optional>
#define STD_OPTIONAL std::experimental::optional
#define STD_NULLOPT std::experimental::nullopt

#endif

/// @namespace sctf
namespace sctf
{
/// @namespace comp
namespace comp
{
/**
 * The Comparison struct.
 * Boolean convertible, thus checkable in conditions.
 * Dereferencing operator for access of the error message.
 *
 * !!! The error message may only be accessed, if the Comparison returns false to
 * conditions !!!
 *
 * @brief Result of an actual comparison performed by any comparator.
 * @note As of C++17/14 optionals are available in the STL, so they are used
 * conditionally.
 */
struct Comparison
{
#if __cplusplus >= 201703L || __cplusplus == 201402L
    constexpr Comparison() : _failure(STD_NULLOPT)
    {}

    Comparison(const std::string& comp_str, const std::string& value,
               const std::string& expect)
    {
        std::string msg;
        msg.reserve(15 + comp_str.length() + value.length() + expect.length());
        msg = "Expected '";
        msg.append(value)
            .append("' ")
            .append(comp_str)
            .append(" '")
            .append(expect)
            .append("'");
        _failure = msg;
    }

    explicit operator bool()
    {
        return !_failure;
    }

    const std::string& operator*() const
    {
        return *_failure;
    }

private:
    STD_OPTIONAL<std::string> _failure;
#else
    constexpr Comparison() : _success(true)
    {}

    Comparison(const std::string& comp_str, const std::string& value,
               const std::string& expect)
        : _success(false)
    {
        std::string msg;
        msg.reserve(15 + comp_str.length() + value.length() + expect.length());
        msg = "Expected '";
        msg.append(value)
            .append("' ")
            .append(comp_str)
            .append(" '")
            .append(expect)
            .append("'");
        error() = msg;
    }

    explicit operator bool()
    {
        return _success;
    }

    const std::string& operator*() const
    {
        return error();
    }

private:
    const bool _success;

    std::string& error() const
    {
        static thread_local std::string err_msg;
        return err_msg;
    }
#endif
};

/**
 * @typedef Comparator
 * @brief Function pointer to function comparing two elements
 * @tparam T The type of elements
 */
template<typename T>
using Comparator = Comparison (*)(const T&, const T&);

/// @brief Default successful Comparison.
constexpr Comparison success = Comparison();

}  // namespace comp
}  // namespace sctf

/**
 * @def PROVIDE_COMPARATOR
 * @brief Provide a shortwrite function which returns the address of the respective
 * Comparator.
 * @param COMP The Comparator function
 * @param NAME The final name, usually COMP in uppercase
 */
#define PROVIDE_COMPARATOR(COMP, NAME) \
    namespace sctf                     \
    {                                  \
    namespace comp                     \
    {                                  \
    template<typename T>               \
    inline static Comparator<T> NAME() \
    {                                  \
        return &COMP<T>;               \
    }                                  \
    }                                  \
    }

/**
 * @def COMPARATOR
 * @brief Create a Comparator function.
 * @param NAME The final name of this function
 * @param COMPSTR A string representing the comparison constraint, like "to be equals"
 * @param PRED The comparison predicate as code
 * @note In PRED the two elements are named 'value' and 'expect', where 'value' is the
 * actual value and 'expect' is the expected value.
 */
#define COMPARATOR(NAME, COMPSTR, PRED)                                     \
    namespace sctf                                                          \
    {                                                                       \
    namespace comp                                                          \
    {                                                                       \
    constexpr const char* NAME##_comp_str = COMPSTR;                        \
    template<typename T>                                                    \
    inline static Comparison NAME(const T& value, const T& expect)          \
    {                                                                       \
        return (PRED) ? success                                             \
                      : Comparison(NAME##_comp_str, util::serialize(value), \
                                   util::serialize(expect));                \
    }                                                                       \
    }                                                                       \
    }

#endif /* COMPARATOR_COMPARATORS_HPP_ */
