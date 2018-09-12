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

#ifndef SCTF_SRC_COMPARATOR_EQUALS_HPP_
#define SCTF_SRC_COMPARATOR_EQUALS_HPP_

#include <algorithm>
#include <cmath>
#include <limits>
#include <type_traits>

#include "comparators.hpp"

/**
 * Define an equals comparator.
 */
COMPARATOR(equals, "to be equals", value == expect)

/**
 * Provide a Comparator shortwrite.
 */
PROVIDE_COMPARATOR(equals, EQUALS)
PROVIDE_COMPARATOR(equals, EQ)

namespace sctf
{
namespace comp
{
/**
 * Specialized equals for type 'double'.
 * Takes care about floating point precision.
 */
template<typename T,
         typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
Comparison equals(const T& value, const T& expect)
{
    return (std::abs(value - expect) <= std::max(std::abs(value), std::abs(expect))
                                            * std::numeric_limits<T>::epsilon())
               ? success
               : Comparison(equals_comp_str, util::serialize(value),
                            util::serialize(expect));
}

}  // namespace comp
}  // namespace sctf

#endif  // SCTF_SRC_COMPARATOR_EQUALS_HPP_
