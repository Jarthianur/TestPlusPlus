/*
    Copyright (C) 2017 Jarthianur

    This file is part of simple-cpp-test-framework.

    simple-cpp-test-framework is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    simple-cpp-test-framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with simple-cpp-test-framework.  If not, see <https://www.gnu.org/licenses/>.
*/

/// @file

#ifndef SCTF_ASSERT_HPP
#define SCTF_ASSERT_HPP

#include <tuple>

#include "comparator/comparator.hpp"

#include "assertion_failure.hpp"
#include "duration.hpp"
#include "loc.hpp"
#include "types.hpp"

/**
 * Generic assertion to compare two values.
 *
 * EXAMPLE:
 * @code
 * ASSERT(1, EQ, 1);
 * @endcode
 *
 * @param VAL is the actual value.
 * @param CMP is the comparator to use.
 * @param EXP is the expected value.
 */
#define ASSERT(VAL, CMP, EXP)                                                  \
    sctf::intern::assert_statement(std::forward_as_tuple(VAL, EXP), sctf::CMP, \
                                   sctf::intern::loc{__FILE__, __LINE__})

/**
 * Generic assertion to compare two values, where the comparison is logically negated.
 * This is equivalent to prefixing a comparator with '!'.
 *
 * EXAMPLE:
 * @code
 * ASSERT_NOT(2, EQ, 1);
 * @endcode
 *
 * @param VAL is the actual value.
 * @param CMP is the comparator to use.
 * @param EXP is the expected value.
 */
#define ASSERT_NOT(VAL, CMP, EXP)                                               \
    sctf::intern::assert_statement(std::forward_as_tuple(VAL, EXP), !sctf::CMP, \
                                   sctf::intern::loc{__FILE__, __LINE__})

/**
 * Assert two values to be equal.
 * This is equivalent to using ASSERT with EQUALS comparator.
 *
 * EXAMPLE:
 * @code
 * ASSERT_EQ(1, 1);
 * @endcode
 *
 * @param VAL is the actual value.
 * @param EXP is the expected value.
 */
#define ASSERT_EQ(VAL, EXP)                                                         \
    sctf::intern::assert_statement(std::forward_as_tuple(VAL, EXP), sctf::EQUALS(), \
                                   sctf::intern::loc{__FILE__, __LINE__})

/**
 * Assert a value to be true.
 *
 * EXAMPLE:
 * @code
 * ASSERT_TRUE(true);
 * @endcode
 *
 * @param VAL is the actual value.
 */
#define ASSERT_TRUE(VAL)                                                             \
    sctf::intern::assert_statement(std::forward_as_tuple(VAL, true), sctf::EQUALS(), \
                                   sctf::intern::loc{__FILE__, __LINE__})

/**
 * Assert a value to be false.
 *
 * EXAMPLE:
 * @code
 * ASSERT_FALSE(false);
 * @endcode
 *
 * @param VAL is the actual value.
 */
#define ASSERT_FALSE(VAL)                                                             \
    sctf::intern::assert_statement(std::forward_as_tuple(VAL, false), sctf::EQUALS(), \
                                   sctf::intern::loc{__FILE__, __LINE__})

/**
 * Assert a pointer to be nullptr.
 *
 * EXAMPLE:
 * @code
 * ASSERT_NULL(&var);
 * @endcode
 *
 * @param PTR is the actual pointer.
 */
#define ASSERT_NULL(PTR)                                                                          \
    sctf::intern::assert_statement(std::forward_as_tuple(static_cast<void const*>(PTR), nullptr), \
                                   sctf::EQUALS(), sctf::intern::loc{__FILE__, __LINE__})

/**
 * Assert a pointer to be not nullptr.
 *
 * EXAMPLE:
 * @code
 * ASSERT_NOT_NULL(&var);
 * @endcode
 *
 * @param PTR is the actual pointer.
 */
#define ASSERT_NOT_NULL(PTR)                                                                      \
    sctf::intern::assert_statement(std::forward_as_tuple(static_cast<void const*>(PTR), nullptr), \
                                   !sctf::EQUALS(), sctf::intern::loc{__FILE__, __LINE__})

/**
 * Assert a value to be zero.
 *
 * EXAMPLE:
 * @code
 * ASSERT_ZERO(0);
 * @endcode
 *
 * @param VAL is the actual value.
 */
#define ASSERT_ZERO(VAL)                                                                      \
    sctf::intern::assert_statement(std::forward_as_tuple(VAL, static_cast<decltype(VAL)>(0)), \
                                   sctf::EQUALS(), sctf::intern::loc{__FILE__, __LINE__})

/**
 * Assert an expression to throw a specific throwable type.
 *
 * EXAMPLE:
 * @code
 * ASSERT_THROWS(func(), std::exception);
 * @endcode
 *
 * @param FN is the expression / invokation.
 * @param TRW is the expected throwable type.
 */
#define ASSERT_THROWS(FN, TRW) \
    sctf::intern::assert_throws<TRW>([&] { FN; }, #TRW, sctf::intern::loc{__FILE__, __LINE__})

/**
 * Assert an expression to not throw anything.
 *
 * EXAMPLE:
 * @code
 * ASSERT_NOTHROW(func());
 * @endcode
 *
 * @param FN is the expression / invokation.
 */
#define ASSERT_NOTHROW(FN) \
    sctf::intern::assert_nothrow([&] { FN; }, sctf::intern::loc{__FILE__, __LINE__})

/**
 * Assert an expression to run in certain amount of time.
 * This may be usefull, when testing fixed timing constraints.
 *
 * EXAMPLE:
 * @code
 * ASSERT_RUNTIME(func());
 * @endcode
 *
 * @param FN is the expression / invokation.
 * @param MAX is the maximum amount of time in milliseconds.
 */
#define ASSERT_RUNTIME(FN, MAX) \
    sctf::intern::assert_runtime([&] { FN; }, MAX, sctf::intern::loc{__FILE__, __LINE__})

namespace sctf
{
namespace intern
{
/**
 * Apply an assertion on two values by using the specified comparator.
 *
 * @tparam C is the type of cmp_.
 * @tparam V is the type of val_.
 * @tparam E is the type of exp_.
 * @param val_ is the actual value.
 * @param exp_ is the expected value.
 * @param cmp_ is the comparator to use.
 * @param loc_ is the line of code where the assertion took place.
 * @throw sctf::intern::assertion_failure if the assertion fails according to the comparator.
 */
template<typename C, typename V>
static void assert_statement(V&& val_, C&& cmp_, loc const& loc_) {
    comparison res = cmp_(std::get<0>(val_), std::get<1>(val_));
    if (!res) {
        throw assertion_failure(*res, loc_);
    }
}

/**
 * Apply a throw-assertion on a function invokation.
 *
 * @tparam T is the throwable type to expect.
 * @param fn_ is the wrapper function to assert for.
 * @param tname_ is the throwable types name.
 * @param loc_ is the line of code where the assertion took place.
 * @throw sctf::intern::assertion_failure if a different, or no throwable type is thrown by fn_.
 */
template<typename T>
static void assert_throws(void_function&& fn_, char const* tname_, loc const& loc_) {
    try {
        fn_();
    } catch (T const&) {
        return;
    } catch (std::exception const& e) {
        throw assertion_failure("Wrong exception thrown, caught " + to_string(e), loc_);
    } catch (...) {
        throw assertion_failure("Wrong exception thrown", loc_);
    }
    throw assertion_failure(std::string("No exception thrown, expected ") + tname_, loc_);
}

/**
 * Apply a nothrow-assertion on a function invokation.
 *
 * @param fn_ is the wrapper function to assert for.
 * @param loc_ is the line of code where the assertion took place.
 * @throw sctf::intern::assertion_failure if a any throwable type is thrown by fn_.
 */
static void assert_nothrow(void_function&& fn_, loc const& loc_) {
    try {
        fn_();
    } catch (std::exception const& e) {
        throw assertion_failure("Expected no exception, caught " + to_string(e), loc_);
    } catch (...) {
        throw assertion_failure("Expected no exception", loc_);
    }
}

/**
 * Apply a runtime-assertion on a function invokation.
 *
 * @param fn_ is the wrapper function to assert for.
 * @param max_ms_ is the maximum amount of time in milliseconds.
 * @param loc_ is the line of code where the assertion took place.
 * @throw sctf::intern::assertion_failure if fn_ does not complete within max_ms_, or any throwable
 * type is thrown by fn_.
 */
static void assert_runtime(void_function&& fn_, double max_ms_, loc const& loc_) {
    try {
        duration dur;
        fn_();
        double dur_ms = dur.get();
        if (dur_ms > max_ms_) {
            throw assertion_failure("runtime > " + to_string(max_ms_) + "ms", loc_);
        }
    } catch (std::exception const& e) {
        throw assertion_failure(e.what(), loc_);
    } catch (...) {
        throw assertion_failure("Unknown exception thrown", loc_);
    }
}
}  // namespace intern
}  // namespace sctf

#endif  // SCTF_ASSERT_HPP
