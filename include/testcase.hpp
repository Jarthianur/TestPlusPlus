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

#ifndef SCTF_TESTCASE_HPP
#define SCTF_TESTCASE_HPP

#include <cstdint>
#include <string>
#include <tuple>
#include <utility>

#include "assertion_failure.hpp"
#include "duration.hpp"
#include "types.hpp"

namespace sctf
{
namespace intern
{
/**
 * A wrapper, that encapsulates a single testcase, as it is used in testsuites.
 */
class testcase
{
public:
    testcase(testcase const&) = delete;
    auto operator=(testcase const&) -> testcase& = delete;
    ~testcase() noexcept                         = default;

    /**
     * @param ctx_  is the contextual description. 0 => name, 1 => testsuites name.
     * @param fn_   is the function performing the actual test.
     */
    testcase(std::tuple<char const*, char const*>&& ctx_, void_function&& fn_)
        : m_name(std::get<0>(ctx_)), m_context(std::get<1>(ctx_)), m_test_func(std::move(fn_)) {}

    testcase(testcase&& other_) noexcept
        : m_name(other_.m_name),
          m_context(other_.m_context),
          m_state(other_.m_state),
          m_duration(other_.m_duration),
          m_err_msg(std::move(other_.m_err_msg)),
          m_test_func(std::move(other_.m_test_func)) {}

    auto operator=(testcase&& other_) noexcept -> testcase& {
        m_name      = other_.m_name;
        m_context   = other_.m_context;
        m_state     = other_.m_state;
        m_duration  = other_.m_duration;
        m_err_msg   = std::move(other_.m_err_msg);
        m_test_func = std::move(other_.m_test_func);
        return *this;
    }

    /**
     * The resulting state of a testcase.
     */
    enum class result : std::int_fast8_t
    {
        NONE,    ///< not yet performed
        PASSED,  ///< test passed successfully
        FAILED,  ///< at least one assertion failed
        ERROR    ///< an unexpected exception was thrown
    };

    /**
     * Perform the test. Returns immediately if the test was alreday performed.
     */
    void operator()() {
        if (m_state != result::NONE) {
            return;
        }
        class duration dur;
        try {
            m_test_func();
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
    inline auto state() const -> result {
        return m_state;
    }

    /**
     * Get the duration of the test run in milliseconds.
     */
    inline auto duration() const -> double {
        return m_duration;
    }

    /**
     * Get the error, or failure reason.
     * If the test passed, it is empty.
     */
    inline auto reason() const -> std::string const& {
        return m_err_msg;
    }

    /**
     * Get the name/description of this testcase.
     */
    inline auto name() const -> char const* {
        return m_name;
    }

    /**
     * Get the context (testsuite), where this testcase lives.
     */
    inline auto context() const -> char const* {
        return m_context;
    }

    /**
     * Set the captured stdout content for this testcase.
     *
     * @param str_ is the output.
     */
    inline void cout(std::string const& str_) {
        m_cout = str_;
    }

    /**
     * Set the captured stderr content for this testcase.
     *
     * @param str_ is the output.
     */
    inline void cerr(std::string const& str_) {
        m_cerr = str_;
    }

    /**
     * Get the captured output from stdout.
     */
    inline auto cout() const -> std::string const& {
        return m_cout;
    }

    /**
     * Get the captured output from stderr.
     */
    inline auto cerr() const -> std::string const& {
        return m_cerr;
    }

private:
    /**
     * Mark this testcase as successfully passed.
     */
    inline void pass() {
        m_state = result::PASSED;
    }

    /**
     * Mark this testcase as failed.
     *
     * @param msg_ is the failure reason.
     */
    inline void fail(char const* msg_) {
        m_state   = result::FAILED;
        m_err_msg = msg_;
    }

    /**
     * Mark this testcase as erroneous.
     *
     * @param msg_ is the error reason.
     */
    inline void error(char const* msg_ = "unknown error") {
        m_state   = result::ERROR;
        m_err_msg = msg_;
    }

    char const*   m_name;     ///< Name or description of this testcase.
    char const*   m_context;  ///< Context description (testsuite) where this testcase lives.
    result        m_state    = result::NONE;  ///< Result produced by the test function.
    double        m_duration = 0.0;  ///< Time in milliseconds, that the test function consumed.
    std::string   m_err_msg;         ///< Message describing the reason for failure, or error.
    std::string   m_cout;            ///< Captured output to stdout.
    std::string   m_cerr;            ///< Captured output to stderr.
    void_function m_test_func;       ///< Test function that performs the actual test.
};
}  // namespace intern
}  // namespace sctf

#endif  // SCTF_TESTSUITE_TESTCASE_HPP
