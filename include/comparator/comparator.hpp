/*
    Copyright (C) 2017 Jarthianur

    This file is part of simple-cpp-test-framework.

    simple-cpp-test-framework is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    simple-cpp-test-framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with simple-cpp-test-framework.  If not, see <https://www.gnu.org/licenses/>.
*/

/// @file

#ifndef SCTF_COMPARATOR_COMPARATOR_HPP
#define SCTF_COMPARATOR_COMPARATOR_HPP

#include <memory>

#include "cpp_meta.hpp"
#include "stringify.hpp"

#ifdef SCTF_CPP_V17

#    include <optional>

#else

#    include <cstring>

#endif

namespace sctf
{
namespace intern
{
/**
 * Result of an actual comparison performed by any comparator.
 */
struct comparison final
{
#ifdef SCTF_CPP_V17

    comparison() : m_failure(std::nullopt) {}

    comparison(char const* comp_str_, std::string const& val_, std::string const& expect_)
        : m_failure("Expected " + val_ + " " + comp_str_ + " " + expect_) {}

    explicit operator bool() {
        return !m_failure;
    }

    std::string const& operator*() const {
        return *m_failure;
    }

private:
    std::optional<std::string> const m_failure;

#else

    comparison() = default;

    comparison(char const* comp_str_, std::string const& val_, std::string const& expect_)
        : m_success(false) {
        std::string msg;
        msg.reserve(15 + std::strlen(comp_str_) + val_.length() + expect_.length());
        msg = "Expected ";
        msg.append(val_).append(" ").append(comp_str_).append(" ").append(expect_);
        error() = msg;
    }

    explicit operator bool() const {
        return m_success;
    }

    std::string const& operator*() const {
        return error();
    }

private:
    bool const m_success = true;

    std::string& error() const {
        static thread_local std::string err_msg;
        return err_msg;
    }

#endif
};
}  // namespace intern
}  // namespace sctf

/**
 * Define a comparator.
 * In PRED the two elements are named 'actual_value' and 'expected_value'.
 * The comparison is considered successful if PRED returns true, while false results in failure.
 *
 * @param NAME is the name of the comparator.
 * @param CMPSTR is a cstring representing the comparison constraint, like "equals".
 * @param PRED is the comparison predicate / condition.
 */
#define SCTF_COMPARATOR(NAME, CMPSTR, PRED)                                                   \
    namespace sctf                                                                            \
    {                                                                                         \
    namespace intern                                                                          \
    {                                                                                         \
    class NAME                                                                                \
    {                                                                                         \
        static constexpr char const* m_cmp_str     = "to be " CMPSTR;                         \
        static constexpr char const* m_neg_cmp_str = "to be not " CMPSTR;                     \
        bool                         m_neg         = false;                                   \
                                                                                              \
    public:                                                                                   \
        NAME& operator!() {                                                                   \
            m_neg = !m_neg;                                                                   \
            return *this;                                                                     \
        }                                                                                     \
        template<typename V, typename E = V>                                                  \
        comparison operator()(V const& actual_value, E const& expected_value) const {         \
            return (PRED) != m_neg ?                                                          \
                       comparison() :                                                         \
                       comparison(m_neg ? m_neg_cmp_str : m_cmp_str, to_string(actual_value), \
                                  to_string(expected_value));                                 \
        }                                                                                     \
    };                                                                                        \
    }                                                                                         \
    }

/**
 * Provide a shortwrite function which returns the respective
 * comparator. This shortwrite can then be used in assertions.
 *
 * @param COMP is the comparator to use.
 * @param NAME is the shortwrite function name.
 */
#define SCTF_PROVIDE_COMPARATOR(COMP, NAME)               \
    namespace sctf                                        \
    {                                                     \
    template<typename... Args>                            \
    static intern::COMP NAME(Args&&... args) {            \
        return intern::COMP(std::forward<Args>(args)...); \
    }                                                     \
    }

#endif  // SCTF_COMPARATOR_COMPARATOR_HPP
