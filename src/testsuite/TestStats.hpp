#ifndef TESTSUITE_TESTSTATS_HPP_
#define TESTSUITE_TESTSTATS_HPP_

#include <cstdint>

namespace testsuite
{

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
    std::uint32_t num_of_errs = 0;
};

} // testsuite

#endif /* TESTSUITE_TESTSTATS_HPP_ */
