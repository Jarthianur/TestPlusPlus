#ifndef TESTSUITE_TESTCASE_HPP_
#define TESTSUITE_TESTCASE_HPP_

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace testsuite
{

class TestCase;

using TestCase_shared = std::shared_ptr<TestCase>;

class TestCase
{

public:
    TestCase(const TestCase&) = delete;
    TestCase& operator=(const TestCase&) = delete;

    inline static TestCase_shared create(const std::string& name,
                                         const std::string& expected,
                                         const std::vector<std::string>& args,
                                         const std::string& assert)
    {
        return TestCase_shared(new TestCase(name, expected, args, assert));
    }

    inline virtual ~TestCase()
    {
    }

    inline void pass(bool pass)
    {
        passed = pass;
    }

    inline void erroneous(const std::string& err)
    {
        passed = false;
        error = true;
        value = err;
    }

    bool passed = true;
    bool error = false;
    std::uint64_t time = 0;
    std::string name;
    std::string value;
    std::string expected;
    std::vector<std::string> args;
    const std::string assertion;

private:
    inline TestCase(const std::string& name, const std::string& expected,
                    const std::vector<std::string>& args, const std::string& assert)
            : name(name),
              expected(expected),
              args(args),
              assertion(assert)
    {
    }
};

} // testsuite

#endif /* TESTSUITE_TESTCASE_HPP_ */
