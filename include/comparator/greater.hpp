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

#ifndef SCTF_COMPARATOR_GREATER_HPP_
#define SCTF_COMPARATOR_GREATER_HPP_

#include "comparator/comparators.hpp"

/**
 * Define a greater comparator
 */
COMPARATOR(greater_than, "to be greater than", value > expect)

/**
 * Provide a Comparator shortwrite.
 */
PROVIDE_COMPARATOR(greater_than, GREATER)
PROVIDE_COMPARATOR(greater_than, GT)

#endif  // SCTF_COMPARATOR_GREATER_HPP_
