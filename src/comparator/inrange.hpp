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

#ifndef SCTF_SRC_COMPARATOR_CONTAINS_HPP_
#define SCTF_SRC_COMPARATOR_CONTAINS_HPP_

#include <algorithm>
#include <utility>

#include "../util/interval.hpp"
#include "../util/traits.hpp"

#include "comparators.hpp"

namespace sctf
{
namespace comp
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
template<typename V, typename R,
         typename std::enable_if<util::is_iterable<R>::value and
                                 not std::is_same<R, std::string>::value>::type* = nullptr>
static Comparison in_range(const V& value, const R& range)
{
    return std::find(range.begin(), range.end(), value) != range.end() ?
               success :
               Comparison(in_range_comp_str, util::serialize(value), util::serialize(range));
}

/**
 * @brief Check for string types to contain a value.
 * @tparam V The value type
 * @tparam R The range/container type
 * @param value The value to check
 * @param range The range/container to search
 * @return whether the value was found
 */
template<typename V, typename R = V,
         typename std::enable_if<std::is_same<R, std::string>::value>::type* = nullptr>
Comparison in_range(const V& value, const R& range)
{
    return range.find(value) != std::string::npos ?
               success :
               Comparison(in_range_comp_str, util::serialize(value), util::serialize(range));
}

/**
 * @brief Check for pair to contain a value, where the value provides an equality
 * operator.
 * @tparam V The value type
 * @tparam R The range/container type
 * @param value The value to check
 * @param range The range/container to search
 * @return whether the value was found
 */
template<typename V, typename R,
         typename std::enable_if<std::is_same<R, std::pair<V, V>>::value and
                                 util::is_equal_comparable<V, V>::value>::type* = nullptr>
Comparison in_range(const V& value, const R& range)
{
    return value == range.first || value == range.second ?
               success :
               Comparison(in_range_comp_str, util::serialize(value), util::serialize(range));
}

/**
 * @brief Check for pair to contain a value, where the value provides an unequality, but
 * no equality operator.
 * @tparam V The value type
 * @tparam R The range/container type
 * @param value The value to check
 * @param range The range/container to search
 * @return whether the value was found
 */
template<typename V, typename R,
         typename std::enable_if<std::is_same<R, std::pair<V, V>>::value and
                                 not util::is_equal_comparable<V, V>::value and
                                 util::is_unequal_comparable<V, V>::value>::type* = nullptr>
Comparison in_range(const V& value, const R& range)
{
    return value != range.first && value != range.second ?
               Comparison(in_range_comp_str, util::serialize(value), util::serialize(range)) :
               success;
}

/**
 * @brief Check for a value to be in interval's lower and upper bounds.
 * @tparam V The value type
 * @tparam R The range/container type
 * @param value The value to check
 * @param range The range/container to search
 * @return whether the value was found
 */
template<typename V, typename R,
         typename std::enable_if<std::is_same<R, util::interval<V>>::value>::type* = nullptr>
Comparison in_range(const V& value, const R& bounds)
{
    return value < bounds.lower || value > bounds.upper ?
               Comparison(in_range_comp_str, util::serialize(value), util::serialize(bounds)) :
               success;
}

}  // namespace comp
}  // namespace sctf

/**
 * Provide a Comparator shortwrite
 */
PROVIDE_COMPARATOR(in_range, IN_RANGE)
PROVIDE_COMPARATOR(in_range, IN)

#endif  // SCTF_SRC_COMPARATOR_CONTAINS_HPP_
