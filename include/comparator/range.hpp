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

#ifndef SCTF_COMPARATOR_RANGE_HPP
#define SCTF_COMPARATOR_RANGE_HPP

#include <algorithm>

#include "comparator/comparator.hpp"

#include "traits.hpp"

namespace sctf
{
namespace private_
{
class in_range
{
    static constexpr char const* m_cmp_str     = "to be in range of";
    static constexpr char const* m_neg_cmp_str = "to be not in range of";
    bool                         m_neg         = false;

public:
    in_range& operator!() {
        m_neg = !m_neg;
        return *this;
    }

    template<typename V, typename E = V,
             ENABLE_IF(HAS_ITERATOR_CAPABILITY(E) AND NOT IS_TYPE(E, std::string))>
    comparison operator()(V const& actual_value, E const& expected_value) {
        return (std::find(expected_value.cbegin(), expected_value.cend(), actual_value) !=
                expected_value.cend()) != m_neg ?
                   comparison() :
                   comparison(m_neg ? m_neg_cmp_str : m_cmp_str, to_string(actual_value),
                              to_string(expected_value));
    }

    template<typename V, typename E = V, ENABLE_IF(IS_TYPE(E, std::string))>
    comparison operator()(V const& actual_value, E const& expected_value) {
        return (expected_value.find(actual_value) != std::string::npos) != m_neg ?
                   comparison() :
                   comparison(m_neg ? m_neg_cmp_str : m_cmp_str, to_string(actual_value),
                              to_string(expected_value));
    }
};

}  // namespace private_
}  // namespace sctf

PROVIDE_COMPARATOR(in_range, IN_RANGE)
PROVIDE_COMPARATOR(in_range, IN)

#endif  // SCTF_COMPARATOR_RANGE_HPP
