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

#ifndef SRC_UTIL_INTERVAL_HPP_
#define SRC_UTIL_INTERVAL_HPP_

#include "traits.hpp"

namespace sctf
{
namespace util
{
/**
 * @brief A utility used for bounds checking for in_range comparator.
 * @tparam T The type used as bounds
 * @note T must provide ordinal relation with operators < and >.
 */
template<typename T, typename std::enable_if<is_ordinal<T>::value>::type* = nullptr>
struct Interval final
{
    Interval(const T& l, const T& u) : lower(l), upper(u)
    {}

    const T lower;
    const T upper;
};

}  // namespace util
}  // namespace sctf

#endif  // SRC_UTIL_INTERVAL_HPP_
