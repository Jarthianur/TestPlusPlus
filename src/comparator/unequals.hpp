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

#ifndef SRC_COMPARATOR_UNEQUALS_HPP_
#define SRC_COMPARATOR_UNEQUALS_HPP_

#include <algorithm>
#include <cmath>
#include <limits>

#include "comparators.hpp"

/**
 * Define an unequals comparator.
 */
COMPARATOR(unequals, "to be unequals", value != expect)

/**
 * Provide a Comparator shortwrite.
 */
PROVIDE_COMPARATOR(unequals, UNEQUALS)
PROVIDE_COMPARATOR(unequals, UNEQ)

namespace sctf
{
namespace comp
{
/**
 * Specialized unequals for type 'double'.
 * Takes care about floating point precision.
 */
template<typename T,
         typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
Comparison unequals(const T& value, const T& expect)
{
    T diff_abs = std::abs(value - expect);
    T max      = std::max(std::abs(value), std::abs(expect));
    return (diff_abs <= max * std::numeric_limits<T>::epsilon()
            || diff_abs <= max * static_cast<T>(0.000001))
               ? Comparison(unequals_comp_str, util::serialize(value),
                            util::serialize(expect))
               : success;
}

}  // namespace comp
}  // namespace sctf

#endif  // SRC_COMPARATOR_UNEQUALS_HPP_
