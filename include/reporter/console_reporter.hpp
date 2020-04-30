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

#ifndef SCTF_REPORTER_CONSOLE_REPORTER_HPP
#define SCTF_REPORTER_CONSOLE_REPORTER_HPP

#include "reporter/reporter.hpp"

namespace sctf
{
namespace intern
{
namespace fmt
{
/// Escape sequence for red foreground color.
static constexpr char const* const ANSI_RED = "\x1b[31m";
/// Escape sequence for green foreground color.
static constexpr char const* const ANSI_GREEN = "\x1b[32m";
/// Escape sequence for yellow foreground color.
static constexpr char const* const ANSI_YELLOW = "\x1b[33m";
/// Escape sequence for blue foreground color.
static constexpr char const* const ANSI_BLUE = "\x1b[34m";
/// Escape sequence for magenta foreground color.
static constexpr char const* const ANSI_MAGENTA = "\x1b[35m";
/// Escape sequence for cyan foreground color.
static constexpr char const* const ANSI_CYAN = "\x1b[36m";
/// Escape sequence for reseting the foreground color.
static constexpr char const* const ANSI_RESET = "\x1b[0m";
}  // namespace fmt
}  // namespace intern

/**
 * Reporter implementation with informative console output.
 */
class console_reporter : public intern::reporter
{
public:
    ~console_reporter() noexcept override     = default;
    console_reporter(console_reporter const&) = delete;
    console_reporter& operator=(console_reporter const&) = delete;
    console_reporter& operator=(console_reporter&& other_) noexcept = delete;

    console_reporter(console_reporter&& other_) noexcept
        : reporter(std::move(other_)), m_color(other_.m_color), m_capture(other_.m_capture) {}

    /**
     * Create a console reporter.
     *
     * @param stream_ is the stream where to print the report. (default: stdout)
     * @param color_ is the flag to enable colored results. (default: false)
     * @param capture_ is the flag to enable reporting of captured output. (default: false)
     */
    static reporter_ptr create(std::ostream& stream_ = std::cout, bool color_ = false,
                               bool capture_ = false) {
        return reporter_ptr(new console_reporter(stream_, color_, capture_));
    }

    /**
     * Create a console reporter.
     *
     * @param fname_ is the filename where to print the report.
     * @param color_ is the flag to enable colored results. (default: false)
     * @param capture_ is the flag to enable reporting of captured output. (default: false)
     */
    static reporter_ptr create(char const* fname_, bool color_ = false, bool capture_ = false) {
        return reporter_ptr(new console_reporter(fname_, color_, capture_));
    }

private:
    console_reporter(std::ostream& stream_, bool color_, bool capture_)
        : reporter(stream_), m_color(color_), m_capture(capture_) {}

    console_reporter(char const* fname_, bool color_, bool capture_)
        : reporter(fname_), m_color(color_), m_capture(capture_) {}

    void report_testsuite(intern::testsuite_ptr const ts_) override {
        *this << "Run Testsuite [" << ts_->name() << "]; time = " << ts_->execution_duration()
              << "ms" << intern::fmt::LF;

        reporter::report_testsuite(ts_);
    }

    void report_testcase(intern::testcase const& tc_) override {
        *this << intern::fmt::SPACE << "Run Testcase [" << tc_.name() << "](" << tc_.context()
              << "); time = " << tc_.duration() << "ms" << intern::fmt::LF << intern::fmt::XSPACE;
        if (m_capture) {
            *this << "stdout = \"" << tc_.cout() << "\"" << intern::fmt::LF << intern::fmt::XSPACE;
            *this << "stderr = \"" << tc_.cerr() << "\"" << intern::fmt::LF << intern::fmt::XSPACE;
        }
        switch (tc_.state()) {
            case intern::testcase::result::ERROR:
                *this << (m_color ? intern::fmt::ANSI_MAGENTA : "") << "ERROR! " << tc_.reason();
                break;
            case intern::testcase::result::FAILED:
                *this << (m_color ? intern::fmt::ANSI_RED : "") << "FAILED! " << tc_.reason();
                break;
            case intern::testcase::result::PASSED:
                *this << (m_color ? intern::fmt::ANSI_GREEN : "") << "PASSED!";
                break;
            default: break;
        }
        *this << (m_color ? intern::fmt::ANSI_RESET : "") << intern::fmt::LF;
    }

    void begin_report() override {}

    void end_report() override {
        if (m_abs_fails >= (m_abs_tests + 1) / 2) {
            *this << (m_color ? intern::fmt::ANSI_YELLOW : "");
        } else {
            *this << (m_color ? intern::fmt::ANSI_CYAN : "");
        }
        *this << "Result:: passed: " << m_abs_tests - m_abs_fails - m_abs_errs << "/" << m_abs_tests
              << " ; failed: " << m_abs_fails << "/" << m_abs_tests << " ; errors: " << m_abs_errs
              << "/" << m_abs_tests << " ; time = " << m_abs_time << "ms"
              << (m_color ? intern::fmt::ANSI_RESET : "") << intern::fmt::LF;
    }

    bool m_color;    ///< Flags whether print colored results.
    bool m_capture;  ///< Flags whether to report captured output from testcases.
};
}  // namespace sctf

#endif  // SCTF_REPORTER_CONSOLE_REPORTER_HPP
