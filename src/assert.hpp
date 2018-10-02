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

#ifndef SCTF_SRC_ASSERT_HPP_
#define SCTF_SRC_ASSERT_HPP_

#include "comparator/comparators.hpp"
#include "comparator/equals.hpp"
#include "comparator/inrange.hpp"
#include "comparator/unequals.hpp"
#include "util/duration.hpp"
#include "util/interval.hpp"
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
 * @note Type is deduced from arguments.
 * @param VALUE The actual value
 * @param COMP The Comparator
 * @param EXPECT The expected value
 */
#define assert(VALUE, COMP, EXPECT)                                                  \
    sctf::_assertStatement(VALUE, EXPECT, COMP<decltype(VALUE), decltype(EXPECT)>(), \
                           __FILE__, __LINE__)

/**
 * @def assertT(VALUE, EXPECT, COMP, TYPE)
 * @brief Assert wrapper. Test for successful comparison.
 * @note Type is specialized.
 * @param VALUE The actual value
 * @param COMP The Comparator
 * @param EXPECT The expected value
 * @param TYPE The value type
 */
#define assertT(VALUE, COMP, EXPECT, TYPE)                                          \
    sctf::_assertStatement<TYPE, TYPE>(VALUE, EXPECT, COMP<TYPE, TYPE>(), __FILE__, \
                                       __LINE__)

/**
 * @def assertEquals(VALUE, EXPECT)
 * @brief Assert wrapper. Test for equality.
 * @param VALUE The actual value
 * @param EXPECT The expected value
 */
#define assertEquals(VALUE, EXPECT) assertT(VALUE, EQUALS, EXPECT, decltype(VALUE))

/**
 * @def assertInInterval(VALUE, LOWER, UPPER)
 * @brief Assert wrapper. Test for VALUE to be in an interval between LOWER and UPPER.
 * @note Bounds are inclusive.
 * @param VALUE The actual value
 * @param LOWER The lower bound
 * @param UPPER The upper bound
 */
#define assertInInterval(VALUE, LOWER, UPPER) \
    assert(VALUE, IN_RANGE, (sctf::util::interval<decltype(LOWER)>{LOWER, UPPER}))

/**
 * @def assertTrue(VALUE)
 * @brief Assert wrapper. Test value to be true.
 * @param VALUE The value
 */
#define assertTrue(VALUE) \
    sctf::_assertStatement<bool>(VALUE, true, sctf::EQUALS<bool>(), __FILE__, __LINE__)

/**
 * @def assertFalse(VALUE)
 * @brief Assert wrapper. Test value to be false.
 * @param VALUE The value
 */
#define assertFalse(VALUE) \
    sctf::_assertStatement<bool>(VALUE, false, sctf::EQUALS<bool>(), __FILE__, __LINE__)

/**
 * @def assertNotNull(VALUE)
 * @brief Assert wrapper. Test value to be not nullptr.
 * @param VALUE The value
 */
#define assertNotNull(VALUE)                                                        \
    sctf::_assertStatement(static_cast<void* const>(VALUE), nullptr,                \
                           sctf::UNEQUALS<void* const, std::nullptr_t>(), __FILE__, \
                           __LINE__)

/**
 * @def assertZero(VALUE, TYPE)
 * @brief Assert wrapper. Test value to be 0 as specified type.
 * @param VALUE The value
 * @param TYPE The type of value
 */
#define assertZero(VALUE)                                          \
    sctf::_assertStatement(VALUE, static_cast<decltype(VALUE)>(0), \
                           sctf::EQUALS<decltype(VALUE)>(), __FILE__, __LINE__)

/**
 * @def assertException(FUNC, TYPE)
 * @brief Assert exception wrapper. Test for FUNC to throw TYPE.
 * @param FUNC The function call
 * @param TYPE The exception type
 */
#define assertException(FUNC, TYPE) \
    sctf::_assertException<TYPE>([&]() { FUNC; }, __FILE__, __LINE__)

/**
 * @def assertNoExcept(FUNC)
 * @brief Assert no exception wrapper. Test for FUNC not to throw any exception.
 * @param FUNC The function call
 */
#define assertNoExcept(FUNC) sctf::_assertNoExcept([&]() { FUNC; }, __FILE__, __LINE__)

/**
 * @def assertPerformance(FUNC, MILLIS)
 * @brief Assert performance wrapper. Test for FUNC to run shorter than MILLIS.
 * @param FUNC The function call
 * @param MILLIS The max amount of milliseconds
 */
#define assertPerformance(FUNC, MILLIS) \
    sctf::_assertPerformance([&]() { FUNC; }, MILLIS, __FILE__, __LINE__)

namespace sctf
{
/**
 * @brief Assert a value to be compared successfully to an expected value.
 * @tparam V The type of value
 * * @tparam V The type of expect
 * @param value The actual value
 * @param expect The expected value
 * @param comp The Comparator
 * @throw AssertionFailure if the assertion failed.
 */
template<typename V, typename E = V>
static void _assertStatement(const V& value, const E& expect, comp::Comparator<V, E> comp,
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
static void _assertException(const test::test_function& func, const char* file, int line)
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
                                   + util::serialize(e) + "'",
                               file, line);
    }
    catch(...)
    {
        throw AssertionFailure("Wrong exception thrown", file, line);
    }
    throw AssertionFailure(std::string("No exception thrown, expected '")
                               + util::name_for_type<T>() + "'",
                           file, line);
}

/**
 * @brief Assert a function not to throw any exception.
 * @param func The test function
 * @param file The source file
 * @param line The source line in file
 * @throw AssertionFailure if any exception is caught.
 */
inline static void _assertNoExcept(const test::test_function& func, const char* file,
                                   int line)
{
    try
    {
        func();
    }
    catch(const std::exception& e)
    {
        throw AssertionFailure(std::string("Expected no exception, caught '")
                                   + util::serialize(e) + "'",
                               file, line);
    }
    catch(...)
    {
        throw AssertionFailure("Expected no exception", file, line);
    }
}

/**
 * @brief Assert a given test function to run under given time.
 * @param func The test function
 * @param max_millis The max duration in milliseconds
 */
inline static void _assertPerformance(const test::test_function& func, double max_millis,
                                      const char* file, int line)
{
    try
    {
        util::duration dur;
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

#endif  // SCTF_SRC_ASSERT_HPP_
