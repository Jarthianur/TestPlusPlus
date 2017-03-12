#ifndef TESTSUITE_TESTSUITE_HPP_
#define TESTSUITE_TESTSUITE_HPP_

#include <chrono>
#include <cstdint>
#include <exception>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

#include "../comparator/Comparators.h"
#include "../comparator/ComparatorStrategy.hpp"
#include "TestCase.hpp"
#include "TestStats.hpp"
#include "TestSuite_shared.h"
#include "../util/serialize.hpp"

namespace testsuite
{

class TestSuite: public std::enable_shared_from_this<TestSuite>
{
public:
    TestSuite(const TestSuite&) = delete;
    TestSuite& operator=(const TestSuite&) = delete;

    inline static TestSuite_shared create(const std::string& name)
    {
        return TestSuite_shared(new TestSuite(name));
    }

    inline virtual ~TestSuite() throw ()
    {
    }

    /**
     * TODO
     * add assertException
     */

    /**
     * assertPerformance
     */
    template<typename F, typename ... Args>
    inline TestSuite_shared assertPerformance(const std::string& descr, F func,
                                              std::chrono::milliseconds maxTime,
                                              const Args&... args)
    {
        TestCase_shared tc = TestCase::create(descr, std::to_string(maxTime.count()), {},
                                              "ms runtime less then");
        timestamp = std::chrono::high_resolution_clock::now();

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
                std::chrono::high_resolution_clock::now() - timestamp).count();
        time += tc->time;
        double runtime = (double) tc->time / 1000.0;
        tc->value = std::to_string(runtime);

        if ((std::uint64_t) runtime < maxTime.count())
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
     * Create a TestCase for given
     * descr: TestCase name
     * func: functor (method to test)
     * expected: expected return value
     * comp: Comparator for return value
     * args...: Argument list for func
     */
    template<typename T, typename F, typename ...Args>
    inline TestSuite_shared assert(const std::string& descr, F func, const T& expected,
                                   comparator::Comparator comp, const Args&... args)
    {
        timestamp = std::chrono::high_resolution_clock::now();

        auto list = { args... };
        std::vector<std::string> str_args;
        for (auto arg = list.begin(); arg != list.end(); arg++)
        {
            str_args.push_back(util::serialize(*arg));
        }
        TestCase_shared tc = TestCase::create(descr, util::serialize(expected),
                                              str_args, comp->assertion);

        _assert(func, expected, comp, tc, args...);

        tc->time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::high_resolution_clock::now() - timestamp).count();
        time += tc->time;

        return shared_from_this();
    }

    /**
     * Create a TestCase for given
     * descr: TestCase name
     * func: functor (method to test)
     * expected: expected return value
     * comp: Comparator for return value
     */
    template<typename T, typename F>
    inline TestSuite_shared assert(const std::string& descr, F func, const T& expected,
                                   comparator::Comparator comp)
    {
        timestamp = std::chrono::high_resolution_clock::now();

        TestCase_shared tc = TestCase::create(descr, util::serialize(expected), {},
                                              comp->assertion);

        _assert(func, expected, comp, tc);

        tc->time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::high_resolution_clock::now() - timestamp).count();
        time += tc->time;

        return shared_from_this();
    }

    std::string name;
    std::uint64_t time = 0;
    TestStats stats;
    std::vector<TestCase_shared> testcases;
    std::chrono::time_point<std::chrono::high_resolution_clock> timestamp;

private:
    inline TestSuite(const std::string& name)
            : name(name)
    {
    }

    template<typename T, typename F, typename ...Args>
    inline void _assert(F func, const T& e, comparator::Comparator c, TestCase_shared tc,
                        const Args&... args)
    {
        stats.num_of_tests++;

        try
        {
            const T result = func(args...);

            if (c->compare(result, e))
            {
                tc->pass(true);
            }
            else
            {
                stats.num_of_fails++;
                tc->pass(false);
            }
            tc->value = util::serialize(result);
        }
        catch (const std::exception& e)
        {
            stats.num_of_errs++;
            tc->erroneous(e.what());
        }

        testcases.push_back(tc);
    }

};

} // testsuite

#endif /* TESTSUITE_TESTSUITE_HPP_ */
