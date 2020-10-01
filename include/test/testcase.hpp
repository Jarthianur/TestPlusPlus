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

#ifndef TPP_TEST_TESTCASE_HPP
#define TPP_TEST_TESTCASE_HPP

#include <functional>
#include <string>
#include <utility>

#include "test/assertion_failure.hpp"
#include "test/duration.hpp"

namespace tpp
{
namespace intern
{
namespace test
{
using test_function = std::function<void()>;

/**
 * Record a test name and related testsuite.
 */
struct test_context final
{
    char const* const tc_name;  ///< testcase name
    char const* const ts_name;  ///< testsuite name
};

/**
 * A wrapper, that encapsulates a single testcase, as it is used in testsuites.
 */
class testcase
{
public:
    testcase(testcase const&) = delete;
    ~testcase() noexcept      = default;
    auto
    operator=(testcase const&) -> testcase& = delete;

    /**
     * @param ctx_  is the contextual description. 0 => name, 1 => testsuites name.
     * @param fn_   is the function performing the actual test.
     */
    testcase(test_context&& ctx_, test_function&& fn_)
        : m_name(ctx_.tc_name), m_suite_name(ctx_.ts_name), m_test_fn(std::move(fn_)) {}

    testcase(testcase&& other_) noexcept
        : m_name(other_.m_name),
          m_suite_name(other_.m_suite_name),
          m_result(other_.m_result),
          m_duration(other_.m_duration),
          m_err_msg(std::move(other_.m_err_msg)),
          m_test_fn(std::move(other_.m_test_fn)) {}

    auto
    operator=(testcase&& other_) noexcept -> testcase& {
        m_name       = other_.m_name;
        m_suite_name = other_.m_suite_name;
        m_result     = other_.m_result;
        m_duration   = other_.m_duration;
        m_err_msg    = std::move(other_.m_err_msg);
        m_test_fn    = std::move(other_.m_test_fn);
        return *this;
    }

    /**
     * The resulting state of a testcase.
     */
    enum results
    {
        IS_UNDONE,   ///< not yet performed
        HAS_PASSED,  ///< test passed successfully
        HAS_FAILED,  ///< at least one assertion failed
        HAD_ERROR    ///< an unexpected exception was thrown
    };

    /**
     * Perform the test. Returns immediately if the test was alreday performed.
     */
    void
    operator()() {
        if (m_result != IS_UNDONE) {
            return;
        }
        class duration dur;
        try {
            m_test_fn();
            pass();
        } catch (assertion_failure const& e) {
            fail(e.what());
        } catch (std::exception const& e) {
            error(e.what());
        } catch (...) {
            error();
        }
        m_duration = dur.get();
    }

    /**
     * Get the result state.
     */
    inline auto
    result() const -> results {
        return m_result;
    }

    /**
     * Get the duration of the test run in milliseconds.
     */
    inline auto
    duration() const -> double {
        return m_duration;
    }

    /**
     * Get the error, or failure reason.
     * If the test passed, it is empty.
     */
    inline auto
    reason() const -> std::string const& {
        return m_err_msg;
    }

    /**
     * Get the name/description of this testcase.
     */
    inline auto
    name() const -> char const* {
        return m_name;
    }

    /**
     * Get the context (testsuite), where this testcase lives.
     */
    inline auto
    suite_name() const -> char const* {
        return m_suite_name;
    }

    /**
     * Set the captured stdout content for this testcase.
     *
     * @param str_ is the output.
     */
    inline void
    cout(std::string const& str_) {
        m_cout = str_;
    }

    /**
     * Set the captured stderr content for this testcase.
     *
     * @param str_ is the output.
     */
    inline void
    cerr(std::string const& str_) {
        m_cerr = str_;
    }

    /**
     * Get the captured output from stdout.
     */
    inline auto
    cout() const -> std::string const& {
        return m_cout;
    }

    /**
     * Get the captured output from stderr.
     */
    inline auto
    cerr() const -> std::string const& {
        return m_cerr;
    }

private:
    /**
     * Mark this testcase as successfully passed.
     */
    inline void
    pass() {
        m_result = HAS_PASSED;
    }

    /**
     * Mark this testcase as failed.
     *
     * @param msg_ is the failure reason.
     */
    inline void
    fail(char const* msg_) {
        m_result  = HAS_FAILED;
        m_err_msg = msg_;
    }

    /**
     * Mark this testcase as erroneous.
     *
     * @param msg_ is the error reason.
     */
    inline void
    error(char const* msg_ = "unknown error") {
        m_result  = HAD_ERROR;
        m_err_msg = msg_;
    }

    char const*   m_name;               ///< Name or description of this testcase.
    char const*   m_suite_name;         ///< Context description (testsuite) where this testcase lives.
    results       m_result{IS_UNDONE};  ///< Result produced by the test function.
    double        m_duration{.0};       ///< Time in milliseconds, that the test function consumed.
    std::string   m_err_msg;            ///< Message describing the reason for failure, or error.
    std::string   m_cout;               ///< Captured output to stdout.
    std::string   m_cerr;               ///< Captured output to stderr.
    test_function m_test_fn;            ///< Test function that performs the actual test.
};
}  // namespace test
}  // namespace intern
}  // namespace tpp

#endif  // TPP_TEST_TESTCASE_HPP
