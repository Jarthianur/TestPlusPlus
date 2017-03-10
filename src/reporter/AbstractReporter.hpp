/*
 * AbstractReporter.hpp
 *
 *  Created on: 10.03.2017
 *      Author: julian
 */

#ifndef REPORTER_ABSTRACTREPORTER_HPP_
#define REPORTER_ABSTRACTREPORTER_HPP_

#include <sstream>
#include <string>
#include <vector>

#include "../testsuite/TestSuite_shared.h"

#define LF "\n"

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

    template<typename T>
    inline std::stringstream& operator<<(const T& rep)
    {
        _report << rep;
        return _report;
    }

    inline void report()
    {
        generate();
        out_stream << _report.str();
    }

    inline void registerTestSuite(testsuite::TestSuite_shared ts)
    {
        suites.push_back(ts);
    }

protected:
    std::ostream& out_stream;
    std::stringstream _report;
    std::vector<testsuite::TestSuite_shared> suites;
    virtual void generate() = 0;
};

#endif /* REPORTER_ABSTRACTREPORTER_HPP_ */
