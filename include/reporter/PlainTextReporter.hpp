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

#ifndef SCTF_SRC_REPORTER_PLAINTEXTREPORTER_HPP_
#define SCTF_SRC_REPORTER_PLAINTEXTREPORTER_HPP_

#include <cstddef>
#include <iostream>
#include <string>

#include "common/types.h"
#include "reporter/AbstractReporter.hpp"
#include "testsuite/TestCase.hpp"
#include "testsuite/TestStats.hpp"
#include "testsuite/TestSuite.hpp"

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
class PlainTextReporter : public _::AbstractReporter
{
public:
    /**
     * @brief Constructor
     * @param stream The stream to write to
     * @param color Whether to print colored text
     */
    explicit PlainTextReporter(std::ostream& stream, bool color = false, bool out = false)
        : AbstractReporter(stream), m_color(color), m_out(out)
    {}

    /**
     * @brief Constructor
     * @param fname The file to write to
     * @param color Whether to print colored text
     */
    explicit PlainTextReporter(const char* fname, bool color = false, bool out = false)
        : AbstractReporter(fname), m_color(color), m_out(out)
    {}

    ~PlainTextReporter() noexcept override = default;

private:
    /**
     * @brief Implement AbstractReporter#report_ts
     */
    void report_ts(const TestSuite_shared ts) override
    {
        *this << "Run Testsuite [" << ts->name() << "]; time = " << ts->time() << "ms" << LF;

        AbstractReporter::report_ts(ts);
    }

    /**
     * @brief Implement AbstractReporter#report_tc
     */
    void report_tc(const _::TestCase& tc) override
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
            case _::TestCase::State::ERROR:
                *this << (m_color ? ANSI_MAGENTA : "") << "ERROR! " << tc.err_msg();
                break;
            case _::TestCase::State::FAILED:
                *this << (m_color ? ANSI_RED : "") << "FAILED! " << tc.err_msg();
                break;
            case _::TestCase::State::PASSED:
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
        if (abs_fails() >= (abs_tests() + 1) / 2)
        {
            *this << (m_color ? ANSI_YELLOW : "");
        }
        else
        {
            *this << (m_color ? ANSI_CYAN : "");
        }
        *this << "Result:: passed: " << abs_tests() - abs_fails() - abs_errs() << "/" << abs_tests()
              << " ; failed: " << abs_fails() << "/" << abs_tests() << " ; errors: " << abs_errs()
              << "/" << abs_tests() << " ; time = " << abs_time() << "ms"
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
static AbstractReporter_shared createPlainTextReporter(std::ostream& stream, bool color = false,
                                                       bool out = false)
{
    return std::make_shared<PlainTextReporter>(stream, color, out);
}

/**
 * @brief Create a PlainTextReporter
 * @note Prints to stdout.
 * @param color Whether to print colored text
 * @return a shared pointer to the reporter
 */
static AbstractReporter_shared createPlainTextReporter(bool color = false, bool out = false)
{
    return std::make_shared<PlainTextReporter>(std::cout, color, out);
}

/**
 * @brief Create a PlainTextReporter
 * @param file The filename to use
 * @param color Whether to print colored text
 * @return a shared pointer to the reporter
 */
static AbstractReporter_shared createPlainTextReporter(const char* file, bool color = false,
                                                       bool out = false)
{
    return std::make_shared<PlainTextReporter>(file, color, out);
}

}  // namespace sctf

#undef ANSI_RED
#undef ANSI_BLUE
#undef ANSI_CYAN
#undef ANSI_GREEN
#undef ANSI_RESET
#undef ANSI_YELLOW
#undef ANSI_MAGENTA

#endif  // SCTF_SRC_REPORTER_PLAINTEXTREPORTER_HPP_
