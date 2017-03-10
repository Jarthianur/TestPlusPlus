/*
 * TestSuite_shared.h
 *
 *  Created on: 10.03.2017
 *      Author: julian
 */

#ifndef TESTSUITE_TESTSUITE_SHARED_H_
#define TESTSUITE_TESTSUITE_SHARED_H_

#include <memory>

namespace testsuite
{
class TestSuite;
using TestSuite_shared = std::shared_ptr<TestSuite>;
}
#endif /* TESTSUITE_TESTSUITE_SHARED_H_ */
