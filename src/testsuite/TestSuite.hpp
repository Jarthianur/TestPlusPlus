#ifndef TESTSUITE_TESTSUITE_HPP_
#define TESTSUITE_TESTSUITE_HPP_

#include <memory>
#include <string>

#include "TestCase.hpp"

class AbstractReporter;

namespace testsuite
{

inline std::shared_ptr<TestCase> test(const std::string& test,
                                      std::shared_ptr<AbstractReporter> reporter)
{
    //do reporter stuff
    return TestCase::create(reporter);
}

} // testsuite

#endif /* TESTSUITE_TESTSUITE_HPP_ */
