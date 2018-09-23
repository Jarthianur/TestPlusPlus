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

#ifndef SCTF_SRC_TESTSUITE_TESTSUITESRUNNER_HPP_
#define SCTF_SRC_TESTSUITE_TESTSUITESRUNNER_HPP_

#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "../types.h"
#include "../util/serialize.hpp"
#include "TestSuite.hpp"
#include "TestSuiteParallel.hpp"

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
     * @brief Register a TestSuite.
     * @param ts A shared ptr to the TestSuite
     * @return a shared pointer to the TestSuite
     */
    TestSuite_shared register_ts(TestSuite_shared ts)
    {
        m_testsuites.push_back(ts);
        return ts;
    }

    /**
     * @brief Run TestSuites.
     */
    void run() noexcept
    {
        for(TestSuite_shared& ts : m_testsuites)
        {
            ts->run();
        }
    }

    /**
     * @brief Get the TestSuites.
     * @return the testsuites
     */
    const std::vector<TestSuite_shared>& testsuites()
    {
        return m_testsuites;
    }

private:
    /// @brief The registered TestSuites.
    std::vector<TestSuite_shared> m_testsuites;
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
    return runner.register_ts(TestSuiteParallel::create(name, context));
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
    return runner.register_ts(TestSuiteParallel::create(name, util::name_for_type<T>()));
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
    return runner.register_ts(TestSuite::create(name, context));
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
    return runner.register_ts(TestSuite::create(name, util::name_for_type<T>()));
}

}  // namespace sctf

#endif  // SCTF_SRC_TESTSUITE_TESTSUITESRUNNER_HPP_
