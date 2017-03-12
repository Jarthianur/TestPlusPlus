#ifndef TESTSUITE_TESTSUITE_SHARED_H_
#define TESTSUITE_TESTSUITE_SHARED_H_

#include <memory>

namespace testsuite
{

class TestSuite;
using TestSuite_shared = std::shared_ptr<TestSuite>;

} // testsuite

#endif /* TESTSUITE_TESTSUITE_SHARED_H_ */
