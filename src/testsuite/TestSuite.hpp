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
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <omp.h>
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
     */
    inline static TestSuite_shared create(const std::string& name)
    {
        return TestSuite_shared(new TestSuite(name));
    }

    /**
     * d'tor
     */
    inline virtual ~TestSuite() throw ()
    {
    }

    /**
     * TODO
     * add assertException
     */

    inline void execute()
    {
        for (auto& tc : testcases)
        {
            stats.num_of_tests++;
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

    inline void executeParallel()
    {
#pragma omp parallel for
        for (auto tc = testcases.begin(); tc < testcases.end(); tc++)
        {
            stats.num_of_tests++;
            switch (tc->execute())
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
            time += tc->duration;
        }
    }

    inline TestSuite_shared test(const std::string& descr, const std::string& cn,
                                 test_function func)
    {
        testcases.push_back(TestCase(descr, cn, func));
        return shared_from_this();
    }

    /**
     * name/description
     */
    std::string name;

    /**
     * runtime in nanoseconds
     */
    std::uint64_t time = 0;

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
