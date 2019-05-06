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

#ifndef SCTF_COMPARATOR_EQUALS_HPP_
#define SCTF_COMPARATOR_EQUALS_HPP_

#include <algorithm>
#include <cmath>
#include <limits>

#include "common/traits.hpp"
#include "comparator/comparators.hpp"

namespace sctf
{
namespace _
{
/// @brief The constraint string for equals.
constexpr const char* equals_comp_str = "to be equals";

/**
 * @brief Check a value to be equal to an expected.
 * @note Applies to all non-floating-point types for V, which provide an equality
 * operator.
 * @tparam V The type of value
 * @tparam E The type of expect
 * @param value The value to check
 * @param expect The expected value
 * @return true if value is equals expect, else false
 */
template<typename V, typename E = V, ENABLE_IF(NOT IS_FLOAT(V) AND IS_EQUAL_COMPARABLE(V, E))>
static comparison equals(const V& value, const E& expect)
{
    return value == expect ? success :
                             comparison(equals_comp_str, to_string(value), to_string(expect));
}

/**
 * @brief Check a value to be equal to an expected.
 * @note Applies to all non-floating-point types for V, which don't provide an equality,
 * but an unequality operator.
 * @tparam V The type of value
 * @tparam E The type of expect
 * @param value The value to check
 * @param expect The expected value
 * @return true if value is equals expect, else false
 */
template<typename V, typename E = V,
         ENABLE_IF(NOT IS_FLOAT(V) AND NOT IS_EQUAL_COMPARABLE(V, E)
                       AND                 IS_UNEQUAL_COMPARABLE(V, E))>
static comparison equals(const V& value, const E& expect)
{
    return value != expect ? comparison(equals_comp_str, to_string(value), to_string(expect)) :
                             success;
}

/**
 * @brief Check a value to be equal to an expected.
 * @note Applies to all floating-point types for V.
 * @tparam V The type of value
 * @tparam E The type of expect
 * @param value The value to check
 * @param expect The expected value
 * @return true if value is equals expect, else false
 */
template<typename V, typename E = V, ENABLE_IF(IS_FLOAT(V) AND IS_FLOAT(E))>
comparison equals(const V& value, const E& expect)
{
#if defined(SCTF_EXTERN_EPSILON) || defined(SCTF_EPSILON)
    static V epsilon_ = static_cast<V>(epsilon);
#else
    static V epsilon_ = std::numeric_limits<V>::epsilon();
#endif
    return (std::abs(value - expect) <= std::max(std::abs(value), std::abs(expect)) * epsilon_) ?
               success :
               comparison(equals_comp_str, to_string(value), to_string(expect));
}

}  // namespace _
}  // namespace sctf

/**
 * Provide a Comparator shortwrite
 */
PROVIDE_COMPARATOR(equals, EQUALS)
PROVIDE_COMPARATOR(equals, EQ)

#endif  // SCTF_COMPARATOR_EQUALS_HPP_
