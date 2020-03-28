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

#ifndef SCTF_COMPARATOR_EQUALITY_HPP
#define SCTF_COMPARATOR_EQUALITY_HPP

#include <algorithm>
#include <cmath>
#include <limits>

#include "comparator/comparator.hpp"

#include "traits.hpp"

namespace sctf
{
namespace intern
{
/**
 * Comparator to check for equality.
 */
class equals
{
    static constexpr char const* m_cmp_str     = "to be equals";
    static constexpr char const* m_neg_cmp_str = "to be not equals";
    bool                         m_neg         = false;

public:
    equals& operator!() {
        m_neg = !m_neg;
        return *this;
    }

    template<typename V, typename E = V,
             SCTF_INTERN_ENABLE_IF(!SCTF_INTERN_IS_FLOAT(V) &&
                                   SCTF_INTERN_HAS_EQUALITY_CAPABILITY(V, E))>
    comparison operator()(V const& actual_value, E const& expected_value) {
        return (actual_value == expected_value) != m_neg ?
                   comparison() :
                   comparison(m_neg ? m_neg_cmp_str : m_cmp_str, to_string(actual_value),
                              to_string(expected_value));
    }

    template<typename V, typename E = V,
             SCTF_INTERN_ENABLE_IF(!SCTF_INTERN_IS_FLOAT(V) &&
                                   !SCTF_INTERN_HAS_EQUALITY_CAPABILITY(V, E) &&
                                   SCTF_INTERN_HAS_UNEQUALITY_CAPABILITY(V, E))>
    comparison operator()(V const& actual_value, E const& expected_value) {
        return (actual_value != expected_value) != m_neg ?
                   comparison(m_neg ? m_neg_cmp_str : m_cmp_str, to_string(actual_value),
                              to_string(expected_value)) :
                   comparison();
    }

    template<typename V, typename E = V,
             SCTF_INTERN_ENABLE_IF(SCTF_INTERN_IS_FLOAT(V) && SCTF_INTERN_IS_FLOAT(E))>
    comparison operator()(V const& actual_value, E const& expected_value) {
        V epsilon_ = static_cast<V>(epsilon);
        return (std::abs(actual_value - expected_value) <=
                std::max(std::abs(actual_value), std::abs(expected_value)) * epsilon_) != m_neg ?
                   comparison() :
                   comparison(m_neg ? m_neg_cmp_str : m_cmp_str, to_string(actual_value),
                              to_string(expected_value));
    }
};

/**
 * Comparator to check for unequality.
 */
class unequals
{
    static constexpr char const* m_cmp_str     = "to be unequals";
    static constexpr char const* m_neg_cmp_str = "to be not unequals";
    bool                         m_neg         = false;

public:
    unequals& operator!() {
        m_neg = !m_neg;
        return *this;
    }

    template<typename V, typename E = V,
             SCTF_INTERN_ENABLE_IF(!SCTF_INTERN_IS_FLOAT(V) &&
                                   SCTF_INTERN_HAS_UNEQUALITY_CAPABILITY(V, E))>
    comparison operator()(V const& actual_value, E const& expected_value) {
        return (actual_value != expected_value) != m_neg ?
                   comparison() :
                   comparison(m_neg ? m_neg_cmp_str : m_cmp_str, to_string(actual_value),
                              to_string(expected_value));
    }

    template<typename V, typename E = V,
             SCTF_INTERN_ENABLE_IF(!SCTF_INTERN_IS_FLOAT(V) &&
                                   !SCTF_INTERN_HAS_UNEQUALITY_CAPABILITY(V, E) &&
                                   SCTF_INTERN_HAS_EQUALITY_CAPABILITY(V, E))>
    comparison operator()(V const& actual_value, E const& expected_value) {
        return (actual_value == expected_value) != m_neg ?
                   comparison(m_neg ? m_neg_cmp_str : m_cmp_str, to_string(actual_value),
                              to_string(expected_value)) :
                   comparison();
    }

    template<typename V, typename E = V,
             SCTF_INTERN_ENABLE_IF(SCTF_INTERN_IS_FLOAT(V) && SCTF_INTERN_IS_FLOAT(E))>
    comparison operator()(V const& actual_value, E const& expected_value) {
        V epsilon_ = static_cast<V>(epsilon);
        return (std::abs(actual_value - expected_value) <=
                std::max(std::abs(actual_value), std::abs(expected_value)) * epsilon_) != m_neg ?
                   comparison(m_neg ? m_neg_cmp_str : m_cmp_str, to_string(actual_value),
                              to_string(expected_value)) :
                   comparison();
    }
};
}  // namespace intern
}  // namespace sctf

SCTF_PROVIDE_COMPARATOR(unequals, UNEQUALS)
SCTF_PROVIDE_COMPARATOR(unequals, NE)
SCTF_PROVIDE_COMPARATOR(equals, EQUALS)
SCTF_PROVIDE_COMPARATOR(equals, EQ)

#endif  // SCTF_COMPARATOR_EQUALITY_HPP
