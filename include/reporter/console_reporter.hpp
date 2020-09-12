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

#include <cstdint>
#include <memory>

#include "reporter/reporter.hpp"

namespace sctf
{
namespace intern
{
/**
 * Reporter implementation with informative console output.
 */
class console_reporter : public reporter
{
public:
    console_reporter(console_reporter const&)     = delete;
    console_reporter(console_reporter&&) noexcept = delete;
    ~console_reporter() noexcept override         = default;
    auto
    operator=(console_reporter const&) -> console_reporter& = delete;
    auto
    operator=(console_reporter&&) noexcept -> console_reporter& = delete;

    /**
     * Create a console reporter.
     *
     * @param stream_ is the stream where to print the report. (default: stdout)
     */
    static auto
    create(std::ostream& stream_ = std::cout) -> std::shared_ptr<console_reporter> {
        return std::make_shared<console_reporter>(enable{}, stream_);
    }

    /**
     * Create a console reporter.
     *
     * @param fname_ is the filename where to print the report.
     */
    static auto
    create(char const* fname_) -> std::shared_ptr<console_reporter> {
        return std::make_shared<console_reporter>(enable{}, fname_);
    }

    /**
     * Enable ansi colors in report.
     *
     * @return this reporter again.
     */
    auto
    with_color() -> std::shared_ptr<console_reporter> {
        m_color = true;
        return std::static_pointer_cast<console_reporter>(shared_from_this());
    }

    /**
     * Enable reporting of captured output.
     *
     * @return this reporter again.
     */
    auto
    with_captured_output() -> std::shared_ptr<console_reporter> {
        m_capture = true;
        return std::static_pointer_cast<console_reporter>(shared_from_this());
    }

    /// Constructor for std::make_shared.
    explicit console_reporter(enable, std::ostream& stream_) : reporter(stream_) {}

    /// Constructor for std::make_shared.
    explicit console_reporter(enable, char const* fname_) : reporter(fname_) {}

private:
    void
    report_testsuite(testsuite_ptr const& ts_) override {
        *this << "--- " << ts_->name() << " (" << ts_->execution_duration() << "ms) ---" << fmt::LF;

        reporter::report_testsuite(ts_);
    }

    void
    report_testcase(testcase const& tc_) override {
        *this << fmt::SPACE << tc_.name() << " (" << tc_.duration() << "ms)" << fmt::LF << fmt::XSPACE;
        if (m_capture) {
            *this << "stdout = \"" << tc_.cout() << "\"" << fmt::LF << fmt::XSPACE;
            *this << "stderr = \"" << tc_.cerr() << "\"" << fmt::LF << fmt::XSPACE;
        }
        switch (tc_.state()) {
            case testcase::result::ERROR: *this << colored(RED) << "ERROR! " << tc_.reason(); break;
            case testcase::result::FAILED: *this << colored(BLUE) << "FAILED! " << tc_.reason(); break;
            case testcase::result::PASSED: *this << colored(GREEN) << "PASSED!"; break;
            default: break;
        }
        *this << colored() << fmt::LF;
    }

    void
    end_report() override {
        if (m_abs_errs > 0) {
            *this << colored(YELLOW);
        } else if (m_abs_fails > 0) {
            *this << colored(BLUE);
        } else {
            *this << colored(CYAN);
        }
        *this << "=== Result ===" << fmt::LF << "passes: " << m_abs_tests - m_abs_fails - m_abs_errs << "/"
              << m_abs_tests << " failures: " << m_abs_fails << "/" << m_abs_tests << " errors: " << m_abs_errs << "/"
              << m_abs_tests << " (" << m_abs_time << "ms)" << colored() << fmt::LF;
    }

    enum color : std::int_fast8_t
    {
        RED,
        GREEN,
        YELLOW,
        BLUE,
        CYAN,
        RESET
    };

    auto
    colored(color c_ = RESET) const -> char const* {
        if (!m_color) {
            return "";
        }
        switch (c_) {
            case RED: return "\x1b[31m";
            case GREEN: return "\x1b[32m";
            case YELLOW: return "\x1b[33m";
            case BLUE: return "\x1b[34m";
            case CYAN: return "\x1b[36m";
            default: return "\x1b[0m";
        }
    }

    bool m_color   = false;  ///< Flags whether print colored results.
    bool m_capture = false;  ///< Flags whether to report captured output from testcases.
};
}  // namespace intern

using console_reporter = intern::console_reporter;
}  // namespace sctf

#endif  // SCTF_REPORTER_CONSOLE_REPORTER_HPP
