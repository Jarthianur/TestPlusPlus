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

#ifndef COMPARATOR_GREATER_HPP_
#define COMPARATOR_GREATER_HPP_

#include <string>

#include "ComparatorStrategy.hpp"

namespace testsuite
{
namespace comparator
{

/**
 * Concrete comparator strategy
 * for greater-than comparison.
 * Non-copyable
 */
template<typename T>
class Greater: public ComparatorStrategy<T>
{
public:
    Greater(const Greater&) = delete;
    Greater& operator=(const Greater&) = delete;

    /**
     * c'tor
     */
    inline Greater(const std::string& a)
            : ComparatorStrategy<T>(a)
    {
    }

    /**
     * d'tor
     */
    inline virtual ~Greater() throw ()
    {
    }

    /**
     * Template - compare
     */
    inline bool compare(const T& a, const T& b)
    {
        return a > b;
    }

};

} // comparator
} // testsuite

#endif /* COMPARATOR_GREATER_HPP_ */
