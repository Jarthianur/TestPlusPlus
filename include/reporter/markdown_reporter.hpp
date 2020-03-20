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

#ifndef SCTF_REPORTER_MARKDOWN_REPORTER_HPP
#define SCTF_REPORTER_MARKDOWN_REPORTER_HPP

#include <sstream>

#include "reporter/reporter.hpp"
#include "testsuite/statistic.hpp"
#include "testsuite/testsuite.hpp"

#include "testcase.hpp"

#define SCTF_XLF (SCTF_LF SCTF_LF)

namespace sctf
{
/**
 * Reporter implementation with markdown format.
 */
class markdown_reporter : public private_::reporter
{
public:
    ~markdown_reporter() noexcept override = default;

    /**
     * @param stream_  The stream to report to
     */
    markdown_reporter(std::ostream& stream_, bool capture_)
        : reporter(stream_), m_capture(capture_) {}

    /**
     * @param fname_   The name of the file where the report will be written
     */
    markdown_reporter(char const* fname_, bool capture_) : reporter(fname_), m_capture(capture_) {}

protected:
    void report_testsuite(private_::testsuite_ptr const ts_) override {
        *this << "## " << ts_->name() << SCTF_XLF << "|Tests|Successes|Failures|Errors|Time|"
              << SCTF_LF << "|-|-|-|-|-|" << SCTF_LF << "|" << ts_->statistics().tests() << "|"
              << ts_->statistics().successes() << "|" << ts_->statistics().failures() << "|"
              << ts_->statistics().errors() << "|" << ts_->execution_time() << "ms|" << SCTF_XLF
              << "### Tests" << SCTF_XLF << "|Name|Context|Time|Status|"
              << (m_capture ? "System-Out|System-Err|" : "") << SCTF_LF << "|-|-|-|-|"
              << (m_capture ? "-|-|" : "") << SCTF_LF;
        reporter::report_testsuite(ts_);
        *this << SCTF_XLF;
    }

    void report_testcase(private_::testcase const& tc_) override {
        char const* status = "";
        switch (tc_.state()) {
            case private_::testcase::result::ERROR: status = "ERROR"; break;
            case private_::testcase::result::FAILED: status = "FAILED"; break;
            case private_::testcase::result::PASSED: status = "PASSED"; break;
            default: break;
        }
        *this << "|" << tc_.name() << "|" << tc_.context() << "|" << tc_.duration() << "ms|"
              << status << "|";
        if (m_capture) {
            print_system_out(tc_.cout());
            print_system_out(tc_.cerr());
        }
        *this << SCTF_LF;
    }

    void begin_report() override {
        *this << "# Test Report" << SCTF_XLF;
    }

    void end_report() override {
        *this << "## Summary" << SCTF_XLF << "|Tests|Successes|Failures|Errors|Time|" << SCTF_LF
              << "|-|-|-|-|-|" << SCTF_LF << "|" << m_abs_tests << "|"
              << (m_abs_tests - m_abs_errs - m_abs_fails) << "|" << m_abs_fails << "|" << m_abs_errs
              << "|" << m_abs_time << "ms|" << SCTF_LF;
    }

    void print_system_out(std::string const& out_) {
        std::string        line;
        std::istringstream io_;
        io_.str(out_);
        bool first = true;
        while (std::getline(io_, line)) {
            *this << (first ? "" : "<br>") << "`" << line << "`";
            first = false;
        }
        *this << "|";
    }

    bool m_capture;
};

/**
 * Create a markdown reporter.
 * @param stream_  The stream to report to (default: stdout)
 */
static reporter_ptr create_markdown_reporter(std::ostream& stream_  = std::cout,
                                             bool          capture_ = false) {
    return std::make_shared<markdown_reporter>(stream_, capture_);
}

/**
 * Create a markdown reporter. The specified file will be overwritten if it already exists.
 * @param fname_   The name of the file where the report will be written
 */
static reporter_ptr create_markdown_reporter(char const* file_, bool capture_ = false) {
    return std::make_shared<markdown_reporter>(file_, capture_);
}
}  // namespace sctf

#undef SCTF_XLF

#endif  // SCTF_REPORTER_MARKDOWN_REPORTER_HPP
