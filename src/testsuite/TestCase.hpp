#ifndef TESTSUITE_TESTCASE_HPP_
#define TESTSUITE_TESTCASE_HPP_

#include <cstdint>
#include <string>
#include <vector>

class TestCase
{

public:
    inline TestCase(const std::string& name, const std::string& value,
                    const std::string& expected,
                    const std::vector<std::string>& args)
            : name(name),
              value(value),
              expected(expected),
              args(args)
    {
    }

    inline virtual ~TestCase()
    {
    }

    inline void pass(bool fail)
    {
        failed = fail;
    }

    bool failed = true;
    std::uint64_t time = 0;
    std::string name;
    std::string value;
    std::string expected;
    std::vector<std::string> args;
};

#endif /* TESTSUITE_TESTCASE_HPP_ */
