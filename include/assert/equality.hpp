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

/// @file

#ifndef TPP_ASSERT_EQUALITY_HPP
#define TPP_ASSERT_EQUALITY_HPP

#include <algorithm>
#include <cmath>
#include <tuple>

#include "assert/assert.hpp"

#include "traits.hpp"

/**
 * Define a global epsilon value, that will be used for every floating point equality comparison by
 * default. This can be used only once per linked binary.
 *
 * @param E is the epsilon value.
 */
#define TPP_EPSILON(E) double tpp::epsilon = E;

namespace tpp
{
/// Epsilon that is used as floating point threshold in equality comparisons.
extern double epsilon;

namespace intern
{
namespace assert
{
namespace ns_equals
{
static constexpr auto CMP_STR     = "to be equals";
static constexpr auto NEG_CMP_STR = "to be not equals";
}  // namespace ns_equals

struct assert_equals
{
    template<typename V, typename E = V, TPP_INTERN_ENABLE_IF(!TPP_INTERN_IS_FLOAT(V) || !TPP_INTERN_IS_FLOAT(E))>
    assert_equals(V&& v_, E&& e_, bool neg_, loc&& loc_) {
        if ((v_ == e_) == neg_) {
            fail_assertion(
              std::forward_as_tuple(to_string(v_), (neg_ ? ns_equals::NEG_CMP_STR : ns_equals::CMP_STR), to_string(e_)),
              loc_);
        }
    }

    template<typename F, typename V, typename E = V,
             TPP_INTERN_ENABLE_IF(TPP_INTERN_IS_FLOAT(V) && TPP_INTERN_IS_FLOAT(E))>
    assert_equals(V&& v_, E&& e_, F&& eps_, bool neg_, loc&& loc_) {
        static_assert(TPP_INTERN_IS_FLOAT(F),
                      "An epsilon used in floating point comparison must be itself a floating point!");

        if ((std::abs(v_ - e_) <= std::max(std::abs(v_), std::abs(e_)) * eps_) == neg_) {
            fail_assertion(
              std::forward_as_tuple(to_string(v_), (neg_ ? ns_equals::NEG_CMP_STR : ns_equals::CMP_STR), to_string(e_)),
              loc_);
        }
    }

    template<typename V, typename E = V, TPP_INTERN_ENABLE_IF(TPP_INTERN_IS_FLOAT(V) && TPP_INTERN_IS_FLOAT(E))>
    assert_equals(V&& v_, E&& e_, bool neg_, loc&& loc_) {
        typename std::decay<E>::type eps_ = static_cast<E>(epsilon);
        if ((std::abs(v_ - e_) <= std::max(std::abs(v_), std::abs(e_)) * eps_) == neg_) {
            fail_assertion(
              std::forward_as_tuple(to_string(v_), (neg_ ? ns_equals::NEG_CMP_STR : ns_equals::CMP_STR), to_string(e_)),
              loc_);
        }
    }
};
}  // namespace assert
}  // namespace intern
}  // namespace tpp

TPP_PROVIDE_ASSERTION(assert_equals, EQ)

/**
 * Assert two values to be equal.
 * This is equivalent to using ASSERT with EQUALS comparator.
 *
 * EXAMPLE:
 * @code
 * ASSERT_EQ(1, 1);
 * @endcode
 */
#define ASSERT_EQ(...)                                                       \
    tpp::intern::assert::make_assertion<tpp::intern::assert::assert_equals>( \
      __VA_ARGS__, false, tpp::intern::assert::loc{__FILE__, __LINE__})

#define ASSERT_NOT_EQ(...)                                                   \
    tpp::intern::assert::make_assertion<tpp::intern::assert::assert_equals>( \
      __VA_ARGS__, true, tpp::intern::assert::loc{__FILE__, __LINE__})

#endif  // TPP_ASSERT_EQUALITY_HPP
