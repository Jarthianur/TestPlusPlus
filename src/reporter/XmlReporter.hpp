/*
 Copyright_License {

 Copyright (C) 2017 Julian P. Becht
 Author: Julian P. Becht

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License version 3
 as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 }
 */

#ifndef REPORTER_XMLREPORTER_HPP_
#define REPORTER_XMLREPORTER_HPP_

#include <chrono>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>

#include "../testsuite/TestCase.hpp"
#include "../testsuite/TestStats.hpp"
#include "../testsuite/TestSuite.hpp"
#include "AbstractReporter.hpp"

namespace testsuite
{
namespace reporter
{

/**
 * Concrete reporter,
 * featuring JUnit XML format.
 */
class XmlReporter: public AbstractReporter
{
public:
    /**
     * c'tor with stream
     */
    inline XmlReporter(std::ostream& stream)
            : AbstractReporter(stream)
    {
    }

    /**
     * d'tor
     */
    inline virtual ~XmlReporter() throw ()
    {
    }

    /**
     * Implementation of generate()
     */
    inline virtual std::int32_t generate()
    {
        std::int32_t ret_val = 0;
        *this << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << LF << "<testsuites>"
              << LF;

        for (auto ts : suites)
        {
            ret_val += (ts->stats.num_of_errs + ts->stats.num_of_fails);
            std::time_t stamp = std::chrono::system_clock::to_time_t(
                    ts->timestamp);
            char buff[128];
            std::strftime(buff, 127, "%FT%T", std::localtime(&stamp));

            *this << SPACE << "<testsuite name=\"" << ts->name << "\" errors=\""
                  << ts->stats.num_of_errs << "\" tests=\"" << ts->stats.num_of_tests
                  << "\" failures=\"" << ts->stats.num_of_fails << "\" time=\""
                  << (double) ts->time / 1000 << "\" timestamp=\"" << buff << "\">" << LF;

            for (auto tc : ts->testcases)
            {
                *this << XSPACE << "<testcase name=\"" << tc->name << "\" classname=\""
                      << tc->classname << "\" time=\"" << (double) tc->time / 1000 << "\">"
                      << LF;

                if (tc->error)
                {
                    *this << XSPACE << XSPACE << "<error message=\"" << tc->value
                          << "\"></error>" << LF;
                }
                else if (!tc->passed)
                {
                    *this << XSPACE << XSPACE
                          << "<failure message=\"Assertion failed, expected ("
                          << tc->value << ") " << tc->assertion << " (" << tc->expected
                          << ")\"></failure>" << LF;
                }

                *this << XSPACE << "</testcase>" << LF;
            }
            *this << SPACE << "</testsuite>" << LF;
        }
        *this << "</testsuites>" << LF;
        return ret_val;
    }
};

} // reporter
} // testsuite

#endif /* REPORTER_XMLREPORTER_HPP_ */
