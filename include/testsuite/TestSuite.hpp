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

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "common/serialize.hpp"
#include "common/streambuf_proxy.hpp"
#include "common/types.h"
#include "testsuite/TestCase.hpp"
#include "testsuite/TestStats.hpp"

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

    virtual ~TestSuite() noexcept = default;

    /**
     * @brief Execute all TestCases sequentially.
     */
    virtual void run()
    {
        if (m_state != State::DONE)
        {
            m_stats.m_num_of_tests = m_testcases.size();
            _::streambuf_proxy buf_cout(std::cout);
            _::streambuf_proxy buf_cerr(std::cerr);

            SCTF_EXEC_SILENT(m_setup_func)
            std::for_each(m_testcases.begin(), m_testcases.end(),
                          [this, &buf_cerr, &buf_cout](_::TestCase& tc) {
                              if (tc.state() == _::TestCase::State::NONE)
                              {
                                  SCTF_EXEC_SILENT(m_pre_test_func)
                                  tc();
                                  switch (tc.state())
                                  {
                                      case _::TestCase::State::FAILED:
                                          ++m_stats.m_num_of_fails;
                                          break;
                                      case _::TestCase::State::ERROR:
                                          ++m_stats.m_num_of_errs;
                                          break;
                                      default: break;
                                  }
                                  m_time += tc.duration();
                                  SCTF_EXEC_SILENT(m_post_test_func)
                                  tc.set_cout(buf_cout.str());
                                  tc.set_cerr(buf_cerr.str());
                                  buf_cout.clear();
                                  buf_cerr.clear();
                              }
                          });
            m_state = State::DONE;
        }
    }

    /**
     * @brief Add a TestCase to this TestSuite.
     * @tparam T The class context for testing methods
     * @param name The name/description
     * @param t_func The test function
     * @return this as shared pointer
     */
    template<typename T>
    TestSuite_shared test(const std::string& name, _::test_function&& t_func)
    {
        m_testcases.push_back(_::TestCase(name, _::name_for_type<T>(), std::move(t_func)));
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
                          _::test_function&& t_func)
    {
        m_testcases.push_back(_::TestCase(name, context, std::move(t_func)));
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
    TestSuite_shared test(const std::string& name, _::test_function&& t_func)
    {
        m_testcases.push_back(_::TestCase(name, m_context, std::move(t_func)));
        m_state = State::PENDING;
        return shared_from_this();
    }

    /**
     * @brief Set a setup function, which will be executed once before all testcases.
     * @note Exceptions thrown by this get ignored.
     * @param t_func The function
     * @return this as shared pointer
     */
    TestSuite_shared setup(_::test_function&& t_func)
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
    TestSuite_shared before(_::test_function&& t_func)
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
    TestSuite_shared after(_::test_function&& t_func)
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
    inline const _::TestStats& statistics() const
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
    inline const std::vector<_::TestCase>& testcases() const
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
    _::TestStats m_stats;

    /// @brief The testcases.
    std::vector<_::TestCase> m_testcases;

    /// @brief The execution state.
    State m_state = State::PENDING;

    /// @brief The optional setup function, executed once before all testcases.
    _::test_function m_setup_func;

    /// @brief The optional pre-test function, executed before each testcase.
    _::test_function m_pre_test_func;

    /// @brief The optional post-test function, executed after each testcase.
    _::test_function m_post_test_func;
};

}  // namespace sctf

#endif  // SCTF_SRC_TESTSUITE_TESTSUITE_HPP_
