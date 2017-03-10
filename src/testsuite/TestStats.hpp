#ifndef TESTSUITE_TESTSTATS_HPP_
#define TESTSUITE_TESTSTATS_HPP_

#include <cstdint>

class TestStats
{
public:
    inline TestStats()
    {
    }
    inline virtual ~TestStats() throw ()
    {
    }

    std::uint32_t num_of_tests = 0;
    std::uint32_t num_of_fails = 0;
};

#endif /* TESTSUITE_TESTSTATS_HPP_ */
