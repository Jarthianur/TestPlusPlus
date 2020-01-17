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

#ifndef SCTF_COMPARATOR_UNEQUALS_HPP
#define SCTF_COMPARATOR_UNEQUALS_HPP

#include <algorithm>
#include <cmath>
#include <limits>

#include "common/traits.hpp"
#include "comparator/comparators.hpp"

namespace sctf
{
namespace _
{
// ENABLE_IF(NOT IS_FLOAT(V) AND IS_UNEQUAL_COMPARABLE(V, E))
template<typename V, typename E = V, typename = void>
class unequals  //<V,E,std::enable_if<!std::is_floating_point<V>::value &&
                //_::is_unequal_comparable<V,E>::value>>
{
    static constexpr char const* m_cmp_str     = "to be unequals";
    static constexpr char const* m_neg_cmp_str = "to be not unequals";
    bool                         m_neg         = false;

public:
    unequals()           = default;
    ~unequals() noexcept = default;
    comparison operator()(V const& actual_value, E const& expected_value)
    {
        return (actual_value != expected_value) != m_neg ?
                   SUCCESS :
                   comparison(m_neg ? m_neg_cmp_str : m_cmp_str, to_string(actual_value),
                              to_string(expected_value));
    }
    unequals& operator!()
    {
        m_neg = !m_neg;
        return *this;
    }
};

// ENABLE_IF(NOT IS_FLOAT(V) AND NOT IS_UNEQUAL_COMPARABLE(V, E) AND IS_EQUAL_COMPARABLE(V, E))
template<typename V, typename E>
class unequals<
    V, E,
    std::enable_if<!std::is_floating_point<V>::value && !is_unequal_comparable<V, E>::value &&
                   is_equal_comparable<V, E>::value>>
{
    static constexpr char const* m_cmp_str     = "to be unequals";
    static constexpr char const* m_neg_cmp_str = "to be not unequals";
    bool                         m_neg         = false;

public:
    unequals()           = default;
    ~unequals() noexcept = default;
    comparison operator()(V const& actual_value, E const& expected_value)
    {
        return (actual_value == expected_value) != m_neg ?
                   comparison(m_neg ? m_neg_cmp_str : m_cmp_str, to_string(actual_value),
                              to_string(expected_value)) :
                   SUCCESS;
    }
    unequals& operator!()
    {
        m_neg = !m_neg;
        return *this;
    }
};

// ENABLE_IF(IS_FLOAT(V) AND IS_FLOAT(E))
template<typename V, typename E>
class unequals<V, E,
               std::enable_if<std::is_floating_point<V>::value && std::is_floating_point<E>::value>>
{
    static constexpr char const* m_cmp_str     = "to be unequals";
    static constexpr char const* m_neg_cmp_str = "to be not unequals";
    bool                         m_neg         = false;

public:
    unequals()           = default;
    ~unequals() noexcept = default;
    comparison operator()(V const& actual_value, E const& expected_value)
    {
#if defined(SCTF_EXTERN_EPSILON) || defined(SCTF_EPSILON)
        static V epsilon_ = static_cast<V>(epsilon);
#else
        static V epsilon_ = std::numeric_limits<V>::epsilon();
#endif
        return (std::abs(actual_value - expected_value) <=
                std::max(std::abs(actual_value), std::abs(expected_value)) * epsilon_) != m_neg ?
                   comparison(m_neg ? m_neg_cmp_str : m_cmp_str, to_string(actual_value),
                              to_string(expected_value)) :
                   SUCCESS;
    }
    unequals& operator!()
    {
        m_neg = !m_neg;
        return *this;
    }
};
}  // namespace _
}  // namespace sctf

PROVIDE_COMPARATOR(unequals, UNEQUALS)
PROVIDE_COMPARATOR(unequals, NE)

#endif  // SCTF_COMPARATOR_UNEQUALS_HPP
