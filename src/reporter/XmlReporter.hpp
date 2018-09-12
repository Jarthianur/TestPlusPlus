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
 * @brief Concrete reporter featuring JUnit like XML format.
 */
class XmlReporter : public AbstractReporter
{
public:
    /**
     * @brief Constructor
     * @param stream The stream to write to
     */
    explicit XmlReporter(std::ostream& stream) : AbstractReporter(stream)
    {}

    /**
     * @brief Constructor
     * @param fname The file to write to
     */
    explicit XmlReporter(const char* fnam) : AbstractReporter(fnam)
    {}

    /**
     * @brief Destructor
     */
    ~XmlReporter() noexcept
    {}

private:
    /**
     * @brief Implement AbstractReporter#reportTestSuite
     */
    void report_ts(TestSuite_shared ts) override
    {
        std::time_t stamp = std::chrono::system_clock::to_time_t(ts->timestamp());
        char buff[128];
        std::strftime(buff, 127, "%FT%T", std::localtime(&stamp));

        *this << SPACE << "<testsuite id=\"" << m_id++ << "\" name=\"" << ts->name()
              << "\" errors=\"" << ts->statistics().errors() << "\" tests=\""
              << ts->statistics().tests() << "\" failures=\""
              << ts->statistics().failures() << "\" skipped=\"0\" time=\"" << ts->time()
              << "\" timestamp=\"" << buff << "\">" << LF;

        AbstractReporter::report_ts(ts);

        *this << SPACE << "</testsuite>" << LF;
    }

    /**
     * @brief Implement AbstractReporter#reportTestCase
     */
    void report_tc(const test::TestCase& tc) override
    {
        *this << XSPACE << "<testcase name=\"" << tc.name() << "\" classname=\""
              << tc.context() << "\" time=\"" << tc.duration() << "\"";
        switch(tc.state())
        {
            case test::TestCase::TestState::ERROR:
                *this << ">" << LF << XSPACE << SPACE << "<error message=\""
                      << tc.err_msg() << "\"></error>" << LF << XSPACE << "</testcase>";
                break;
            case test::TestCase::TestState::FAILED:
                *this << ">" << LF << XSPACE << SPACE << "<failure message=\""
                      << tc.err_msg() << "\"></failure>" << LF << XSPACE << "</testcase>";
                break;
            case test::TestCase::TestState::PASSED:
                *this << "/>";
                break;
            default:
                break;
        }
        *this << LF;
    }

    /**
     * @brief Implement AbstractReporter#beginReport
     */
    void begin_report() override
    {
        *this << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" << LF << "<testsuites>"
              << LF;
    }

    /**
     * @brief Implement AbstractReporter#endReport
     */
    void end_report() override
    {
        *this << "</testsuites>" << LF;
    }

    /// @brief The incremental testsuite id
    mutable std::size_t m_id = 0;
};

}  // namespace rep

/**
 * @brief Create a XmlReporter
 * @param stream The stream to use, defaults to stdout
 * @return a shared pointer to the reporter
 */
inline static rep::AbstractReporter_shared createXmlReporter(std::ostream& stream
                                                             = std::cout)
{
    return std::make_shared<rep::XmlReporter>(stream);
}

/**
 * @brief Create a XmlReporter
 * @param file The filename to use
 * @return a shared pointer to the reporter
 */
inline static rep::AbstractReporter_shared createXmlReporter(const char* file)
{
    return std::make_shared<rep::XmlReporter>(file);
}

}  // namespace sctf

#endif  // SCTF_SRC_REPORTER_XMLREPORTER_HPP_
