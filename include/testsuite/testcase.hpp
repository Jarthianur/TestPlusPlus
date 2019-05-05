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

#ifndef SCTF_TESTSUITE_TESTCASE_HPP_
#define SCTF_TESTSUITE_TESTCASE_HPP_

#include <cstdint>
#include <string>
#include <utility>

#include "common/assertion_failure.hpp"
#include "common/duration.hpp"
#include "common/types.h"

namespace sctf
{
namespace _
{
/**
 * @brief Represent a testcase.
 *
 * A TestCase has a test function with assertions inside, which failes if any assertion
 * fails and respectively passes if not. The state is stored with an error message in case
 * of failure.
 */
class testcase
{
public:
    testcase(const testcase&) = delete;
    testcase& operator=(const testcase&) = delete;

    /**
     * @brief Constructor
     * @param name The name/description of the test case
     * @param context The context of the test function
     * @param t_func The test function
     */
    testcase(const std::string& name, const std::string& context, test_function&& t_func)
        : m_name(name), m_context("test." + context), m_test_func(std::move(t_func))
    {}

    /**
     * @brief Move-constructor
     * @param other The other TestCase
     */
    testcase(testcase&& other)
        : m_name(std::move(other.m_name)),
          m_context(std::move(other.m_context)),
          m_state(other.m_state),
          m_duration(other.m_duration),
          m_err_msg(std::move(other.m_err_msg)),
          m_test_func(std::move(other.m_test_func))
    {}

    ~testcase() noexcept = default;

    /**
     * @brief Move-assignment
     * @param other The other TestCase
     * @return this
     */
    testcase& operator=(testcase&& other)
    {
        m_name      = std::move(other.m_name);
        m_context   = std::move(other.m_context);
        m_state     = other.m_state;
        m_duration  = other.m_duration;
        m_err_msg   = std::move(other.m_err_msg);
        m_test_func = std::move(other.m_test_func);
        return *this;
    }

    /**
     * @brief The test state.
     */
    enum class result : std::int_fast8_t
    {
        /// not yet executed
        NONE,
        /// passed
        PASSED,
        /// failed
        FAILED,
        /// erroneous
        ERROR
    };

    /**
     * @brief Execute the test function and store results.
     * @note If already executed, this does nothing.
     */
    void operator()()
    {
        if (m_state != result::NONE) return;
        struct duration dur;
        try
        {
            m_test_func();
            pass();
        }
        catch (const assertion_failure& e)
        {
            fail(e.what());
        }
        catch (const std::exception& e)
        {
            erroneous(e.what());
        }
        catch (...)
        {
            erroneous();
        }
        m_duration = dur.get();
    }

    /**
     * @brief Get the TestState.
     * @return the test state
     */
    inline result state() const
    {
        return m_state;
    }

    /**
     * @brief Get the Duration.
     * @return the duration
     */
    inline double duration() const
    {
        return m_duration;
    }

    /**
     * @brief Get the error message.
     * @return the error message
     */
    inline const std::string& err_msg() const
    {
        return m_err_msg;
    }

    /**
     * @brief Get the name.
     * @return the name
     */
    inline const std::string& name() const
    {
        return m_name;
    }

    /**
     * @brief Get the context.
     * @return the context
     */
    inline const std::string& context() const
    {
        return m_context;
    }

    /**
     * @brief Set the captured output from stdout.
     * @param str The output
     */
    inline void set_cout(const std::string& str)
    {
        m_cout = str;
    }

    /**
     * @brief Set the captured output from stderr.
     * @param str The output
     */
    inline void set_cerr(const std::string& str)
    {
        m_cerr = str;
    }

    /**
     * @brief Get the output from stdout.
     * @return The output
     */
    inline const std::string& cout() const
    {
        return m_cout;
    }

    /**
     * @brief Get the output from stderr.
     * @return The output
     */
    inline const std::string& cerr() const
    {
        return m_cerr;
    }

private:
    /**
     * @brief Pass this test.
     */
    inline void pass()
    {
        m_state = result::PASSED;
    }

    /**
     * @brief Fail this test.
     * @param msg The failure reason
     */
    inline void fail(const char* msg)
    {
        m_state   = result::FAILED;
        m_err_msg = msg;
    }

    /**
     * @brief Fail this test with an error.
     * @param error The error msg
     */
    inline void erroneous(const std::string& error = "unknown error")
    {
        m_state   = result::ERROR;
        m_err_msg = error;
    }

    /// @brief The testcase name
    std::string m_name;

    /// @brief The testcase context
    std::string m_context;

    /// @brief The test state
    result m_state = result::NONE;

    /// @brief The duration in milliseconds
    double m_duration = 0.0;

    /// @brief The error message
    std::string m_err_msg;

    /// @brief The test function
    test_function m_test_func;

    /// @brief The coutput from stdout
    std::string m_cout;

    /// @brief The coutput from stderr
    std::string m_cerr;
};

}  // namespace _
}  // namespace sctf

#endif  // SCTF_TESTSUITE_TESTCASE_HPP_
