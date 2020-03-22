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

#include "reporter/reporter.hpp"

namespace sctf
{
/**
 * Reporter implementation with JUnit like XML format.
 */
class xml_reporter : public intern::reporter
{
public:
    ~xml_reporter() noexcept override = default;

    /**
     * Create a xml reporter.
     * @param stream_  The stream to report to (default: stdout)
     */
    static reporter_ptr create(std::ostream& stream_ = std::cout, bool capture_ = false) {
        return std::make_shared<xml_reporter>(stream_, capture_);
    }

    /**
     * Create a xml reporter. The specified file will be overwritten if it already exists.
     * @param fname_   The name of the file where the report will be written
     */
    static reporter_ptr create(char const* fname_, bool capture_ = false) {
        return std::make_shared<xml_reporter>(fname_, capture_);
    }

protected:
    /**
     * @param stream_  The stream to report to
     */
    xml_reporter(std::ostream& stream_, bool capture_) : reporter(stream_), m_capture(capture_) {}

    /**
     * @param fname_   The name of the file where the report will be written
     */
    xml_reporter(char const* fname_, bool capture_) : reporter(fname_), m_capture(capture_) {}

    void report_testsuite(intern::testsuite_ptr const ts_) override {
        std::time_t stamp = std::chrono::system_clock::to_time_t(ts_->timestamp());
        char        buff[128];
        std::strftime(buff, 127, "%FT%T", std::localtime(&stamp));
        *this << SCTF_SPACE << "<testsuite id=\"" << m_id++ << "\" name=\"" << ts_->name()
              << "\" errors=\"" << ts_->statistics().errors() << "\" tests=\""
              << ts_->statistics().tests() << "\" failures=\"" << ts_->statistics().failures()
              << "\" skipped=\"0\" time=\"" << ts_->execution_time() << "\" timestamp=\"" << buff
              << "\">" << SCTF_LF;
        reporter::report_testsuite(ts_);
        *this << SCTF_SPACE << "</testsuite>" << SCTF_LF;
    }

    void report_testcase(intern::testcase const& tc_) override {
        *this << SCTF_XSPACE << "<testcase name=\"" << tc_.name() << "\" classname=\""
              << tc_.context() << "\" time=\"" << tc_.duration() << "\"";
        switch (tc_.state()) {
            case intern::testcase::result::ERROR:
                *this << ">" << SCTF_LF << SCTF_XSPACE << SCTF_SPACE << "<error message=\""
                      << tc_.reason() << "\"></error>" << SCTF_LF;
                if (m_capture) {
                    print_system_out(tc_);
                }
                *this << SCTF_XSPACE << "</testcase>";
                break;
            case intern::testcase::result::FAILED:
                *this << ">" << SCTF_LF << SCTF_XSPACE << SCTF_SPACE << "<failure message=\""
                      << tc_.reason() << "\"></failure>" << SCTF_LF;
                if (m_capture) {
                    print_system_out(tc_);
                }
                *this << SCTF_XSPACE << "</testcase>";
                break;
            default:
                if (m_capture) {
                    *this << ">" << SCTF_LF;
                    print_system_out(tc_);
                    *this << SCTF_XSPACE << "</testcase>";
                } else {
                    *this << "/>";
                }
                break;
        }
        *this << SCTF_LF;
    }

    void begin_report() override {
        *this << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" << SCTF_LF << "<testsuites>"
              << SCTF_LF;
    }

    void end_report() override {
        *this << "</testsuites>" << SCTF_LF;
    }

    void print_system_out(intern::testcase const& tc_) {
        *this << SCTF_XSPACE << SCTF_SPACE << "<system-out>" << tc_.cout() << "</system-out>"
              << SCTF_LF;
        *this << SCTF_XSPACE << SCTF_SPACE << "<system-err>" << tc_.cerr() << "</system-err>"
              << SCTF_LF;
    }

    std::size_t mutable m_id = 0;
    bool m_capture;
};
}  // namespace sctf

#endif  // SCTF_REPORTER_XML_REPORTER_HPP
