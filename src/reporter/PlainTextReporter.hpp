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

#ifndef SRC_REPORTER_COLOREDREPORTER_HPP_
#define SRC_REPORTER_COLOREDREPORTER_HPP_

#include <cstddef>
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
// ANSI colors
#define ANSI_RED "\x1b[31m"
#define ANSI_GREEN "\x1b[32m"
#define ANSI_YELLOW "\x1b[33m"
#define ANSI_BLUE "\x1b[34m"
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_CYAN "\x1b[36m"
#define ANSI_RESET "\x1b[0m"

/**
 * @brief Concrete reporter featuring , optionally colored, plain text format.
 */
class PlainTextReporter : public AbstractReporter
{
public:
    /**
     * @brief Constructor
     * @param stream The stream to write to
     * @param color Whether to print colored text
     */
    explicit PlainTextReporter(std::ostream& stream, bool color = false)
        : AbstractReporter(stream), m_color(color)
    {}

    /**
     * @brief Constructor
     * @param fname The file to write to
     * @param color Whether to print colored text
     */
    explicit PlainTextReporter(const char* fname, bool color = false)
        : AbstractReporter(fname), m_color(color)
    {}

    /**
     * @brief Destructor
     */
    virtual ~PlainTextReporter() noexcept
    {}

private:
    /**
     * @brief Implement AbstractReporter#reportTestSuite
     */
    virtual void reportTestSuite(TestSuite_shared ts) override
    {
        *this << "Run Testsuite [" << ts->name << "]; time = " << ts->getTime() << "ms"
              << LF;

        m_abs_tests += ts->getTestStats().getNumTests();
        m_abs_fails += ts->getTestStats().getNumFails();
        m_abs_errs += ts->getTestStats().getNumErrs();
        m_abs_time += ts->getTime();

        AbstractReporter::reportTestSuite(ts);
    }

    /**
     * @brief Implement AbstractReporter#reportTestCase
     */
    virtual void reportTestCase(const test::TestCase& tc) override
    {
        *this << SPACE << "Run Testcase [" << tc.name << "](" << tc.context
              << "); time = " << tc.getDuration() << "ms" << LF << XSPACE;
        switch(tc.getState())
        {
            case test::TestCase::TestState::ERROR:
                *this << (m_color ? ANSI_MAGENTA : "") << "ERROR! " << tc.getErrMsg();
                break;
            case test::TestCase::TestState::FAILED:
                *this << (m_color ? ANSI_RED : "") << "FAILED! " << tc.getErrMsg();
                break;
            case test::TestCase::TestState::PASSED:
                *this << (m_color ? ANSI_GREEN : "") << "PASSED!";
                break;
            default:
                break;
        }
        *this << (m_color ? ANSI_RESET : "") << LF;
    }

    /**
     * @brief Implement AbstractReporter#beginReport
     */
    virtual void beginReport() override
    {}

    /**
     * @brief Implement AbstractReporter#endReport
     */
    virtual void endReport() override
    {
        if(m_abs_fails >= (m_abs_tests + 1) / 2)
        {
            *this << (m_color ? ANSI_YELLOW : "");
        }
        else
        {
            *this << (m_color ? ANSI_CYAN : "");
        }
        *this << "Result:: passed: " << m_abs_tests - m_abs_fails - m_abs_errs << "/"
              << m_abs_tests << " ; failed: " << m_abs_fails << "/" << m_abs_tests
              << " ; errors: " << m_abs_errs << "/" << m_abs_tests
              << " ; time = " << m_abs_time << "ms" << (m_color ? ANSI_RESET : "") << LF;
    }

    /// @brief Use colors flag.
    bool m_color;

    /// @brief The amount of tests.
    std::size_t m_abs_tests = 0;

    /// @brief The amount of failed tests.
    std::size_t m_abs_fails = 0;

    /// @brief The amount of erroneous tests.
    std::size_t m_abs_errs = 0;

    /// @brief The accumulated runtime.
    double m_abs_time = 0;
};

}  // namespace rep

/**
 * @brief Create a PlainTextReporter
 * @param stream The stream to use
 * @param color Whether to print colored text
 * @return a shared pointer to the reporter
 */
inline static rep::AbstractReporter_shared createPlainTextReporter(std::ostream& stream,
                                                                   bool color = false)
{
    return std::make_shared<rep::PlainTextReporter>(stream, color);
}

/**
 * @brief Create a PlainTextReporter
 * @note Prints to stdout.
 * @param color Whether to print colored text
 * @return a shared pointer to the reporter
 */
inline static rep::AbstractReporter_shared createPlainTextReporter(bool color = false)
{
    return std::make_shared<rep::PlainTextReporter>(std::cout, color);
}

/**
 * @brief Create a PlainTextReporter
 * @param file The filename to use
 * @param color Whether to print colored text
 * @return a shared pointer to the reporter
 */
inline static rep::AbstractReporter_shared createPlainTextReporter(const char* file,
                                                                   bool color = false)
{
    return std::make_shared<rep::PlainTextReporter>(file, color);
}

}  // namespace sctf

#endif  // SRC_REPORTER_COLOREDREPORTER_HPP_
