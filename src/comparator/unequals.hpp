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

#ifndef SCTF_SRC_COMPARATOR_UNEQUALS_HPP_
#define SCTF_SRC_COMPARATOR_UNEQUALS_HPP_

#include <algorithm>
#include <cmath>
#include <limits>

#include "comparators.hpp"

namespace sctf
{
namespace comp
{
/// @brief constraint string
constexpr const char* unequals_comp_str = "to be unequals";

/**
 * @brief Check a value to be unequal to an expected.
 * @note Applies to all non-floating-point types for V.
 * @tparam V The type of value
 * @tparam E The type of expect
 * @param value The value to check
 * @param expect The expected value
 * @return true if value is unequals expect, else false
 */
template<typename V, typename E = V,
         typename std::enable_if<not std::is_floating_point<V>::value>::type* = nullptr>
static Comparison unequals(const V& value, const E& expect)
{
    return (value != expect) ? success
                             : Comparison(unequals_comp_str, util::serialize(value),
                                          util::serialize(expect));
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
template<typename V, typename E = V,
         typename std::enable_if<std::is_floating_point<V>::value>::type* = nullptr>
Comparison unequals(const V& value, const E& expect)
{
#ifdef SCTF_CUSTOM_EPSILON
    static V epsilon = SCTF_CUSTOM_EPSILON;
#else
    static V epsilon = std::numeric_limits<V>::epsilon();
#endif
    return (std::abs(value - expect)
            <= std::max(std::abs(value), std::abs(expect)) * epsilon)
               ? Comparison(unequals_comp_str, util::serialize(value),
                            util::serialize(expect))
               : success;
}

}  // namespace comp
}  // namespace sctf

/**
 * Provide a Comparator shortwrite.
 */
PROVIDE_COMPARATOR(unequals, UNEQUALS)
PROVIDE_COMPARATOR(unequals, NE)

#endif  // SCTF_SRC_COMPARATOR_UNEQUALS_HPP_
