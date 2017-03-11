#ifndef TESTSUITE_TESTCASE_HPP_
#define TESTSUITE_TESTCASE_HPP_

#include <cstdint>
#include <string>
#include <vector>

class TestCase
{

public:
    inline TestCase(const std::string& name, const std::string& expected,
            const std::vector<std::string>& args, const std::string& assert)
            : name(name),
              expected(expected),
              args(args),
              assertion(assert)
    {
    }

    inline virtual ~TestCase()
    {
    }

    inline void pass(bool fail)
    {
        passed = fail;
    }

    inline void erroneous(const std::string& err) {
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
};

#endif /* TESTSUITE_TESTCASE_HPP_ */
