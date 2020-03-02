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
namespace private_
{
/**
 * Result of an actual comparison performed by any comparator.
 */
struct comparison final
{
#if __cplusplus >= 201402L
    /**
     * Initialize the comparison as successful.
     */
    constexpr comparison() : m_failure(nullopt) {}

    /**
     * Initialize the comparison as failed.
     * @param comp_str_ The expression of the performed comparison
     * @param val_      The actual value
     * @param expect_   The expected value
     */
    comparison(char const* comp_str_, std::string const& val_, std::string const& expect_)
        : m_failure("Expected " + val_ + ' ' + comp_str_ + ' ' + expect_)
    {}

    /**
     * Allow conversion to boolean.
     * @return true, if comparison was successful, else false
     */
    explicit operator bool()
    {
        return !m_failure;
    }

    /**
     * Get the message describing the reason for failure.
     * May only be called, if the comparison returns false.
     */
    std::string const& operator*() const
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
        msg = "Expected ";
        msg.append(val_).append(' ').append(comp_str_).append(' ').append(expect_);
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
}  // namespace private_
}  // namespace sctf

/**
 * Define a comparator.
 * In PRED the two elements are named 'actual_value' and 'expected_value'.
 * The comparison is considered successful if PRED returns true, while false results in failure.
 * @param NAME   The name of this function
 * @param CMPSTR A string representing the comparison constraint, like "equals"
 * @param PRED   The comparison predicate / condition
 */
#define COMPARATOR(NAME, CMPSTR, PRED)                                                        \
    namespace sctf                                                                            \
    {                                                                                         \
    namespace private_                                                                        \
    {                                                                                         \
    class NAME                                                                                \
    {                                                                                         \
        static constexpr char const* m_cmp_str     = "to be " CMPSTR;                         \
        static constexpr char const* m_neg_cmp_str = "to be not " CMPSTR;                     \
        bool                         m_neg         = false;                                   \
                                                                                              \
    public:                                                                                   \
        NAME& operator!()                                                                     \
        {                                                                                     \
            m_neg = !m_neg;                                                                   \
            return *this;                                                                     \
        }                                                                                     \
        template<typename V, typename E = V>                                                  \
        comparison operator()(V const& actual_value, E const& expected_value)                 \
        {                                                                                     \
            return (PRED) != m_neg ?                                                          \
                       SUCCESS :                                                              \
                       comparison(m_neg ? m_neg_cmp_str : m_cmp_str, to_string(actual_value), \
                                  to_string(expected_value));                                 \
        }                                                                                     \
    };                                                                                        \
    }                                                                                         \
    }

/**
 * Provide a shortwrite function which returns the address of the respective
 * comparator.
 * @param COMP The comparator function
 * @param NAME The final shortwrite
 */
#define PROVIDE_COMPARATOR(COMP, NAME) \
    namespace sctf                     \
    {                                  \
    static private_::COMP NAME()       \
    {                                  \
        return private_::COMP();       \
    }                                  \
    }

/**
 * Define the epsilon value used by equality comparison of floating point numbers.
 * Only used when SCTF_EXTERN_EPSILON is defined.
 * @param E The epsilon value
 */
#define SCTF_SET_EPSILON(E) \
    namespace sctf          \
    {                       \
    namespace private_      \
    {                       \
    double epsilon = E;     \
    }                       \
    }

#endif  // SCTF_COMPARATOR_COMPARATORS_HPP
