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

#ifndef SCTF_SRC_TESTSUITE_TESTSUITE_HPP_
#define SCTF_SRC_TESTSUITE_TESTSUITE_HPP_

#include <chrono>
#include <cstddef>
#include <functional>
#include <iterator>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "../types.h"
#include "../util/serialize.hpp"

#include "TestCase.hpp"
#include "TestStats.hpp"

/// @brief Call a functor silently, catching all exceptions and only if it points to a
/// target.
#define SCTF_EXEC_SILENT(F) \
    if (F)                  \
    {                       \
        try                 \
        {                   \
            F();            \
        }                   \
        catch (...)         \
        {}                  \
    }

namespace sctf
{
/**
 * @brief Testsuite class for managing sequential testcases.
 * @note Not-copyable
 */
class TestSuite : public std::enable_shared_from_this<TestSuite>
{
public:
    TestSuite(const TestSuite&) = delete;
    TestSuite& operator=(const TestSuite&) = delete;

    /**
     * @brief Create a TestSuite.
     * @param name The name/description
     * @param context The context
     * @return a shared pointer to the created TestSuite
     */
    static TestSuite_shared create(const std::string& name, const std::string& context)
    {
        return TestSuite_shared(new TestSuite(name, context));
    }

    /**
     * @brief Destructor
     */
    virtual ~TestSuite() noexcept = default;

    /**
     * @brief Execute all TestCases sequentially.
     */
    virtual void run() noexcept
    {
        if (m_state == State::DONE) return;
        m_stats.m_num_of_tests = m_testcases.size();
        SCTF_EXEC_SILENT(m_setup_func)
        for (test::TestCase& tc : m_testcases)
        {
            if (tc.state() != test::TestCase::State::NONE) continue;
            SCTF_EXEC_SILENT(m_pre_test_func)
            tc();
            switch (tc.state())
            {
                case test::TestCase::State::FAILED: ++m_stats.m_num_of_fails; break;
                case test::TestCase::State::ERROR: ++m_stats.m_num_of_errs; break;
                default: break;
            }
            m_time += tc.duration();
            SCTF_EXEC_SILENT(m_post_test_func)
        }
        m_state = State::DONE;
    }

    /**
     * @brief Add a TestCase to this TestSuite.
     * @tparam T The class context for testing methods
     * @param name The name/description
     * @param t_func The test function
     * @return this as shared pointer
     */
    template<typename T>
    TestSuite_shared test(const std::string& name, test::test_function&& t_func)
    {
        m_testcases.push_back(test::TestCase(name, util::name_for_type<T>(), std::move(t_func)));
        m_state = State::PENDING;
        return shared_from_this();
    }

    /**
     * @brief Add a TestCase to this TestSuite.
     * @param name The name/description
     * @param context The testing context
     * @param t_func The test function
     * @return this as shared pointer
     */
    TestSuite_shared test(const std::string& name, const std::string& context,
                          test::test_function&& t_func)
    {
        m_testcases.push_back(test::TestCase(name, context, std::move(t_func)));
        m_state = State::PENDING;
        return shared_from_this();
    }

    /**
     * @brief Add a TestCase to this TestSuite.
     * @note The test context is inherited from the test suite.
     * @param name The name/description
     * @param t_func The test function
     * @return this as shared pointer
     */
    TestSuite_shared test(const std::string& name, test::test_function&& t_func)
    {
        m_testcases.push_back(test::TestCase(name, m_context, std::move(t_func)));
        m_state = State::PENDING;
        return shared_from_this();
    }

    /**
     * @brief Set a setup function, which will be executed once before all testcases.
     * @note Exceptions thrown by this get ignored.
     * @param t_func The function
     * @return this as shared pointer
     */
    TestSuite_shared setup(test::test_function&& t_func)
    {
        m_setup_func = std::move(t_func);
        return shared_from_this();
    }

    /**
     * @brief Set a pre-test function, which will be executed before every testcase.
     * @note Exceptions thrown by this get ignored.
     * @param t_func The function
     * @return this as shared pointer
     */
    TestSuite_shared before(test::test_function&& t_func)
    {
        m_pre_test_func = std::move(t_func);
        return shared_from_this();
    }

    /**
     * @brief Set a post-test function, which will be executed after every testcase.
     * @note Exceptions thrown by this get ignored.
     * @param t_func The function
     * @return this as shared pointer
     */
    TestSuite_shared after(test::test_function&& t_func)
    {
        m_post_test_func = std::move(t_func);
        return shared_from_this();
    }

    /**
     * @brief Get the testsuite name.
     * @return The name
     */
    inline const std::string& name() const
    {
        return m_name;
    }

    /**
     * @brief Get the testsuite timestamp.
     * @return The timestamp
     */
    inline const std::chrono::system_clock::time_point& timestamp() const
    {
        return m_timestamp;
    }

    /**
     * @brief Get the test statistics.
     * @return the TestStats
     */
    inline const test::TestStats& statistics() const
    {
        return m_stats;
    }

    /**
     * @brief Get the accumulated time.
     * @return the time
     */
    inline double time() const
    {
        return m_time;
    }

    /**
     * @brief Get the TestCases.
     * @return the test cases
     */
    inline const std::vector<test::TestCase>& testcases() const
    {
        return m_testcases;
    }

protected:
    /**
     * @brief The state of all testcases.
     */
    enum class State : std::int32_t
    {
        /// Still testcases to execute
        PENDING,
        /// All testcases executed
        DONE
    };

    /**
     * @brief Constructor
     * @param name The name/description
     * @param context The context description
     */
    TestSuite(const std::string& name, const std::string& context)
        : m_name(name), m_context(context), m_timestamp(std::chrono::system_clock::now())
    {}

    /// @brief The name/description
    const std::string m_name;

    /// @brief The context description.
    const std::string m_context;

    /// @brief The start timestamp
    const std::chrono::system_clock::time_point m_timestamp;

    /// @brief The accumulated runtime of all tests.
    double m_time = 0.0;

    /// @brief The test statistics.
    test::TestStats m_stats;

    /// @brief The testcases.
    std::vector<test::TestCase> m_testcases;

    /// @brief The execution state.
    State m_state = State::PENDING;

    /// @brief The optional setup function, executed once before all testcases.
    test::test_function m_setup_func;

    /// @brief The optional pre-test function, executed before each testcase.
    test::test_function m_pre_test_func;

    /// @brief The optional post-test function, executed after each testcase.
    test::test_function m_post_test_func;
};

}  // namespace sctf

#endif  // SCTF_SRC_TESTSUITE_TESTSUITE_HPP_
