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

#ifndef COMPARATOR_EQUALS_HPP_
#define COMPARATOR_EQUALS_HPP_

#include <algorithm>
#include <cmath>
#include <limits>
#include <string>

#include "ComparatorStrategy.hpp"

namespace testsuite
{
namespace comparator
{

/**
 * Concrete comparator strategy
 * for equals comparison.
 * Non-copyable
 */
template<typename T>
class Equals: public ComparatorStrategy<T>
{
public:
    Equals(const Equals&) = delete;
    Equals& operator=(const Equals&) = delete;

    /**
     * c'tor
     */
    inline Equals(const std::string& a)
            : ComparatorStrategy<T>(a)
    {
    }

    /**
     * d'tor
     */
    inline virtual ~Equals() throw ()
    {
    }

    /**
     * Template - compare
     */
    inline bool compare(const T& a, const T& b)
    {
        return a == b;
    }

};

/**
 * Specialized compare for type 'double'.
 * Takes care about floating point precision.
 */
template<>
inline bool Equals<double>::compare(const double& a, const double& b)
{
    return a == b || std::abs(a - b)
            < std::abs(std::min(a, b)) * std::numeric_limits<double>::epsilon();
}

} // comparator
} // testsuite

#endif /* COMPARATOR_EQUALS_HPP_ */
