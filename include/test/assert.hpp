/*
    Copyright (C) 2017  Jarthianur

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/// @file

#ifndef TPP_TEST_ASSERT_HPP
#define TPP_TEST_ASSERT_HPP

#include <tuple>

#include "compare/comparator.hpp"
#include "test/assertion_failure.hpp"
#include "test/duration.hpp"
#include "test/loc.hpp"

#include "stringify.hpp"
#include "traits.hpp"

/**
 * Generic assertion to compare two values.
 *
 * EXAMPLE:
 * @code
 * ASSERT(1, EQ(), 1);
 * @endcode
 *
 * @param VAL is the actual value.
 * @param CMP is the comparator to use.
 * @param EXP is the expected value.
 */
#define ASSERT(VAL, CMP, EXP)                                                 \
    tpp::intern::test::assert_statement(std::forward_as_tuple(CMP, VAL, EXP), \
                                        tpp::intern::test::loc{__FILE__, __LINE__})

/**
 * Generic assertion to compare two values, where the comparison is logically negated.
 * This is equivalent to prefixing a comparator with '!'.
 *
 * EXAMPLE:
 * @code
 * ASSERT_NOT(2, EQ(), 1);
 * @endcode
 *
 * @param VAL is the actual value.
 * @param CMP is the comparator to use.
 * @param EXP is the expected value.
 */
#define ASSERT_NOT(VAL, CMP, EXP)                                              \
    tpp::intern::test::assert_statement(std::forward_as_tuple(!CMP, VAL, EXP), \
                                        tpp::intern::test::loc{__FILE__, __LINE__})

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
#define ASSERT_EQ(VAL, EXP)                                                             \
    tpp::intern::test::assert_statement(std::forward_as_tuple(tpp::EQUALS(), VAL, EXP), \
                                        tpp::intern::test::loc{__FILE__, __LINE__})

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
#define ASSERT_TRUE(VAL)                                                                 \
    tpp::intern::test::assert_statement(std::forward_as_tuple(tpp::EQUALS(), VAL, true), \
                                        tpp::intern::test::loc{__FILE__, __LINE__})

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
#define ASSERT_FALSE(VAL)                                                                 \
    tpp::intern::test::assert_statement(std::forward_as_tuple(tpp::EQUALS(), VAL, false), \
                                        tpp::intern::test::loc{__FILE__, __LINE__})

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
#define ASSERT_NULL(PTR)                                                                                              \
    tpp::intern::test::assert_statement(std::forward_as_tuple(tpp::EQUALS(), static_cast<void const*>(PTR), nullptr), \
                                        tpp::intern::test::loc{__FILE__, __LINE__})

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
#define ASSERT_NOT_NULL(PTR)                                                                                           \
    tpp::intern::test::assert_statement(std::forward_as_tuple(!tpp::EQUALS(), static_cast<void const*>(PTR), nullptr), \
                                        tpp::intern::test::loc{__FILE__, __LINE__})

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
#define ASSERT_ZERO(VAL)                                                                                          \
    tpp::intern::test::assert_statement(std::forward_as_tuple(tpp::EQUALS(), VAL, static_cast<decltype(VAL)>(0)), \
                                        tpp::intern::test::loc{__FILE__, __LINE__})

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
 * @return the instance of TRW that was caught.
 */
#define ASSERT_THROWS(FN, TRW) \
    tpp::intern::test::assert_throws<TRW>([&] { FN; }, #TRW, tpp::intern::test::loc{__FILE__, __LINE__})

/**
 * Assert an expression to not throw anything.
 *
 * EXAMPLE:
 * @code
 * ASSERT_NOTHROW(func());
 * @endcode
 *
 * @param FN is the expression / invokation.
 * @return the return value of FN, if there is any.
 */
#define ASSERT_NOTHROW(FN) tpp::intern::test::assert_nothrow([&] { FN; }, tpp::intern::test::loc{__FILE__, __LINE__})

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
 * @return the return value of FN, if there is any.
 */
#define ASSERT_RUNTIME(FN, MAX) \
    tpp::intern::test::assert_runtime([&] { FN; }, MAX, tpp::intern::test::loc{__FILE__, __LINE__})

namespace tpp
{
namespace intern
{
namespace test
{
/**
 * Apply an assertion on two values by using the specified comparator.
 *
 * @tparam S is the statement to assert, constisting of comparator, actual value, and expected
 * value.
 * @param stmt_ is the comparison statement to assert.
 * @param loc_ is the line of code where the assertion took place.
 * @throw tpp::intern::assertion_failure if the assertion fails according to the comparator.
 */
template<typename S>
void
assert_statement(S&& stmt_, loc const& loc_) {
    compare::comparison res = std::get<0>(stmt_)(std::get<1>(stmt_), std::get<2>(stmt_));
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
 * @return the instance of T that was caught.
 * @throw tpp::intern::assertion_failure if a different throwable type, or nothing is thrown by
 * fn_.
 */
template<typename T, typename F>
auto
assert_throws(F&& fn_, char const* tname_, loc const& loc_) -> T {
    try {
        fn_();
    } catch (T const& e) {
        return e;
    } catch (std::exception const& e) {
        throw assertion_failure(
          "Wrong exception thrown, caught " + to_string(e) + "(\"" + escaped_string(e.what()) + "\")", loc_);
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
 * @return the return value of fn_.
 * @throw tpp::intern::assertion_failure if a any throwable type is thrown by fn_.
 */
template<typename F, TPP_INTERN_ENABLE_IF(!TPP_INTERN_IS_TYPE(decltype(std::declval<F>()()), void))>
auto
assert_nothrow(F&& fn_, loc const& loc_) -> decltype(fn_()) {
    try {
        return fn_();
    } catch (std::exception const& e) {
        throw assertion_failure(
          "Expected no exception, caught " + to_string(e) + "(\"" + escaped_string(e.what()) + "\")", loc_);
    } catch (...) {
        throw assertion_failure("Expected no exception", loc_);
    }
}

/**
 * Apply a nothrow-assertion on a function invokation.
 *
 * @param fn_ is the wrapper function to assert for.
 * @param loc_ is the line of code where the assertion took place.
 * @throw tpp::intern::assertion_failure if a any throwable type is thrown by fn_.
 */
template<typename F, TPP_INTERN_ENABLE_IF(TPP_INTERN_IS_TYPE(decltype(std::declval<F>()()), void))>
void
assert_nothrow(F&& fn_, loc const& loc_) {
    try {
        fn_();
    } catch (std::exception const& e) {
        throw assertion_failure(
          "Expected no exception, caught " + to_string(e) + "(\"" + escaped_string(e.what()) + "\")", loc_);
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
 * @return the return value of fn_.
 * @throw tpp::intern::assertion_failure if fn_ does not complete within max_ms_.
 */
template<typename F, TPP_INTERN_ENABLE_IF(!TPP_INTERN_IS_TYPE(decltype(std::declval<F>()()), void))>
auto
assert_runtime(F&& fn_, double max_ms_, loc const& loc_) -> decltype(fn_()) {
    duration        dur;
    decltype(fn_()) res    = fn_();
    double          dur_ms = dur.get();
    if (dur_ms > max_ms_) {
        throw assertion_failure("runtime > " + to_string(max_ms_) + "ms", loc_);
    }
    return res;
}

/**
 * Apply a runtime-assertion on a function invokation.
 *
 * @param fn_ is the wrapper function to assert for.
 * @param max_ms_ is the maximum amount of time in milliseconds.
 * @param loc_ is the line of code where the assertion took place.
 * @throw tpp::intern::assertion_failure if fn_ does not complete within max_ms_.
 */
template<typename F, TPP_INTERN_ENABLE_IF(TPP_INTERN_IS_TYPE(decltype(std::declval<F>()()), void))>
void
assert_runtime(F&& fn_, double max_ms_, loc const& loc_) {
    duration dur;
    fn_();
    double dur_ms = dur.get();
    if (dur_ms > max_ms_) {
        throw assertion_failure("runtime > " + to_string(max_ms_) + "ms", loc_);
    }
}
}  // namespace test
}  // namespace intern
}  // namespace tpp

#endif  // TPP_TEST_ASSERT_HPP