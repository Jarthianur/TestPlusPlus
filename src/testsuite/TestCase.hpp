#ifndef TESTSUITE_TESTCASE_HPP_
#define TESTSUITE_TESTCASE_HPP_

#include <memory>
#include <string>

#include "../comparator/Comparators.h"
#include "../comparator/ComparatorStrategy.hpp"
#include "TestStatistic.hpp"

class AbstractReporter;

class TestCase: public std::enable_shared_from_this<TestCase>
{

public:
    TestCase(const TestCase&) = delete;
    TestCase& operator=(const TestCase&) = delete;

    inline static std::shared_ptr<TestCase> create(std::shared_ptr<AbstractReporter> rep)
    {
        return std::shared_ptr<TestCase>(new TestCase(rep));
    }

    inline virtual ~TestCase()
    {
    }

    template<typename T, typename ...Args>
    inline std::shared_ptr<TestCase> assert(const std::string& descr, const T& value,
                                            const T& expected,
                                            comparator::Comparator comp,
                                            const Args&... args)
    {
        teststatistic::num_of_tests++;
        // report
        if (comp->compare(value, expected))
        {
            pass(descr);
        }
        else
        {
            fail(descr, expected);
        }
        return shared_from_this();
    }

    template<typename T>
    inline std::shared_ptr<TestCase> assert(const std::string& descr, const T& value,
                                            const T& expected,
                                            comparator::Comparator comp)
    {
        teststatistic::num_of_tests++;
        // report
        if (comp->compare(value, expected))
        {
            pass(descr);
        }
        else
        {
            fail(descr, expected);
        }
        return shared_from_this();
    }

private:
    inline TestCase(std::shared_ptr<AbstractReporter> rep)
            : reporter(rep)
    {
    }

    std::shared_ptr<AbstractReporter> reporter;

    inline void pass(const std::string& descr)
    {
        //report
    }

    template<typename T>
    inline void fail(const std::string& descr, const T& expected)
    {
        //report
        teststatistic::num_of_fails++;
    }
};

#endif /* TESTSUITE_TESTCASE_HPP_ */
