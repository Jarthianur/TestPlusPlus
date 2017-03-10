/*
 * PlainTextReporter.hpp
 *
 *  Created on: 10.03.2017
 *      Author: julian
 */

#ifndef REPORTER_PLAINTEXTREPORTER_HPP_
#define REPORTER_PLAINTEXTREPORTER_HPP_

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

    inline virtual void generate() {

    }
};

#endif /* REPORTER_PLAINTEXTREPORTER_HPP_ */
