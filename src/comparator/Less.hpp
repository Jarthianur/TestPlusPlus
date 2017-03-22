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

#ifndef COMPARATOR_LESS_HPP_
#define COMPARATOR_LESS_HPP_

#include <string>

#include "ComparatorStrategy.hpp"

namespace testsuite
{
namespace comparator
{

/**
 * Concrete comparator strategy
 * for less-than comparison.
 * Non-copyable
 */
template<typename T>
class Less: public ComparatorStrategy<T>
{
public:
    Less(const Less&) = delete;
    Less& operator=(const Less&) = delete;

    /**
     * c'tor
     */
    Less(const std::string& comp)
            : ComparatorStrategy<T>(comp)
    {
    }

    /**
     * d'tor
     */
    virtual ~Less() noexcept
    {
    }

    /**
     * Template - compare
     */
    inline bool compare(const T& val, const T& expect) noexcept override
    {
        return val < expect;
    }

};

template<>
inline bool Less<bool>::compare(const bool& val, const bool& expect) noexcept
{
    return val == false;
}

} // comparator
} // testsuite

#endif /* COMPARATOR_LESS_HPP_ */
