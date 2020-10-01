/*
    Copyright (C) 2017  Jarthianur

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef TPP_COMPARE_RANGE_HPP
#define TPP_COMPARE_RANGE_HPP

#include <algorithm>

#include "compare/comparator.hpp"

#include "traits.hpp"

namespace tpp
{
namespace intern
{
namespace compare
{
namespace ns_in_range
{
static constexpr auto CMP_STR     = "to be in range of";
static constexpr auto NEG_CMP_STR = "to be not in range of";
}  // namespace ns_in_range
/**
 * Comparator to check for existence of elements in ranges.
 */
template<typename>
class in_range
{
    bool m_neg{false};

public:
    auto
    operator!() -> decltype(*this)& {
        m_neg = !m_neg;
        return *this;
    }

    template<typename V, typename E = V,
             TPP_INTERN_ENABLE_IF(TPP_INTERN_HAS_ITERATOR_CAPABILITY(E) && !TPP_INTERN_IS_TYPE(E, std::string))>
    auto
    operator()(V const& actual_value, E const& expected_value) const -> comparison {
        return (std::find(expected_value.cbegin(), expected_value.cend(), actual_value) != expected_value.cend()) !=
                   m_neg ?
                 comparison() :
                 comparison(m_neg ? ns_in_range::NEG_CMP_STR : ns_in_range::CMP_STR,
                            std::forward_as_tuple(to_string(actual_value), to_string(expected_value)));
    }

    template<typename V, typename E = V, TPP_INTERN_ENABLE_IF(TPP_INTERN_IS_TYPE(E, std::string))>
    auto
    operator()(V const& actual_value, E const& expected_value) const -> comparison {
        return (expected_value.find(actual_value) != std::string::npos) != m_neg ?
                 comparison() :
                 comparison(m_neg ? ns_in_range::NEG_CMP_STR : ns_in_range::CMP_STR,
                            std::forward_as_tuple(to_string(actual_value), to_string(expected_value)));
    }
};
}  // namespace compare
}  // namespace intern
}  // namespace tpp

TPP_PROVIDE_COMPARATOR(in_range, IN_RANGE)
TPP_PROVIDE_COMPARATOR(in_range, IN)

#endif  // TPP_COMPARE_RANGE_HPP
