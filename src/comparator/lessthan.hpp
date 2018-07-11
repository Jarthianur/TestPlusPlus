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

#ifndef SRC_COMPARATOR_LESS_HPP_
#define SRC_COMPARATOR_LESS_HPP_

#include "comparators.hpp"

/**
 * Define a less comparator.
 */
COMPARATOR(less_than, "to be less than", value < expect)

/**
 * Provide a Comparator shortwrite.
 */
PROVIDE_COMPARATOR(less_than, LESS)
PROVIDE_COMPARATOR(less_than, LT)

#endif  // SRC_COMPARATOR_LESS_HPP_
