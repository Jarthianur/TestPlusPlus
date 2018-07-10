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

#ifndef SRC_TESTSUITE_TESTSUITESRUNNER_HPP_
#define SRC_TESTSUITE_TESTSUITESRUNNER_HPP_

#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "../types.h"
#include "../util/serialize.hpp"
#include "TestSuite.hpp"

namespace sctf
{
namespace test
{
/**
 * @brief A runner to manage and run TestSuites.
 */
class TestSuitesRunner
{
public:
    /**
     * @brief Constructor
     */
    TestSuitesRunner()
    {}

    /**
     * @brief Destructor
     */
    ~TestSuitesRunner() noexcept
    {}

    /**
     * @brief The execution state.
     */
    enum class ExecState : std::uint32_t
    {
        /// No TestSuites executed yet
        NONE,
        /// All sequential TestSuites executed
        SEQUENTIAL,
        /// All parallel TestSuites executed
        PARALLEL,
        /// All TestSuites executed
        ALL
    };

    /**
     * @brief Register a TestSuite.
     * @param ts A shared ptr to the TestSuite
     * @param parallel Whether to execute in parallel or sequential
     * @return a shared pointer to the TestSuite
     */
    TestSuite_shared registerTestSuite(TestSuite_shared ts, bool parallel)
    {
        if(parallel)
        {
            m_parallel_suites.push_back(ts);
        }
        else
        {
            m_sequential_suites.push_back(ts);
        }
        return ts;
    }

    /**
     * @brief Execute parallel TestSuites.
     */
    void executeParallel() noexcept
    {
        if(m_exec_state != ExecState::PARALLEL && m_exec_state != ExecState::ALL)
        {
            for(auto ts : m_parallel_suites)
            {
                ts->executeParallel();
            }
            m_exec_state
                = m_exec_state == ExecState::NONE ? ExecState::PARALLEL : ExecState::ALL;
        }
    }

    /**
     * @brief Execute sequential TestSuites.
     */
    void executeSequential() noexcept
    {
        if(m_exec_state != ExecState::SEQUENTIAL && m_exec_state != ExecState::ALL)
        {
            for(auto ts : m_sequential_suites)
            {
                ts->execute();
            }
            m_exec_state = m_exec_state == ExecState::NONE ? ExecState::SEQUENTIAL
                                                           : ExecState::ALL;
        }
    }

    /**
     * @brief Execute both parallel and sequential TestSuites.
     */
    void executeAll() noexcept
    {
        executeParallel();
        executeSequential();
    }

    /**
     * @brief Get the execution state.
     * @return the ExecState
     */
    inline ExecState getStatus() const
    {
        return m_exec_state;
    }

    /**
     * @brief Get parallel and sequential test suites.
     * @note All have to be executed until call.
     * @return a pair with all testsuites, where first are the sequential ones and second
     * are the parallel ones
     * @throw std::logic_error if not all TestSuites were executed yet
     */
    const std::pair<std::vector<TestSuite_shared>&, std::vector<TestSuite_shared>&>
    getTestSuites()
    {
        if(m_exec_state == ExecState::ALL)
        {
            return std::pair<std::vector<TestSuite_shared>&,
                             std::vector<TestSuite_shared>&>(m_sequential_suites,
                                                             m_parallel_suites);
        }
        else
        {
            throw std::logic_error("TestSuites not yet executed");
        }
    }

private:
    /// @brief The registered sequential TestSuites.
    std::vector<TestSuite_shared> m_sequential_suites;

    /// @brief The registered parallel TestSuites.
    std::vector<TestSuite_shared> m_parallel_suites;

    /// @brief The execution state.
    ExecState m_exec_state = ExecState::NONE;
};

}  // namespace test

/**
 * @brief Describe and register a TestSuite to the given runner.
 * @note All test cases in this suite will be executed in parallel.
 * @param name The name/description
 * @param context The component/context to test as string (default="")
 * @param runner The TestSuitesRunner to register
 * @return a shared pointer to the created TestSuite
 */
inline static TestSuite_shared describeParallel(const std::string& name,
                                                test::TestSuitesRunner& runner,
                                                const std::string& context = "")
{
    return runner.registerTestSuite(TestSuite::create(name, context), true);
}

/**
 * @brief Describe and register a TestSuite to the given runner.
 * @note All test cases in this suite will be executed in parallel.
 * @tparam T The component/context classname where this suite belongs to
 * @param  name The name/description
 * @param runner  The TestSuitesRunner to register
 * @return a shared pointer to the created TestSuite
 */
template<typename T>
static TestSuite_shared describeParallel(const std::string& name,
                                         test::TestSuitesRunner& runner)
{
    return runner.registerTestSuite(TestSuite::create(name, util::typeName<T>()), true);
}

/**
 * @brief Describe and register a TestSuite to the given runner.
 * @note All test cases in this suite will be executed sequentially.
 * @param name The name/description
 * @param context The component/context to test as string (default="")
 * @param runner The TestSuitesRunner to register
 * @return a shared pointer to the created TestSuite
 */
inline static TestSuite_shared describe(const std::string& name,
                                        test::TestSuitesRunner& runner,
                                        const std::string& context = "")
{
    return runner.registerTestSuite(TestSuite::create(name, context), false);
}

/**
 * @brief Describe and register a TestSuite to the given runner.
 * @note All test cases in this suite will be executed sequentially.
 * @tparam T The component/context classname where this suite belongs to
 * @param  name The name/description
 * @param runner  The TestSuitesRunner to register
 * @return a shared pointer to the created TestSuite
 */
template<typename T>
static TestSuite_shared describe(const std::string& name, test::TestSuitesRunner& runner)
{
    return runner.registerTestSuite(TestSuite::create(name, util::typeName<T>()), false);
}

}  // namespace sctf

#endif  // SRC_TESTSUITE_TESTSUITESRUNNER_HPP_
