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

#ifndef SCTF_COMPARATOR_ORDERING_HPP
#define SCTF_COMPARATOR_ORDERING_HPP

#include "comparator/comparator.hpp"

SCTF_COMPARATOR(greater_than, "greater than", actual_value > expected_value)
SCTF_PROVIDE_COMPARATOR(greater_than, GREATER_THAN)
SCTF_PROVIDE_COMPARATOR(greater_than, GREATER)
SCTF_PROVIDE_COMPARATOR(greater_than, GT)

SCTF_COMPARATOR(less_than, "less than", actual_value < expected_value)
SCTF_PROVIDE_COMPARATOR(less_than, LESS_THAN)
SCTF_PROVIDE_COMPARATOR(less_than, LESS)
SCTF_PROVIDE_COMPARATOR(less_than, LT)

#endif  // SCTF_COMPARATOR_ORDERING_HPP
