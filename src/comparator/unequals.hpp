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

#ifndef SRC_COMPARATOR_UNEQUALS_HPP
#define SRC_COMPARATOR_UNEQUALS_HPP

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

namespace sctf
{
namespace comp
{
/**
 * Specialized unequals for type 'double'.
 * Takes care about floating point precision.
 */
template<>
Comparison unequals<double>(const double& value, const double& expect)
{
    double diff_abs = std::abs(value - expect);
    double max      = std::max(std::abs(value), std::abs(expect));
    return (diff_abs <= max * std::numeric_limits<double>::epsilon()
            || diff_abs <= max * 0.000001)
               ? Comparison(unequals_comp_str, util::serialize<double>(value),
                            util::serialize<double>(expect))
               : success;
}

/**
 * Specialized unequals for type 'float'.
 * Takes care about floating point precision.
 */
template<>
Comparison unequals<float>(const float& value, const float& expect)
{
    float diff_abs = std::abs(value - expect);
    float max      = std::max(std::abs(value), std::abs(expect));
    return (diff_abs <= max * std::numeric_limits<float>::epsilon())
               ? Comparison(unequals_comp_str, util::serialize<float>(value),
                            util::serialize<float>(expect))
               : success;
}

}  // namespace comp
}  // namespace sctf

#endif  // SRC_COMPARATOR_UNEQUALS_HPP
