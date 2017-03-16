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

#ifndef TESTSUITE_TESTSUITE_HPP_
#define TESTSUITE_TESTSUITE_HPP_

#include <chrono>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include "../util/types.h"
#include "TestCase.hpp"
#include "TestStats.hpp"

namespace testsuite
{

/**
 * Testsuite class, providing some assertion methods.
 * Non-copyable
 */
class TestSuite: public std::enable_shared_from_this<TestSuite>
{
public:
    TestSuite(const TestSuite&) = delete;
    TestSuite& operator=(const TestSuite&) = delete;

    /**
     * Create a TestSuite
     * name: name/description
     */
    inline static TestSuite_shared create(const std::string& name)
    {
        return TestSuite_shared(new TestSuite(name));
    }

    /**
     * d'tor
     */
    inline virtual ~TestSuite() noexcept
    {
    }

    /**
     * Execute all test cases sequentially.
     */
    inline void execute() noexcept
    {
        stats.num_of_tests = testcases.size();
        for (auto& tc : testcases)
        {
            switch (tc.execute())
            {
                case TestCase::FAILED:
                    stats.num_of_fails++;
                    break;
                case TestCase::ERROR:
                    stats.num_of_errs++;
                    break;
                default:
                    break;
            }
            time += tc.duration;
        }
    }

    /**
     * Execute all test cases in parallel, using openmp.
     */
    inline void executeParallel() noexcept
    {
        stats.num_of_tests = testcases.size();
#pragma omp parallel
        {
            double tmp = 0.0;
            std::uint32_t fails = 0;
            std::uint32_t errs = 0;
#pragma omp for schedule(dynamic)
            for (auto tc = testcases.begin(); tc < testcases.end(); ++tc)
            {
                switch (tc->execute())
                {
                    case TestCase::FAILED:
                        ++fails;
                        break;
                    case TestCase::ERROR:
                        ++errs;
                        break;
                    default:
                        break;
                }
                tmp += tc->duration;
            }
#pragma omp atomic
            stats.num_of_fails += fails;
#pragma omp atomic
            stats.num_of_errs += errs;
#pragma omp critical
            {
                if (time < tmp)
                {
                    time = tmp;
                }
            }
        }
    }

    /**
     * Create a test case.
     * name: name/description
     * classname: class/context for testing methods
     * func: test function, exec ops and assertions
     * Chainable
     */
    inline TestSuite_shared test(const std::string& name, const std::string& classname,
                                 test_function func)
    {
        testcases.push_back(TestCase(name, classname, func));
        return shared_from_this();
    }

    /**
     * name/description
     */
    std::string name;

    /**
     * runtime in milliseconds
     */
    double time = 0.0;

    /**
     * statistics
     */
    TestStats stats;

    /**
     * registered testcases
     */
    std::vector<TestCase> testcases;

    /**
     * timestamp, the testsuite was started
     */
    const std::chrono::system_clock::time_point timestamp;

private:
    /**
     * c'tor with name, setting timestamp.
     */
    inline TestSuite(const std::string& name)
            : name(name),
              timestamp(std::chrono::system_clock::now())
    {
    }
};

} // testsuite

#endif /* TESTSUITE_TESTSUITE_HPP_ */
