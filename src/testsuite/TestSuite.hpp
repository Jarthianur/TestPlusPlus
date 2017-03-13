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
#include <exception>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>
#include <typeinfo>

#include "../comparator/ComparatorStrategy.hpp"
#include "../reporter/AbstractReporter.hpp"
#include "TestCase.hpp"
#include "TestStats.hpp"
#include "TestSuite_shared.h"
#include "../util/serialize.hpp"

//disable assert macro
#ifdef assert
#undef assert
#endif

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

    /**
     * Assertion of performance of a given
     * non-static member method of class.
     * Does not check expected return value.
     * Runtime of given method must be less
     * the given number of milliseconds.
     * Consider that there may be an inevitable overhead.
     * Chainable
     * descr: representative name/description
     * func: method adr to test
     * instance: instance of class with func
     * maxTime: max allowed runtime (ms)
     * args: arguments fo given method to test
     */
    template<typename F, typename C, typename ... Args>
    inline TestSuite_shared assertPerformance(const std::string& descr, F func,
                                              C instance, std::uint64_t maxTime,
                                              const Args&... args)
    {
        stats.num_of_tests++;
        TestCase_shared tc = TestCase::create(descr, typeid(instance).name(),
                                              std::to_string(maxTime), {},
                                              "ms runtime less then");
        auto start = std::chrono::high_resolution_clock::now();

        try
        {
            std::bind(func, instance, args...)();
        }
        catch (const std::exception& e)
        {
            stats.num_of_errs++;
            tc->erroneous(e.what());
        }

        tc->time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::high_resolution_clock::now() - start).count();
        time += tc->time;
        double runtime = (double) tc->time / 1000.0;
        tc->value = std::to_string(runtime);

        if ((std::uint64_t) runtime < maxTime)
        {
            tc->pass(true);
        }
        else
        {
            tc->pass(false);
            stats.num_of_fails++;
        }

        testcases.push_back(tc);
        return shared_from_this();
    }

    /**
     * Assertion of performance of a given
     * static method.
     * Does not check expected return value.
     * Runtime of given method must be less
     * the given number of milliseconds.
     * Consider that there may be an inevitable overhead.
     * Chainable
     * descr: representative name/description
     * func: method to test
     * maxTime: max allowed runtime (ms)
     * args: arguments fo given method to test
     */
    template<typename F, typename ... Args>
    inline TestSuite_shared assertPerformance(const std::string& descr, F func,
                                              std::uint64_t maxTime, const Args&... args)
    {
        stats.num_of_tests++;
        TestCase_shared tc = TestCase::create(descr, "TestCase", std::to_string(maxTime),
                                              {}, "ms runtime less then");
        auto start = std::chrono::high_resolution_clock::now();

        try
        {
            func(args...);
        }
        catch (const std::exception& e)
        {
            stats.num_of_errs++;
            tc->erroneous(e.what());
        }

        tc->time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::high_resolution_clock::now() - start).count();
        time += tc->time;
        double runtime = (double) tc->time / 1000.0;
        tc->value = std::to_string(runtime);

        if ((std::uint64_t) runtime < maxTime)
        {
            tc->pass(true);
        }
        else
        {
            tc->pass(false);
            stats.num_of_fails++;
        }

        testcases.push_back(tc);
        return shared_from_this();
    }

    /**
     * Assertion for correct return value of
     * given non-static member method on instance of class.
     * Chainable
     * descr: TestCase name
     * func: method adr (method to test)
     * instance: instance of class with func
     * expected: expected return value
     * comp: Comparator for return value
     * args...: Argument list for func
     */
    template<typename T, typename F, typename C, typename ...Args>
    inline TestSuite_shared assert(const std::string& descr, F func, C instance,
                                   const T& expected, comparator::Comparator<T> comp,
                                   const Args&... args)
    {
        stats.num_of_tests++;
        auto start = std::chrono::high_resolution_clock::now();
        TestCase_shared tc = TestCase::create(descr, typeid(instance).name(),
                                              util::serialize(expected),
                                              serializeArgs(args...), comp->assertion);

        try
        {
            const T result = std::bind(func, instance, args...)();
            _assert(result, expected, comp, tc);
        }
        catch (const std::exception& e)
        {
            stats.num_of_errs++;
            tc->erroneous(e.what());
        }

        testcases.push_back(tc);

        tc->time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::high_resolution_clock::now() - start).count();
        time += tc->time;

        return shared_from_this();
    }

    /**
     * Assertion for correct return value of given static method.
     * Chainable
     * descr: TestCase name
     * func: functor (method to test)
     * expected: expected return value
     * comp: Comparator for return value
     * args: argument list for func
     */
    template<typename T, typename F, typename ... Args>
    inline TestSuite_shared assert(const std::string& descr, F func, const T& expected,
                                   comparator::Comparator<T> comp, const Args&... args)
    {
        stats.num_of_tests++;
        auto start = std::chrono::high_resolution_clock::now();
        TestCase_shared tc = TestCase::create(descr, "TestCase",
                                              util::serialize(expected),
                                              serializeArgs(args...), comp->assertion);

        try
        {
            const T result = func(args...); //std::bind(func, args...)();
            _assert(result, expected, comp, tc);
        }
        catch (const std::exception& e)
        {
            stats.num_of_errs++;
            tc->erroneous(e.what());
        }

        testcases.push_back(tc);

        tc->time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::high_resolution_clock::now() - start).count();
        time += tc->time;

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
    std::vector<TestCase_shared> testcases;

    /**
     * timestamp, the testsuite was started
     */
    const std::chrono::time_point<std::chrono::system_clock> timestamp;

private:
    /**
     * c'tor with name, setting timestamp.
     */
    inline TestSuite(const std::string& name)
            : name(name),
              timestamp(std::chrono::system_clock::now())
    {
    }

    /**
     * serialize args
     */
    template<typename ...Args>
    inline const std::vector<std::string> serializeArgs(const Args&... args)
    {
        auto list = { args... };
        std::vector<std::string> str_args;
        for (auto arg = list.begin(); arg != list.end(); arg++)
        {
            str_args.push_back(util::serialize(*arg));
        }
        return str_args;
    }

    inline const std::vector<std::string> serializeArgs()
    {
        return
        {};
    }

    /**
     * Internal assert method to prevent code duplication.
     */
    template<typename T>
    inline void _assert(const T& res, const T& e, comparator::Comparator<T> c,
                        TestCase_shared tc)
    {
        if (c->compare(res, e))
        {
            tc->pass(true);
        }
        else
        {
            stats.num_of_fails++;
            tc->pass(false);
        }
        tc->value = util::serialize(res);
    }

};

/**
 * Factory method to create a TestSuite with given reporter.
 * test: representative name/description
 */
inline TestSuite_shared test(const std::string& test,
                             std::shared_ptr<reporter::AbstractReporter> reporter)
{
    TestSuite_shared ts = TestSuite::create(test);
    reporter->registerTestSuite(ts);
    return ts;
}

} // testsuite

#endif /* TESTSUITE_TESTSUITE_HPP_ */
