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

#ifndef SCTF_SRC_REPORTER_HTMLREPORTER_HPP_
#define SCTF_SRC_REPORTER_HTMLREPORTER_HPP_

#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>

#include "../testsuite/TestCase.hpp"
#include "../testsuite/TestStats.hpp"
#include "../testsuite/TestSuite.hpp"
#include "../types.h"
#include "AbstractReporter.hpp"

namespace sctf
{
namespace rep
{
/**
 * @def TD
 * @brief HTML table column start tag
 */
#define TD "<td>"

/**
 * @def TD_
 * @brief HTML table column end tag
 */
#define TD_ "</td>"

/**
 * @def TR
 * @brief HTML table row start tag
 */
#define TR "<tr>"

/**
 * @def TR_
 * @brief HTML table row end tag
 */
#define TR_ "</tr>"

/**
 * @def TH
 * @brief HTML table head start tag
 */
#define TH "<th>"

/**
 * @def TH_
 * @brief HTML table head end tag
 */
#define TH_ "</th>"

/**
 * @brief Concrete reporter featuring HTML format.
 */
class HtmlReporter : public AbstractReporter
{
public:
    /**
     * @brief Constructor
     * @param stream The stream to write to
     */
    HtmlReporter(std::ostream& stream) : AbstractReporter(stream)
    {}

    /**
     * @brief Constructor
     * @param fname The file to write to
     */
    HtmlReporter(const char* fname) : AbstractReporter(fname)
    {}

    /**
     * @brief Destructor
     */
    ~HtmlReporter() noexcept
    {}

private:
    /**
     * @brief Implement AbstractReporter#reportTestSuite
     */
    void reportTestSuite(TestSuite_shared ts) override
    {
        m_abs_tests += ts->getTestStats().tests();
        m_abs_fails += ts->getTestStats().failures();
        m_abs_errs += ts->getTestStats().errors();
        m_abs_time += ts->getTime();

        *this << "<h3>" << ts->name << "</h3>"
              << "<p>Tests: " << ts->getTestStats().tests()
              << " Failures: " << ts->getTestStats().failures()
              << " Errors: " << ts->getTestStats().errors() << " Time: " << ts->getTime()
              << "ms</p><table><thead>" << TR << TH << "Name" << TH_ << TH << "Context"
              << TH_ << TH << "Time" << TH_ << TH << "Status" << TH_ << TR_
              << "</thead><tbody>";

        AbstractReporter::reportTestSuite(ts);

        *this << "</tbody></table>";
    }

    /**
     * @brief Implement AbstractReporter#reportTestCase
     */
    void reportTestCase(const test::TestCase& tc) override
    {
        std::string status;
        switch(tc.getState())
        {
            case test::TestCase::TestState::ERROR:
                status = "error";
                break;
            case test::TestCase::TestState::FAILED:
                status = "failed";
                break;
            case test::TestCase::TestState::PASSED:
                status = "passed";
                break;
            default:
                break;
        }
        *this << "<tr class=\"" << status << "\">" << TD << tc.name << TD_ << TD
              << tc.context << TD_ << TD << tc.getDuration() << "ms" << TD_ << TD
              << status << TD_ << TR_;
    }

    /**
     * @brief Implement AbstractReporter#beginReport
     */
    void beginReport() override
    {
        *this
            << "<!DOCTYPE html><html><head><meta charset=\"utf-8\"/>"
               "<style>table{border-collapse: collapse;min-width: 50%}"
               "tr,th,td{border: 1px solid black;padding: 2px}.failed{background: lightskyblue}"
               ".passed{background: lightgreen}.error{background: lightcoral}</style>"
               "</head><body><header><h1>Test Report</h1></header>";
    }

    /**
     * @brief Implement AbstractReporter#endReport
     */
    void endReport() override
    {
        *this << "<footer><h3>Summary</h3><p>Tests: " << m_abs_tests
              << " Failures: " << m_abs_fails << " Errors: " << m_abs_errs
              << " Time: " << m_abs_time << "ms</p></footer></body></html>";
    }

    /// @brief The amount of tests.
    std::size_t m_abs_tests = 0;

    /// @brief The amount of failed tests.
    std::size_t m_abs_fails = 0;

    /// @brief The amount of erroneous tests.
    std::size_t m_abs_errs = 0;

    /// @brief The accumulated runtime.
    double m_abs_time = 0;
};

}  // namespace rep

/**
 * @brief Create a HtmlReporter
 * @param stream The stream to use, defaults to stdout
 * @return a shared pointer to the reporter
 */
inline static rep::AbstractReporter_shared createHtmlReporter(std::ostream& stream
                                                              = std::cout)
{
    return std::make_shared<rep::HtmlReporter>(stream);
}

/**
 * @brief Create a HtmlReporter
 * @param file The filename to use
 * @return a shared pointer to the reporter
 */
inline static rep::AbstractReporter_shared createHtmlReporter(const char* file)
{
    return std::make_shared<rep::HtmlReporter>(file);
}

}  // namespace sctf

#endif  // SCTF_SRC_REPORTER_HTMLREPORTER_HPP_
