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

#ifndef SRC_TESTSUITE_TESTSUITE_HPP_
#define SRC_TESTSUITE_TESTSUITE_HPP_

#include <chrono>
#include <cstddef>
#include <iterator>
#include <memory>
#include <string>
#include <typeinfo>
#include <vector>

#include "../types.h"
#include "TestCase.hpp"
#include "TestStats.hpp"

namespace sctf
{
namespace test
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
    void execute() noexcept
    {
        m_stats.m_num_of_tests = m_testcases.size();
        for(auto& tc : m_testcases)
        {
            switch(tc.execute())
            {
                case TestCase::TestState::FAILED:
                    ++m_stats.m_num_of_fails;
                    break;
                case TestCase::TestState::ERROR:
                    ++m_stats.m_num_of_errs;
                    break;
                default:
                    break;
            }
            m_time += tc.getDuration();
        }
    }

    /**
     * @brief Execute all TestCases in parallel.
     */
    void executeParallel() noexcept
    {
        m_stats.m_num_of_tests = m_testcases.size();
#pragma omp parallel
        {
            double tmp        = 0.0;
            std::size_t fails = 0;
            std::size_t errs  = 0;
#pragma omp for schedule(dynamic)
            for(auto tc = m_testcases.begin(); tc < m_testcases.end(); ++tc)
            {
                switch(tc->execute())
                {
                    case TestCase::TestState::FAILED:
                        ++fails;
                        break;
                    case TestCase::TestState::ERROR:
                        ++errs;
                        break;
                    default:
                        break;
                }
                tmp += tc->getDuration();
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
    TestSuite_shared test(const std::string& name, test_function t_func)
    {
        m_testcases.push_back(TestCase(name, typeid(T).name(), t_func));
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
                          test_function t_func)
    {
        m_testcases.push_back(TestCase(name, context, t_func));
        return shared_from_this();
    }

    /**
     * @brief Add a TestCase to this TestSuite.
     * @note The test context is inherited from the test suite.
     * @param name The name/description
     * @param t_func The test function
     * @return this as shared pointer
     */
    TestSuite_shared test(const std::string& name, test_function func)
    {
        m_testcases.push_back(TestCase(name, m_context, func));
        return shared_from_this();
    }

    /**
     * @brief Get the test statistics.
     * @return the TestStats
     */
    inline const TestStats& getTestStats() const
    {
        return m_stats;
    }

    /**
     * @brief Get the accumulated time.
     * @return the time
     */
    inline double getTime() const
    {
        return m_time;
    }

    /**
     * @brief Get the TestCases.
     * @return the test cases
     */
    inline const std::vector<TestCase>& getTestCases() const
    {
        return m_testcases;
    }

    /// @brief The name/description
    const std::string name;

    /// @brief The start timestamp
    const std::chrono::system_clock::time_point timestamp;

private:
    /**
     * @brief Constructor
     * @param name The name/description
     * @param context The context description
     */
    TestSuite(const std::string& name, const std::string& context)
        : name(name), timestamp(std::chrono::system_clock::now()), m_context(context)
    {}

    /// @brief The accumulated runtime of all tests.
    double m_time = 0.0;

    /// @brief The test statistics.
    TestStats m_stats;

    /// @brief The testcases.
    std::vector<TestCase> m_testcases;

    /// @brief The context description.
    const std::string m_context;
};

}  // namespace test
}  // namespace sctf

#endif  // SRC_TESTSUITE_TESTSUITE_HPP_
