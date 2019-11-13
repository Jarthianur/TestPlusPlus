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

#ifndef SCTF_REPORTER_HTML_REPORTER_HPP
#define SCTF_REPORTER_HTML_REPORTER_HPP

#include "common/types.hpp"
#include "reporter/reporter.hpp"
#include "testsuite/statistics.hpp"
#include "testsuite/testcase.hpp"
#include "testsuite/testsuite.hpp"

#define TD "<td>"
#define TD_ "</td>"
#define TR "<tr>"
#define TR_ "</tr>"
#define TH "<th>"
#define TH_ "</th>"

namespace sctf
{
/**
 * @brief Concrete reporter featuring HTML format.
 */
class html_reporter : public _::reporter
{
public:
    ~html_reporter() noexcept override = default;

    /**
     * @brief Constructor
     * @param stream The stream to write to
     */
    explicit html_reporter(std::ostream& stream_) : reporter(stream_) {}

    /**
     * @brief Constructor
     * @param fname The file to write to
     */
    explicit html_reporter(char const* fname_) : reporter(fname_) {}

protected:
    /**
     * @brief Implement AbstractReporter#report_ts
     */
    void report_testsuite(testsuite_ptr const ts_) override
    {
        *this << "<h3>" << ts_->name() << "</h3>"
              << "<p>Tests: " << ts_->statistics().tests()
              << " Failures: " << ts_->statistics().failures()
              << " Errors: " << ts_->statistics().errors() << " Time: " << ts_->time()
              << "ms</p><table><thead>" << TR << TH << "Name" << TH_ << TH << "Context" << TH_ << TH
              << "Time" << TH_ << TH << "Status" << TH_ << TR_ << "</thead><tbody>";
        reporter::report_testsuite(ts_);
        *this << "</tbody></table>";
    }

    /**
     * @brief Implement AbstractReporter#report_tc
     */
    void report_testcase(_::testcase const& tc_) override
    {
        std::string status;
        switch (tc_.state())
        {
            case _::testcase::result::ERROR: status = "error"; break;
            case _::testcase::result::FAILED: status = "failed"; break;
            case _::testcase::result::PASSED: status = "passed"; break;
            default: break;
        }
        *this << "<tr class=\"" << status << "\">" << TD << tc_.name() << TD_ << TD << tc_.context()
              << TD_ << TD << tc_.duration() << "ms" << TD_ << TD << status << TD_ << TR_;
    }

    /**
     * @brief Implement AbstractReporter#begin_report
     */
    void begin_report() override
    {
        *this << "<!DOCTYPE html><html><head><meta charset=\"utf-8\"/>"
                 "<style>table{border-collapse: collapse;min-width: 50%}"
                 "tr,th,td{border: 1px solid black;padding: 2px}.failed{background: lightskyblue}"
                 ".passed{background: lightgreen}.error{background: lightcoral}</style>"
                 "</head><body><header><h1>Test Report</h1></header>";
    }

    /**
     * @brief Implement AbstractReporter#end_report
     */
    void end_report() override
    {
        *this << "<footer><h3>Summary</h3><p>Tests: " << m_abs_tests << " Failures: " << m_abs_fails
              << " Errors: " << m_abs_errs << " Time: " << m_abs_time
              << "ms</p></footer></body></html>";
    }
};

/**
 * @brief Create a HtmlReporter
 * @param stream The stream to use, defaults to stdout
 * @return a shared pointer to the reporter
 */
static reporter_ptr create_html_reporter(std::ostream& stream_ = std::cout)
{
    return std::make_shared<html_reporter>(stream_);
}

/**
 * @brief Create a HtmlReporter
 * @param file The filename to use
 * @return a shared pointer to the reporter
 */
static reporter_ptr create_html_reporter(char const* file_)
{
    return std::make_shared<html_reporter>(file_);
}
}  // namespace sctf

#undef TD
#undef TD_
#undef TR
#undef TR_
#undef TH
#undef TH_

#endif  // SCTF_REPORTER_HTML_REPORTER_HPP
