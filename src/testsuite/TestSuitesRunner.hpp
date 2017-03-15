/*
 * TestSuitesRunner.hpp
 *
 *  Created on: 14.03.2017
 *      Author: julian
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

class TestSuitesRunner
{
public:
    inline TestSuitesRunner()
    {
    }

    inline ~TestSuitesRunner() noexcept
    {
    }

    enum ExecStatus
        : std::uint32_t
        {
            NONE,
        SYNC,
        PARALLEL,
        ALL
    };

    inline TestSuite_shared registerTestSuite(TestSuite_shared ts, bool parallel)
    {
        if (parallel)
        {
            parallelSuites.push_back(ts);
        }
        else
        {
            syncSuites.push_back(ts);
        }
        return ts;
    }

    inline void executeParallel()
    {
        for (auto ts : parallelSuites)
        {
            ts->executeParallel();
        }
        if (exec == SYNC)
        {
            exec = ALL;
        }
        else if (exec == NONE)
        {
            exec = PARALLEL;
        }
    }

    inline void executeSync()
    {
        for (auto ts : syncSuites)
        {
            ts->execute();
        }
        if (exec == PARALLEL)
        {
            exec = ALL;
        }
        else if (exec == NONE)
        {
            exec = SYNC;
        }
    }

    inline void executeAll()
    {
        executeParallel();
        executeSync();
    }

    inline const ExecStatus getStatus()
    {
        return exec;
    }

    inline const std::pair<std::vector<TestSuite_shared>&, std::vector<TestSuite_shared>&> getTestSuites()
    {
        if (exec == ALL)
        {
            return std::pair<std::vector<TestSuite_shared>&,
                    std::vector<TestSuite_shared>&>(syncSuites, parallelSuites);
        }
        else
        {
            throw std::logic_error("TestSuites not yet executed");
        }
    }

private:
    std::vector<TestSuite_shared> syncSuites;
    std::vector<TestSuite_shared> parallelSuites;
    ExecStatus exec = NONE;
};

inline TestSuite_shared describeParallel(const std::string& descr, TestSuitesRunner& run)
{
    return run.registerTestSuite(TestSuite::create(descr), true);
}

inline TestSuite_shared describe(const std::string& descr, TestSuitesRunner& run)
{
    return run.registerTestSuite(TestSuite::create(descr), false);
}

} // testsuite

#endif /* SRC_TESTSUITE_TESTSUITESRUNNER_HPP_ */
