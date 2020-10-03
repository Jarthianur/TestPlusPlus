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

#ifndef TPP_ASSERT_RANGE_HPP
#define TPP_ASSERT_RANGE_HPP

#include <algorithm>

#include "assert/assert.hpp"

#include "traits.hpp"

namespace tpp
{
namespace intern
{
namespace assert
{
namespace ns_in
{
static constexpr auto CMP_STR     = "to be in";
static constexpr auto NEG_CMP_STR = "to be not in";
}  // namespace ns_in

struct assert_in
{
    template<typename V, typename E = V,
             TPP_INTERN_ENABLE_IF(TPP_INTERN_HAS_ITERATOR_CAPABILITY(E) && !TPP_INTERN_IS_TYPE(E, std::string))>
    assert_in(V&& v_, E&& e_, bool neg_, loc const& loc_) {
        if ((std::find(e_.cbegin(), e_.cend(), v_) != e_.cend()) == neg_) {
            fail_assertion(
              std::forward_as_tuple(to_string(v_), (neg_ ? ns_in::NEG_CMP_STR : ns_in::CMP_STR), to_string(e_)), loc_);
        }
    }

    template<typename V, typename E = V, TPP_INTERN_ENABLE_IF(TPP_INTERN_IS_TYPE(E, std::string))>
    assert_in(V&& v_, E&& e_, bool neg_, loc const& loc_) {
        if ((e_.find(v_) != std::string::npos) == neg_) {
            fail_assertion(
              std::forward_as_tuple(to_string(v_), (neg_ ? ns_in::NEG_CMP_STR : ns_in::CMP_STR), to_string(e_)), loc_);
        }
    }
};

using IN = assert_in;
}  // namespace assert
}  // namespace intern
}  // namespace tpp

#define ASSERT_IN(...)                                                                      \
    tpp::intern::assert::make_assertion<tpp::intern::assert::assert_in>(__VA_ARGS__, false, \
                                                                        tpp::intern::assert::loc{__FILE__, __LINE__})

#define ASSERT_NOT_IN(...)                                                                 \
    tpp::intern::assert::make_assertion<tpp::intern::assert::assert_in>(__VA_ARGS__, true, \
                                                                        tpp::intern::assert::loc{__FILE__, __LINE__})

#endif  // TPP_ASSERT_RANGE_HPP
