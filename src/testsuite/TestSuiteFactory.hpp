#ifndef TESTSUITE_TESTSUITEFACTORY_HPP_
#define TESTSUITE_TESTSUITEFACTORY_HPP_

#include <memory>
#include <string>

#include "../reporter/AbstractReporter.hpp"
#include "TestSuite.hpp"
#include "TestSuite_shared.h"

namespace testsuite {

/**
 * Create a TestSuite specializing a reporter.
 * test: TestSuite name
 */
inline TestSuite_shared test(const std::string& test,
                             std::shared_ptr<reporter::AbstractReporter> reporter)
{
    TestSuite_shared ts = TestSuite::create(test);
    reporter->registerTestSuite(ts);
    return ts;
}

} // testsuite

#endif /* TESTSUITE_TESTSUITEFACTORY_HPP_ */
