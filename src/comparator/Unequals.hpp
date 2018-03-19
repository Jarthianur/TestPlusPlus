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

#ifndef COMPARATOR_UNEQUALS_HPP
#define COMPARATOR_UNEQUALS_HPP

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
 * for unequals comparison.
 * Non-copyable
 */
template<typename T>
class Unequals : public ComparatorStrategy<T>
{
public:
    Unequals(const Unequals&) = delete;
    Unequals& operator=(const Unequals&) = delete;

    /**
     * c'tor
     */
    Unequals(const std::string& comp) : ComparatorStrategy<T>(comp)
    {}

    /**
     * d'tor
     */
    virtual ~Unequals() noexcept
    {}

    /**
     * Template - compare
     */
    inline bool compare(const T& val, const T& expect) noexcept override
    {
        return val != expect;
    }
};

/**
 * Specialized compare for type 'double'.
 * Takes care about floating point precision.
 */
template<>
inline bool Unequals<double>::compare(const double& val, const double& expect) noexcept
{
    double diff_abs = std::abs(val - expect);
    double max      = std::max(std::abs(val), std::abs(expect));

    return !(val == expect || diff_abs < max * std::numeric_limits<double>::epsilon()
             || diff_abs < max * 0.000001);
}

/**
 * Factory method for Unequals comparator.
 */
template<typename T>
inline Comparator<T> UNEQUALS()
{
    return Comparator<T>(new Unequals<T>("to be unequal"));
}

}  // namespace comparator
}  // namespace testsuite

#endif  // COMPARATOR_UNEQUALS_HPP
