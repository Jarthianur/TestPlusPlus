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

#ifndef SRC_TESTSUITE_TESTCASE_HPP_
#define SRC_TESTSUITE_TESTCASE_HPP_

#include <cstdint>
#include <string>

#include "../AssertionFailure.hpp"
#include "../types.h"
#include "../util/Duration.hpp"

namespace sctf
{
namespace test
{
/**
 * @brief Represent a testcase.
 *
 * A TestCase has a test function with assertions inside, which failes if any assertion
 * fails and respectively passes if not. The state is stored with an error message in case
 * of failure.
 */
class TestCase
{
public:
    /**
     * @brief Constructor
     * @param name The name/description of the test case
     * @param context The context of the test function
     * @param t_func The test function
     */
    TestCase(const std::string& name, const std::string& context, test_function t_func)
        : name(name), context("test." + context), mTestFunc(t_func)
    {}

    /**
     * @brief Destructor
     */
    ~TestCase() noexcept
    {}

    /**
     * @brief The test state.
     */
    enum class TestState : std::uint32_t
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
     * @return the resulting TestState
     */
    TestState execute() noexcept
    {
        util::Duration dur;
        try
        {
            mTestFunc();
            pass();
        }
        catch(const AssertionFailure& e)
        {
            fail(e.what());
        }
        catch(const std::exception& e)
        {
            erroneous(e.what());
        }
        catch(...)
        {
            erroneous();
        }
        m_duration = dur.get();
        return m_state;
    }

    /**
     * @brief Get the TestState.
     * @return the test state
     */
    inline TestState getState() const
    {
        return m_state;
    }

    /**
     * @brief Get the Duration.
     * @return the duration
     */
    inline double getDuration() const
    {
        return m_duration;
    }

    /**
     * @brief Get the error message.
     * @return the error message
     */
    inline const std::string& getErrMsg() const
    {
        return m_err_msg;
    }

    const std::string name;
    const std::string context;

private:
    /**
     * @brief Pass this test.
     */
    inline void pass()
    {
        m_state = TestState::PASSED;
    }

    /**
     * @brief Fail this test.
     * @param msg The failure reason
     */
    inline void fail(const char* msg)
    {
        m_state   = TestState::FAILED;
        m_err_msg = msg;
    }

    /**
     * @brief Fail this test with an error.
     * @param error The error msg
     */
    inline void erroneous(const std::string& error = "")
    {
        m_state   = TestState::ERROR;
        m_err_msg = error;
    }

    /// @brief The test state.
    TestState m_state = TestState::NONE;

    /// @brief The duration in milliseconds.
    double m_duration = 0.0;

    /// @brief The error message.
    std::string m_err_msg;

    /// @brief The test function.
    test_function mTestFunc;
};

}  // namespace test
}  // namespace sctf

#endif  // SRC_TESTSUITE_TESTCASE_HPP_
