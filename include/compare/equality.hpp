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

#ifndef TPP_COMPARE_EQUALITY_HPP
#define TPP_COMPARE_EQUALITY_HPP

#include <algorithm>
#include <cmath>
#include <limits>

#include "compare/comparator.hpp"

#include "traits.hpp"

TPP_COMPARATOR(equals, "equals", actual_value == expected_value)
TPP_PROVIDE_COMPARATOR(equals, EQUALS)
TPP_PROVIDE_COMPARATOR(equals, EQ)

/**
 * Define a global epsilon value, that will be used for every floating point equality comparison by
 * default. This can be used only once per linked binary.
 *
 * @param E is the epsilon value.
 */
#define TPP_EPSILON(E) double tpp::epsilon = E;

namespace tpp
{
/// Epsilon that is used as floating point threshold in equality comparisons.
extern double epsilon;

namespace intern
{
namespace compare
{
/**
 * Comparator to check for equality floating point numbers.
 */
template<typename T>
class f_equals
{
    static_assert(TPP_INTERN_IS_FLOAT(T),
                  "The floating point comparator must not be used with other types than float, or double!");

    static constexpr char const* m_cmp_str     = "to be equals";
    static constexpr char const* m_neg_cmp_str = "to be not equals";
    bool                         m_neg         = false;
    T                            m_eps;

public:
    explicit f_equals(T eps_) : m_eps(eps_) {}

    auto
    operator!() -> decltype(*this)& {
        m_neg = !m_neg;
        return *this;
    }

    auto
    operator()(T actual_value, T expected_value) const -> comparison {
        return (std::abs(actual_value - expected_value) <=
                std::max(std::abs(actual_value), std::abs(expected_value)) * m_eps) != m_neg ?
                 comparison() :
                 comparison(m_neg ? m_neg_cmp_str : m_cmp_str,
                            std::forward_as_tuple(to_string(actual_value), to_string(expected_value)));
    }
};

template<>
class f_equals<void>
{
    static constexpr char const* m_cmp_str     = "to be equals";
    static constexpr char const* m_neg_cmp_str = "to be not equals";
    bool                         m_neg         = false;

public:
    f_equals() = default;

    auto
    operator!() -> decltype(*this)& {
        m_neg = !m_neg;
        return *this;
    }

    template<typename V>
    auto
    operator()(V const& actual_value, V const& expected_value) const -> comparison {
        static_assert(TPP_INTERN_IS_FLOAT(V),
                      "The floating point comparator must not be used with other types than float, or double!");

        typename std::decay<V>::type epsilon_ = static_cast<V>(epsilon);
        return (std::abs(actual_value - expected_value) <=
                std::max(std::abs(actual_value), std::abs(expected_value)) * epsilon_) != m_neg ?
                 comparison() :
                 comparison(m_neg ? m_neg_cmp_str : m_cmp_str,
                            std::forward_as_tuple(to_string(actual_value), to_string(expected_value)));
    }
};
}  // namespace compare
}  // namespace intern
}  // namespace tpp

TPP_PROVIDE_COMPARATOR(f_equals, F_EQUALS)
TPP_PROVIDE_COMPARATOR(f_equals, FEQ)

#endif  // TPP_COMPARE_EQUALITY_HPP
