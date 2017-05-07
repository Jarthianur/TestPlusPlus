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

#ifndef SRC_UTIL_ASSERT_HPP_
#define SRC_UTIL_ASSERT_HPP_

#include <string>
#include <typeinfo>

#include "../comparator/ComparatorStrategy.hpp"
#include "AssertionFailure.hpp"
#include "duration.hpp"
#include "types.h"
#include "serialize.hpp"

//disable assert macro
#ifdef assert
#undef assert
#endif

namespace testsuite
{

/**
 * Assert a value to expected value.
 * value: given value
 * expected: what value should be according to
 * comp: comparator
 * Throws AssertionFailure if assertion failed.
 */
template<typename T>
inline void assert(const T& value, const T& expected, comparator::Comparator<T> comp)
{
    if (!comp->compare(value, expected))
    {
        throw AssertionFailure(
                std::string("Expected ") + util::serialize(value) + " "
                + comp->comparison + " " + util::serialize(expected));
    }
}

/**
 * Assert a function to throw a specific exception.
 * func: test function
 * Exception given in template.
 * Throws AssertionFailure if any other/no exception is caught.
 */
template<typename T>
inline void assertException(test_function func)
{
    try
    {
        func();
    } catch (const T&)
    {
        return;
    } catch (const std::exception& e)
    {
        throw AssertionFailure(
                std::string("Wrong exception thrown, caught: ") + typeid(e).name());
    } catch (...)
    {
        throw AssertionFailure("Wrong exception thrown, caught: Unknown");
    }
    throw AssertionFailure(
            std::string("No exception thrown, expected: ") + typeid(T).name());
}

/**
 * Assert a given test function to run under given time.
 * func: test function wrapper
 * maxMillis: max duration in milliseconds
 */
inline void assertPerformance(test_function func, double maxMillis)
{
    try
    {
        util::duration dur;
        func();
        double dur_ms = dur.get();
        if (dur_ms > maxMillis)
        {
            throw AssertionFailure(
                    std::string("runtime > ") + util::serialize(maxMillis));
        }
    } catch (const std::exception& e)
    {
        throw AssertionFailure(e.what());
    } catch (...)
    {
        throw AssertionFailure("Unknown exception thrown");
    }
}

}  // namespace testsuite

#endif /* SRC_UTIL_ASSERT_HPP_ */
