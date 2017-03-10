#ifndef TESTSUITE_TESTSUITE_HPP_
#define TESTSUITE_TESTSUITE_HPP_

#include <chrono>
#include <cstdint>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

#include "../comparator/Comparators.h"
#include "../comparator/ComparatorStrategy.hpp"
#include "TestCase.hpp"
#include "TestStats.hpp"
#include "TestSuite_shared.h"

namespace testsuite
{

inline std::string serialize(const char*& arg)
{
    return std::string(arg);
}

inline std::string serialize(const int& arg)
{
    return std::to_string(arg);
}

inline std::string serialize(const double& arg)
{
    return std::to_string(arg);
}

inline std::string serialize(const std::string& arg)
{
    return arg;
}

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

    template<typename T, typename F, typename ...Args>
    inline TestSuite_shared assert(const std::string& descr, F func, const T& expected,
                                   comparator::Comparator comp, const Args&... args)
    {
        auto list = { args... };
        std::vector<std::string> str_args;
        for (auto arg = list.begin(); arg != list.end(); arg++)
        {
            str_args.push_back(serialize(*arg));
        }
        TestCase tc(descr, serialize(func(args...)), serialize(expected), str_args);

        stats.num_of_tests++;

        auto start = std::chrono::system_clock::now();

        // value as method?
        if (comp->compare(func(args...), expected))
        {
            tc.pass(true);
        }
        else
        {
            stats.num_of_fails++;
            tc.pass(false);
        }

        tc.time = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now() - start).count();
        time += tc.time;

        testcases.push_back(tc);
        return shared_from_this();
    }

    template<typename T, typename F>
    inline TestSuite_shared assert(const std::string& descr, F func, const T& expected,
                                   comparator::Comparator comp)
    {
        TestCase tc(descr, "", serialize(expected), {});
        stats.num_of_tests++;

        auto start = std::chrono::system_clock::now();

        // value as method?
        if (comp->compare(func(), expected))
        {
            tc.pass(true);
        }
        else
        {
            stats.num_of_fails++;
            tc.pass(false);
        }

        tc.time = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now() - start).count();
        time += tc.time;

        testcases.push_back(tc);
        return shared_from_this();;
    }

    std::string name;
    std::uint64_t time = 0;
    TestStats stats;
    std::vector<TestCase> testcases;

private:
    inline TestSuite(const std::string& name)
            : name(name)
    {
    }

};

} // testsuite

#endif /* TESTSUITE_TESTSUITE_HPP_ */
