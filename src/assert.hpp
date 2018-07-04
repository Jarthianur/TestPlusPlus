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

#ifndef SRC_ASSERT_HPP_
#define SRC_ASSERT_HPP_

#include <string>
#include <typeinfo>

#include "comparator/comparators.hpp"
#include "util/Duration.hpp"
#include "util/serialize.hpp"
#include "AssertionFailure.hpp"
#include "types.h"

// disable assert macro
#ifdef assert
#undef assert
#endif

// provide assertion macros and wrappers
/**
 * @def assert(VALUE, EXPECT, COMP)
 * @brief Assert wrapper. Test for successful comparison.
 *
 * Type is deduced from arguments.
 *
 * @param VALUE The actual value
 * @param EXPECT The expected value
 * @param COMP The Comparator
 */
#define assert(VALUE, EXPECT, COMP) \
    _assertStatement(VALUE, EXPECT, COMP, __FILE__, __LINE__)

/**
 * @def assertT(VALUE, EXPECT, COMP, TYPE)
 * @brief Assert wrapper. Test for successful comparison.
 *
 * Type is specialized.
 *
 * @param VALUE The actual value
 * @param EXPECT The expected value
 * @param COMP The Comparator
 * @param TYPE The value type
 */
#define assertT(VALUE, EXPECT, COMP, TYPE) \
    _assertStatement<TYPE>(VALUE, EXPECT, COMP, __FILE__, __LINE__)

/**
 * @def assertTrue(VALUE)
 * @brief Assert wrapper. Test value to be true.
 * @param VALUE The value
 */
#define assertTrue(VALUE) \
    _assertStatement<bool>(VALUE, true, sctf::comp::EQUALS<bool>(), __FILE__, __LINE__)

/**
 * @def assertFalse(VALUE)
 * @brief Assert wrapper. Test value to be false.
 * @param VALUE The value
 */
#define assertFalse(VALUE) \
    _assertStatement<bool>(VALUE, false, sctf::comp::EQUALS<bool>(), __FILE__, __LINE__)

/**
 * @def assertZero(VALUE, TYPE)
 * @brief Assert wrapper. Test value to be 0 as specified type.
 * @param VALUE The value
 * @param TYPE The type of value
 */
#define assertZero(VALUE, TYPE)                                                     \
    _assertStatement<TYPE>(VALUE, static_cast<TYPE>(0), sctf::comp::EQUALS<TYPE>(), \
                           __FILE__, __LINE__)

/**
 * @def assertException(FUNC, EXCEPT)
 * @brief Assert exception wrapper. Test for FUNC to throw EXCEPT.
 * @param FUNC The function
 * @param EXCEPT The exception type
 */
#define assertException(FUNC, EXCEPT) _assertException<EXCEPT>(FUNC, __FILE__, __LINE__)

/**
 * @def assertPerformance(FUNC, MILLIS)
 * @brief Assert performance wrapper. Test for FUNC to run shorter than MILLIS.
 * @param FUNC The function
 * @param MILLIS The max amount of milliseconds
 */
#define assertPerformance(FUNC, MILLIS) \
    _assertPerformance(FUNC, MILLIS, __FILE__, __LINE__)

namespace sctf
{
/**
 * @brief Assert a value to be compared successfully to expected value.
 * @tparam T The type of value and expected
 * @param value The actual value
 * @param expect The expected value
 * @param comp The Comparator
 * @throw AssertionFailure if the assertion failed.
 */
template<typename T>
static void _assertStatement(const T& value, const T& expect, comp::Comparator<T> comp,
                             const char* file, int line)
{
    comp::Comparison res = (*comp)(value, expect);
    if(!res)
    {
        throw AssertionFailure(*res, file, line);
    }
}

/**
 * @brief Assert a function to throw a specific exception.
 * @tparam T The exception type
 * @param func The test function
 * @param file The source file
 * @param line The source line in file
 * @throw AssertionFailure if any other or no exception is caught at all.
 */
template<typename T>
static void _assertException(test::test_function func, const char* file, int line)
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
 * @brief Assert a given test function to run under given time.
 * @param func The test function
 * @param max_millis The max duration in milliseconds
 */
static void _assertPerformance(test::test_function func, double max_millis,
                               const char* file, int line)
{
    try
    {
        util::Duration dur;
        func();
        double dur_ms = dur.get();
        if(dur_ms > max_millis)
        {
            throw AssertionFailure(std::string("runtime > ") + util::serialize(max_millis)
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

}  // namespace sctf

#endif  // SRC_ASSERT_HPP_
