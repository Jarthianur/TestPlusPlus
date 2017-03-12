/*
 * AbstractReporter.hpp
 *
 *  Created on: 10.03.2017
 *      Author: julian
 */

#ifndef REPORTER_ABSTRACTREPORTER_HPP_
#define REPORTER_ABSTRACTREPORTER_HPP_

#include <cstdint>
#include <iostream>
#include <vector>

#include "../testsuite/TestSuite_shared.h"

namespace testsuite
{
namespace reporter
{

#define LF "\n"
#define SPACE "  "
#define XSPACE "    "

class AbstractReporter
{

public:
    inline AbstractReporter(std::ostream& stream)
            : out_stream(stream)
    {
    }

    inline virtual ~AbstractReporter() throw ()
    {
    }

    inline std::int32_t report()
    {
        return generate();
    }

    inline void registerTestSuite(TestSuite_shared ts)
    {
        suites.push_back(ts);
    }

protected:
    std::ostream& out_stream;
    std::vector<TestSuite_shared> suites;

    virtual std::int32_t generate() = 0;

    template<typename T>
    inline std::ostream& operator<<(const T& rep)
    {
        out_stream << rep;
        return out_stream;
    }
};

} // reporter
} // testsuite

#endif /* REPORTER_ABSTRACTREPORTER_HPP_ */
