/*
 * AbstractReporter.hpp
 *
 *  Created on: 10.03.2017
 *      Author: julian
 */

#ifndef REPORTER_ABSTRACTREPORTER_HPP_
#define REPORTER_ABSTRACTREPORTER_HPP_

#include <string>
#include <vector>
#include <iostream>
#include "../testsuite/TestSuite.hpp"

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

    template<typename T>
    inline std::ostream& operator<<(const T& rep)
    {
        out_stream << rep;
        return out_stream;
    }

    inline void report()
    {
        generate();
    }

    inline void registerTestSuite(testsuite::TestSuite_shared ts)
    {
        suites.push_back(ts);
    }

protected:
    std::ostream& out_stream;
    std::vector<testsuite::TestSuite_shared> suites;
    virtual void generate() = 0;
};

#endif /* REPORTER_ABSTRACTREPORTER_HPP_ */
