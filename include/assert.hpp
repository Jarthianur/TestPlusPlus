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

#ifndef SCTF_ASSERT_HPP
#define SCTF_ASSERT_HPP

#include "common/assertion_failure.hpp"
#include "common/duration.hpp"
#include "common/stringify.hpp"
#include "common/types.hpp"
#include "comparator/comparators.hpp"
#include "comparator/equals.hpp"
#include "comparator/inrange.hpp"
#include "comparator/unequals.hpp"

// provide assertion macros and wrappers
/**
 * @def assert(VALUE, EXPECT, COMP)
 * @brief Assert wrapper. Test for successful comparison.
 * @note Type is deduced from arguments.
 * @param VALUE The actual value
 * @param COMP The Comparator
 * @param EXPECT The expected value
 */
#define ASSERT(VALUE, COMP, EXPECT)                                                     \
    sctf::_::assert_statement(VALUE, EXPECT, COMP<decltype(VALUE), decltype(EXPECT)>(), \
                              sctf::_::code_location{__FILE__, __LINE__})

/**
 * @def assertT(VALUE, EXPECT, COMP, TYPE)
 * @brief Assert wrapper. Test for successful comparison.
 * @note Type is specialized.
 * @param VALUE The actual value
 * @param COMP The Comparator
 * @param EXPECT The expected value
 * @param TYPE The value type
 */
#define ASSERT_T(VALUE, COMP, EXPECT, TYPE)                                  \
    sctf::_::assert_statement<TYPE, TYPE>(VALUE, EXPECT, COMP<TYPE, TYPE>(), \
                                          sctf::_::code_location{__FILE__, __LINE__})

/**
 * @def assertEquals(VALUE, EXPECT)
 * @brief Assert wrapper. Test for equality.
 * @param VALUE The actual value
 * @param EXPECT The expected value
 */
#define ASSERT_EQUALS(VALUE, EXPECT) ASSERT_T(VALUE, EQUALS, EXPECT, decltype(VALUE))

/**
 * @def assertTrue(VALUE)
 * @brief Assert wrapper. Test value to be true.
 * @param VALUE The value
 */
#define ASSERT_TRUE(VALUE)                                             \
    sctf::_::assert_statement<bool>(VALUE, true, sctf::EQUALS<bool>(), \
                                    sctf::_::code_location{__FILE__, __LINE__})

/**
 * @def assertFalse(VALUE)
 * @brief Assert wrapper. Test value to be false.
 * @param VALUE The value
 */
#define ASSERT_FALSE(VALUE)                                             \
    sctf::_::assert_statement<bool>(VALUE, false, sctf::EQUALS<bool>(), \
                                    sctf::_::code_location{__FILE__, __LINE__})

/**
 * @def assertNotNull(VALUE)
 * @brief Assert wrapper. Test value to be not nullptr.
 * @param VALUE The value
 */
#define ASSERT_NULL(VALUE)                                                 \
    sctf::_::assert_statement(static_cast<void* const>(VALUE), nullptr,    \
                              sctf::EQUALS<void* const, std::nullptr_t>(), \
                              sctf::_::code_location{__FILE__, __LINE__})

/**
 * @def assertNotNull(VALUE)
 * @brief Assert wrapper. Test value to be not nullptr.
 * @param VALUE The value
 */
#define ASSERT_NOT_NULL(VALUE)                                               \
    sctf::_::assert_statement(static_cast<void* const>(VALUE), nullptr,      \
                              sctf::UNEQUALS<void* const, std::nullptr_t>(), \
                              sctf::_::code_location{__FILE__, __LINE__})

/**
 * @def assertZero(VALUE, TYPE)
 * @brief Assert wrapper. Test value to be 0 as specified type.
 * @param VALUE The value
 * @param TYPE The type of value
 */
#define ASSERT_ZERO(VALUE)                                            \
    sctf::_::assert_statement(VALUE, static_cast<decltype(VALUE)>(0), \
                              sctf::EQUALS<decltype(VALUE)>(),        \
                              sctf::_::code_location{__FILE__, __LINE__})

/**
 * @def assertException(FUNC, TYPE)
 * @brief Assert exception wrapper. Test for FUNC to throw TYPE.
 * @param FUNC The function call
 * @param TYPE The exception type
 */
#define ASSERT_THROWS(FUNC, TYPE) \
    sctf::_::assert_throws<TYPE>([&] { FUNC; }, sctf::_::code_location{__FILE__, __LINE__})

/**
 * @def assertNoExcept(FUNC)
 * @brief Assert no exception wrapper. Test for FUNC not to throw any exception.
 * @param FUNC The function call
 */
#define ASSERT_NOTHROW(FUNC) \
    sctf::_::assert_nothrow([&] { FUNC; }, sctf::_::code_location{__FILE__, __LINE__})

/**
 * @def assertPerformance(FUNC, MILLIS)
 * @brief Assert performance wrapper. Test for FUNC to run shorter than MILLIS.
 * @param FUNC The function call
 * @param MILLIS The max amount of milliseconds
 */
#define ASSERT_PERFORMANCE(FUNC, MILLIS) \
    sctf::_::assert_performance([&] { FUNC; }, MILLIS, sctf::_::code_location{__FILE__, __LINE__})

namespace sctf
{
namespace _
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
static void assert_statement(V const& val_, E const& expect_, comparator<V, E> comp_,
                             code_location const& loc_)
{
    comparison res = (*comp_)(val_, expect_);
    if (!res)
    {
        throw assertion_failure(*res, loc_);
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
static void assert_throws(test_function&& fn_, code_location const& loc_)
{
    try
    {
        fn_();
    }
    catch (T const&)
    {
        return;
    }
    catch (std::exception const& e)
    {
        throw assertion_failure(
            std::string("Wrong exception thrown, caught '") + to_string(e) + "'", loc_);
    }
    catch (...)
    {
        throw assertion_failure("Wrong exception thrown", loc_);
    }
    throw assertion_failure(
        std::string("No exception thrown, expected '") + name_for_type<T>() + "'", loc_);
}

/**
 * @brief Assert a function not to throw any exception.
 * @param func The test function
 * @param file The source file
 * @param line The source line in file
 * @throw AssertionFailure if any exception is caught.
 */
static void assert_nothrow(test_function&& fn_, code_location const& loc_)
{
    try
    {
        fn_();
    }
    catch (const std::exception& e)
    {
        throw assertion_failure(std::string("Expected no exception, caught '") + to_string(e) + "'",
                                loc_);
    }
    catch (...)
    {
        throw assertion_failure("Expected no exception", loc_);
    }
}

/**
 * @brief Assert a given test function to run under given time.
 * @param func The test function
 * @param max_millis The max duration in milliseconds
 */
static void assert_performance(test_function&& fn_, double max_ms_, code_location const& loc_)
{
    try
    {
        duration dur;
        fn_();
        double dur_ms = dur.get();
        if (dur_ms > max_ms_)
        {
            throw assertion_failure(std::string("runtime > ") + to_string(max_ms_) + "ms", loc_);
        }
    }
    catch (std::exception const& e)
    {
        throw assertion_failure(e.what(), loc_);
    }
    catch (...)
    {
        throw assertion_failure("Unknown exception thrown", loc_);
    }
}
}  // namespace _
}  // namespace sctf

#endif  // SCTF_ASSERT_HPP
