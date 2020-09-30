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

#ifndef TPP_COMPARE_ORDERING_HPP
#define TPP_COMPARE_ORDERING_HPP

#include "compare/comparator.hpp"

TPP_COMPARATOR(greater_than, "greater than", actual_value > expected_value)
TPP_PROVIDE_COMPARATOR(greater_than, GREATER_THAN)
TPP_PROVIDE_COMPARATOR(greater_than, GREATER)
TPP_PROVIDE_COMPARATOR(greater_than, GT)

TPP_COMPARATOR(less_than, "less than", actual_value < expected_value)
TPP_PROVIDE_COMPARATOR(less_than, LESS_THAN)
TPP_PROVIDE_COMPARATOR(less_than, LESS)
TPP_PROVIDE_COMPARATOR(less_than, LT)

#endif  // TPP_COMPARE_ORDERING_HPP
