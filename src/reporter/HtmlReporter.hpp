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

#ifndef SRC_REPORTER_HTMLREPORTER_HPP_
#define SRC_REPORTER_HTMLREPORTER_HPP_

#include <cstdint>
#include <iostream>
#include <string>
#include <fstream>

#include "../testsuite/TestCase.hpp"
#include "../testsuite/TestStats.hpp"
#include "../testsuite/TestSuite.hpp"
#include "../util/types.h"
#include "AbstractReporter.hpp"

namespace testsuite
{
namespace reporter
{

#define TD "<td>"
#define TD_ "</td>"
#define TR "<tr>"
#define TR_ "</tr>"
#define TH "<th>"
#define TH_ "</th>"

/**
 * Concrete reporter,
 * featuring html website format.
 */
class HtmlReporter: public AbstractReporter
{
public:
    /**
     * c'tor with stream
     * Defaults to stdout.
     */
    HtmlReporter(std::ostream& stream)
            : AbstractReporter(stream)
    {
    }

    /**
     * c'tor with filename
     */
    HtmlReporter(const char* fnam)
            : AbstractReporter(fnam)
    {
    }

    /**
     * d'tor
     */
    virtual ~HtmlReporter() noexcept
    {
    }

protected:
    /**
     * impl
     */
    virtual void reportTestSuite(TestSuite_shared ts)
    {
        abs_tests += ts->getTestStats().getNumTests();
        abs_fails += ts->getTestStats().getNumFails();
        abs_errs += ts->getTestStats().getNumErrs();
        abs_time += ts->getTime();

        *this << "<h3>" << ts->mName << "</h3>" << "<p>Tests: "
              << ts->getTestStats().getNumTests() << " Failures: "
              << ts->getTestStats().getNumFails() << " Errors: "
              << ts->getTestStats().getNumErrs() << " Time: " << ts->getTime()
              << "ms</p><table><thead>" << TR << TH << "Name" << TH_ << TH << "Classname"
              << TH_ << TH << "Time" << TH_ << TH << "Status" << TH_ << TR_
              << "</thead><tbody>";

        AbstractReporter::reportTestSuite(ts);

        *this << "</tbody></table>";
    }

    /**
     * impl
     */
    virtual void reportTestCase(const TestCase& tc) override
    {
        std::string status;
        switch (tc.getState())
        {
            case TestCase::ERROR:
                status = "error";
                break;
            case TestCase::FAILED:
                status = "failed";
                break;
            case TestCase::PASSED:
                status = "passed";
                break;
            default:
                break;
        }
        *this << "<tr class=\"" << status << "\">" << TD << tc.mName << TD_ << TD
              << tc.mClassname << TD_ << TD << tc.getDuration() << "ms" << TD_ << TD
              << status << TD_ << TR_;
    }

    /**
     * impl
     */
    inline virtual void beginReport() override
    {
        *this << "<!DOCTYPE html><html><head><meta charset=\"utf-8\"/>"
              "<style>body{background-color: linen}table{border-collapse: collapse;min-width: 50%}"
              "tr,th,td{border: 1px solid black;padding: 2px}.failed{background: lightskyblue}"
              ".passed{background: lightgreen}.error{background: lightcoral}</style>"
              "</head><body><header><h1>Test Report</h1></header>";
    }

    /**
     * impl
     */
    inline virtual void endReport() override
    {
        *this << "<footer><h3>Summary</h3><p>Tests: " << abs_tests << " Failures: "
              << abs_fails << " Errors: " << abs_errs << " Time: " << abs_time
              << "ms</p></footer></body></html>";
    }

private:
    double abs_time = 0;
    std::uint32_t abs_tests = 0;
    std::uint32_t abs_fails = 0;
    std::uint32_t abs_errs = 0;
};

} // reporter
} // testsuite

#endif /* SRC_REPORTER_HTMLREPORTER_HPP_ */
