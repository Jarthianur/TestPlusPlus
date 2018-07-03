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

#ifndef COMPARATOR_LESS_HPP_
#define COMPARATOR_LESS_HPP_

#include <string>

#include "../util/serialize.hpp"
#include "comparators.hpp"

namespace testsuite
{
namespace comparator
{
constexpr const char* less_comp = "to be less than";

template<typename T>
inline static Comparison less(const T& _1, const T& _2)
{
    return _1 < _2 ? success
                   : Comparison(less_comp, util::serialize(_1), util::serialize(_2));
}

PROVIDE_COMPARATOR(less, LESS)

}  // comparator
}  // testsuite

#endif /* COMPARATOR_LESS_HPP_ */
