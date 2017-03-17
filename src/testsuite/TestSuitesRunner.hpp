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

#include "../util/types.h"
#include "TestSuite.hpp"

namespace testsuite
{
using namespace reporter;

/**
 * Test suite runner,
 * holding all test suites.
 */
class TestSuitesRunner
{
public:
    /**
     * c'tor
     */
    inline TestSuitesRunner()
    {
    }

    /**
     * d'tor
     */
    inline ~TestSuitesRunner() noexcept
    {
    }

    /**
     * Status of execution
     */
    enum ExecStatus
        : std::uint32_t
        {
            /**
             * No test suites executed.
             */
            NONE,
        /**
         * Sequential test suites executed.
         */
        SEQUENTIAL,
        /**
         * Parallel test suites executed.
         */
        PARALLEL,
        /**
         * All test suites executed.
         * State must be set to this to be able to report.
         */
        ALL
    };

    /**
     * Register a test suite.
     * ts: shared ptr to test suite
     * parallel: exec as parallel or sequential
     */
    inline TestSuite_shared registerTestSuite(TestSuite_shared ts, bool parallel)
    {
        if (parallel)
        {
            mParallelTSs.push_back(ts);
        }
        else
        {
            mSequentialTSs.push_back(ts);
        }
        return ts;
    }

    /**
     * Execute parallel test suites.
     */
#ifdef _OPENMP
    inline void executeParallel() noexcept
    {
        for (auto ts : mParallelTSs)
        {
            ts->executeParallel();
        }
        if (mExec == SEQUENTIAL)
        {
            mExec = ALL;
        }
        else if (mExec == NONE)
        {
            mExec = PARALLEL;
        }
    }
#endif

    /**
     * Execute sequential test suites.
     */
    inline void executeSequential() noexcept
    {
        for (auto ts : mSequentialTSs)
        {
            ts->execute();
        }
        if (mExec == PARALLEL)
        {
            mExec = ALL;
        }
        else if (mExec == NONE)
        {
            mExec = SEQUENTIAL;
        }
    }

    /**
     * Execute all test suites.
     */
    inline void executeAll() noexcept
    {
#ifdef _OPENMP
        executeParallel();
#endif
        executeSequential();
    }

    /**
     * Get execution status.
     */
    inline const ExecStatus getStatus() const
    {
        return mExec;
    }

    /**
     * Get parallel and sequential test suites as pair.
     * All have to be executed until call,
     * logic_error is thrown if not.
     */
    inline const std::pair<std::vector<TestSuite_shared>&, std::vector<TestSuite_shared>&> getTestSuites()
    {
        if (mExec == ALL)
        {
            return std::pair<std::vector<TestSuite_shared>&,
                    std::vector<TestSuite_shared>&>(mSequentialTSs, mParallelTSs);
        }
        else
        {
            throw std::logic_error("TestSuites not yet executed");
        }
    }

private:
    /**
     * Sequential test suites
     */
    std::vector<TestSuite_shared> mSequentialTSs;

    /**
     * Parallel test suites
     */
    std::vector<TestSuite_shared> mParallelTSs;

    /**
     * Execution status
     */
    ExecStatus mExec = NONE;
};

/**
 * Describe and register a test suite to the given runner.
 * All test cases in this suite will be executed in parallel!
 */
#ifdef _OPENMP
inline TestSuite_shared describeParallel(const std::string& name,
        TestSuitesRunner& runner)
{
    return runner.registerTestSuite(TestSuite::create(name), true);
}
#endif

/**
 * Describe and register a test suite to the given runner.
 * All test cases in this suite will be executed sequentially!
 */
inline TestSuite_shared describe(const std::string& name, TestSuitesRunner& runner)
{
    return runner.registerTestSuite(TestSuite::create(name), false);
}

} // testsuite

#endif /* SRC_TESTSUITE_TESTSUITESRUNNER_HPP_ */
