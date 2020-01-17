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

/**
 * Test for successful comparison.
 * @param VALUE The actual value
 * @param COMP The Comparator
 * @param EXPECT The expected value
 */
#define ASSERT(VALUE, COMP, EXPECT)                                                     \
    sctf::_::assert_statement(VALUE, EXPECT, COMP<decltype(VALUE), decltype(EXPECT)>(), \
                              sctf::_::code_location{__FILE__, __LINE__})

/**
 * Test for successful negated comparison.
 * @param VALUE The actual value
 * @param COMP The Comparator
 * @param EXPECT The expected value
 */
#define ASSERT_NOT(VALUE, COMP, EXPECT)                                                  \
    sctf::_::assert_statement(VALUE, EXPECT, !COMP<decltype(VALUE), decltype(EXPECT)>(), \
                              sctf::_::code_location{__FILE__, __LINE__})

/**
 * Test for equality.
 * @param VALUE The actual value
 * @param EXPECT The expected value
 */
#define ASSERT_EQUALS(VALUE, EXPECT)                                                            \
    sctf::_::assert_statement(VALUE, EXPECT, sctf::EQUALS<decltype(VALUE), decltype(EXPECT)>(), \
                              sctf::_::code_location{__FILE__, __LINE__})

/**
 * Test value to be true.
 * @param VALUE The value
 */
#define ASSERT_TRUE(VALUE)                                  \
    sctf::_::assert_statement<sctf::_::equals<bool>, bool>( \
        VALUE, true, sctf::EQUALS<bool>(), sctf::_::code_location{__FILE__, __LINE__})

/**
 * Test value to be false.
 * @param VALUE The value
 */
#define ASSERT_FALSE(VALUE)                                 \
    sctf::_::assert_statement<sctf::_::equals<bool>, bool>( \
        VALUE, false, sctf::EQUALS<bool>(), sctf::_::code_location{__FILE__, __LINE__})

/**
 * Test value to be nullptr.
 * @param VALUE The value
 */
#define ASSERT_NULL(VALUE)                                                 \
    sctf::_::assert_statement(static_cast<void const*>(VALUE), nullptr,    \
                              sctf::EQUALS<void const*, std::nullptr_t>(), \
                              sctf::_::code_location{__FILE__, __LINE__})

/**
 * Test value not to be nullptr.
 * @param VALUE The value
 */
#define ASSERT_NOT_NULL(VALUE)                                               \
    sctf::_::assert_statement(static_cast<void const*>(VALUE), nullptr,      \
                              sctf::UNEQUALS<void const*, std::nullptr_t>(), \
                              sctf::_::code_location{__FILE__, __LINE__})

/**
 * Test value to be 0.
 * @param VALUE The value
 */
#define ASSERT_ZERO(VALUE)                                            \
    sctf::_::assert_statement(VALUE, static_cast<decltype(VALUE)>(0), \
                              sctf::EQUALS<decltype(VALUE)>(),        \
                              sctf::_::code_location{__FILE__, __LINE__})

/**
 * Test for FUNC to throw TYPE.
 * @param FUNC The function call
 * @param TYPE The exception type
 */
#define ASSERT_THROWS(FUNC, TYPE) \
    sctf::_::assert_throws<TYPE>([&] { FUNC; }, sctf::_::code_location{__FILE__, __LINE__})

/**
 * Test for FUNC not to throw any exception.
 * @param FUNC The function call
 */
#define ASSERT_NOTHROW(FUNC) \
    sctf::_::assert_nothrow([&] { FUNC; }, sctf::_::code_location{__FILE__, __LINE__})

/**
 * Test for FUNC to run faster than MILLIS.
 * @param FUNC The function call
 * @param MILLIS The max amount of milliseconds
 */
#define ASSERT_PERFORMANCE(FUNC, MILLIS) \
    sctf::_::assert_performance([&] { FUNC; }, MILLIS, sctf::_::code_location{__FILE__, __LINE__})

namespace sctf
{
namespace _
{
template<typename C, typename V, typename E = V>
static void assert_statement(V const& val_, E const& expect_, C&& cmp_, code_location const& loc_)
{
    comparison res = cmp_(val_, expect_);
    if (!res)
    {
        throw assertion_failure(*res, loc_);
    }
}

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
