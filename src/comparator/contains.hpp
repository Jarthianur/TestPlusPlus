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

#ifndef SRC_COMPARATOR_CONTAINS_HPP
#define SRC_COMPARATOR_CONTAINS_HPP

#include <algorithm>
#include "../util/serialize.hpp"
#include "comparators.hpp"

COMPARATOR(contains, "to contain",
           std::find(value.begin(), value.end(), expect) != value.end())

PROVIDE_COMPARATOR(contains, CONTAINS)

namespace sctf
{
namespace comp
{
template<>
Comparison contains<std::string>(const std::string& value,
                                        const std::string& expect)
{
    return value.find(expect) != std::string::npos
               ? success
               : Comparison(contains_comp_str, util::serialize(value),
                            util::serialize(expect));
}

template<>
Comparison contains<std::string, const char*>(const std::string& value,
                                                     const char* const& expect)
{
    return value.find(expect) != std::string::npos
               ? success
               : Comparison(contains_comp_str, util::serialize(value),
                            util::serialize(expect));
}
}
}

#endif  // SRC_COMPARATOR_CONTAINS_HPP
