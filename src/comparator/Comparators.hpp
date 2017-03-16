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

#ifndef COMPARATOR_COMPARATORS_H_
#define COMPARATOR_COMPARATORS_H_

#include "ComparatorStrategy.hpp"
#include "Equals.hpp"
#include "Greater.hpp"
#include "Less.hpp"

namespace testsuite
{
namespace comparator
{

/**
 * Comparator factory.
 * Allows to call comparators as easy as possible.
 * Define factory method for all comparators in here.
 */

/**
 * Factory method for Equals comparator.
 */
template<typename T>
inline Comparator<T> EQUALS()
{
    return Comparator<T>(new Equals<T>("to be equal"));
}

/**
 * Factory method for Less comparator.
 */
template<typename T>
inline Comparator<T> LESS()
{
    return Comparator<T>(new Less<T>("to be less than"));
}

/**
 * Factory method for Greater comparator.
 */
template<typename T>
inline Comparator<T> GREATER()
{
    return Comparator<T>(new Greater<T>("to be greater than"));
}

} // comparator
} // testsuite

#endif /* COMPARATOR_COMPARATORS_H_ */
