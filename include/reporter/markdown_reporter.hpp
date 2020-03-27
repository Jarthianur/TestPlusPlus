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

namespace sctf
{
/**
 * Reporter implementation with markdown format.
 */
class markdown_reporter : public intern::reporter
{
public:
    ~markdown_reporter() noexcept override = default;

    /**
     * Create a markdown reporter.
     *
     * @param stream_ is the stream where to print the report. (default: stdout)
     * @param capture_ is the flag to enable reporting of captured output. (default: false)
     */
    static reporter_ptr create(std::ostream& stream_ = std::cout, bool capture_ = false) {
        return std::make_shared<markdown_reporter>(stream_, capture_);
    }

    /**
     * Create a markdown reporter.
     *
     * @param fname_ is the filename where to print the report.
     * @param capture_ is the flag to enable reporting of captured output. (default: false)
     */
    static reporter_ptr create(char const* fname_, bool capture_ = false) {
        return std::make_shared<markdown_reporter>(fname_, capture_);
    }

private:
    markdown_reporter(std::ostream& stream_, bool capture_)
        : reporter(stream_), m_capture(capture_) {}

    markdown_reporter(char const* fname_, bool capture_) : reporter(fname_), m_capture(capture_) {}
    void report_testsuite(intern::testsuite_ptr const ts_) override {
        *this << "## " << ts_->name() << XLF << "|Tests|Successes|Failures|Errors|Time|" << LF
              << "|-|-|-|-|-|" << LF << "|" << ts_->statistics().tests() << "|"
              << ts_->statistics().successes() << "|" << ts_->statistics().failures() << "|"
              << ts_->statistics().errors() << "|" << ts_->execution_duration() << "ms|" << XLF
              << "### Tests" << XLF << "|Name|Context|Time|Status|"
              << (m_capture ? "System-Out|System-Err|" : "") << LF << "|-|-|-|-|"
              << (m_capture ? "-|-|" : "") << LF;
        reporter::report_testsuite(ts_);
        *this << XLF;
    }

    void report_testcase(intern::testcase const& tc_) override {
        char const* status = "";
        switch (tc_.state()) {
            case intern::testcase::result::ERROR: status = "ERROR"; break;
            case intern::testcase::result::FAILED: status = "FAILED"; break;
            case intern::testcase::result::PASSED: status = "PASSED"; break;
            default: break;
        }
        *this << "|" << tc_.name() << "|" << tc_.context() << "|" << tc_.duration() << "ms|"
              << status << "|";
        if (m_capture) {
            print_system_out(tc_.cout());
            print_system_out(tc_.cerr());
        }
        *this << LF;
    }

    void begin_report() override {
        *this << "# Test Report" << XLF;
    }

    void end_report() override {
        *this << "## Summary" << XLF << "|Tests|Successes|Failures|Errors|Time|" << LF
              << "|-|-|-|-|-|" << LF << "|" << m_abs_tests << "|"
              << (m_abs_tests - m_abs_errs - m_abs_fails) << "|" << m_abs_fails << "|" << m_abs_errs
              << "|" << m_abs_time << "ms|" << LF;
    }

    /**
     * Print the captured output of a testcase to the report.
     *
     * @param out_ is the captured output.
     */
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

    bool m_capture;  ///< Flags whether to report captured output from testcases.
};
}  // namespace sctf

#endif  // SCTF_REPORTER_MARKDOWN_REPORTER_HPP
