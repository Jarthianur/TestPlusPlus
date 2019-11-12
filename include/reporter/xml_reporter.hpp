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

#ifndef SCTF_REPORTER_XML_REPORTER_HPP
#define SCTF_REPORTER_XML_REPORTER_HPP

#include <chrono>
#include <ctime>

#include "common/types.hpp"
#include "reporter/reporter.hpp"
#include "testsuite/statistics.hpp"
#include "testsuite/testcase.hpp"
#include "testsuite/testsuite.hpp"

namespace sctf
{
/**
 * @brief Concrete reporter featuring JUnit like XML format.
 */
class xml_reporter : public _::reporter
{
public:
    ~xml_reporter() noexcept override = default;

    /**
     * @brief Constructor
     * @param stream The stream to write to
     */
    explicit xml_reporter(std::ostream& stream_) : reporter(stream_) {}

    /**
     * @brief Constructor
     * @param fname The file to write to
     */
    explicit xml_reporter(char const* fname_) : reporter(fname_) {}

protected:
    /**
     * @brief Implement AbstractReporter#report_ts
     */
    void report_ts(testsuite_shared const ts_) override
    {
        std::time_t stamp = std::chrono::system_clock::to_time_t(ts_->timestamp());
        char        buff[128];
        std::strftime(buff, 127, "%FT%T", std::localtime(&stamp));

        *this << SCTF_SPACE << "<testsuite id=\"" << m_id++ << "\" name=\"" << ts_->name()
              << "\" errors=\"" << ts_->statistics().errors() << "\" tests=\""
              << ts_->statistics().tests() << "\" failures=\"" << ts_->statistics().failures()
              << "\" skipped=\"0\" time=\"" << ts_->time() << "\" timestamp=\"" << buff << "\">"
              << SCTF_LF;

        reporter::report_ts(ts_);

        *this << SCTF_SPACE << "</testsuite>" << SCTF_LF;
    }

    /**
     * @brief Implement AbstractReporter#report_tc
     */
    void report_tc(_::testcase const& tc_) override
    {
        *this << SCTF_XSPACE << "<testcase name=\"" << tc_.name() << "\" classname=\""
              << tc_.context() << "\" time=\"" << tc_.duration() << "\"";
        switch (tc_.state())
        {
            case _::testcase::result::ERROR:
                *this << ">" << SCTF_LF << SCTF_XSPACE << SCTF_SPACE << "<error message=\""
                      << tc_.err_msg() << "\"></error>" << SCTF_LF << SCTF_XSPACE << "</testcase>";
                break;
            case _::testcase::result::FAILED:
                *this << ">" << SCTF_LF << SCTF_XSPACE << SCTF_SPACE << "<failure message=\""
                      << tc_.err_msg() << "\"></failure>" << SCTF_LF << SCTF_XSPACE
                      << "</testcase>";
                break;
            case _::testcase::result::PASSED: *this << "/>"; break;
            default: break;
        }
        *this << SCTF_LF;
    }

    /**
     * @brief Implement AbstractReporter#begin_report
     */
    void begin_report() override
    {
        *this << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" << SCTF_LF << "<testsuites>"
              << SCTF_LF;
    }

    /**
     * @brief Implement AbstractReporter#end_report
     */
    void end_report() override
    {
        *this << "</testsuites>" << SCTF_LF;
    }

    /// @brief The incremental testsuite id
    std::size_t mutable m_id = 0;
};

/**
 * @brief Create a XmlReporter
 * @param stream The stream to use, defaults to stdout
 * @return a shared pointer to the reporter
 */
static reporter_shared create_xml_reporter(std::ostream& stream_ = std::cout)
{
    return std::make_shared<xml_reporter>(stream_);
}

/**
 * @brief Create a XmlReporter
 * @param file The filename to use
 * @return a shared pointer to the reporter
 */
static reporter_shared create_xml_reporter(char const* file_)
{
    return std::make_shared<xml_reporter>(file_);
}
}  // namespace sctf

#endif  // SCTF_REPORTER_XML_REPORTER_HPP
