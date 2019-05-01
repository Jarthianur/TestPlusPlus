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

#ifndef SCTF_SRC_REPORTER_XMLREPORTER_HPP_
#define SCTF_SRC_REPORTER_XMLREPORTER_HPP_

#include <chrono>
#include <cstddef>
#include <ctime>
#include <iostream>
#include <string>

#include "common/types.h"
#include "reporter/AbstractReporter.hpp"
#include "testsuite/TestCase.hpp"
#include "testsuite/TestStats.hpp"
#include "testsuite/TestSuite.hpp"

namespace sctf
{
/**
 * @brief Concrete reporter featuring JUnit like XML format.
 */
class XmlReporter : public _::AbstractReporter
{
public:
    /**
     * @brief Constructor
     * @param stream The stream to write to
     */
    explicit XmlReporter(std::ostream& stream) : AbstractReporter(stream) {}

    /**
     * @brief Constructor
     * @param fname The file to write to
     */
    explicit XmlReporter(const char* fname) : AbstractReporter(fname) {}

    ~XmlReporter() noexcept override = default;

private:
    /**
     * @brief Implement AbstractReporter#report_ts
     */
    void report_ts(const TestSuite_shared ts) override
    {
        std::time_t stamp = std::chrono::system_clock::to_time_t(ts->timestamp());
        char        buff[128];
        std::strftime(buff, 127, "%FT%T", std::localtime(&stamp));

        *this << SCTF_SPACE << "<testsuite id=\"" << m_id++ << "\" name=\"" << ts->name()
              << "\" errors=\"" << ts->statistics().errors() << "\" tests=\""
              << ts->statistics().tests() << "\" failures=\"" << ts->statistics().failures()
              << "\" skipped=\"0\" time=\"" << ts->time() << "\" timestamp=\"" << buff << "\">"
              << SCTF_LF;

        AbstractReporter::report_ts(ts);

        *this << SCTF_SPACE << "</testsuite>" << SCTF_LF;
    }

    /**
     * @brief Implement AbstractReporter#report_tc
     */
    void report_tc(const _::TestCase& tc) override
    {
        *this << SCTF_XSPACE << "<testcase name=\"" << tc.name() << "\" classname=\""
              << tc.context() << "\" time=\"" << tc.duration() << "\"";
        switch (tc.state())
        {
            case _::TestCase::State::ERROR:
                *this << ">" << SCTF_LF << SCTF_XSPACE << SCTF_SPACE << "<error message=\""
                      << tc.err_msg() << "\"></error>" << SCTF_LF << SCTF_XSPACE << "</testcase>";
                break;
            case _::TestCase::State::FAILED:
                *this << ">" << SCTF_LF << SCTF_XSPACE << SCTF_SPACE << "<failure message=\""
                      << tc.err_msg() << "\"></failure>" << SCTF_LF << SCTF_XSPACE << "</testcase>";
                break;
            case _::TestCase::State::PASSED: *this << "/>"; break;
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
    mutable std::size_t m_id = 0;
};

/**
 * @brief Create a XmlReporter
 * @param stream The stream to use, defaults to stdout
 * @return a shared pointer to the reporter
 */
static AbstractReporter_shared createXmlReporter(std::ostream& stream = std::cout)
{
    return std::make_shared<XmlReporter>(stream);
}

/**
 * @brief Create a XmlReporter
 * @param file The filename to use
 * @return a shared pointer to the reporter
 */
static AbstractReporter_shared createXmlReporter(const char* file)
{
    return std::make_shared<XmlReporter>(file);
}

}  // namespace sctf

#endif  // SCTF_SRC_REPORTER_XMLREPORTER_HPP_
