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

#ifndef TPP_ASSERT_REGEX_HPP
#define TPP_ASSERT_REGEX_HPP

#include <regex>

#include "assert/assert.hpp"

namespace tpp
{
namespace intern
{
namespace assert
{
namespace ns_match
{
static constexpr auto CMP_STR     = "to match";
static constexpr auto NEG_CMP_STR = "to not match";
}  // namespace ns_match
namespace ns_like
{
static constexpr auto CMP_STR     = "to be like";
static constexpr auto NEG_CMP_STR = "to be not like";
}  // namespace ns_like

struct assert_match
{
    template<typename R, typename V, typename E = V>
    assert_match(V&& v_, E&& e_, R& r_, bool neg_, loc&& loc_) {
        if (std::regex_match(v_, r_, std::regex(e_)) == neg_) {
            fail_assertion(
              std::forward_as_tuple(to_string(v_), (neg_ ? ns_match::NEG_CMP_STR : ns_match::CMP_STR), to_string(e_)),
              loc_);
        }
    }

    template<typename V, typename E = V>
    assert_match(V&& v_, E&& e_, bool neg_, loc&& loc_) {
        if (std::regex_match(v_, std::regex(e_)) == neg_) {
            fail_assertion(
              std::forward_as_tuple(to_string(v_), (neg_ ? ns_match::NEG_CMP_STR : ns_match::CMP_STR), to_string(e_)),
              loc_);
        }
    }
};

struct assert_like
{
    template<typename R, typename V, typename E = V>
    assert_like(V&& v_, E&& e_, R& r_, bool neg_, loc&& loc_) {
        if (std::regex_search(v_, r_, std::regex(e_)) == neg_) {
            fail_assertion(
              std::forward_as_tuple(to_string(v_), (neg_ ? ns_like::NEG_CMP_STR : ns_like::CMP_STR), to_string(e_)),
              loc_);
        }
    }

    template<typename V, typename E = V>
    assert_like(V&& v_, E&& e_, bool neg_, loc&& loc_) {
        if (std::regex_search(v_, std::regex(e_)) == neg_) {
            fail_assertion(
              std::forward_as_tuple(to_string(v_), (neg_ ? ns_like::NEG_CMP_STR : ns_like::CMP_STR), to_string(e_)),
              loc_);
        }
    }
};
}  // namespace assert
}  // namespace intern
}  // namespace tpp

TPP_PROVIDE_ASSERTION(assert_match, MATCH)
TPP_PROVIDE_ASSERTION(assert_like, LIKE)

#define ASSERT_MATCH(...)                                                   \
    tpp::intern::assert::make_assertion<tpp::intern::assert::assert_match>( \
      __VA_ARGS__, false, tpp::intern::assert::loc{__FILE__, __LINE__})

#define ASSERT_NOT_MATCH(...)                                               \
    tpp::intern::assert::make_assertion<tpp::intern::assert::assert_match>( \
      __VA_ARGS__, true, tpp::intern::assert::loc{__FILE__, __LINE__})

#define ASSERT_LIKE(...)                                                   \
    tpp::intern::assert::make_assertion<tpp::intern::assert::assert_like>( \
      __VA_ARGS__, false, tpp::intern::assert::loc{__FILE__, __LINE__})

#define ASSERT_NOT_LIKE(...)                                               \
    tpp::intern::assert::make_assertion<tpp::intern::assert::assert_like>( \
      __VA_ARGS__, true, tpp::intern::assert::loc{__FILE__, __LINE__})

#endif  // TPP_ASSERT_REGEX_HPP
