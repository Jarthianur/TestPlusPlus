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
    inline Equals(const std::string& comp)
            : ComparatorStrategy<T>(comp)
    {
    }

    /**
     * d'tor
     */
    inline virtual ~Equals() noexcept
    {
    }

    /**
     * Template - compare
     */
    inline bool compare(const T& val, const T& expect) noexcept
    {
        return val == expect;
    }

};

/**
 * Specialized compare for type 'double'.
 * Takes care about floating point precision.
 */
template<>
inline bool Equals<double>::compare(const double& val, const double& expect) noexcept
{
    return val == expect || std::abs(val - expect)
            < std::abs(std::min(val, expect)) * std::numeric_limits<double>::epsilon();
}

} // comparator
} // testsuite

#endif /* COMPARATOR_EQUALS_HPP_ */
