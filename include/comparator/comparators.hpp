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

#ifndef SCTF_COMPARATOR_COMPARATORS_HPP
#define SCTF_COMPARATOR_COMPARATORS_HPP

#include <memory>

#include "common/stringify.hpp"

#if __cplusplus >= 201703L

#    include <optional>
using std::nullopt;
using std::optional;

#elif __cplusplus >= 201402L

#    include <experimental/optional>
using std::experimental::nullopt;
using std::experimental::optional;

#else

#    include <cstring>

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
    constexpr comparison() : m_failure(nullopt) {}

    comparison(const char* comp_str, const std::string& value, const std::string& expect)
        : m_failure("Expected '" + value + "' " + comp_str + " '" + expect + "'")
    {}

    explicit operator bool()
    {
        return !m_failure;
    }

    const std::string& operator*() const
    {
        return *m_failure;
    }

private:
    optional<std::string> const m_failure;
#else
    constexpr comparison() : m_success(true) {}

    comparison(char const* comp_str_, std::string const& val_, std::string const& expect_)
        : m_success(false)
    {
        std::string msg;
        msg.reserve(15 + std::strlen(comp_str_) + val_.length() + expect_.length());
        msg = "Expected '";
        msg.append(val_).append("' ").append(comp_str_).append(" '").append(expect_).append("'");
        error() = msg;
    }

    explicit operator bool()
    {
        return m_success;
    }

    const std::string& operator*() const
    {
        return error();
    }

private:
    bool const m_success;

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
using comparator = comparison (*)(V const&, E const&);

/// @brief Default successful comparison.
#if __cplusplus >= 201402L
#    define SUCCESS comparison()
#else
constexpr comparison SUCCESS = comparison();
#endif

#ifdef SCTF_EPSILON
static double epsilon = SCTF_EPSILON;
#elif defined(SCTF_EXTERN_EPSILON)
extern double        epsilon;
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
#define COMPARATOR(NAME, COMPSTR, PRED)                                      \
    namespace sctf                                                           \
    {                                                                        \
    namespace _                                                              \
    {                                                                        \
    constexpr char const* NAME##_comp_str = COMPSTR;                         \
    template<typename V, typename E = V>                                     \
    static comparison NAME(V const& actual_value, E const& expected_value)   \
    {                                                                        \
        return (PRED) ? SUCCESS :                                            \
                        comparison(NAME##_comp_str, to_string(actual_value), \
                                   to_string(expected_value));               \
    }                                                                        \
    }                                                                        \
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

#endif  // SCTF_COMPARATOR_COMPARATORS_HPP
