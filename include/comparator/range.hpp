/*
    Copyright (C) 2017 Jarthianur

    This file is part of simple-cpp-test-framework.

    simple-cpp-test-framework is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    simple-cpp-test-framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with simple-cpp-test-framework.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef SCTF_COMPARATOR_RANGE_HPP
#define SCTF_COMPARATOR_RANGE_HPP

#include <algorithm>

#include "comparator/comparator.hpp"

#include "traits.hpp"

namespace sctf
{
namespace intern
{
/**
 * Comparator to check for existence of elements in ranges.
 */
class in_range
{
    static constexpr char const* m_cmp_str     = "to be in range of";
    static constexpr char const* m_neg_cmp_str = "to be not in range of";
    bool                         m_neg         = false;

public:
    auto
    operator!() -> in_range& {
        m_neg = !m_neg;
        return *this;
    }

    template<typename V, typename E = V,
             SCTF_INTERN_ENABLE_IF(SCTF_INTERN_HAS_ITERATOR_CAPABILITY(E) && !SCTF_INTERN_IS_TYPE(E, std::string))>
    auto
    operator()(V const& actual_value, E const& expected_value) const -> comparison {
        return (std::find(expected_value.cbegin(), expected_value.cend(), actual_value) != expected_value.cend()) !=
                   m_neg ?
                 comparison() :
                 comparison(m_neg ? m_neg_cmp_str : m_cmp_str,
                            std::forward_as_tuple(to_string(actual_value), to_string(expected_value)));
    }

    template<typename V, typename E = V, SCTF_INTERN_ENABLE_IF(SCTF_INTERN_IS_TYPE(E, std::string))>
    auto
    operator()(V const& actual_value, E const& expected_value) const -> comparison {
        return (expected_value.find(actual_value) != std::string::npos) != m_neg ?
                 comparison() :
                 comparison(m_neg ? m_neg_cmp_str : m_cmp_str,
                            std::forward_as_tuple(to_string(actual_value), to_string(expected_value)));
    }
};
}  // namespace intern
}  // namespace sctf

SCTF_PROVIDE_COMPARATOR(in_range, IN_RANGE)
SCTF_PROVIDE_COMPARATOR(in_range, IN)

#endif  // SCTF_COMPARATOR_RANGE_HPP
