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

#include <memory>

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
    console_reporter(console_reporter const&)     = delete;
    console_reporter(console_reporter&&) noexcept = delete;
    auto operator=(console_reporter const&) -> console_reporter& = delete;
    auto operator=(console_reporter&&) noexcept -> console_reporter& = delete;
    ~console_reporter() noexcept override                            = default;

    /**
     * Create a console reporter.
     *
     * @param stream_ is the stream where to print the report. (default: stdout)
     */
    static auto create(std::ostream& stream_ = std::cout) -> std::shared_ptr<console_reporter> {
        return std::make_shared<console_reporter>(enable{}, stream_);
    }

    /**
     * Create a console reporter.
     *
     * @param fname_ is the filename where to print the report.
     */
    static auto create(char const* fname_) -> std::shared_ptr<console_reporter> {
        return std::make_shared<console_reporter>(enable{}, fname_);
    }

    /**
     * Enable ansi colors in report.
     *
     * @return this reporter again.
     */
    auto with_color() -> std::shared_ptr<console_reporter> {
        m_color = true;
        return std::static_pointer_cast<console_reporter>(shared_from_this());
    }

    /**
     * Enable reporting of captured output.
     *
     * @return this reporter again.
     */
    auto with_captured_output() -> std::shared_ptr<console_reporter> {
        m_capture = true;
        return std::static_pointer_cast<console_reporter>(shared_from_this());
    }

    /// Constructor for std::make_shared.
    explicit console_reporter(enable, std::ostream& stream_) : reporter(stream_) {}

    /// Constructor for std::make_shared.
    explicit console_reporter(enable, char const* fname_) : reporter(fname_) {}

private:
    void report_testsuite(intern::testsuite_ptr const& ts_) override {
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

    bool m_color   = false;  ///< Flags whether print colored results.
    bool m_capture = false;  ///< Flags whether to report captured output from testcases.
};
}  // namespace sctf

#endif  // SCTF_REPORTER_CONSOLE_REPORTER_HPP
