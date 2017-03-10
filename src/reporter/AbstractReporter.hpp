/*
 * AbstractReporter.hpp
 *
 *  Created on: 10.03.2017
 *      Author: julian
 */

#ifndef REPORTER_ABSTRACTREPORTER_HPP_
#define REPORTER_ABSTRACTREPORTER_HPP_

#include <iostream>
#include <sstream>

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

protected:
    std::ostream& out_stream;
    std::stringstream _report;

    virtual void generate() = 0;
};

#endif /* REPORTER_ABSTRACTREPORTER_HPP_ */
