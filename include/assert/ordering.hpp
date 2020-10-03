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

#ifndef TPP_ASSERT_ORDERING_HPP
#define TPP_ASSERT_ORDERING_HPP

#include "assert/assert.hpp"

TPP_DEFINE_ASSERTION(assert_greater, "greater than", v_ > e_)
TPP_DEFINE_ASSERTION(assert_less, "less than", v_ < e_)

namespace tpp
{
namespace intern
{
namespace assert
{
using GT = assert_greater;
using LT = assert_less;
}  // namespace assert
}  // namespace intern
}  // namespace tpp

#define ASSERT_GT(...)                                                        \
    tpp::intern::assert::make_assertion<tpp::intern::assert::assert_greater>( \
      __VA_ARGS__, false, tpp::intern::assert::loc{__FILE__, __LINE__})

#define ASSERT_NOT_GT(...)                                                    \
    tpp::intern::assert::make_assertion<tpp::intern::assert::assert_greater>( \
      __VA_ARGS__, true, tpp::intern::assert::loc{__FILE__, __LINE__})

#define ASSERT_LT(...)                                                     \
    tpp::intern::assert::make_assertion<tpp::intern::assert::assert_less>( \
      __VA_ARGS__, false, tpp::intern::assert::loc{__FILE__, __LINE__})

#define ASSERT_NOT_LT(...)                                                 \
    tpp::intern::assert::make_assertion<tpp::intern::assert::assert_less>( \
      __VA_ARGS__, true, tpp::intern::assert::loc{__FILE__, __LINE__})

#endif  // TPP_ASSERT_ORDERING_HPP
