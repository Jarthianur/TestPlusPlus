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

#ifndef SCTF_TESTSUITE_TESTSUITE_HPP
#define SCTF_TESTSUITE_TESTSUITE_HPP

#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "common/streambuf_proxy.hpp"
#include "common/stringify.hpp"
#include "common/types.hpp"
#include "testsuite/statistics.hpp"
#include "testsuite/testcase.hpp"

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
class testsuite : public std::enable_shared_from_this<testsuite>
{
public:
    virtual ~testsuite() noexcept = default;

    /**
     * @brief Create a TestSuite.
     * @param name The name/description
     * @param context The context
     * @return a shared pointer to the created TestSuite
     */
    static testsuite_shared create(char const* name_, char const* ctx_)
    {
        return testsuite_shared(new testsuite(name_, ctx_));
    }

    /**
     * @brief Execute all TestCases sequentially.
     */
    virtual void run()
    {
        if (m_state != execution_state::DONE)
        {
            m_stats.m_num_of_tests = m_testcases.size();
            _::streambuf_proxy buf_cout(std::cout);
            _::streambuf_proxy buf_cerr(std::cerr);

            SCTF_EXEC_SILENT(m_setup_func)
            std::for_each(m_testcases.begin(), m_testcases.end(),
                          [this, &buf_cerr, &buf_cout](_::testcase& tc_) {
                              if (tc_.state() == _::testcase::result::NONE)
                              {
                                  SCTF_EXEC_SILENT(m_pre_test_func)
                                  tc_();
                                  switch (tc_.state())
                                  {
                                      case _::testcase::result::FAILED:
                                          ++m_stats.m_num_of_fails;
                                          break;
                                      case _::testcase::result::ERROR:
                                          ++m_stats.m_num_of_errs;
                                          break;
                                      default: break;
                                  }
                                  m_time += tc_.duration();
                                  SCTF_EXEC_SILENT(m_post_test_func)
                                  tc_.set_cout(buf_cout.str());
                                  tc_.set_cerr(buf_cerr.str());
                                  buf_cout.clear();
                                  buf_cerr.clear();
                              }
                          });
            m_state = execution_state::DONE;
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
    testsuite_shared test(char const* name_, _::test_function&& fn_)
    {
        m_testcases.push_back(_::testcase(name_, _::name_for_type<T>(), std::move(fn_)));
        m_state = execution_state::PENDING;
        return shared_from_this();
    }

    /**
     * @brief Add a TestCase to this TestSuite.
     * @param name The name/description
     * @param context The testing context
     * @param t_func The test function
     * @return this as shared pointer
     */
    testsuite_shared test(char const* name_, char const* ctx_, _::test_function&& fn_)
    {
        m_testcases.push_back(_::testcase(name_, ctx_, std::move(fn_)));
        m_state = execution_state::PENDING;
        return shared_from_this();
    }

    /**
     * @brief Add a TestCase to this TestSuite.
     * @note The test context is inherited from the test suite.
     * @param name The name/description
     * @param t_func The test function
     * @return this as shared pointer
     */
    testsuite_shared test(char const* name_, _::test_function&& fn_)
    {
        m_testcases.push_back(_::testcase(name_, m_context, std::move(fn_)));
        m_state = execution_state::PENDING;
        return shared_from_this();
    }

    /**
     * @brief Set a setup function, which will be executed once before all testcases.
     * @note Exceptions thrown by this get ignored.
     * @param t_func The function
     * @return this as shared pointer
     */
    testsuite_shared setup(_::test_function&& fn_)
    {
        m_setup_func = std::move(fn_);
        return shared_from_this();
    }

    /**
     * @brief Set a pre-test function, which will be executed before every testcase.
     * @note Exceptions thrown by this get ignored.
     * @param t_func The function
     * @return this as shared pointer
     */
    testsuite_shared before(_::test_function&& fn_)
    {
        m_pre_test_func = std::move(fn_);
        return shared_from_this();
    }

    /**
     * @brief Set a post-test function, which will be executed after every testcase.
     * @note Exceptions thrown by this get ignored.
     * @param t_func The function
     * @return this as shared pointer
     */
    testsuite_shared after(_::test_function&& fn_)
    {
        m_post_test_func = std::move(fn_);
        return shared_from_this();
    }

    /**
     * @brief Get the testsuite name.
     * @return The name
     */
    inline char const* name() const
    {
        return m_name;
    }

    /**
     * @brief Get the testsuite timestamp.
     * @return The timestamp
     */
    inline std::chrono::system_clock::time_point const& timestamp() const
    {
        return m_timestamp;
    }

    /**
     * @brief Get the test statistics.
     * @return the TestStats
     */
    inline _::statistics const& statistics() const
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
    inline std::vector<_::testcase> const& testcases() const
    {
        return m_testcases;
    }

protected:
    /**
     * @brief The state of all testcases.
     */
    enum class execution_state : std::int_fast8_t
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
    testsuite(char const* name_, char const* ctx_)
        : m_name(name_), m_context(ctx_), m_timestamp(std::chrono::system_clock::now())
    {}

    /// @brief The name/description
    char const* m_name;

    /// @brief The context description.
    char const* m_context;

    /// @brief The start timestamp
    std::chrono::system_clock::time_point const m_timestamp;

    /// @brief The accumulated runtime of all tests.
    double m_time = 0.0;

    /// @brief The test statistics.
    _::statistics m_stats;

    /// @brief The testcases.
    std::vector<_::testcase> m_testcases;

    /// @brief The execution state.
    execution_state m_state = execution_state::PENDING;

    /// @brief The optional setup function, executed once before all testcases.
    _::test_function m_setup_func;

    /// @brief The optional pre-test function, executed before each testcase.
    _::test_function m_pre_test_func;

    /// @brief The optional post-test function, executed after each testcase.
    _::test_function m_post_test_func;
};

}  // namespace sctf

#endif  // SCTF_TESTSUITE_TESTSUITE_HPP
