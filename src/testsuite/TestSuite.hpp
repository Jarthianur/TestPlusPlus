#ifndef TESTSUITE_TESTSUITE_HPP_
#define TESTSUITE_TESTSUITE_HPP_

#include <cstdint>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>
#include <sstream>

#include "../comparator/Comparators.h"
#include "../comparator/ComparatorStrategy.hpp"
#include "../reporter/AbstractReporter.hpp"
#include "TestCase.hpp"
#include "TestStats.hpp"
#include "TestSuite_shared.h"

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

    template<typename T, typename ...Args>
    inline TestSuite_shared assert(const std::string& descr, const T& value,
                                   const T& expected, comparator::Comparator comp,
                                   const Args&... args)
    {
        auto list = { args... };
        std::vector<const std::string> str_args;
        std::stringstream ss;
        for (auto arg : list)
        {
            ss << arg;
            str_args.push_back(ss.str());
            ss.clear();
        }
        TestCase tc(descr, std::string(value), std::string(expected), str_args);
        stats.num_of_tests++;
        //eval duration add to time
        // value as method?
        if (comp->compare(value, expected))
        {
            tc.pass(true/*duration*/);
        }
        else
        {
            stats.num_of_fails++;
            tc.pass(false/*duration*/);
        }
        testcases.push_back(tc);
        return shared_from_this();
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

inline TestSuite_shared test(const std::string& test,
                             std::shared_ptr<AbstractReporter> reporter)
{
    TestSuite_shared ts = TestSuite::create(test);
    reporter->registerTestSuite(ts);
    return ts;
}

} // testsuite

#endif /* TESTSUITE_TESTSUITE_HPP_ */
