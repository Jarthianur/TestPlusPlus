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

#ifndef SCTF_COMPARATOR_CONTAINS_HPP
#define SCTF_COMPARATOR_CONTAINS_HPP

#include <algorithm>

#include "common/traits.hpp"
#include "comparator/comparators.hpp"

namespace sctf
{
namespace _
{
/// @brief The constraint string for in_range.
constexpr const char* in_range_comp_str = "to be in range of";

/**
 * @brief Check for iterable non-string types to contain a value.
 * @tparam V The value type
 * @tparam R The range/container type
 * @param value The value to check
 * @param range The range/container to search
 * @return whether the value was found
 */
template<typename V, typename R, ENABLE_IF(IS_ITERABLE(R) AND NOT IS_TYPE(R, std::string))>
static comparison in_range(V const& val_, R const& range_)
{
    return std::find(range_.begin(), range_.end(), val_) != range_.end() ?
               SUCCESS :
               comparison(in_range_comp_str, to_string(val_), to_string(range_));
}

/**
 * @brief Check for string types to contain a value.
 * @tparam V The value type
 * @tparam R The range/container type
 * @param value The value to check
 * @param range The range/container to search
 * @return whether the value was found
 */
template<typename V, typename R = V, ENABLE_IF(IS_TYPE(R, std::string))>
comparison in_range(V const& val_, R const& range_)
{
    return range_.find(val_) != std::string::npos ?
               SUCCESS :
               comparison(in_range_comp_str, to_string(val_), to_string(range_));
}
}  // namespace _
}  // namespace sctf

/**
 * Provide a Comparator shortwrite
 */
PROVIDE_COMPARATOR(in_range, IN_RANGE)
PROVIDE_COMPARATOR(in_range, IN)

#endif  // SCTF_COMPARATOR_CONTAINS_HPP
