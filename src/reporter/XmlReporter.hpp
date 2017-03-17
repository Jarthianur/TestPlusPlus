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
#include <string>

#include "../testsuite/TestCase.hpp"
#include "../testsuite/TestStats.hpp"
#include "../testsuite/TestSuite.hpp"
#include "../util/types.h"
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
    inline virtual ~XmlReporter() noexcept
    {
    }

protected:
    /**
     * impl
     */
    inline virtual void reportTestSuite(TestSuite_shared ts)
    {
        std::time_t stamp = std::chrono::system_clock::to_time_t(ts->mTimestamp);
        char buff[128];
        std::strftime(buff, 127, "%FT%T", std::localtime(&stamp));

        *this << SPACE << "<testsuite id=\"" << id++ << "\" name=\"" << ts->mName
              << "\" errors=\"" << ts->getTestStats().getNumErrs() << "\" tests=\""
              << ts->getTestStats().getNumTests() << "\" failures=\""
              << ts->getTestStats().getNumFails() << "\" skipped=\"0\" time=\""
              << ts->getTime() << "\" timestamp=\"" << buff << "\">" << LF;

        AbstractReporter::reportTestSuite(ts);

        *this << SPACE << "</testsuite>" << LF;
    }

    /**
     * impl
     */
    virtual void reportTestCase(const TestCase& tc)
    {
        *this << XSPACE << "<testcase name=\"" << tc.mName << "\" classname=\""
              << tc.mClassname << "\" time=\"" << tc.getDuration() << "\"";
        switch (tc.getState())
        {
            case TestCase::ERROR:
                *this << ">" << LF << XSPACE << SPACE << "<error message=\""
                      << tc.getErrMsg() << "\"></error>" << LF << XSPACE << "</testcase>";
                break;
            case TestCase::FAILED:
                *this << ">" << LF << XSPACE << SPACE << "<failure message=\""
                      << tc.getErrMsg() << "\"></failure>" << LF << XSPACE
                      << "</testcase>";
                break;
            case TestCase::PASSED:
                *this << "/>";
                break;
            default:
                break;
        }
        *this << LF;
    }

    /**
     * impl
     */
    virtual void beginReport()
    {
        *this << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" << LF << "<testsuites>"
              << LF;
    }

    /**
     * impl
     */
    virtual void endReport()
    {
        *this << "</testsuites>" << LF;
    }

private:
    std::uint32_t id = 0;

};

} // reporter
} // testsuite

#endif /* REPORTER_XMLREPORTER_HPP_ */
