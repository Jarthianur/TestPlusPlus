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
#include "serialize.hpp"
#include "types.h"

// disable assert macro
#ifdef assert
#undef assert
#endif
#define assert(V, E, C, T) _assertStatement<T>(V, E, C, __FILE__, __LINE__)
#define assertException(F, T) _assertException<T>(F, __FILE__, __LINE__)
#define assertPerformance(F, M) _assertPerformance(F, M, __FILE__, __LINE__)

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
inline void _assertStatement(const T& value, const T& expected,
                             comparator::Comparator<T> comp, const char* file, int line)
{
    if(!comp->compare(value, expected))
    {
        throw AssertionFailure(std::string("Expected '") + util::serialize(value) + "' "
                                   + comp->comparison + " '" + util::serialize(expected)
                                   + "'",
                               file, line);
    }
}

/**
 * Assert a function to throw a specific exception.
 * func: test function
 * Exception given in template.
 * Throws AssertionFailure if any other/no exception is caught.
 */
template<typename T>
inline void _assertException(test_function func, const char* file, int line)
{
    try
    {
        func();
    }
    catch(const T&)
    {
        return;
    }
    catch(const std::exception& e)
    {
        throw AssertionFailure(std::string("Wrong exception thrown, caught '")
                                   + typeid(e).name() + "'",
                               file, line);
    }
    catch(...)
    {
        throw AssertionFailure("Wrong exception thrown", file, line);
    }
    throw AssertionFailure(std::string("No exception thrown, expected '")
                               + typeid(T).name() + "'",
                           file, line);
}

/**
 * Assert a given test function to run under given time.
 * func: test function wrapper
 * maxMillis: max duration in milliseconds
 */
inline void _assertPerformance(test_function func, double maxMillis, const char* file,
                               int line)
{
    try
    {
        util::duration dur;
        func();
        double dur_ms = dur.get();
        if(dur_ms > maxMillis)
        {
            throw AssertionFailure(std::string("runtime > ") + util::serialize(maxMillis)
                                       + "ms",
                                   file, line);
        }
    }
    catch(const std::exception& e)
    {
        throw AssertionFailure(e.what(), file, line);
    }
    catch(...)
    {
        throw AssertionFailure("Unknown exception thrown", file, line);
    }
}

}  // namespace testsuite

#endif /* SRC_UTIL_ASSERT_HPP_ */
