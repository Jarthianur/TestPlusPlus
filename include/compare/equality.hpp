/*
    Copyright (C) 2017  Jarthianur

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/// @file

#ifndef TPP_COMPARE_EQUALITY_HPP
#define TPP_COMPARE_EQUALITY_HPP

#include <algorithm>
#include <cmath>

#include "compare/comparator.hpp"

#include "traits.hpp"

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
namespace ns_equals
{
static constexpr auto CMP_STR     = "to be equals";
static constexpr auto NEG_CMP_STR = "to be not equals";
}  // namespace ns_equals

/**
 * Comparator to check for equality floating point numbers.
 */
template<typename T>
class equals
{
    static_assert(TPP_INTERN_IS_FLOAT(T),
                  "The floating point comparator must not be used with other types than float, or double!");

    bool m_neg{false};
    T    m_eps;

public:
    explicit equals(T eps_) : m_eps(eps_) {}

    auto
    operator!() -> decltype(*this)& {
        m_neg = !m_neg;
        return *this;
    }

    template<typename V, typename E = V, TPP_INTERN_ENABLE_IF(!TPP_INTERN_IS_FLOAT(V) || !TPP_INTERN_IS_FLOAT(E))>
    auto
    operator()(V const& actual_value, E const& expected_value) const -> comparison {
        return (actual_value == expected_value) != m_neg ?
                 comparison() :
                 comparison(m_neg ? ns_equals::NEG_CMP_STR : ns_equals::CMP_STR,
                            std::forward_as_tuple(to_string(actual_value), to_string(expected_value)));
    }

    template<typename V, typename E = V, TPP_INTERN_ENABLE_IF(TPP_INTERN_IS_FLOAT(V) && TPP_INTERN_IS_FLOAT(E))>
    auto
    operator()(V actual_value, E expected_value) const -> comparison {
        return (std::abs(actual_value - expected_value) <=
                std::max(std::abs(actual_value), std::abs(expected_value)) * m_eps) != m_neg ?
                 comparison() :
                 comparison(m_neg ? ns_equals::NEG_CMP_STR : ns_equals::CMP_STR,
                            std::forward_as_tuple(to_string(actual_value), to_string(expected_value)));
    }
};

template<>
class equals<void>
{
    bool m_neg{false};

public:
    auto
    operator!() -> decltype(*this)& {
        m_neg = !m_neg;
        return *this;
    }

    template<typename V, typename E = V, TPP_INTERN_ENABLE_IF(!TPP_INTERN_IS_FLOAT(V) || !TPP_INTERN_IS_FLOAT(E))>
    auto
    operator()(V const& actual_value, E const& expected_value) const -> comparison {
        return (actual_value == expected_value) != m_neg ?
                 comparison() :
                 comparison(m_neg ? ns_equals::NEG_CMP_STR : ns_equals::CMP_STR,
                            std::forward_as_tuple(to_string(actual_value), to_string(expected_value)));
    }

    template<typename V, typename E = V, TPP_INTERN_ENABLE_IF(TPP_INTERN_IS_FLOAT(V) && TPP_INTERN_IS_FLOAT(E))>
    auto
    operator()(V const& actual_value, E const& expected_value) const -> comparison {
        typename std::decay<E>::type epsilon_ = static_cast<E>(epsilon);
        return (std::abs(actual_value - expected_value) <=
                std::max(std::abs(actual_value), std::abs(expected_value)) * epsilon_) != m_neg ?
                 comparison() :
                 comparison(m_neg ? ns_equals::NEG_CMP_STR : ns_equals::CMP_STR,
                            std::forward_as_tuple(to_string(actual_value), to_string(expected_value)));
    }
};
}  // namespace compare
}  // namespace intern
}  // namespace tpp

TPP_PROVIDE_COMPARATOR(equals, EQUALS)
TPP_PROVIDE_COMPARATOR(equals, EQ)

#endif  // TPP_COMPARE_EQUALITY_HPP
