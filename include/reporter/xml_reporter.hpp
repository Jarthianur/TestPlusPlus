/*
    Copyright (C) 2017 Jarthianur

    This file is part of simple-cpp-test-framework.

    simple-cpp-test-framework is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    simple-cpp-test-framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with simple-cpp-test-framework.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef SCTF_REPORTER_XML_REPORTER_HPP
#define SCTF_REPORTER_XML_REPORTER_HPP

#include <chrono>
#include <ctime>

#include "reporter/reporter.hpp"

namespace sctf
{
/**
 * Reporter implementation with JUnit like XML format.
 */
class xml_reporter : public intern::reporter
{
public:
    xml_reporter(xml_reporter const&)     = delete;
    xml_reporter(xml_reporter&&) noexcept = delete;
    xml_reporter& operator=(xml_reporter const&) = delete;
    xml_reporter& operator=(xml_reporter&&) noexcept = delete;
    ~xml_reporter() noexcept override                = default;

    /**
     * Create a xml reporter.
     *
     * @param stream_ is the stream where to print the report. (default: stdout)
     * @param capture_ is the flag to enable reporting of captured output. (default: false)
     */
    static reporter_ptr create(std::ostream& stream_ = std::cout, bool capture_ = false) {
        return reporter_ptr(new xml_reporter(stream_, capture_));
    }

    /**
     * Create a xml reporter.
     *
     * @param fname_ is the filename where to print the report.
     * @param capture_ is the flag to enable reporting of captured output. (default: false)
     */
    static reporter_ptr create(char const* fname_, bool capture_ = false) {
        return reporter_ptr(new xml_reporter(fname_, capture_));
    }

private:
    xml_reporter(std::ostream& stream_, bool capture_) : reporter(stream_), m_capture(capture_) {}
    xml_reporter(char const* fname_, bool capture_) : reporter(fname_), m_capture(capture_) {}

    void report_testsuite(intern::testsuite_ptr const ts_) override {
        std::time_t stamp = std::chrono::system_clock::to_time_t(ts_->timestamp());
        char        buff[128];
        std::strftime(buff, 127, "%FT%T", std::localtime(&stamp));
        *this << intern::fmt::SPACE << "<testsuite id=\"" << m_id++ << "\" name=\"" << ts_->name()
              << "\" errors=\"" << ts_->statistics().errors() << "\" tests=\""
              << ts_->statistics().tests() << "\" failures=\"" << ts_->statistics().failures()
              << "\" skipped=\"0\" time=\"" << ts_->execution_duration() << "\" timestamp=\""
              << buff << "\">" << intern::fmt::LF;
        reporter::report_testsuite(ts_);
        *this << intern::fmt::SPACE << "</testsuite>" << intern::fmt::LF;
    }

    void report_testcase(intern::testcase const& tc_) override {
        *this << intern::fmt::XSPACE << "<testcase name=\"" << tc_.name() << "\" classname=\""
              << tc_.context() << "\" time=\"" << tc_.duration() << "\"";
        switch (tc_.state()) {
            case intern::testcase::result::ERROR:
                *this << ">" << intern::fmt::LF << intern::fmt::XSPACE << intern::fmt::SPACE
                      << "<error message=\"" << tc_.reason() << "\"></error>" << intern::fmt::LF;
                if (m_capture) {
                    print_system_out(tc_);
                }
                *this << intern::fmt::XSPACE << "</testcase>";
                break;
            case intern::testcase::result::FAILED:
                *this << ">" << intern::fmt::LF << intern::fmt::XSPACE << intern::fmt::SPACE
                      << "<failure message=\"" << tc_.reason() << "\"></failure>"
                      << intern::fmt::LF;
                if (m_capture) {
                    print_system_out(tc_);
                }
                *this << intern::fmt::XSPACE << "</testcase>";
                break;
            default:
                if (m_capture) {
                    *this << ">" << intern::fmt::LF;
                    print_system_out(tc_);
                    *this << intern::fmt::XSPACE << "</testcase>";
                } else {
                    *this << "/>";
                }
                break;
        }
        *this << intern::fmt::LF;
    }

    void begin_report() override {
        *this << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" << intern::fmt::LF << "<testsuites>"
              << intern::fmt::LF;
    }

    void end_report() override {
        *this << "</testsuites>" << intern::fmt::LF;
    }

    /**
     * Print the captured output of a testcase to the report.
     *
     * @param tc_ is the testcase whose output to print.
     */
    void print_system_out(intern::testcase const& tc_) {
        *this << intern::fmt::XSPACE << intern::fmt::SPACE << "<system-out>" << tc_.cout()
              << "</system-out>" << intern::fmt::LF;
        *this << intern::fmt::XSPACE << intern::fmt::SPACE << "<system-err>" << tc_.cerr()
              << "</system-err>" << intern::fmt::LF;
    }

    std::size_t mutable m_id = 0;  ///< Report wide incremental ID for testsuites.
    bool m_capture;                ///< Flags whether to report captured output from testcases.
};
}  // namespace sctf

#endif  // SCTF_REPORTER_XML_REPORTER_HPP
