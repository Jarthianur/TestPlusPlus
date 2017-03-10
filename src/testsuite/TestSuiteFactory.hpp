/*
 * TestSuiteFactory.hpp
 *
 *  Created on: 10.03.2017
 *      Author: julian
 */

#ifndef TESTSUITE_TESTSUITEFACTORY_HPP_
#define TESTSUITE_TESTSUITEFACTORY_HPP_

#include <memory>
#include <string>

#include "../reporter/AbstractReporter.hpp"
#include "TestSuite.hpp"
#include "TestSuite_shared.h"

namespace testsuite {


inline TestSuite_shared test(const std::string& test,
                             std::shared_ptr<AbstractReporter> reporter)
{
    TestSuite_shared ts = TestSuite::create(test);
    reporter->registerTestSuite(ts);
    return ts;
}

}

#endif /* TESTSUITE_TESTSUITEFACTORY_HPP_ */
