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

#ifndef COMPARATOR_COMPARATORSTRATEGY_HPP_
#define COMPARATOR_COMPARATORSTRATEGY_HPP_

#include <memory>
#include <string>

namespace testsuite
{
namespace comparator
{

/**
 * Template interface for comparators
 * used by the testsuite.
 */
template<typename T>
class ComparatorStrategy
{
public:
    /**
     * Construct a comparator with an 'assertion'
     * string, used by reporters.
     */
    inline ComparatorStrategy(const std::string& assert)
            : assertion(assert)
    {
    }

    /**
     * d'tor
     */
    virtual ~ComparatorStrategy() throw ()
    {
    }

    /**
     * The compare method to implement
     * in concrete comparators.
     */
    virtual bool compare(const T&, const T&) = 0;

    /**
     * String that represents the assertion.
     * E.g. "to be equal"
     */
    const std::string assertion;
};

/**
 * Template comparator type as shared ptr.
 * T: type to compare (e.g. int)
 */
template<typename T>
using Comparator = std::shared_ptr<ComparatorStrategy<T>>;

} // comparator
} // testsuite

#endif /* COMPARATOR_COMPARATORSTRATEGY_HPP_ */
