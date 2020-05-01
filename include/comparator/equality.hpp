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

#ifndef SCTF_COMPARATOR_EQUALITY_HPP
#define SCTF_COMPARATOR_EQUALITY_HPP

#include <algorithm>
#include <cmath>
#include <limits>
#include <tuple>

#include "comparator/comparator.hpp"

#include "traits.hpp"

SCTF_COMPARATOR(equals, "equals", actual_value == expected_value)
SCTF_PROVIDE_COMPARATOR(equals, EQUALS)
SCTF_PROVIDE_COMPARATOR(equals, EQ)

/**
 * Define a global epsilon value, that will be used for every floating point equality comparison by
 * default. This can be used only once per linked binary.
 *
 * @param E is the epsilon value.
 */
#define SCTF_EPSILON(E) double sctf::epsilon = E;

namespace sctf
{
/// Epsilon that is used as floating point threshold in equality comparisons.
extern double epsilon;

namespace intern
{
/**
 * Comparator to check for equality floating point numbers.
 */
class f_equals
{
    static constexpr char const* m_cmp_str     = "to be equals";
    static constexpr char const* m_neg_cmp_str = "to be not equals";
    bool                         m_neg         = false;
    double                       m_eps         = epsilon;

public:
    f_equals() = default;

    explicit f_equals(double eps_) : m_eps(eps_) {}

    auto operator!() -> f_equals& {
        m_neg = !m_neg;
        return *this;
    }

    template<typename V, typename E = V>
    auto operator()(V const& actual_value, E const& expected_value) const -> comparison {
        static_assert(
            SCTF_INTERN_IS_FLOAT(V) && SCTF_INTERN_IS_FLOAT(E),
            "The floating point comparator must not be used with other types than float, or double!");

        typename std::decay<V>::type epsilon_ = static_cast<V>(m_eps);
        return (std::abs(actual_value - expected_value) <=
                std::max(std::abs(actual_value), std::abs(expected_value)) * epsilon_) != m_neg ?
                   comparison() :
                   comparison(
                       m_neg ? m_neg_cmp_str : m_cmp_str,
                       std::forward_as_tuple(to_string(actual_value), to_string(expected_value)));
    }
};
}  // namespace intern
}  // namespace sctf

SCTF_PROVIDE_COMPARATOR(f_equals, F_EQUALS)
SCTF_PROVIDE_COMPARATOR(f_equals, FEQ)

#endif  // SCTF_COMPARATOR_EQUALITY_HPP
