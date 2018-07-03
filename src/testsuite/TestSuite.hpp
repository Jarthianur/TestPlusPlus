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
#include <iterator>
#include <memory>
#include <string>
#include <typeinfo>
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
class TestSuite : public std::enable_shared_from_this<TestSuite>
{
public:
    TestSuite(const TestSuite&) = delete;
    TestSuite& operator=(const TestSuite&) = delete;

    /**
     * Create a TestSuite
     * name: name/description
     */
    inline static TestSuite_shared create(const std::string& name,
                                          const std::string& ctxt)
    {
        return TestSuite_shared(new TestSuite(name, ctxt));
    }

    /**
     * d'tor
     */
    virtual ~TestSuite() noexcept
    {}

    /**
     * Execute all test cases sequentially.
     */
    void execute() noexcept
    {
        mStats.num_of_tests = mTestCases.size();
        for(auto& tc : mTestCases)
        {
            switch(tc.execute())
            {
                case TestCase::FAILED:
                    mStats.num_of_fails++;
                    break;
                case TestCase::ERROR:
                    mStats.num_of_errs++;
                    break;
                default:
                    break;
            }
            mTime += tc.getDuration();
        }
    }

    /**
     * Execute all test cases in parallel, using openmp.
     */
    void executeParallel() noexcept
    {
        mStats.num_of_tests = mTestCases.size();
#pragma omp parallel
        {
            double tmp          = 0.0;
            std::uint32_t fails = 0;
            std::uint32_t errs  = 0;
#pragma omp for schedule(dynamic)
            for(auto tc = mTestCases.begin(); tc < mTestCases.end(); ++tc)
            {
                switch(tc->execute())
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
                tmp += tc->getDuration();
            }
#pragma omp atomic
            mStats.num_of_fails += fails;
#pragma omp atomic
            mStats.num_of_errs += errs;
#pragma omp critical
            {
                if(mTime < tmp)
                {
                    mTime = tmp;
                }
            }
        }
    }

    /**
     * Create a test case.
     * template T: class for testing methods as classtype
     * name: name/description
     * func: test function, exec ops and assertions
     * Chainable
     */
    template<typename T>
    TestSuite_shared test(const std::string& name, test_function func)
    {
        mTestCases.push_back(TestCase(name, typeid(T).name(), func));
        return shared_from_this();
    }

    /**
     * Create a test case.
     * name: name/description
     * classname: class/context for testing methods as string
     * func: test function, exec ops and assertions
     * Chainable
     */
    TestSuite_shared test(const std::string& name, const std::string& classname,
                          test_function func)
    {
        mTestCases.push_back(TestCase(name, classname, func));
        return shared_from_this();
    }

    /**
     * Create a test case.
     * This will use the test suites context for classname.
     * name: name/description
     * classname: class/context for testing methods as string
     * func: test function, exec ops and assertions
     * Chainable
     */
    TestSuite_shared test(const std::string& name, test_function func)
    {
        mTestCases.push_back(TestCase(name, mContext, func));
        return shared_from_this();
    }

    inline const TestStats& getTestStats() const
    {
        return mStats;
    }

    inline double getTime() const
    {
        return mTime;
    }

    inline const std::vector<TestCase>& getTestCases() const
    {
        return mTestCases;
    }

    const std::string mName;
    const std::chrono::system_clock::time_point mTimestamp;

private:
    /**
     * c'tor with name, setting timestamp.
     */
    TestSuite(const std::string& name, const std::string& ctxt)
        : mName(name), mTimestamp(std::chrono::system_clock::now()), mContext(ctxt)
    {}

    /**
     * milliseconds
     */
    double mTime = 0.0;
    TestStats mStats;
    std::vector<TestCase> mTestCases;
    const std::string mContext;
};

}  // testsuite

#endif /* TESTSUITE_TESTSUITE_HPP_ */
