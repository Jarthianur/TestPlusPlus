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

#ifndef COMPARATOR_EQUALS_HPP_
#define COMPARATOR_EQUALS_HPP_

#include <algorithm>
#include <cmath>
#include <limits>
#include <string>

#include "../util/serialize.hpp"
#include "comparators.hpp"

namespace testsuite
{
namespace comparator
{
constexpr const char* equals_comp = "to be equal";

template<typename T>
inline static Comparison equals(const T& _1, const T& _2)
{
    return _1 == _2 ? success
                    : Comparison(equals_comp, util::serialize(_1), util::serialize(_2));
}

/**
 * Specialized compare for type 'double'.
 * Takes care about floating point precision.
 */
template<>
inline Comparison equals<double>(const double& val, const double& expect)
{
    double diff_abs = std::abs(val - expect);
    double max      = std::max(std::abs(val), std::abs(expect));

    return (diff_abs < max * std::numeric_limits<double>::epsilon()
            || diff_abs < max * 0.000001)
               ? success
               : Comparison(equals_comp, util::serialize<double>(val),
                            util::serialize<double>(expect));
}

/**
 * Specialized compare for type 'float'.
 * Takes care about floating point precision.
 */
template<>
inline Comparison equals<float>(const float& val, const float& expect)
{
    float diff_abs = std::abs(val - expect);
    float max      = std::max(std::abs(val), std::abs(expect));

    return (diff_abs < max * std::numeric_limits<float>::epsilon()
            || diff_abs < max * static_cast<float>(0.000001))
               ? success
               : Comparison(equals_comp, util::serialize<float>(val),
                            util::serialize<float>(expect));
}

PROVIDE_COMPARATOR(equals, EQUALS)

}  // namespace comparator
}  // namespace testsuite

#endif /* COMPARATOR_EQUALS_HPP_ */
