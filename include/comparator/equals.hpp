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

#ifndef SCTF_COMPARATOR_EQUALS_HPP
#define SCTF_COMPARATOR_EQUALS_HPP

#include <algorithm>
#include <cmath>
#include <limits>

#include "common/traits.hpp"
#include "comparator/comparators.hpp"

namespace sctf
{
namespace private_
{
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

    template<typename V, typename E = V, ENABLE_IF(NOT IS_FLOAT(V) AND IS_EQUAL_COMPARABLE(V, E))>
    comparison operator()(V const& actual_value, E const& expected_value) {
        return (actual_value == expected_value) != m_neg ?
                   SUCCESS :
                   comparison(m_neg ? m_neg_cmp_str : m_cmp_str, to_string(actual_value),
                              to_string(expected_value));
    }

    template<typename V, typename E = V,
             ENABLE_IF(NOT IS_FLOAT(V) AND NOT IS_EQUAL_COMPARABLE(V, E)
                           AND                 IS_UNEQUAL_COMPARABLE(V, E))>
    comparison operator()(V const& actual_value, E const& expected_value) {
        return (actual_value != expected_value) != m_neg ?
                   comparison(m_neg ? m_neg_cmp_str : m_cmp_str, to_string(actual_value),
                              to_string(expected_value)) :
                   SUCCESS;
    }

    template<typename V, typename E = V, ENABLE_IF(IS_FLOAT(V) AND IS_FLOAT(E))>
    comparison operator()(V const& actual_value, E const& expected_value) {
#if defined(SCTF_EXTERN_EPSILON) || defined(SCTF_EPSILON)
        static V epsilon_ = static_cast<V>(epsilon);
#else
        static V epsilon_ = std::numeric_limits<V>::epsilon();
#endif
        return (std::abs(actual_value - expected_value) <=
                std::max(std::abs(actual_value), std::abs(expected_value)) * epsilon_) != m_neg ?
                   SUCCESS :
                   comparison(m_neg ? m_neg_cmp_str : m_cmp_str, to_string(actual_value),
                              to_string(expected_value));
    }
};
}  // namespace private_
}  // namespace sctf

PROVIDE_COMPARATOR(equals, EQUALS)
PROVIDE_COMPARATOR(equals, EQ)

#endif  // SCTF_COMPARATOR_EQUALS_HPP
