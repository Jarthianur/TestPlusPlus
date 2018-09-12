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
#include <iterator>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "../types.h"
#include "../util/serialize.hpp"
#include "TestCase.hpp"
#include "TestStats.hpp"

namespace sctf
{
/**
 * Testsuite class, providing some assertion methods.
 * Non-copyable
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
    ~TestSuite() noexcept
    {}

    /**
     * @brief Execute all TestCases sequentially.
     */
    void run() noexcept
    {
        m_stats.m_num_of_tests = m_testcases.size();
        for(test::TestCase& tc : m_testcases)
        {
            switch(tc.execute())
            {
                case test::TestCase::TestState::FAILED:
                    ++m_stats.m_num_of_fails;
                    break;
                case test::TestCase::TestState::ERROR:
                    ++m_stats.m_num_of_errs;
                    break;
                default:
                    break;
            }
            m_time += tc.duration();
        }
    }

    /**
     * @brief Execute all TestCases in parallel.
     */
    void run_parallel() noexcept
    {
        m_stats.m_num_of_tests = m_testcases.size();
#pragma omp parallel
        {
            thread_local double tmp        = 0.0;
            thread_local std::size_t fails = 0;
            thread_local std::size_t errs  = 0;
#pragma omp for schedule(dynamic)
            for(auto tc = m_testcases.begin(); tc < m_testcases.end(); ++tc)
            {
                switch(tc->execute())
                {
                    case test::TestCase::TestState::FAILED:
                        ++fails;
                        break;
                    case test::TestCase::TestState::ERROR:
                        ++errs;
                        break;
                    default:
                        break;
                }
                tmp += tc->duration();
            }
#pragma omp atomic
            m_stats.m_num_of_fails += fails;
#pragma omp atomic
            m_stats.m_num_of_errs += errs;
#pragma omp critical
            {
                if(m_time < tmp)
                {
                    m_time = tmp;
                }
            }
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
    TestSuite_shared test(const std::string& name, test::test_function&& t_func)
    {
        m_testcases.push_back(
            test::TestCase(name, util::typeName<T>(), std::move(t_func)));
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

private:
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
};

}  // namespace sctf

#endif  // SCTF_SRC_TESTSUITE_TESTSUITE_HPP_
