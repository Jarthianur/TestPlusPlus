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
/// @brief The constraint string for unequals.
constexpr const char* unequals_comp_str = "to be unequals";

/**
 * @brief Check a value to be unequal to an expected.
 * @note Applies to all non-floating-point types for V, which provide an unequality
 * operator.
 * @tparam V The type of value
 * @tparam E The type of expect
 * @param value The value to check
 * @param expect The expected value
 * @return true if value is unequals expect, else false
 */
template<typename V, typename E = V, ENABLE_IF(NOT IS_FLOAT(V) AND IS_UNEQUAL_COMPARABLE(V, E))>
static comparison unequals(V const& val_, E const& expect_)
{
    return val_ != expect_ ? SUCCESS :
                             comparison(unequals_comp_str, to_string(val_), to_string(expect_));
}

/**
 * @brief Check a value to be unequal to an expected.
 * @note Applies to all non-floating-point types for V, which provide an equality, but no
 * unequality operator.
 * @tparam V The type of value
 * @tparam E The type of expect
 * @param value The value to check
 * @param expect The expected value
 * @return true if value is unequals expect, else false
 */
template<typename V, typename E = V,
         ENABLE_IF(NOT IS_FLOAT(V) AND NOT IS_UNEQUAL_COMPARABLE(V, E)
                       AND                 IS_EQUAL_COMPARABLE(V, E))>
static comparison unequals(const V& val_, const E& expect_)
{
    return val_ == expect_ ? comparison(unequals_comp_str, to_string(val_), to_string(expect_)) :
                             SUCCESS;
}

/**
 * @brief Check a value to be unequal to an expected.
 * @note Applies to all floating-point types for V.
 * @tparam V The type of value
 * @tparam E The type of expect
 * @param value The value to check
 * @param expect The expected value
 * @return true if value is unequals expect, else false
 */
template<typename V, typename E = V, ENABLE_IF(IS_FLOAT(V) AND IS_FLOAT(E))>
comparison unequals(const V& val_, const E& expect_)
{
#if defined(SCTF_EXTERN_EPSILON) || defined(SCTF_EPSILON)
    static V epsilon_ = static_cast<V>(epsilon);
#else
    static V epsilon_ = std::numeric_limits<V>::epsilon();
#endif
    return (std::abs(val_ - expect_) <= std::max(std::abs(val_), std::abs(expect_)) * epsilon_) ?
               comparison(unequals_comp_str, to_string(val_), to_string(expect_)) :
               SUCCESS;
}
}  // namespace _
}  // namespace sctf

/**
 * Provide a Comparator shortwrite.
 */
PROVIDE_COMPARATOR(unequals, UNEQUALS)
PROVIDE_COMPARATOR(unequals, NE)

#endif  // SCTF_COMPARATOR_UNEQUALS_HPP
