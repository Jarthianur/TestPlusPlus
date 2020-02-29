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

#ifndef SCTF_REPORTER_PLAINTEXT_REPORTER_HPP
#define SCTF_REPORTER_PLAINTEXT_REPORTER_HPP

#include "common/types.hpp"
#include "reporter/reporter.hpp"
#include "testsuite/statistics.hpp"
#include "testsuite/testcase.hpp"
#include "testsuite/testsuite.hpp"

#define ANSI_RED "\x1b[31m"
#define ANSI_GREEN "\x1b[32m"
#define ANSI_YELLOW "\x1b[33m"
#define ANSI_BLUE "\x1b[34m"
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_CYAN "\x1b[36m"
#define ANSI_RESET "\x1b[0m"

namespace sctf
{
/**
 * Reporter implementation with optionally colored text output.
 * Used for console printing.
 */
class console_reporter : public _::reporter
{
public:
    ~console_reporter() noexcept override = default;

    /**
     * @param stream_  The stream to report to
     * @param color_   Whether to print ANSI colored text
     * @param capture_ Whether to report captured stdout/stderr
     */
    explicit console_reporter(std::ostream& stream_, bool color_ = false, bool capture_ = false)
        : reporter(stream_), m_color(color_), m_capture(capture_)
    {}

    /**
     * @param fname_   The name of the file where the report will be written
     * @param color_   Whether to print ANSI colored text
     * @param capture_ Whether to report captured stdout/stderr
     */
    explicit console_reporter(char const* fname_, bool color_ = false, bool capture_ = false)
        : reporter(fname_), m_color(color_), m_capture(capture_)
    {}

protected:
    void report_testsuite(testsuite_ptr const ts_) override
    {
        *this << "Run Testsuite [" << ts_->name() << "]; time = " << ts_->execution_time() << "ms" << SCTF_LF;

        reporter::report_testsuite(ts_);
    }

    void report_testcase(_::testcase const& tc_) override
    {
        *this << SCTF_SPACE << "Run Testcase [" << tc_.name() << "](" << tc_.context()
              << "); time = " << tc_.duration() << "ms" << SCTF_LF << SCTF_XSPACE;
        if (m_capture)
        {
            *this << "stdout = '" << tc_.cout() << '\'' << SCTF_LF << SCTF_XSPACE;
            *this << "stderr = '" << tc_.cerr() << '\'' << SCTF_LF << SCTF_XSPACE;
        }
        switch (tc_.state())
        {
            case _::testcase::result::ERROR:
                *this << (m_color ? ANSI_MAGENTA : "") << "ERROR! " << tc_.err_msg();
                break;
            case _::testcase::result::FAILED:
                *this << (m_color ? ANSI_RED : "") << "FAILED! " << tc_.err_msg();
                break;
            case _::testcase::result::PASSED:
                *this << (m_color ? ANSI_GREEN : "") << "PASSED!";
                break;
            default: break;
        }
        *this << (m_color ? ANSI_RESET : "") << SCTF_LF;
    }

    void begin_report() override {}

    void end_report() override
    {
        if (m_abs_fails >= (m_abs_tests + 1) / 2)
        {
            *this << (m_color ? ANSI_YELLOW : "");
        }
        else
        {
            *this << (m_color ? ANSI_CYAN : "");
        }
        *this << "Result:: passed: " << m_abs_tests - m_abs_fails - m_abs_errs << "/" << m_abs_tests
              << " ; failed: " << m_abs_fails << "/" << m_abs_tests << " ; errors: " << m_abs_errs
              << "/" << m_abs_tests << " ; time = " << m_abs_time << "ms"
              << (m_color ? ANSI_RESET : "") << SCTF_LF;
    }

    bool m_color;
    bool m_capture;
};

/**
 * Create a console reporter.
 * @param stream_  The stream to report to
 * @param color_   Whether to print ANSI colored text (default: false)
 * @param capture_ Whether to report captured stdout/stderr (default: false)
 */
static reporter_ptr create_console_reporter(std::ostream& stream_, bool color_ = false,
                                            bool capture_ = false)
{
    return std::make_shared<console_reporter>(stream_, color_, capture_);
}

/**
 * Create a console reporter. The report is printed to stdout.
 * @param color_   Whether to print ANSI colored text (default: false)
 * @param capture_ Whether to report captured stdout/stderr (default: false)
 */
static reporter_ptr create_console_reporter(bool color_ = false, bool capture_ = false)
{
    return std::make_shared<console_reporter>(std::cout, color_, capture_);
}

/**
 * Create a console reporter. The specified file will be overwritten if it already exists.
 * @param fname_   The name of the file where the report will be written
 * @param color_   Whether to print ANSI colored text (default: false)
 * @param capture_ Whether to report captured stdout/stderr (default: false)
 */
static reporter_ptr create_console_reporter(char const* fname_, bool color_ = false,
                                            bool capture_ = false)
{
    return std::make_shared<console_reporter>(fname_, color_, capture_);
}
}  // namespace sctf

#undef ANSI_RED
#undef ANSI_BLUE
#undef ANSI_CYAN
#undef ANSI_GREEN
#undef ANSI_RESET
#undef ANSI_YELLOW
#undef ANSI_MAGENTA

#endif  // SCTF_REPORTER_PLAINTEXT_REPORTER_HPP
