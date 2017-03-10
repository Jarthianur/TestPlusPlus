/*
 * ColoredReporter.hpp
 *
 *  Created on: 10.03.2017
 *      Author: julian
 */

#ifndef REPORTER_COLOREDREPORTER_HPP_
#define REPORTER_COLOREDREPORTER_HPP_

#include <iostream>
#include <string>

#include "AbstractReporter.hpp"

class PlainTextReporter: public AbstractReporter
{
public:
    inline PlainTextReporter(std::ostream& stream)
            : AbstractReporter(stream)
    {
    }

    inline virtual ~PlainTextReporter() throw ()
    {
    }

    inline virtual void generate()
    {

    }

private:
    enum class ANSI_COLORS
        : std::string
        {
            RED = "\x1b[31m",
        GREEN = "\x1b[32m",
        YELLOW = "\x1b[33m",
        BLUE = "\x1b[34m",
        MAGENTA = "\x1b[35m",
        CYAN = "\x1b[36m",
        RESET = "\x1b[0m"
    };
};

#endif /* REPORTER_COLOREDREPORTER_HPP_ */
