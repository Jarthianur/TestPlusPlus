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

#ifndef SCTF_TESTCASE_HPP
#define SCTF_TESTCASE_HPP

#include <cstdint>
#include <string>
#include <utility>

#include "assertion_failure.hpp"
#include "duration.hpp"
#include "types.hpp"

namespace sctf
{
namespace private_
{
/**
 * A wrapper, that encapsulates a single testcase, as it is used in testsuites.
 */
class testcase
{
public:
    testcase(testcase const&) = delete;
    testcase& operator=(testcase const&) = delete;

    ~testcase() noexcept = default;

    /**
     * @param name_ is the name/description for this testcase.
     * @param ctx_  is the context (testsuite), where this testcase lives.
     * @param fn_   is the function performing the actual test.
     */
    testcase(char const* name_, char const* ctx_, void_function&& fn_)
        : m_name(name_), m_context(ctx_), m_test_func(std::move(fn_)) {}

    testcase(testcase&& other_)
        : m_name(other_.m_name),
          m_context(other_.m_context),
          m_state(other_.m_state),
          m_duration(other_.m_duration),
          m_err_msg(std::move(other_.m_err_msg)),
          m_test_func(std::move(other_.m_test_func)) {}

    testcase& operator=(testcase&& other_) {
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
        if (m_state != result::NONE) return;
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
    inline result state() const {
        return m_state;
    }

    /**
     * Get the duration of the test run in milliseconds.
     */
    inline double duration() const {
        return m_duration;
    }

    /**
     * Get the error, or failure reason.
     * If the test passed, it is empty.
     */
    inline std::string const& reason() const {
        return m_err_msg;
    }

    /**
     * Get the name/description of this testcase.
     */
    inline char const* name() const {
        return m_name;
    }

    /**
     * Get the context (testsuite), where this testcase lives.
     */
    inline char const* context() const {
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
    inline std::string const& cout() const {
        return m_cout;
    }

    /**
     * Get the captured output from stderr.
     */
    inline std::string const& cerr() const {
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

    char const*   m_name;
    char const*   m_context;
    result        m_state    = result::NONE;
    double        m_duration = 0.0;
    std::string   m_err_msg;
    std::string   m_cout;
    std::string   m_cerr;
    void_function m_test_func;
};
}  // namespace private_
}  // namespace sctf

#endif  // SCTF_TESTSUITE_TESTCASE_HPP
