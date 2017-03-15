/*
 * types.h
 *
 *  Created on: 15.03.2017
 *      Author: julian
 */

#ifndef SRC_UTIL_TYPES_H_
#define SRC_UTIL_TYPES_H_

#include <functional>

namespace testsuite
{

using test_function = std::function<void()>;
/**
 * Typedef for shared ptr to TestSuite
 */
class TestSuite;
using TestSuite_shared = std::shared_ptr<TestSuite>;

namespace reporter
{

/**
 * Typedef for shared ptr
 */
class AbstractReporter;
using AbstractReporter_shared = std::shared_ptr<AbstractReporter>;

}  // namespace reporter

}  // namespace testsuite

#endif /* SRC_UTIL_TYPES_H_ */
