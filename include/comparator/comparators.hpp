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

#ifndef SCTF_COMPARATOR_COMPARATORS_HPP_
#define SCTF_COMPARATOR_COMPARATORS_HPP_

#include <memory>

#include "common/stringify.hpp"

#if __cplusplus >= 201703L

#    include <optional>
#    define STD_OPTIONAL std::optional
#    define STD_NULLOPT std::nullopt

#elif __cplusplus >= 201402L

#    include <experimental/optional>
#    define STD_OPTIONAL std::experimental::optional
#    define STD_NULLOPT std::experimental::nullopt

#endif

namespace sctf
{
namespace _
{
/**
 * @brief Result of an actual comparison performed by any comparator.
 * @note The error message may only be accessed, if the Comparison returns false to
 * conditions.
 */
struct comparison final
{
#if __cplusplus >= 201402L
    constexpr comparison() : _failure(STD_NULLOPT) {}

    comparison(const std::string& comp_str, const std::string& value, const std::string& expect)
        : _failure("Expected '" + value + "' " + comp_str + " '" + expect + "'")
    {}

    explicit operator bool()
    {
        return !_failure;
    }

    const std::string& operator*() const
    {
        return *_failure;
    }

private:
    const STD_OPTIONAL<std::string> _failure;
#else
    constexpr comparison() : _success(true) {}

    comparison(const std::string& comp_str, const std::string& value, const std::string& expect)
        : _success(false)
    {
        std::string msg;
        msg.reserve(15 + comp_str.length() + value.length() + expect.length());
        msg = "Expected '";
        msg.append(value).append("' ").append(comp_str).append(" '").append(expect).append("'");
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
 * @typedef comparator
 * @brief Function pointer to function comparing two elements
 * @tparam V The left hand type
 * @tparam E The right hand type
 */
template<typename V, typename E = V>
using comparator = comparison (*)(const V&, const E&);

/// @brief Default successful comparison.
#if __cplusplus >= 201402L
#    define success comparison()
#else
constexpr comparison success = comparison();
#endif

}  // namespace _
}  // namespace sctf

/**
 * @def PROVIDE_COMPARATOR
 * @brief Provide a shortwrite function which returns the address of the respective
 * comparator.
 * @param COMP The comparator function
 * @param NAME The final name, usually COMP in uppercase
 */
#define PROVIDE_COMPARATOR(COMP, NAME)   \
    namespace sctf                       \
    {                                    \
    template<typename V, typename E = V> \
    static _::comparator<V, E> NAME()    \
    {                                    \
        return &_::COMP<V, E>;           \
    }                                    \
    }

/**
 * @def COMPARATOR
 * @brief Create a comparator function.
 * @param NAME The final name of this function
 * @param COMPSTR A string representing the comparison constraint, like "to be equals"
 * @param PRED The comparison predicate as code
 * @note In PRED the two elements are named 'value' and 'expect', where 'value' is the
 * actual value and 'expect' is the expected value.
 */
#define COMPARATOR(NAME, COMPSTR, PRED)                                                   \
    namespace sctf                                                                        \
    {                                                                                     \
    namespace _                                                                           \
    {                                                                                     \
    constexpr const char* NAME##_comp_str = COMPSTR;                                      \
    template<typename V, typename E = V>                                                  \
    static comparison NAME(const V& value, const E& expect)                               \
    {                                                                                     \
        return (PRED) ? success :                                                         \
                        comparison(NAME##_comp_str, to_string(value), to_string(expect)); \
    }                                                                                     \
    }                                                                                     \
    }

/**
 * @def SCTF_SET_EPSILON
 * @brief Define the epsilon value used by equality comparison of floating point numbers.
 * @note Only used when SCTF_EXTERN_EPSILON is defined.
 * @param E The epsilon value
 */
#define SCTF_SET_EPSILON(E) \
    namespace sctf          \
    {                       \
    namespace _             \
    {                       \
    double epsilon = E;     \
    }                       \
    }

#endif  // SCTF_COMPARATOR_COMPARATORS_HPP_
