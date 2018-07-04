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

#ifndef COMPARATOR_UNEQUALS_HPP
#define COMPARATOR_UNEQUALS_HPP

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
constexpr const char* unequals_comp = "to be unequal";

template<typename T>
inline static Comparison unequals(const T& _1, const T& _2)
{
    return _1 == _2 ? Comparison(unequals_comp, util::serialize(_1), util::serialize(_2))
                    : success;
}

/**
 * Specialized compare for type 'double'.
 * Takes care about floating point precision.
 */
template<>
inline Comparison unequals<double>(const double& val, const double& expect)
{
    double diff_abs = std::abs(val - expect);
    double max      = std::max(std::abs(val), std::abs(expect));

    return (diff_abs < max * std::numeric_limits<double>::epsilon()
            || diff_abs < max * 0.000001)
               ? Comparison(unequals_comp, util::serialize<double>(val),
                            util::serialize<double>(expect))
               : success;
}

/**
 * Specialized compare for type 'float'.
 * Takes care about floating point precision.
 */
template<>
inline Comparison unequals<float>(const float& val, const float& expect)
{
    float diff_abs = std::abs(val - expect);
    float max      = std::max(std::abs(val), std::abs(expect));

    return (diff_abs < max * std::numeric_limits<float>::epsilon()
            || diff_abs < max * static_cast<float>(0.000001))
               ? Comparison(unequals_comp, util::serialize<float>(val),
                            util::serialize<float>(expect))
               : success;
}

PROVIDE_COMPARATOR(unequals, UNEQUALS)

}  // namespace comparator
}  // namespace testsuite

#endif  // COMPARATOR_UNEQUALS_HPP
