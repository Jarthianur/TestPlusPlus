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

#ifndef SCTF_REPORTER_PLAINTEXT_REPORTER_HPP_
#define SCTF_REPORTER_PLAINTEXT_REPORTER_HPP_

#include "common/types.h"
#include "reporter/abstract_reporter.hpp"
#include "testsuite/statistics.hpp"
#include "testsuite/testcase.hpp"
#include "testsuite/testsuite.hpp"

/// @brief ANSI colors
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
 * @brief Concrete reporter featuring , optionally colored, plain text format.
 */
class plaintext_reporter : public _::abstract_reporter
{
public:
    ~plaintext_reporter() noexcept override = default;

    /**
     * @brief Constructor
     * @param stream The stream to write to
     * @param color Whether to print colored text
     */
    explicit plaintext_reporter(std::ostream& stream, bool color = false, bool out = false)
        : abstract_reporter(stream), m_color(color), m_out(out)
    {}

    /**
     * @brief Constructor
     * @param fname The file to write to
     * @param color Whether to print colored text
     */
    explicit plaintext_reporter(const char* fname, bool color = false, bool out = false)
        : abstract_reporter(fname), m_color(color), m_out(out)
    {}

protected:
    /**
     * @brief Implement AbstractReporter#report_ts
     */
    void report_ts(const testsuite_shared ts) override
    {
        *this << "Run Testsuite [" << ts->name() << "]; time = " << ts->time() << "ms" << SCTF_LF;

        abstract_reporter::report_ts(ts);
    }

    /**
     * @brief Implement AbstractReporter#report_tc
     */
    void report_tc(const _::testcase& tc) override
    {
        *this << SCTF_SPACE << "Run Testcase [" << tc.name() << "](" << tc.context()
              << "); time = " << tc.duration() << "ms" << SCTF_LF << SCTF_XSPACE;
        if (m_out)
        {
            *this << "stdout = '" << tc.cout() << '\'' << SCTF_LF << SCTF_XSPACE;
            *this << "stderr = '" << tc.cerr() << '\'' << SCTF_LF << SCTF_XSPACE;
        }
        switch (tc.state())
        {
            case _::testcase::result::ERROR:
                *this << (m_color ? ANSI_MAGENTA : "") << "ERROR! " << tc.err_msg();
                break;
            case _::testcase::result::FAILED:
                *this << (m_color ? ANSI_RED : "") << "FAILED! " << tc.err_msg();
                break;
            case _::testcase::result::PASSED:
                *this << (m_color ? ANSI_GREEN : "") << "PASSED!";
                break;
            default: break;
        }
        *this << (m_color ? ANSI_RESET : "") << SCTF_LF;
    }

    /**
     * @brief Implement AbstractReporter#begin_report
     */
    void begin_report() override {}

    /**
     * @brief Implement AbstractReporter#end_report
     */
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

    /// @brief Use colors.
    bool m_color;

    /// @brief Report captured output for testcases.
    bool m_out;
};

/**
 * @brief Create a PlainTextReporter
 * @param stream The stream to use
 * @param color Whether to print colored text
 * @return a shared pointer to the reporter
 */
static reporter_shared createPlainTextReporter(std::ostream& stream, bool color = false,
                                               bool out = false)
{
    return std::make_shared<plaintext_reporter>(stream, color, out);
}

/**
 * @brief Create a PlainTextReporter
 * @note Prints to stdout.
 * @param color Whether to print colored text
 * @return a shared pointer to the reporter
 */
static reporter_shared createPlainTextReporter(bool color = false, bool out = false)
{
    return std::make_shared<plaintext_reporter>(std::cout, color, out);
}

/**
 * @brief Create a PlainTextReporter
 * @param file The filename to use
 * @param color Whether to print colored text
 * @return a shared pointer to the reporter
 */
static reporter_shared createPlainTextReporter(const char* file, bool color = false,
                                               bool out = false)
{
    return std::make_shared<plaintext_reporter>(file, color, out);
}

}  // namespace sctf

#undef ANSI_RED
#undef ANSI_BLUE
#undef ANSI_CYAN
#undef ANSI_GREEN
#undef ANSI_RESET
#undef ANSI_YELLOW
#undef ANSI_MAGENTA

#endif  // SCTF_REPORTER_PLAINTEXT_REPORTER_HPP_
